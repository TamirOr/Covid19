#include "libCovid.h"


void printCountriesOneDate(InfoDB* i_dataBase);
char* getString();
int setTypeOfData(char* i_key);

int main(int argc, char* argv[])
{ 
	if (!argv[1]|| !argv[2])// no arg to main
	{
		printf("Arguement Are missed. Please enter date 'yyyy-mm-dd' \n");
		argv[1] = getString();
		printf("Please enter type of data(by word)-\n");
		printf("1. Confirmed\n2. Death\n3. Recovered\n4. Active\n");
		argv[2] = getString();
	}

	int typeNumb = setTypeOfData(argv[2]);

	InfoDB* database = setNewInfoDatabase(argv[1], typeNumb);
	printCountriesOneDate(database);
	freePartOne(database);
	return 0;
}


void printCountriesOneDate(InfoDB* dataBase)
{
	int i;
	char* type = getTypeOfDataPartOne(dataBase->typeOfData);
	printf("All %s on %s in all countries:\n\n", type, dataBase->date);
	for (i = 0; i < (dataBase->sizeOfData); i++)
	{
		if (dataBase->countriesArr[i]->data != -1)
			printf("%s: %d\n", dataBase->countriesArr[i]->countryName, dataBase->countriesArr[i]->data);
	}
	free(type);
}

char* getString()
{
	char* str;
	int logSize = 0;
	char c;

	str = (char*)malloc(sizeof(char)*MAX_LEN);
	c = getchar();

	while (c != '\n')
	{
		str[logSize] = c;
		logSize++;
		c = getchar();
	}

	str = (char*)realloc(str, sizeof(char)*(logSize + 1));
	str[logSize] = '\0';

	return str;
}

int setTypeOfData(char* i_key)
{
	int res;
	if (strstr(i_key, CON))
		res = 1;
	else if (strstr(i_key, DEATH))
		res = 2;
	else if (strstr(i_key, REC))
		res = 3;
	else if (strstr(i_key, ACT))
		res = 4;
	else
		res = 5;
	while (res > 4 || res < 1)
	{
		printf("Invalid key input\n\n");
		printf("1. Confirmed\n2. Death\n3. Recovered\n4. Active\n");
		printf("Please enter number between 1-4\n");
		scanf("%d", &res);
	}


	return res;
}


