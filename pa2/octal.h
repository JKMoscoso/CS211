#include <stdio.h>
#include <string.h>
#include <stdint.h>

int octASCIIToInt(char * str)
{
	int i;
	int value = 0;
	for(i = 0; i < 11; i++)
	{
		switch(str[i])
		{
			case '0':
			value = (value << 3);
			break;
			case '1':
			value = (value << 3) + 1;
			break;
			case '2':
			value = (value << 3) + 2;
			break;
			case '3':
			value = (value << 3) + 3;
			break;
			case '4':
			value = (value << 3) + 4;
			break;
			case '5':
			value = (value << 3) + 5;
			break;
			case '6':
			value = (value << 3) + 6;
			break;
			case '7':
			value = (value << 3) + 7;
			break;
			default:
			break;
		}
	}
	return value;
}

//Converts integers to Oct ASCII String

char* intToOctASCII(int x, char* output)
{
	int n = 0;
	char cstr[2];	
	if(x < 0)
	{
		n = 1;
	}
	x = abs(x);
	while(x!=0)
	{
		cstr[0] = (x%8)+'0';
		cstr[1] = '\0';
		strcat(output,cstr);
		x/=8;
	}
	//strcat(output,"0");
	if(n == 1)
	{
		strcat(output,"-");
	}
	//Reversing the string
	int i = 0;
	int j = strlen(output) - 1;
	char temp;
	while(i < j)
	{
		temp = output[i];
		output[i] = output[j];
		output[j] = temp;
		i++;
		j--;
	}
	return output;
}
