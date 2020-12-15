#include "libCovid.h"

char* getString();
char* setNameOfCountry(char* i_country);
void printCountryList(char** list);

int main(int argc, char* argv[])
{
	if (!argv[1])// no arg to main
	{
		printf("Arguement Are missed. Please enter country name\n");
		argv[1] = getString();
	}
	char* name = setNameOfCountry(argv[1]);

	CountryWD* country = setNewCountryWholeData(name);
	createCSVFile(country);
	printf("csv file was created!\n");
	freePartThree(country);
	return 0; 
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

void printCountryList(char** list)
{
	int i = 0;
	while (list[i])//last one is null
	{
		printf("%d. %s\n", i, list[i++]);
	}
}
