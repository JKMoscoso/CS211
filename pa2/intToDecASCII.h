#include <stdio.h>
#include <string.h>
#include <stdint.h>

//Convert int to decimal ASCII string

char* intToDecASCII(int x, char* output)
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
		cstr[0] = (x%10)+'0';
		cstr[1] = '\0';
		strcat(output,cstr);
		x/=10;
	}
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
