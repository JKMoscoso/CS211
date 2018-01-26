#include <stdio.h>
#include <string.h>
#include <stdint.h>

//Converts binary ASCII string to int
unsigned int binASCIIToInt(char * str)
{
	int i;
	int value = 0;
	for(i = 0; i < 32; i++)
	{
		switch(str[i])
		{
			case '0':
			value = (value << 1);
			break;
			case '1':
			value = (value << 1) + 1;
			break;
			default:
			break;
		}
	}
	return value;
}

//Converts integers to binary ASCII

char* intToBinASCII( unsigned int x, char* output )
{
	unsigned int i;
	unsigned int n;
	char cstr[2];
	for(i = 0; i < 32; i++)
	{
		n = x>>31;
		if(n == 0)
		{
			//Makes null terminated string
			cstr[0] = '0';
			cstr[1] = '\0';
			strcat(output, cstr);
		}
		else if(n == 1)
		{
			cstr[0] = '1';
			cstr[1] = '\0';
			//Maintains null terminated string
			strcat(output, cstr);
		}
		else
		{
			fprintf(stderr, "Weird impossible value %d\n", n);
		}
		x<<=1; //Get next highest bit
	}
	return output;
}
