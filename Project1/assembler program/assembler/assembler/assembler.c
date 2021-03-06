#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

#define memorySize 65536
#define outputLineLength 8
#define maxLineLength 500
#define maxLabelLength 50

void RemoveComment(char *s)
{
	//Removes all comments in the assembly code.
	for (unsigned int i = 0; i < strlen(s); i++)
		if (s[i] == '#')
		{
			s[i] = '\0';
			break;
		}
}

void DecimalToBinary(char *s, char *converted, int len)
{
	//Converts a decimal string to a binary string.
	int data = atoi(s);

	for (unsigned int i = 0; i < len; i++)
	{
		unsigned int mask = 1u << (len - 1 - i);
		converted[i] = (data & mask) ? '1' : '0';
	}
	converted[len] = '\0';
}

void BinToHex(char *binCode, char *hex)
{
	//Converts binary code to hexadecimal.
	char part[5];
	for (unsigned int i = 0; i < strlen(binCode); i += 4)
	{
		memcpy(part, &binCode[i], 4);
		part[4] = '\0';
		sprintf_s(hex + i / 4, strlen(binCode) / 4 - i / 4 + 1, "%X", (int)strtol(part, NULL, 2));
	}
	hex[strlen(binCode) / 4] = '\0';
}

char *ImmToHex(char *s)
{
	//Using the conversion functions, it returns a conversion of the imm code to hexadecimal.
	if (s[1] == 'x'&&s[0] == '0')
		return s + 2;
	else
	{
		//If the imm is decimal it is converted using the decimal converter.
		char converted[17];
		DecimalToBinary(s, converted, 16);
		BinToHex(converted, s);
		return s;
	}
}

char *TextToHex(char *s)
{
	//Converts the known words to binary.
	if (strcmp(s, "add") == 0 || strcmp(s, "$zero") == 0)
		return "0";
	if (strcmp(s, "sub") == 0 || strcmp(s, "$at") == 0)
		return "1";
	if (strcmp(s, "and") == 0 || strcmp(s, "$v0") == 0)
		return "2";
	if (strcmp(s, "or") == 0 || strcmp(s, "$a0") == 0)
		return "3";
	if (strcmp(s, "sll") == 0 || strcmp(s, "$a1") == 0)
		return "4";
	if (strcmp(s, "sra") == 0 || strcmp(s, "$t0") == 0)
		return "5";
	if (strcmp(s, "limm") == 0 || strcmp(s, "$t1") == 0)
		return "6";
	if (strcmp(s, "beq") == 0 || strcmp(s, "$t2") == 0)
		return "7";
	if (strcmp(s, "bgt") == 0 || strcmp(s, "$t3") == 0)
		return "8";
	if (strcmp(s, "ble") == 0 || strcmp(s, "$s0") == 0)
		return "9";
	if (strcmp(s, "bne") == 0 || strcmp(s, "$s1") == 0)
		return "A";
	if (strcmp(s, "jal") == 0 || strcmp(s, "$s2") == 0)
		return "B";
	if (strcmp(s, "lw") == 0 || strcmp(s, "$gp") == 0)
		return "C";
	if (strcmp(s, "sw") == 0 || strcmp(s, "$sp") == 0)
		return "D";
	if (strcmp(s, "jr") == 0 || strcmp(s, "$fp") == 0)
		return "E";
	if (strcmp(s, "halt") == 0 || strcmp(s, "$ra") == 0)
		return "F";
	//In case the input is not a known word, the function returns the input.
	return s;
}

bool IsLabelDeclaration(char *s)
{
	//Checks whether a part of the text is a label declaration.
	if (s[strlen(s) - 1] == ':')
	{
		//Removes the colon in order to save the label's name.
		s[strlen(s) - 1] = '\0';
		return true;
	}
	return false;
}

void LabelAddressToBinary(char *label, char ***labels, char *converted)
{
	//Gets the label's address by its name, and converts it to binary.
	char decPlace[5];
	int err;
	if (label[strlen(label) - 1] == '\n')
		//Cuts the \n in case the label name comes with it.
		label[strlen(label) - 1] = '\0';
	for (int i = 0; i < memorySize; i++)
		if (!strcmp(labels[i], label))
			err = _itoa_s(i, decPlace, 5, 10);
	DecimalToBinary(decPlace, converted, 16);
}

char *AddToEight(char *hex)
{
	//Adds '0' bits to a hexadecimal code's beginning if it is not 8 characters long.
	char *afterAddition[outputLineLength + 1];
	for (unsigned int i = 0; i < outputLineLength - strlen(hex); i++)
		afterAddition[i] = '0';
	afterAddition[outputLineLength - strlen(hex)] = '\0';
	strcat_s(afterAddition, outputLineLength + 1, hex);
	return afterAddition;
}

void HandleWord(char *value, char *place, char *fullCode)
{
	//Converts of the value string to hexadecimal.
	char *fullLine = (char*)malloc(sizeof(char) * 33);
	fullLine[0] = '\0';
	if (strlen(value) > 2)
		if (value[0] == '0'&&value[1] == 'x')
			if (strlen(value) < 10)
			{
				fullLine = AddToEight(value + 2);
			}
			else
				fullLine = value + 2;
	if (fullLine[0] == '\0')
	{
		DecimalToBinary(value, fullLine, 32);
		BinToHex(fullLine, value);
		fullLine = value;
	}
	//Adds the hexadecimal value to the overall output.
	if (strlen(fullCode) <= atoi(place) * outputLineLength)
	{
		for (int i = strlen(fullCode); i < atoi(place) * (outputLineLength + 1); i++)
			if (i % 9 != 8)
				fullCode[i] = '0';
			else
				fullCode[i] = '\n';
	}
	fullCode[(atoi(place)) * (outputLineLength + 1) - 1] = '\n';
	for (int i = 0; i < outputLineLength; i++)
		fullCode[atoi(place) * (outputLineLength + 1) + i] = fullLine[i];
	fullCode[(atoi(place) + 1) * (outputLineLength + 1) - 1] = '\0';
}

int main(int argc, char *argv[])
{
	if (argc != 4)
		exit(1);
	if (strcmp(argv[1], "asm"))
		exit(1);
	FILE *stream, *output;
	errno_t err = fopen_s(&stream, argv[2], "r");
	if (stream == 0) {
		fprintf_s(stderr, "Error opening program.asm");
		exit(1);
	}
	char line[maxLineLength + 1];
	char *context = NULL;
	char **labels = (char**)malloc(sizeof(char) * maxLabelLength * memorySize);
	//Allocates memory to the labels array.
	for (int i = 0; i < memorySize; i++)
		labels[i] = (char*)malloc(sizeof(char) * maxLabelLength);
	int i = 0;
	while (fgets(line, maxLineLength, stream) != NULL)
	{
		//Saves all labels inside the labels array, in the spot of the line they are in.
		RemoveComment(line);
		char *possibleLabel = strtok_s(line, " ,\n\t", &context);
		if (possibleLabel != NULL)
		{
			if (IsLabelDeclaration(possibleLabel))
			{
				strcpy_s(labels[i], maxLabelLength, possibleLabel);
				if (strtok_s(NULL, " ,\n\t", &context) != NULL)
					i++;
			}
			else
				i++;
		}
	}
	fclose(stream);
	//Reopens the input file to read from the beginning.
	err = fopen_s(&stream, argv[2], "r");
	if (stream == 0) {
		fprintf_s(stderr, "Error opening program.asm");
		exit(1);
	}
	char *outputText = (char*)malloc(sizeof(char)*(memorySize * (outputLineLength + 1)) + 1);
	outputText[0] = '\0';
	//Defines the string to which the line of code is converted.
	char *hexCode[outputLineLength + 1];
	while (fgets(line, maxLineLength, stream) != NULL)
	{
		hexCode[0] = '\0';
		//Removes comments from the line.
		RemoveComment(line);
		char *part = strtok_s(line, " ,\n\t", &context);
		if (part != NULL)
		{
			//If there is no label, the command is converted and added.
			if (!IsLabelDeclaration(part))
			{
				strcpy_s(hexCode, outputLineLength + 1, TextToHex(part));
			}
			//If the command is ".word", we skip the line we will handle it in the next reading.
			if (part[0] == '.')
				continue;
		}
		while (part != NULL)
		{
			if (!IsLabelDeclaration(part))
			{
				part = strtok_s(NULL, " ,\t\n", &context);
				if (part != NULL)
				{
					//If the part is a number, it is converted to hexadecimal and added to the overall code.
					if ((part[1] == 'x'&&part[0] == '0') || (part[0] >= '0'&&part[0] <= '9') || part[0] == '-')
					{
						strcat_s(hexCode, outputLineLength + 1, ImmToHex(part));
						part = NULL;
					}
					//If the part starts with a letter, and is not a known command, it is a label and is added as its address to the code.
					else if (((part[0] >= 'a'&&part[0] <= 'z') || (part[0] >= 'A'&&part[0] <= 'Z')) && !strcmp(part, TextToHex(part)))
					{
						char addition2[17];
						addition2[0] = '\0';
						LabelAddressToBinary(part, labels, addition2);
						addition2[16] = '\0';
						char addition3[5];
						addition3[0] = '\0';
						BinToHex(addition2, addition3);
						strcat_s(hexCode, outputLineLength + 1, addition3);
					}
					//If it is neither, it is added to the code.
					else
					{
						char *newHexCode[outputLineLength + 1];
						strcpy_s(newHexCode, outputLineLength + 1, hexCode);
						strcat_s(newHexCode, outputLineLength + 1, TextToHex(part));
						strcpy_s(hexCode, outputLineLength + 1, newHexCode);
					}
				}
			}
		}
		if (hexCode[0] != '\0'&&hexCode[0] != '\n')
		{
			strcat_s(outputText, memorySize * outputLineLength + 1, hexCode);
			strcat_s(outputText, memorySize * outputLineLength + 1, "\n");
		}
		i++;
	}
	fclose(stream);
	err = fopen_s(&stream, argv[2], "r");
	if (stream == 0) {
		fprintf_s(stderr, "Error opening program.asm");
		exit(1);
	}
	while (fgets(line, maxLineLength, stream) != NULL)
	{
		//Handles the .word command using the method and the text that comes after it in the line.
		char *part = strtok_s(line, " ,\n\t", &context);
		if (part != NULL)
			if (part[0] == '.')
				HandleWord(strtok_s(NULL, " ,\n\t", &context), strtok_s(NULL, " ,\n\t", &context), outputText);
	}
	//Prints the hexadecimal code to the output file.
	err = fopen_s(&output, "memin.txt", "w");
	if (stream == 0) {
		fprintf_s(stderr, "Error opening mem.txt");
		exit(1);
	}
	fprintf_s(output, "%s", outputText);
	fclose(stream);
	fclose(output);
	for (int i = 0; i < memorySize; i++)
		if (labels[i] != NULL)
			free(labels[i]);
	free(labels);
	free(outputText);
	return 0;
}
