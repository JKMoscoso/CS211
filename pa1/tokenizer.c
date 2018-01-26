/*
 * tokenizer.c
 */
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
/*
 * Tokenizer type.  You need to fill in the type as part of your implementation.
 */

struct TokenizerT_ {
	char* input;
	char* token;
	int index;
	int length;
	
};

typedef struct TokenizerT_ TokenizerT;

/*
 * TKCreate creates a new TokenizerT object for a given token stream
 * (given as a string).
 * 
 * TKCreate should copy the arguments so that it is not dependent on
 * them staying immutable after returning.  (In the future, this may change
 * to increase efficiency.)
 *
 * If the function succeeds, it returns a non-NULL TokenizerT.
 * Else it returns NULL.
 *
 * You need to fill in this function as part of your implementation.
 */

TokenizerT *TKCreate( char * ts ) {
	TokenizerT *tk =  malloc(sizeof(TokenizerT));
	tk->length = strlen(ts);
	tk->index = 0;
	tk->input = malloc(sizeof(tk->length+1));
	tk->token = malloc(sizeof(tk->length+1));
	strcpy(tk->input,ts);
	return tk;
}

/*
 * TKDestroy destroys a TokenizerT object.  It should free all dynamically
 * allocated memory that is part of the object being destroyed.
 *
 * You need to fill in this function as part of your implementation.
 */

void TKDestroy( TokenizerT * tk ) {
	free(tk);
}

/*
 * TKGetNextToken returns the next token from the token stream as a
 * character string.  Space for the returned token should be dynamically
 * allocated.  The caller is responsible for freeing the space once it is
 * no longer needed.
 *
 * If the function succeeds, it returns a C string (delimited by '\0')
 * containing the token.  Else it returns 0.
 *
 * You need to fill in this function as part of your implementation.
 */

char *TKGetNextToken( TokenizerT * tk ) 
{
	char *resultingToken = malloc(sizeof(char));
	char *type = malloc(sizeof(char));
	//Index of the resulting token	
	int resultIndex = 0;
	//Avoids seg faults
	strcat(tk->input," ");
	//1 if all digits, 0 if hex and float
	int digitCheck = 1;
	while(tk->index < tk->length)
	{

		switch(tk->input[tk->index])
		{
			case ' ': case '\t': case '\n': case '\v': case '\f': case '\r':
				/*if((tk->input[tk->index] == 0x5c) && (tk->input[tk->index+1] == 0x09 || tk->input[tk->index+1] == 0x0c
				|| tk->input[tk->index+1] == 0x0a || tk->input[tk->index+1] == 0x0d || tk->input[tk->index+1] == 0x0b))
				{
					printf("invalid 0x%02x\n", \tk->input[tk->index+1]);	
					tk->index++;
					tk->index++;
				}
				else
				{*/	
					if(tk->input[tk->index]!= ' ')
					{		
						printf("invalid 0x%02x\n", tk->input[tk->index]);
					}
					tk->index++;	
					
				//}
					break;
			case '0':
				//Go into zero finite state machines.
				//Outputting the next value in the token to be checked.
				resultingToken[resultIndex] = tk->input[tk->index];
				tk->index++;
				resultIndex++;
				while(!isspace(tk->input[tk->index]))
				{
					//Octal Case
					if(isdigit(tk->input[tk->index]) && !strstr(resultingToken, "x") && !strstr(resultingToken, "X"))
					{
						if(tk->input[tk->index]<'8')
						{
							resultingToken[resultIndex] = tk->input[tk->index];
						}
						if(tk->input[tk->index+1]=='8' || tk->input[tk->index+1]=='9')
						{
							tk->index++;
							break;
						}
					}
					//Hex Case
					else if(tk->input[tk->index] == 'x' || tk->input[tk->index] == 'X')
					{
						if(!isalnum(tk->input[tk->index+1]))
						{
							//printf("Error: No number after X\n");
							break;
						}
						else
						{
							resultingToken[resultIndex] = tk->input[tk->index];
							digitCheck = 0;
							strcpy(type, "hexadecimal");
						}
					}
					else if(strstr(resultingToken, "x") || strstr(resultingToken, "X"))
					{
						if(isxdigit(tk->input[tk->index]))
						{
							resultingToken[resultIndex] = tk->input[tk->index];
							if((tk->input[tk->index] == 'e' || tk->input[tk->index] == 'E') && tk->input[tk->index+1] == '+')
							{
								tk->index++;
								//printf("Error: + after hex E\n");
								break;
							}  
						}
						else if((tk->input[tk->index] >= 'g' && tk->input[tk->index] <= 'z') || (tk->input[tk->index] >= 'G' && tk->input[tk->index] <= 'z'))
						{
							//printf("Error: Nonhexadecimal char found\n");
							break;
						}
						
					}
					//Float Case
					else if(tk->input[tk->index] == '.')
					{
						
						if(!isalnum(tk->input[tk->index+1]))
						{
							//printf("Error: No number after . \n");
							break;
						}
						else
						{
							resultingToken[resultIndex] = tk->input[tk->index];
							digitCheck = 0;
							strcpy(type, "float");
						}
					}
					else if(tk->input[tk->index] == 'e' || tk->input[tk->index] == 'E')
					{
						resultingToken[resultIndex] = tk->input[tk->index];
						if(!isalnum(tk->input[tk->index+1]))
						{
							if(tk->input[tk->index+1] != '+' && tk->input[tk->index+1] != '-')
							{
								//printf("Error: No number, +, or - after E\n");
								break;
							}
						}
					
					}
					else if(tk->input[tk->index] == '+' || tk->input[tk->index] == '-')
					{
						resultingToken[resultIndex] = tk->input[tk->index];
						if(!isalnum(tk->input[tk->index+1]))
						{
							//printf("Error: No number after + or -\n");
							break;
						}
					}
					tk->index++;
					resultIndex++;
				} 
				if(digitCheck == 1)
				{
					strcpy(type, "octal");
				} 
				if(strlen(resultingToken) == 1)
				{
					strcpy(type, "integer");
				}
				printf("%s ", type);
				printf("%s\n", resultingToken);
				break;
			case '1': case '2': case '3': case '4': case '5':
			case '6': case '7': case '8': case '9': 
				//Go into decimal finite state machine
				resultingToken[resultIndex] = tk->input[tk->index];
				tk->index++;
				resultIndex++;
				while(!isspace(tk->input[tk->index]))
				{
					//Integer Case
					if(isdigit(tk->input[tk->index]))
					{
						resultingToken[resultIndex] = tk->input[tk->index];
					}
					//Float Case
					else if(tk->input[tk->index] == '.')
					{
						if(!isalnum(tk->input[tk->index+1]))
						{
							//printf("Error: No number after .");
							break;
						}
						else
						{
							resultingToken[resultIndex] = tk->input[tk->index];
							digitCheck = 0;
							strcpy(type, "float");
						}
					}
					else if(tk->input[tk->index] == 'e' || tk->input[tk->index] == 'E')
					{
						if(!isalnum(tk->input[tk->index+1]))
						{
							if(tk->input[tk->index+1] != '+' && tk->input[tk->index+1] != '-')
							{
								//printf("Error: No number, +, or - after E\n");
								break;
							}
							resultingToken[resultIndex] = tk->input[tk->index];
							digitCheck = 0;
							strcpy(type, "float");
						}
						
							
						
					}
					else if(tk->input[tk->index] == '+' || tk->input[tk->index] == '-')
					{
						
						if(!isalnum(tk->input[tk->index+1]))
						{
							//printf("Error: No number after + or -\n");
							break;
						}
						else
						{
							resultingToken[resultIndex] = tk->input[tk->index];
						}
					}
					else if(isalpha(tk->input[tk->index]))
					{
						//printf("Error: Alphabetical while parsing Integer.\n");
						break;
					}
					tk->index++;
					resultIndex++;
				}
				if(digitCheck == 1)
				{
					strcpy(type, "integer");
				} 
				printf("%s ", type);
				printf("%s\n", resultingToken);
				break;	
			default :
			printf("invalid 0x%02x\n", tk->input[tk->index]);
			tk->index++;
			break;
		}		
	memset(resultingToken,'\0',strlen(resultingToken));
	strcpy(type, "");	 
	resultIndex = 0;
	digitCheck = 1;			
	}
  free(resultingToken);
  free(type);
  return "";
}

/*
 * main will have a string argument (in argv[1]).
 * The string argument contains the tokens.
 * Print out the tokens in the second string in left-to-right order.
 * Each token should be printed on a separate line.
 */

int main(int argc, char **argv)
{
	if(argc<2)
	{
		printf("Error no input.\n");
		return 0;
	}
	TokenizerT *output = TKCreate(argv[1]);
	printf("%s\n", output->input);
	TKGetNextToken(output);
	TKDestroy(output);
 	return 0;
}
