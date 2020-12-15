#include "libCovid.h"


//user functions(public):



//general:

char* getPathToMainFolder()
{
	char* pathTofile = (char*)calloc(MAX_LEN, sizeof(char));
	char* res = (char*)calloc(MAX_LEN, sizeof(char));
	readlink("/proc/self/exe", pathTofile, MAX_LEN);
	int sizeRes = strlen(pathTofile) - strlen(SHARED_LIB_NAME)+1;
	snprintf(res, sizeRes, "%s", pathTofile);
	free(pathTofile);
	return res;
}

char** getCountryList(char* path)
{
	FILE* countryFile = fopen(path, "r");
	int i = 0, size = GetLengthOfFile(countryFile);
	char** countryList = (char**)malloc(sizeof(char*)*size + 1);
	for (i; i < size; i++)
	{
		countryList[i] = (char*)malloc(sizeof(char)*MAX_LEN);
		fscanf(countryFile, "%s", countryList[i]);
		countryList[i][strlen(countryList[i])] = '\0';
	}
	countryList[size] = NULL;
	fclose(countryFile);
	return countryList;	
}

BOOL checkIfCountryInList(char** list, char* country)
{
	BOOL flag = FALSE;
	int i = 0;
	
	while (list[i+1])//last one is null
	{			
		if (strstr(list[i++], country))
		{
			flag = TRUE;
			break;
		}
	}
	return flag;
}

//Part 1:ListInfoDB; 
void insertInfoDBToEndList(ListInfoDB *lst, InfoDB *newTail)
{
	if (lst->head)
	{
		lst->head = newTail;
		lst->tail = newTail;
	}
	else
	{
		lst->tail->next = newTail;
		lst->tail = newTail;
	}
	newTail->next = NULL;
}

void freeInfoDBList(ListInfoDB *lst)
{
	InfoDB *current;
	InfoDB *next;
	current = lst->head;
	while (current != NULL)
	{
		next = current->next;
		freePartOne(current);
		current = next;
	}
	lst->head = NULL;
	lst->tail = NULL;
}
//Part 2: ListCountryDB;

void freeCountryDBList(ListCountryDB *lst)
{
	CountryDB *current;
	CountryDB *next;
	current = lst->head;
	while (current != NULL)
	{
		next = current->next;
		freePartTwo(current);
		current = next;
	}
	lst->head = NULL;
	lst->tail = NULL;
}

void insertCountryDBToEndList(ListCountryDB *lst, CountryDB *newTail)
{
	if (lst->head)
	{
		lst->head = newTail;
		lst->tail = newTail;
	}
	else
	{
		lst->tail->next = newTail;
		lst->tail = newTail;
	}
	newTail->next = NULL;
}




//part one:
InfoDB* setNewInfoDatabase(char* i_date, int i_key)
{
	InfoDB* database = (InfoDB*)malloc(sizeof(InfoDB));
	database->date = (char*)malloc(sizeof(char)*MAX_LEN);
	database->date= i_date;
	database->typeOfData = i_key;
	char* pathOfCountriesFile = findDircOfFile(NULL);
	int size = 0;
	database->countriesArr = initCountriesArr(pathOfCountriesFile, &size);
	database->sizeOfData = size;
	initDataBasePartOne(database, getPathToMainFolder());
	return database;
}

char* getTypeOfDataPartOne(int i_type)
{
	char* res = (char*)malloc(sizeof(char) * 10);
	switch (i_type)
	{
	case 1:
	{
		strcpy(res, CON);
		break;
	}
	case 2:
	{
		strcpy(res, DEATH);
		break;
	}
	case 3:
	{
		strcpy(res, REC);
		break;
	}
	case 4:
	{
		strcpy(res, ACT);
		break;
	}
	}
	return res;
}

void freePartOne(InfoDB* dataBase)
{
	int i;

	for (i = 0; i < (dataBase->sizeOfData); i++)
	{
		free(dataBase->countriesArr[i]->countryName);
		free(dataBase->countriesArr[i]);
	}
	free(dataBase->countriesArr);
	free(dataBase);
}


//part two:
CountryDB* setNewCountry(char* i_country, int i_key)
{
	CountryDB* country = (CountryDB*)malloc(sizeof(CountryDB));
	setName(country, i_country);
	country->typeOfData = i_key;
	char* countryFilePath = findDircOfFile(i_country);
	initDataBasePartTwo(country, countryFilePath);
	return country;
}

void freePartTwo(CountryDB* country)
{
	free(country->dataArr);
	free(country->countryName);
	free(country);
}


//part three:
CountryWD* setNewCountryWholeData(char* i_country)
{
	CountryWD* country = (CountryWD*)malloc(sizeof(CountryWD));
	if (i_country[strlen(i_country)-1] == ' ')
	{
		i_country[strlen(i_country)-1] = '\0';
	}
	country->countryName = (char*)malloc(sizeof(char)*strlen(i_country));
	strcpy(country->countryName, i_country);

	char* pathToFile = findDircOfFile(i_country);
	country->data = initDataBasePartThree(pathToFile);
	return country;
}

void freePartThree(CountryWD* i_country)
{
	free(i_country->countryName);
	free(i_country->data);
	free(i_country);
}

char* initDataBasePartThree(char* pathToFile)
{
	char* buffer = 0;
	long length;
	FILE* txtFile = fopen(pathToFile, "r");
	assert(txtFile);
	if (txtFile)
	{
		fseek(txtFile, 0, SEEK_END);
		length = ftell(txtFile);
		fseek(txtFile, 0, SEEK_SET);
		buffer = malloc(length+1);
		if (buffer)
		{
			fread(buffer, 1, length, txtFile);
		}
		fclose(txtFile);
	}
	buffer[length] = '\0';
	return buffer;

}

void createCSVFile(CountryWD* i_country)
{
	char* csvName = setCSVName(i_country->countryName);
	FILE* csvFile=fopen(csvName, "w");
	assert(csvFile);
	setHeadLineCsv(csvFile);
	setDataCsv(csvFile, i_country->data);
	fclose(csvFile);
	free(csvName);
}




//library functions(private):

//general:

char* findDircOfFile(char* i_nameOfCountry)
{
	char* res = getPathToMainFolder();
	if (!i_nameOfCountry)// looking for country list
	{
		strcat(res, "countries");
	}
	else
	{
		strcat(res, i_nameOfCountry);
	}
	return res;
}

int getDataFromString(char* str, int dataPlace, char* del)
{
	int i = 0;
	char* check = (char*)malloc(sizeof(char) * INT_MAX);
	check = strtok(str, del);
	for (i; i < dataPlace; i++)
	{
		check = strtok(NULL, del);
	}
	
	return atoi(check);
}

BOOL checkIFInputInFile(char* i_input, char* i_file)
{
	BOOL answer = FALSE;
	char temp[MAX_LEN];
	FILE* txtFile;
	txtFile = fopen(i_file, "r");
	assert(txtFile);
	while (fgets(temp, MAX_LEN, txtFile)!=NULL)
	{
		if ((strstr(temp, i_input)!=NULL))
		{
			answer = TRUE;
			break;
		}
	}
	
	fclose(txtFile);
	return answer;
}

int GetLengthOfFile(FILE* i_file)
{
	int counter = 0;
	char ch = getc(i_file);
	for (ch = getc(i_file); ch != EOF; ch = getc(i_file))
	{
		if (ch == '\n')
		{
			counter++;
		}
	}
	fseek(i_file, 0, SEEK_SET);
	return counter;
}

int getLineInFileByInput(char* i_file, char* i_data)
{
	FILE* txtFile;
	int line = 1;
	char temp[MAX_LEN];

	txtFile = fopen(i_file, "r");
	assert(txtFile);
	while (fgets(temp, MAX_LEN, txtFile) != NULL)
	{
		if ((strstr(temp, i_data)) != NULL)
		{
			break;
		}
		line++;
	}
	fclose(txtFile);
	return line;
}

void freeCountryList(char** arr)
{
	int i = 0;
	while (arr[i])
		free(arr[i++]);
	free(arr[i]);
	free(arr);
}


//part one:
int getDataFromFileByTypeAndLine(char* i_file, char* i_line, int typeOfData)
{
	int data;

	FILE* txtFile = fopen(i_file, "r");
	int line = 1;
	BOOL find = FALSE;
	char lineFromFile[MAX_LEN];
	assert(txtFile);
	
	while (fgets(lineFromFile, MAX_LEN, txtFile) != NULL)
	{
		if (strstr(lineFromFile, i_line))
		{
			find = TRUE;
			break;
			
		}
		line++;
	}
	fclose(txtFile);
	if (find)
		data = getDataFromString(lineFromFile, typeOfData, DELIMITER);
	else
		data = -1;
	return data;
}

void initDataBasePartOne(InfoDB* dataBase, char* pathToFolder)
{
	char* dirctToFile;
	int i;
	for (i = 0; i < dataBase->sizeOfData; i++)
	{
		dirctToFile = findDircOfFile(dataBase->countriesArr[i]->countryName);
		dataBase->countriesArr[i]->data = getDataFromFileByTypeAndLine(dirctToFile, dataBase->date, dataBase->typeOfData);
		free(dirctToFile);
	}
}

CountryData** initCountriesArr(char* pathOfCountriesFile, int* size)
{
	int i, tempSize;
	FILE* countries = fopen(pathOfCountriesFile, "r");
	assert(countries);
	tempSize = GetLengthOfFile(countries);
	CountryData** countryArr = (CountryData**)malloc(sizeof(CountryData*)*tempSize);
	for (i = 0; i < tempSize; i++)
	{
		countryArr[i] = (CountryData*)malloc(sizeof(CountryData));
		countryArr[i]->countryName = (char*)malloc(sizeof(char)*MAX_LEN);
		fscanf(countries, "%s", countryArr[i]->countryName);
	}
	(*size) = tempSize;
	fclose(countries);
	return countryArr;
}

//part two:
void setName(CountryDB* country, char* name)
{
	if(name[strlen(name)-1]==' ')
	{
		name[strlen(name) - 1] = '\0';
	}
	country->countryName = (char*)malloc(sizeof(char)*strlen(name));
	memcpy(country->countryName, name, strlen(name));
}

void setMaxData(CountryDB* country)
{
	//for visualing 
	int max = 0;
	int maxIndex;
	int i;
	for (i = 0; i < country->sizeOfData; i++)
	{
		if (country->dataArr[i] > max)
		{
			max = country->dataArr[i];
			maxIndex = i;
		}
	}
	country->maxDataIndex = maxIndex;
	country->maxData = max;
}

void initDataBasePartTwo(CountryDB* country, char* i_file)
{
	FILE* txtFile;
	int i = 0;
	int numberHolder;
	txtFile = fopen(i_file, "r");
	assert(txtFile);

	country->sizeOfData = GetLengthOfFile(txtFile);
	country->dataArr = (int*)malloc(sizeof(int)*country->sizeOfData);
	char* lineFromFile = (char*)malloc(sizeof(char)*MAX_LEN);
	fgets(lineFromFile, MAX_LEN, txtFile);
	while (!feof(txtFile))
	{
		numberHolder = getDataFromString(lineFromFile, country->typeOfData, DELIMITER);
		country->dataArr[i++] = numberHolder;
		fscanf(txtFile,"%s",lineFromFile);
	}

	setMaxData(country);
	fclose(txtFile);
	free(lineFromFile);
}

//part three:
void setHeadLineCsv(FILE* csvFile)
{
	fprintf(csvFile, "%s", HEADLINE);
	fprintf(csvFile, "\n");
}

void setDataCsv(FILE* csvFile,char* i_country)
{
	fwrite(i_country, sizeof(char)*strlen(i_country), 1, csvFile);
}

char* setCSVName(char* name)
{
	char* CSVName = (char*)malloc(sizeof(char)*(strlen(name) + 4));
	strcpy(CSVName, name);
	if (strstr(CSVName, " "))
	{
		CSVName[strlen(CSVName) - 1] = '\0';
	}
	strcat(CSVName, ".csv");
	return CSVName;
}

