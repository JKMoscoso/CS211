#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "y86emul.h"
int programStart = 0;
int programEnd = 0;

int main(int argc, char ** argv)
{
	if(argc != 2)
	{
		fprintf(stderr, "Error: No input file\n");
		return 1;
	}

	else if(strcmp(argv[1], "-h") == 0)
	{
		printf("Usage: y86emul <y86 input file>\n");
		return 0;
	}

	else
	{
		FILE *ptr_file;
		char buffer[1000];

		ptr_file = fopen(argv[1],"r");

		//If no file    		
		if (!ptr_file)
		{
			fprintf(stderr, "Error: Input file is empty\n");
			return 1;
		}

		else
		{
			int buffline = 0;
			int size;
			int index;
			int tokenLen;
			int textFound = 0;
			int put;
			char* currentToken;
			char throwaway[6];

			while (fgets(buffer,1000, ptr_file)!=NULL)
			{
				//Assuming that the first line is always the size
				if(buffline == 0)
				{
					sscanf(buffer, "%s %x\n",throwaway, &size); 
					memory = malloc(sizeof(char)*size);
				}

				else
				{
					currentToken = strtok(buffer, " \t");
					if(strcmp(currentToken, ".string") == 0)
					{
						currentToken = strtok(NULL, " \t\n");
						sscanf(currentToken, "%x", &index);
						currentToken = strtok(NULL, "\t\n");
						tokenLen = strlen(currentToken);
						stringPut(index, currentToken, tokenLen);
					}
					else if(strcmp(currentToken, ".long") == 0)
					{
						currentToken = strtok(NULL, " \t\n");
						sscanf(currentToken, "%x", &index);
						currentToken = strtok(NULL, " \t\n");
						sscanf(currentToken, "%d", &put);
						memory[index] = put;
					}
					else if(strcmp(currentToken, ".byte") == 0)
					{
						currentToken = strtok(NULL, " \t\n");
						sscanf(currentToken, "%x", &index);
						currentToken = strtok(NULL, " \t\n");
						sscanf(currentToken, "%2x", &put);
						char charput = put;
						memory[index] = charput;
					}
					else if(strcmp(currentToken, ".text") == 0)
					{
						textFound = 1;
						currentToken = strtok(NULL, " \t\n");
						sscanf(currentToken, "%x", &index);
						programStart = index;
						currentToken = strtok(NULL, " \t\n");
						tokenLen = strlen(currentToken);

						if(tokenLen%2 != 0)
						{
							fprintf(stderr, "Error: Invalid Instruction\n");
							return 1;
						}

						else
						{
							programEnd = programStart+tokenLen/2;
							textPut(index, currentToken, programStart, tokenLen);
						}

					}
				}	
				buffline++;
			}
			if(textFound == 0)
			{
				fprintf(stderr, "Error: No instructions found\n");
				return 1;
			}
			status = AOK;
			execute(programStart,programEnd);
		}
		fclose(ptr_file);
		
	}
	
	return 0;
}
