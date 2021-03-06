#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

#define numOfRegisters 16
#define memorySize 65536
#define wordSize 8

void HexToBin(char *str, char *binString)
{
	//Converts a word from hexadecimal to binary.
	binString[0] = '\0';
	for (unsigned int i = 0; i < strlen(str); i++)
	{
		if (str[i] == '0')
			strcat_s(binString, strlen(binString) + 5, "0000");
		if (str[i] == '1')
			strcat_s(binString, strlen(binString) + 5, "0001");
		if (str[i] == '2')
			strcat_s(binString, strlen(binString) + 5, "0010");
		if (str[i] == '3')
			strcat_s(binString, strlen(binString) + 5, "0011");
		if (str[i] == '4')
			strcat_s(binString, strlen(binString) + 5, "0100");
		if (str[i] == '5')
			strcat_s(binString, strlen(binString) + 5, "0101");
		if (str[i] == '6')
			strcat_s(binString, strlen(binString) + 5, "0110");
		if (str[i] == '7')
			strcat_s(binString, strlen(binString) + 5, "0111");
		if (str[i] == '8')
			strcat_s(binString, strlen(binString) + 5, "1000");
		if (str[i] == '9')
			strcat_s(binString, strlen(binString) + 5, "1001");
		if (str[i] == 'A')
			strcat_s(binString, strlen(binString) + 5, "1010");
		if (str[i] == 'B')
			strcat_s(binString, strlen(binString) + 5, "1011");
		if (str[i] == 'C')
			strcat_s(binString, strlen(binString) + 5, "1100");
		if (str[i] == 'D')
			strcat_s(binString, strlen(binString) + 5, "1101");
		if (str[i] == 'E')
			strcat_s(binString, strlen(binString) + 5, "1110");
		if (str[i] == 'F')
			strcat_s(binString, strlen(binString) + 5, "1111");
	}
}

int main(int argc, char *argv[])
{
	errno_t err;
	if (argc != 7)
		exit(1);
	if (strcmp(argv[1], "sim"))
		exit(1);
	//Creates an array of registers of size 16
	int registers[numOfRegisters];
	for (int i = 0; i < numOfRegisters; i++)
	{
		registers[i] = 0;
	}

	//Creates word of zero.
	char zeroWord[wordSize];
	for (int i = 0; i < wordSize; i++)
	{
		zeroWord[i] = '0';
	}

	int PC = 0;
	int count = 0;

	//Reads the memory- memin.txt
	FILE *meminFilePoint = NULL;
	err = fopen_s(&meminFilePoint, argv[2], "r");

	if (meminFilePoint == 0) {
		fprintf_s(stderr, "Error opening memin.txt");
		exit(1);
	}

	//Creates an array of the memory.
	char memory[memorySize][wordSize + 1];
	for (int i = 0; i < memorySize; i++)
	{
		for (int j = 0; j < wordSize; j++)
		{
			memory[i][j] = '0';
		}
	}

	char s[wordSize + 1];
	for (int i = 0; i < memorySize && fgets(s, wordSize + 1, meminFilePoint) != NULL; i++)
	{
		if (strcmp(s, "\n") == 0)
		{
			i--;
		}
		else
		{
			for (int j = 0; j < wordSize; j++)
			{
				memory[i][j] = s[j];
			}
		}
	}
	fclose(meminFilePoint);

	FILE *traceFilePoint;
	err = fopen_s(&traceFilePoint, argv[5], "w");
	if (traceFilePoint == 0) {
		fprintf_s(stderr, "Error opening trace.txt");
		exit(1);
	}

	char fullInstHexa[wordSize + 1];
	char fullInstBinar[wordSize * 4 + 1];

	//an infinite loop which stops at halt and executes one instruction in each loop.
	while (PC < memorySize)
	{
		count++;

		//reading the row number PC
		for (int i = 0; i < wordSize; i++)
		{
			fullInstHexa[i] = memory[PC][i];
		}
		fullInstHexa[wordSize] = '\0';
		fullInstBinar[0] = '\0';
		HexToBin(fullInstHexa, fullInstBinar);
		//dividing it to parts
		int instOpcode = 0;
		int instRd = 0;
		int instRs = 0;
		int instRt = 0;
		int instImm = 0;
		for (int i = 0; i < 4; i++)
		{
			instOpcode += (int)(fullInstBinar[i] - '0') * (int)pow(2, 3 - i);
			instRd += (int)(fullInstBinar[i + 4] - '0') * (int)pow(2, 3 - i);
			instRs += (int)(fullInstBinar[i + 8] - '0') * (int)pow(2, 3 - i);
			instRt += (int)(fullInstBinar[i + 12] - '0') * (int)pow(2, 3 - i);
		}
		instImm = -1 * ((int)(fullInstBinar[16] - '0') * (int)pow(2, 15));
		for (int i = 0; i < 15; i++)
		{
			instImm += (int)(fullInstBinar[17 + i] - '0') * (int)pow(2, 14 - i);
		}
		fullInstBinar[32] = '\0';
		//adding a line in trace.txt
		char traceNextLineCoding[(numOfRegisters + 2)*(wordSize + 1) + 2];
		traceNextLineCoding[(numOfRegisters + 2)*(wordSize + 1)] = '\0';
		for (int i = 0; i < (numOfRegisters + 2); i++)
		{
			char temporary[wordSize + 1];
			temporary[wordSize] = '\0';
			if (i == 0)
			{
				snprintf(temporary, wordSize + 1, "%lX", PC);
			}
			else if (i == 1)
			{
				for (int j = 0; j < wordSize; j++)
				{
					temporary[j] = fullInstHexa[j];
				}
			}
			else
			{
				snprintf(temporary, wordSize + 1, "%lX", registers[i - 2]);
			}
			int len = wordSize - strlen(temporary); // get the missing part size
			memmove(temporary + len, temporary, strlen(temporary)); // move the string characters to the end
			for (int i = 0; i < len; i++) // fill the beginning characters with '0'
				temporary[i] = '0';
			temporary[wordSize] = '\0';
			for (int j = 0; j < wordSize; j++)
			{
				traceNextLineCoding[i*(wordSize + 1) + j] = temporary[j];
			}
			traceNextLineCoding[(i + 1)*(wordSize + 1) - 1] = ' ';
		}
		traceNextLineCoding[(wordSize + 1)*(numOfRegisters + 2) ] = '\n';
		traceNextLineCoding[(wordSize + 1)*(numOfRegisters + 2) + 1] = '\0';
		fprintf(traceFilePoint, traceNextLineCoding);
		//executing according to the opcode
		bool exit = false;
		switch (instOpcode)
		{
		case 0:
			//add: R[rd]=R[rs]+R[rt]
			registers[instRd] = registers[instRs] + registers[instRt]; break;
		case 1:
			//sub: R[rd]=R[rs]-R[rt]
			registers[instRd] = registers[instRs] - registers[instRt]; break;
		case 2:
			//and: R[rd]=R[rs]&R[rt]
			registers[instRd] = registers[instRs] & registers[instRt]; break;
		case 3:
			//or: R[rd]=R[rs]|R[rt]
			registers[instRd] = registers[instRs] | registers[instRt]; break;
		case 4:
			//sll: R[rd]=R[rs]<<R[rt]
			registers[instRd] = registers[instRs] * (int)pow(2, registers[instRt]); break;
		case 5:
			//sra: R[rd]=R[rs]>>R[rt]
			registers[instRd] = registers[instRs] / (int)pow(2, registers[instRt]); break;
		case 6:
			//limm: R[rd] = sign extended imm (bits 31:16=sign, bits 15:0=imm)
			registers[instRd] = instImm; break;
		case 7:
			//beq: if (R[rs] == R[rt]) pc = imm-1
			if (registers[instRs] == registers[instRt])
				PC = instImm - 1;
			break;
		case 8:
			//bgt: if (R[rs] > R[rt]) pc = imm-1
			if (registers[instRs] > registers[instRt])
				PC = instImm - 1;
			break;
		case 9:
			//ble: if (R[rs] <= R[rt]) pc = imm-1
			if (registers[instRs] < registers[instRt])
				PC = instImm - 1;
			break;
		case 10:
			//bne: if (R[rs] != R[rt]) pc = imm-1
			if (registers[instRs] != registers[instRt])
				PC = instImm - 1;
			break;
		case 11:
			//jal: R[15] = pc + 1 (next instruction address), pc = imm-1
			registers[numOfRegisters - 1] = PC + 1;
			PC = instImm - 1;
			break;
		case 12:
		{
			//lw: R[rd] = MEM[R[rs]+imm]
			char temporary[wordSize + 1];
			temporary[wordSize] = '\0';
			for (int i = 0; i < wordSize; i++)
			{
				temporary[i] = memory[registers[instRs] + instImm][i];
			}
			registers[instRd] = (int)strtoul(temporary, NULL, 16);
			break;
		}
		case 13:
		{
			//sw: MEM[R[rs]+imm] = R[rd]
			char temporary[wordSize] = { '\0' };
			snprintf(temporary, wordSize + 1, "%lX", registers[instRd]);
			for (int i = 0; i < wordSize; i++)
			{
				memory[registers[instRs] + instImm][i] = temporary[i];
			}
			break;
		}
		case 14:
			//jr: pc = R[rd]-1
			PC = registers[instRd] - 1;
			break;
		case 15:
			//halt: Halt execution, exit simulator
			exit = true;
			break;
		}
		if (exit == true)
			break;
		PC++;
		registers[0] = 0;
	}
	fclose(traceFilePoint);
	FILE *memoutFilePoint;
	err = fopen_s(&memoutFilePoint, argv[3], "w");
	if (memoutFilePoint == 0) {
		fprintf_s(stderr, "Error opening memout.txt");
		exit(1);
	}

	FILE *regoutFilePoint;
	err = fopen_s(&regoutFilePoint, argv[4], "w");
	if (regoutFilePoint == 0) {
		fprintf_s(stderr, "Error opening regout.txt");
		exit(1);
	}

	FILE *countFilePoint;
	err = fopen_s(&countFilePoint, argv[6], "w");
	if (countFilePoint == 0) {
		fprintf_s(stderr, "Error opening count.txt");
		exit(1);
	}

	//creating register array file - regout.txt
	for (int i = 0; i < numOfRegisters; i++)
	{
		char regoutNextLineCoding[wordSize + 2];
		snprintf(regoutNextLineCoding, wordSize + 1, "%lX", registers[i]);
		int len = wordSize - strlen(regoutNextLineCoding); // get the missing part size
		memmove(regoutNextLineCoding + len, regoutNextLineCoding, strlen(regoutNextLineCoding)); // move the string characters to the end
		for (int i = 0; i < len; i++) // fill the beginning characters with '0'
			regoutNextLineCoding[i] = '0';
		regoutNextLineCoding[wordSize] = '\n';
		regoutNextLineCoding[wordSize+1] = '\0';
		fprintf(regoutFilePoint, regoutNextLineCoding);
	}
	//creating file memout.txt - the memory after the simulator

	for (int i = 0; i < memorySize; i++)
	{
		char temporary[wordSize + 2];
		memcpy(temporary, &memory[i], wordSize);
		temporary[wordSize] = '\0';
		int len = wordSize - strlen(temporary); // get the missing part size
		memmove(temporary + len, temporary, strlen(temporary)); // move the string characters to the end
		for (int j = 0; j < len; j++) // fill the beginning characters with '0'
			temporary[j] = '0';
		temporary[wordSize] = '\n';
		temporary[wordSize+1] = '\0';
		fprintf(memoutFilePoint, temporary);
	}
	for (int i = 0; i < memorySize; i++)
		memory[i][wordSize] = '\0';
	//creating count.txt - how much instructions were executed

	fprintf(countFilePoint, "%d\n", count);

	//closing files
	fclose(memoutFilePoint);
	fclose(regoutFilePoint);
	fclose(countFilePoint);

	return 0;
}