#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <regex.h>
#include <stdbool.h>
#include <string.h>

#define LENGHT 20

bool interrupted = false;

void exit_handler(int s)
{
	printf("[WARNING] Caught signal %d\n", s);
	interrupted = true;
}

int main(void)
{
	int reti;
	FILE * fp;
	FILE * out;
	char * line = NULL;
	size_t len = 0;
	ssize_t read;
	
	regex_t compiledRegex;
	char * name = alloca(sizeof(char)*LENGHT);

	printf("Enter filename (%d): ", LENGHT);
	scanf("%20s", name);
	//fgets(name, LENGHT, stdin);
	printf("[INFO] Opening file..");	
	fp = fopen(name, "r");
	if (fp == NULL)
	{
		printf("[ERROR] Failed to open file!");
		return -1;
	}
	out = fopen("program_output_data.txt", "a");
	if (out == NULL)
	{
		printf("[ERROR] Failed to open file!");
		return -1;
	}

	struct sigaction sigIntHandler;
	sigIntHandler.sa_handler = exit_handler;
	sigemptyset(&sigIntHandler.sa_mask);
	sigIntHandler.sa_flags = 0;
	sigaction(SIGINT, &sigIntHandler, NULL);

	printf("[INFO] Creating regex...");
	reti = regcomp(&compiledRegex, "^.*\\..*,.*$", 0);
	
	if (reti)
	{
		printf("[ERROR] Could not compile regex!");
		return -2;
	}
	printf("[INFO] Retrieving file data...");

	while ((read = getline(&line, &len, fp)) != -1)
	{	
		if (interrupted)
		{
			fclose(fp);
			fclose(out);
			free(line);
			printf("[INFO ]Exiting...");
			exit(1);
		}

		reti = regexec(&compiledRegex, line, 0, NULL, 0);
	       	if (reti)
		{
			//printf("%s", line);
			fprintf(out, "%s", line);
		}


	}
	fclose(fp);
	fclose(out);
	if(line)
	{
		free(line);
	}
	printf("[INFO] Ouptut saved, program completed!");
	return 0;
}
