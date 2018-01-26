#include 	<stdio.h>
#include 	<string.h>
#include 	<ctype.h>
#include 	<stdlib.h>
#include	"binary.h"
#include	"octal.h"
#include	"decimal.h"
#include	"hex.h"

int convertToInt(char *string)
{
	int i = 0;
	int negcheck;
	int output;
	char *outputNum = malloc(sizeof(char)*strlen(string));
	strncpy(outputNum, string+1, strlen(string));	
	//Check if negative
	if(string[0] == '-')
	{
		negcheck = 1;
		i++;
		strncpy(outputNum, outputNum+1, strlen(outputNum));
	}
	//Converts the number into what was given
	switch(string[i])
	{
		case 'b':
			output = binASCIIToInt(outputNum);
			break;
		case 'o':
			output = octASCIIToInt(outputNum);
			break;
		case 'd':
			output = decASCIIToInt(outputNum);
			break;
		case 'x':
			output = hexASCIIToInt(outputNum);
			break;
	}
	//Implements the negative as a negative one times the number. Such as -1 * hex A
	if(negcheck == 1)
	{
		output *= -1;
	}
	free(outputNum);
	return output;
}

//Converts back to the desired base
char* desiredConvert(int number, char base, char *output)
{
	if(number == 0)
	{
		return "0";
	}
	switch(base)
	{
		case 'b':
			output = intToBinASCII(number, output);
			break;
		case 'o':
			output = intToOctASCII(number, output);
			break;
		case 'd':
			output = intToDecASCII(number, output);
			break;
		case 'x':
			output = intToHexASCII(number, output);
			break;
	}	
	return output;
}

//Removes leading zeroes in the string.
char* removeZeroes(char * string)
{
	if(strlen(string) == 1)
	{
		return string;
	}
	int i = 0;
	while(string[i] == '0')
	{
		i++;
	}
	string += i;	
	return string;
}

/*
 * main will have a string argument (in argv[1]).
 * The string argument contains the operator, two arguments, and the desired outcome.
 * Print out the result in the disired format.
 */

int main(int argc, char **argv)
{
	if(argc<2)
	{
		printf("ERROR: No input.\n");
		return 0;
	}
	char op;
	char *first;
	char *second;
	char opb;
	if(argv[1][0] == '+' || argv[1][0] == '-' || argv[1][0] == '*')
	{
		op = argv[1][0];
		if(argv[2][0] == 'b' || argv[2][0] == 'o' || argv[2][0] == 'd' || argv[2][0] == 'x' || argv[2][0] == '-')
		{
			first = argv[2];
			if(argv[3][0] == 'b' || argv[3][0] == 'o' || argv[3][0] == 'd' || argv[3][0] == 'x' || argv[3][0] == '-')
			{
				second = argv[3];
				if(argv[4][0] == 'b' || argv[4][0] == 'o' || argv[4][0] == 'd' || argv[4][0] == 'x')			
				{
					opb = argv[4][0];
				}
				else
				{
					fprintf(stderr, "ERROR: Format is incorrect. Didn't detect a desired output base.\n");
					return 0;
				}
			}
			else
			{
				fprintf(stderr, "ERROR: Format is incorrect. Didn't detect a second number.\n");
				return 0;
			}
		}
		else
		{
			fprintf(stderr, "ERROR: Format is incorrect. Didn't detect a first number.\n");
			return 0;
		}
	}
	else
	{
		fprintf(stderr, "ERROR: Format is incorrect. Didn't detect +,-, or * first.\n");
		return 0;
	}		
	printf("This is the operator %c, these are the numbers %s, %s, and this is the desired output base %c \n", op, first, second, opb);
	//Conversion time
	int firstNum = convertToInt(first);
	int secondNum = convertToInt(second);
	int outputNum;
	switch(op)
	{
		case '+':
			outputNum = firstNum + secondNum;
			break;
		case '-':
			outputNum = firstNum - secondNum;
			break;
		case '*':
			outputNum = firstNum * secondNum;
			break;
	}
	int negCheck = 0;
	if(outputNum < 0)
	{
		outputNum *= -1;
		negCheck = 1;
	}
	char *output = malloc(sizeof(char));
	free(output);	
	output = desiredConvert(outputNum, opb, output);
	output = removeZeroes(output);
	//Instead of going through the trouble of adding a negative, I just added it before the printed output.
	if(negCheck == 1)
	{
		printf("Output: -%c%s\n", opb, output);
	}
	else
	{	
		printf("Output: %c%s\n", opb, output);
	}	
	return 0;
}
