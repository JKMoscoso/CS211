#include <stdio.h>
#include <string.h>
#include <stdint.h>

//Converts hex ASCII string to int
int hexASCIIToInt(char * str)
{
	int i;
	int value = 0;
	for(i = 0; i < 8; i++)
	{
		switch(str[i])
		{
			case '0':
			value = (value << 4);
			break;
			case '1':
			value = (value << 4) + 1;
			break;
			case '2':
			value = (value << 4) + 2;
			break;
			case '3':
			value = (value << 4) + 3;
			break;
			case '4':
			value = (value << 4) + 4;
			break;
			case '5':
			value = (value << 4) + 5;
			break;
			case '6':
			value = (value << 4) + 6;
			break;
			case '7':
			value = (value << 4) + 7;
			break;
			case '8':
			value = (value << 4) + 8;
			break;
			case '9':
			value = (value << 4) + 9;
			break;
			case 'A':
			value = (value << 4) + 10;
			break;
			case 'B':
			value = (value << 4) + 11;
			break;
			case 'C':
			value = (value << 4) + 12;
			break;
			case 'D':
			value = (value << 4) + 13;
			break;
			case 'E':
			value = (value << 4) + 14;
			break;
			case 'F':
			value = (value << 4) + 15;
			break;
			default:
			break;
		}
	}
	return value;
}

//Converts integers to Hex ASCII String

char* intToHexASCII(unsigned int x, char* output)
{
	unsigned int i,n;
	char const* hexChar = "0123456789ABCDEF";
	char cstr[2];
	output[0] = '\0';
	for(i = 0; i < 8; i++)
	{
		n = x>>28; // preserve last 4 bits
		//printf("%d\n",n);
		cstr[0] = hexChar[n];
		cstr[1] = '\0';
		strcat(output, cstr);
		//printf("%s\n",output);
		x <<= 4;
	}
	return output;
}
