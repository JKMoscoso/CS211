#include <stdio.h>
#include <stdlib.h>

int num[200];
int add(int x, int y)
{
	return x + y;
}

int dothething(int inp)
{
	if(num[inp] == -1)				//L4
	{
		num[inp] = -1;
		if(inp == 0)				//cmpl $0, 8(%ebp)
		{
			num[inp] = 0;
		}
		else if(inp == 1)			//L7
		{
			num[inp] = 1;
		}
		else					//L10
		{
			num[inp] = add(dothething(inp-2),dothething(inp-1));
		}
		if(num[inp] == -1)			//L9
		{
			num[inp] = inp;
		}
	}
	int output = num[inp];				//L12
	return output;					//L6
}

int main(int argc, char **argv)
{
	int var1 = 0;
	int var2 = atoi(argv[1]);
	while(var1 <= 199)
	{
		num[var1] = -1;
		var1++;
	}
	int output = dothething(var2);
	printf("Value:   %d\n", output);
	return 0;
}
