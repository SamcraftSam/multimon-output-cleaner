#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <regex.h>

#define LENGHT 20

int main()
{
	int reti;
	FILE * fp;
	char * line = NULL;
	size_t len = 0;
	ssize_t read;
	regex_t compiledRegex;
	char * name = alloca(sizeof(char)*LENGHT);
	printf("Enter filename (%d): ", LENGHT);
	scanf("%20s", name);
	fp = fopen(name, "r");
	if (fp == NULL)
	{
		return 1;
	}
	//free(name);
	printf("Creating regex...");
	reti = regcomp(&compiledRegex, "^.*\\..*,.*$", 0);
	if (reti)
	{
		printf("Could not compile regex!");
		return -2;
	}
	printf("Retrieving file...");
	
	while ((read = getline(&line, &len, fp)) != -1)
	{
		//printf("%zd", read); debug option
		//printf("%s \n", line);
		//if
		reti = regexec(&compiledRegex, line, 0, NULL, 0);
	       	if (reti)
		{
			printf("%s", line);
		}
	}
	fclose(fp);
	if(line)
	{
		free(line);
	}
	return 0;
}
