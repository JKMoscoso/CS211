#include 	<stdio.h>
#include 	<string.h>
#include 	<ctype.h>
#include 	<stdlib.h>
#include	"floatToASCII.h"
#include	"binary.h"


union {
   int      x;
   float    y;
} intfloat;

//Converts a 32 bit binary string into a IEEE-754 32-bit floating point number
void binASCIIToFloat(char* string, char* output)
{
	
	intfloat.x = binASCIIToInt(string);
	//Getting weird characters at the end of the string	
	floatToASCII(intfloat.y,output);
	//Accounting for negative sign at the beginning;
	if(output[0] == '-')
	{
		output[12] = '\0';
	}
	else
	{
		output[11] = '\0';
	}
}

int main(int argc, char **argv)
{
	if(argc<2)
	{
		fprintf(stderr, "ERROR: No input.\n");
		return 0;
	}
	char* bin = argv[1];
	if(strlen(bin) != 32)
	{
		fprintf(stderr, "ERROR: Binary input is not 32 characters.\n");
		return 0;
	}
	char* type = argv[2];
	if(strcmp(type,"int") == 0)
	{
		printf("%d\n", binASCIIToInt(bin));
	}
	else if(strcmp(type,"float") == 0)
	{
		char* asciiString = malloc(sizeof(char)*10);
		if(strcmp(bin,"01111111100000000000000000000000") == 0)
		{
			printf("pinf\n");
			return 0;
		}
		else if(strcmp(bin,"11111111100000000000000000000000") == 0)
		{
			printf("ninf\n");
			return 0;
		}
		binASCIIToFloat(bin, asciiString);
		printf("%s\n",asciiString);
		free(asciiString);
		
	} 
	else
	{
		fprintf(stderr, "ERROR: You didn't specify an int of float.\n");
		return 0;
	}
	return 0;
}
