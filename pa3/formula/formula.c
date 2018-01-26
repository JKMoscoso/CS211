#include <time.h>
#include <sys/time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "nCr.h"



int main(int argc, char **argv)
{
	if (argc < 2 || strcmp(argv[1], "-h") == 0)
	{
		printf("Usage: %s <positive integer>\n", argv[0]);
		return 0;
	}
	else if(atoi(argv[1]) == 0 && argv[1][0] != '0')
	{
		fprintf(stderr, "ERROR: Input is not number\n");
		return 0;
	}
	else
	{
		int number = atoi(argv[1]);
		char* output = malloc(sizeof(char)* 255);
		char* token  = malloc(sizeof(char)* 100); 
		struct timeval start, end;
		int i = 0;
		gettimeofday(&start, NULL);
		//printf("%d\n", Factorial(number));
		for(; i<=number; i++)
		{
			if(i == 0)
			{
				sprintf(token, "%d", nCr(number,i));
				strcat(output, token);
			}
			else if(i!=number)
			{
				sprintf(token, " + %d*x^%d", nCr(number,i), i);
				strcat(output, token);
				//Break out of loop; overflow error detected
				if(token[3] == '0')
				{
					i = -1;
					break;
				}
			}
			else if(i==number)
			{
				sprintf(token, " + %d*x^%d", nCr(number,i), i);
				strcat(output, token);
			}
		}
		gettimeofday(&end, NULL);
		if(i != -1)
		{
			printf("(1 + x)^%d = %s\n", number, output);
			printf("Time required  = %ld Microseconds\n", ((end.tv_sec * 1000000 + end.tv_usec)
			  - (start.tv_sec * 1000000 + start.tv_usec)));
		}
		else
		{
			fprintf(stderr, "ERROR: Overflow detected\n");
		}
		free(output);
		free(token);
		return 0;
	}
}
