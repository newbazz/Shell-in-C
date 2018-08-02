#include "global.h"

typedef struct Variable
{
	char variableName [50];
	int IntegerValue;
	char StringValue [50];
	bool isString;
	bool isInt;
} Var;
Var globalVariables[10000];

void echo(int i,char** commandTokens,char* command, char typeBitString[]);
