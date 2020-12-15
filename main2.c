#include "libCovid.h"


void printTypeOfDataPartTwo(CountryDB* i_country);
void printDataBaseOneCountryOneData(CountryDB* i_country);
char* getString();
char* setNameOfCountry(char* i_country);
int setTypeOfData(char* i_key);
void printCountryList(char** list);

int main(int argc, char* argv[])
{
	if (!argv[1] || !argv[2])// no arg to main
	{
		printf("Arguement Are missed. Please enter country name \n");
		argv[1] = getString();
		printf("Please enter type of data(by word)- ");
		printf("1. Confirmed\n2. Death\n3. Recovered\n4. Active\n");
		argv[2] = getString();
	}
	char* name = setNameOfCountry(argv[1]);
	int typeNumb=setTypeOfData(argv[2]);

	CountryDB* country = setNewCountry(name,typeNumb);
	printDataBaseOneCountryOneData(country);
	freePartTwo(country);
	return 0;
}

void printTypeOfDataPartTwo(CountryDB* country)
{
	switch (country->typeOfData)
	{
	case 1:
	{
		printf("number of confirmed cases, in %s:", country->countryName);
		break;
	}
	case 2:
	{
		printf("number of death cases, in %s:", country->countryName);
		break;
	}
	case 3:
	{
		printf("number of recovered cases, in %s:", country->countryName);
		break;
	}
	case 4:
	{
		printf("number of active cases, in %s:", country->countryName);
		break;
	}
	}
}

void printDataBaseOneCountryOneData(CountryDB* country)
{
	printTypeOfDataPartTwo(country);
	printf("\n\n");
	int* spacesArr = (int*)malloc(sizeof(int)*country->sizeOfData);
	int spaceNumber;
	int present;
	int i, j;
	spacesArr[country->maxDataIndex] = SCREENSACE;//Depends on the screen resolution. Changeable
	for (i = 0; i < (country->sizeOfData); i++)
	{
		present = country->dataArr[i] * 100 / country->maxData;
		spaceNumber = SCREENSACE * present / 100;
		for (j = 0; j < spaceNumber; j++)
		{
			printf("-");
		}
		printf("%d", country->dataArr[i]);
		printf("\n");
	}
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

char* setNameOfCountry(char* i_country)
{
	char* name = i_country;
	char* pathOfCountriesFile = findDircOfFile(NULL);
	char** countryList = getCountryList(pathOfCountriesFile);
	while (!checkIfCountryInList(countryList, name))
	{
		printf("Country not in the list. Please try again.\n");
		printf("If you not sure, you can enter 'help me' to see country list\nCountry name: ");
		name = getString();
		if (!strcmp(name, "help me"))
		{
			printCountryList(countryList);
			printf("\nPlease enter country name:");
			name = getString();
		}
	}

	freeCountryList(countryList);
	return name;
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

void printCountryList(char** list)
{
	int i = 0;
	while (list[i])//last one is null
	{
		printf("%d. %s\n", i, list[i++]);
	}
}
