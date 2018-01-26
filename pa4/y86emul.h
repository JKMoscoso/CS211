typedef enum
{
	AOK, 
	HLT, 
	ADR,
	INS,

} state;
typedef union
{
	struct codePart
	{
		unsigned char hHalf:4;
		unsigned char lHalf:4;
	}codePart;
	unsigned char byte;
}regByte;

char *memory;
int32_t registers[8] = {0};
state status;
int ZF,SF,OF;
int jumpOccured = 0;
int progIndex;

int withinRange(int reg)
{
	if(reg >= 0 && reg <= 7)
		return 1;
	else
		return 0;
}

void rrmovl(int index)
{
	regByte* byte = (regByte*)(memory+index);
	if(withinRange(byte->codePart.hHalf)== 1 && withinRange(byte->codePart.lHalf)==1)
	{
		registers[byte->codePart.hHalf] = registers[byte->codePart.lHalf];
	} 
	else
	{
		fprintf(stderr, "Error: Invalid Address\n");
		status = ADR;
	}
}

void irmovl(int index)
{
	regByte* byte = (regByte*)(memory+index);
	if(byte->codePart.lHalf == 0xf && withinRange(byte->codePart.hHalf) == 1)
	{
		int32_t* value = (int32_t*)(memory+index+1);
		registers[byte->codePart.hHalf] = *value;
	}
	else
	{
		fprintf(stderr, "Error: Invalid Address\n");
		status = ADR;
	}
}

void rmmovl(int index)
{
	regByte* byte = (regByte*)(memory+index);
	int32_t* displacement = (int32_t*)(memory+index+1);
	if(withinRange(byte->codePart.hHalf)== 1 && withinRange(byte->codePart.lHalf)==1)
	{
		int32_t regU = registers[byte->codePart.hHalf];
		int32_t regL = registers[byte->codePart.lHalf];
		int32_t addr = regU+*displacement;
		int32_t *mem = (int32_t*)(memory+addr);
		*mem =  regL;
	}
	else
	{
		fprintf(stderr, "Error: Invalid Address\n");
		status = ADR;
	}
	
}

void mrmovl(int index)
{
	////printf("%d\n", registers[7]);
	regByte* byte = (regByte*)(memory+index);
	int32_t* displacement = (int32_t*)(memory+index+1);
	if(withinRange(byte->codePart.hHalf)== 1 && withinRange(byte->codePart.lHalf)==1)
	{
		int32_t regU = registers[byte->codePart.hHalf];
		int32_t addr = regU+*displacement;
		int32_t *mem = (int32_t*)(memory+addr);
		registers[byte->codePart.lHalf] = *mem;
	}
	else
	{
		fprintf(stderr, "Error: Invalid Address\n");
		status = ADR;
	}
	
}

void operand(int index)
{
	regByte* byte = (regByte*)(memory+index);
	regByte* doit = (regByte*)(memory+index+1);
	int32_t operU = registers[doit->codePart.hHalf];
	int32_t operL = registers[doit->codePart.lHalf];
	int32_t discard = operU - operL;
	switch(byte->codePart.hHalf)
	{
		case 0x0:
			//printf("addl detected\n");
			registers[doit->codePart.hHalf] = operU + operL;
			if(registers[doit->codePart.hHalf] == 0)
				ZF = 1;
			else
				ZF = 0;
			if ((operL > 0 && operU > 0 && registers[doit->codePart.hHalf] < 0) || (operL < 0 && operU < 0 && registers[doit->codePart.hHalf] > 0))
				OF = 1;
			else
				OF = 0;
			if (registers[doit->codePart.hHalf] < 0)
				SF = 1;
			else
				SF = 0;
			break;

		case 0x1:
			//printf("subl detected\n");
			registers[doit->codePart.hHalf] = operU - operL;
			if(registers[doit->codePart.hHalf] == 0)
				ZF = 1;
			else
				ZF = 0;
			if ((operL < 0 && operU > 0 && registers[doit->codePart.hHalf] > 0) || (operL > 0 && operU < 0 && registers[doit->codePart.hHalf] < 0))
				OF = 1;
			else
				OF = 0;
			if (registers[doit->codePart.hHalf] < 0)
				SF = 1;
			else
				SF = 0;
			break;

		case 0x2:
			////printf("andl detected\n");
			registers[doit->codePart.hHalf] = operU & operL;
			if(registers[doit->codePart.hHalf] == 0)
				ZF = 1;
			else
				ZF = 0;
			if(registers[doit->codePart.hHalf] < 0)
				SF = 1;
			else
				SF = 0;
			OF = 0;
			break;

		case 0x3:
			////printf("xorl detected\n");
			registers[doit->codePart.hHalf] = operU ^ operL;
			if(registers[doit->codePart.hHalf] == 0)
				ZF = 1;
			else
				ZF = 0;
			if(registers[doit->codePart.hHalf] < 0)
				SF = 1;
			else
				SF = 0;
			OF = 0;
			break;

		case 0x4:
			////printf("mull operation detected\n");
			registers[doit->codePart.hHalf] = operU * operL;
			if(registers[doit->codePart.hHalf] == 0)
				ZF = 1;
			else
				ZF = 0;
			if ((operL > 0 && operU > 0 && registers[doit->codePart.hHalf] < 0) || (operL < 0 && operU < 0 && registers[doit->codePart.hHalf] < 0) || (operL < 0 && operU > 0 && registers[doit->codePart.hHalf] > 0) || (operL > 0 && operU < 0 && registers[doit->codePart.hHalf] > 0))
				OF = 1;
			else
				OF = 0;
			if (registers[doit->codePart.hHalf] < 0)
				SF = 1;
			else
				SF = 0;
			break;

		case 0x5:
			////printf("cmpl detected\n");
			if(discard == 0)
				ZF = 1;
			else
				ZF = 0;
			if (discard < 0)
				SF = 1;
			else
				SF = 0;
			OF = 0;
			break;
		default:
			fprintf(stderr, "Error: Invalid Address\n");
			status = ADR;
			break;
	}
}

void jump(int index)
{
	regByte* byte = (regByte*)(memory+index);
	int32_t* jumpNum = (int32_t*)(memory+index+1);
	switch(byte->codePart.hHalf)
	{
		case 0x0:
			//printf("jmp detected\n");
			progIndex = *jumpNum;
			jumpOccured = 1;
			break;
		case 0x1:
			//printf("jle detected\n");
			if (((SF ^ OF) | ZF) == 1)
			{
				progIndex = *jumpNum;
				jumpOccured = 1;
				break;
			}
			else
				break;
		case 0x2:
			//printf("jl detected\n");
			if ((SF ^ OF) == 1)	
			{
				progIndex = *jumpNum;
				jumpOccured = 1;
				break;
			}
			else
				break;
		case 0x3:
			//printf("je detected\n");
			if (ZF == 1)	
			{
				progIndex = *jumpNum;
				jumpOccured = 1;
				break;
			}
			else
				break;
		case 0x4:
			//printf("jne detected\n");
			if (ZF == 0)
			{	
				progIndex = *jumpNum;
				jumpOccured = 1;
				break;
			}
			else
				break;
		case 0x5:
			//printf("jge detected\n");
			if ((SF ^ OF) == 0)	
			{
				progIndex = *jumpNum;
				jumpOccured = 1;
				break;
			}
			else
				break;
		case 0x6:
			//printf("jg detected\n");
			if (((SF ^ OF) & ZF) == 0)	
			{
				progIndex = *jumpNum;
				jumpOccured = 1;
				break;
			}
			else
				break;
		default:
			fprintf(stderr, "Error: Invalid Instrucion\n");
			status = INS;
			break;
	}
}

void callOp(int index)
{
	int32_t* placeNum = (int32_t*)(memory+index);
	registers[4] -= 4;
	*((int32_t*)&memory[registers[4]]) = progIndex;
	progIndex = *placeNum;
	jumpOccured = 1;
}

void returnOp(int index)
{
	progIndex = *((int32_t*)&memory[registers[4]]);
	registers[4] += 4;
	jumpOccured = 1;
}

void push(int index)
{
	regByte* byte = (regByte*)(memory+index);
	if(byte->codePart.hHalf == 0xf && withinRange(byte->codePart.lHalf) == 1)
	{
		registers[4] -= 4;
		*((int*)&memory[registers[4]]) = registers[byte->codePart.lHalf];
	}
	else
	{
		fprintf(stderr, "Error: Invalid Address\n");
		status = ADR;
	}
}

void pop(int index)
{
	regByte* byte = (regByte*)(memory+index);
	if(byte->codePart.hHalf == 0xf && withinRange(byte->codePart.lHalf) == 1)
	{
		registers[byte->codePart.lHalf] = *((int*)&memory[registers[4]]);
		registers[4] += 4;
	}
	else
	{
		fprintf(stderr, "Error: Invalid Address\n");
		status = ADR;
	}
}

void readByte(unsigned char r, int index)
{
	regByte* byte = (regByte*)memory+index;
	int32_t* readNum = (int32_t*)(memory+index+1);
	int put;
	char charR;
	int32_t longR;
	switch(r)
	{
		case 0x0:
			if((byte->codePart.hHalf == 0xf) && withinRange(byte->codePart.lHalf) == 1)
			{
				OF = 0;
				SF = 0;
				if(scanf("%2x", &put) == EOF)
				{
					ZF = 1;
				}
				else
				{	
					ZF = 0;
					charR = put;
					memory[registers[byte->codePart.lHalf]+*readNum] = charR;
				}
			}
			break;
		case 0x1:
			if((byte->codePart.hHalf == 0xf) && withinRange(byte->codePart.lHalf) == 1)
			{
				OF = 0;
				SF = 0;
				if(scanf("%d", &longR) == EOF)
				{
					ZF = 1;
				}
				else
				{
					ZF = 0;
					memory[registers[byte->codePart.lHalf]+*readNum] = longR;
				}
			}
			break;
		default:
			fprintf(stderr, "Error: Invalid Instrucion\n");
			status = INS;
			break;
	}
}

void writeByte(unsigned char w, int index)
{
	regByte* byte = (regByte*)memory+index;
	int32_t* writeNum = (int32_t*)(memory+index+1);
	switch(w)
	{
		case 0x0:
			if((byte->codePart.hHalf == 0xf) && withinRange(byte->codePart.lHalf) == 1)
			{
				unsigned char* charW = (unsigned char*)(memory+registers[byte->codePart.lHalf]+*writeNum);
				printf("%c", *charW);
			}
			break;
		case 0x1:
			if((byte->codePart.hHalf == 0xf) && withinRange(byte->codePart.lHalf) == 1)
			{
				int32_t* longW = (int32_t*)(memory+registers[byte->codePart.lHalf]+*writeNum);
				printf("%d", *longW);
			}
			break;
		default:
			fprintf(stderr, "Error: Invalid Instrucion\n");
			status = INS;
			break;
	}
}

void movsbl(int index)
{
	regByte* byte = (regByte*)(memory+index);
	int32_t value = registers[byte->codePart.hHalf];
	int32_t* displacement = (int32_t*)(memory+index+1);
	registers[byte->codePart.lHalf] = memory[*displacement+value];
}

void stringPut(int index, char* token, int len)
{
	int x = 0;
	for(; x<len; x++)
	{
		if(token[x] != '"')
		{
			memory[index] = token[x];
			index++;
		}
		
	}
}

void textPut(int index, char* token, int programStart, int len)
{
	int loop = 0;
	while(loop < len)
	{
		regByte* byteput = (regByte*)(memory+index);
		char* high  = (char*)malloc(1);
		char* low = (char*)malloc(1);
		strncpy(high,token+loop+1,1);
		strncpy(low,token+loop,1);
		byteput->codePart.hHalf = strtol(high,NULL,16);
		byteput->codePart.lHalf = strtol(low,NULL,16);
		memory[index] = byteput->byte;
		free(high);
		free(low);
		loop+=2;
		index++;
	}
}

void execute(int start, int end)
{
	progIndex = start;
	regByte* byte;
	while(progIndex < end && status == AOK)
	{
		byte = (regByte*)(memory+progIndex);
		switch (byte->codePart.lHalf)
		{
			case 0x0:
				//printf("No operation to be done here\n");
				break;

			case 0x1:
				printf("Program ended with a halt\n");
				status = HLT;
				break;

			case 0x2:
				//printf("rrmovl operation detected:\n");
				rrmovl(progIndex+1);
				progIndex+=1;
				break;

			case 0x3:
				//printf("irmovl operation detected:\n");
				irmovl(progIndex+1);
				progIndex+=5;
				break;

			case 0x4:
				//printf("rmmovl operation detected\n");
				rmmovl(progIndex+1);
				progIndex+=5;
				break;

			case 0x5:
				//printf("mrmovl operation detected\n");
				mrmovl(progIndex+1);
				progIndex+=5;
				break;

			case 0x6:
				/*if(byte->codePart.hHalf == 0)
					printf("addl operation detected\n");
				else if(byte->codePart.hHalf == 1)
					printf("subl operation detected\n");
				else if(byte->codePart.hHalf == 2)
					printf("andl operation detected\n");
				else if(byte->codePart.hHalf == 3)
					printf("xorl operation detected\n");
				else if(byte->codePart.hHalf == 4)
					printf("mull operation detected\n");
				else if(byte->codePart.hHalf == 5)
					printf("cmpl operation detected\n");*/
				operand(progIndex);
				progIndex+=1;
				break;

			case 0x7:
				/*if(byte->codePart.hHalf == 0)
					printf("jmp operation detected\n");
				else if(byte->codePart.hHalf == 1)
					printf("jle operation detected\n");
				else if(byte->codePart.hHalf == 2)
					printf("jl operation detected\n");
				else if(byte->codePart.hHalf == 3)
					printf("je operation detected\n");
				else if(byte->codePart.hHalf == 4)
					printf("jne operation detected\n");
				else if(byte->codePart.hHalf == 5)
					printf("jge operation detected\n");
				else if(byte->codePart.hHalf == 6)
					printf("jge operation detected\n");*/
				jump(progIndex);
				if(jumpOccured==1)
				{
					break;
				}
				else
				{
					progIndex+=4;
					break;
				}

			case 0x8:
				//printf("call operation detected\n");
				callOp(progIndex+1);
				if(jumpOccured==1)
				{
					break;
				}
				else
				{
					progIndex+=4;
					break;
				}

			case 0x9:
				//printf("ret operation detected\n");
				returnOp(progIndex);
				progIndex++;
				break;

			case 0xa:
			{
				//printf("pushl operation detected\n");
				push(progIndex+1);
				progIndex+=1;
				break;
			}

			case 0xb:
			{
				//printf("popl operation detected\n");
				pop(progIndex+1);
				progIndex+=1;
				break;
			}

			case 0xc:
				if(byte->codePart.hHalf == 0x0)
				{
					//printf("readb operation detected\n");
				}
				else if(byte->codePart.hHalf == 0x1)
				{
					//printf("readl operation detected\n");
				}
				readByte(byte->codePart.hHalf, progIndex+1);
				progIndex+=5;
				break;

			case 0xd:
				if(byte->codePart.hHalf == 0x0)
				{
					//printf("writeb operation detected\n");
				}
				else if(byte->codePart.hHalf == 0x1)
				{
					//printf("writel operation detected\n");
				}
				writeByte(byte->codePart.hHalf,progIndex+1);
				progIndex+=5;
				break;

			case 0xe:
				//printf("movsbl operation detected\n");
				movsbl(progIndex+1);
				progIndex+=5;
				break;

			default:
				fprintf(stderr, "Error: Invalid Instrucion\n");
				status = INS;
				break;
		}
		if(jumpOccured == 1)
			jumpOccured = 0;
		else
		{
			progIndex++;
			jumpOccured = 0;
		}
	}
}
