#ifndef _HEADER_FOR_COVID_DATABASE_
#define __HEADER_FOR_COVID_DATABASE__
#define _CRT_SECURE_NO_WARNINGS

#include <limits.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <unistd.h>

//BOOL typedef:
typedef int BOOL;
#define TRUE 1
#define FALSE 0

//defines:
#define MAX_LEN 255
#define DELIMITER ","
#define SCREENSACE 180
#define COUNTRIES_LIST 'countries.txt'
#define SHARED_LIB_NAME "mainx.out"
#define HEADLINE "Date,Confirmed,Deaths,Recovered,Active"
#define CON "Confirmed"
#define DEATH "Deaths"
#define REC "Recovered"
#define ACT "Active"


//list, for future optional programing



//structs:

//data base for specific data, specific date, for all countries: (part 1)
typedef struct countryData
{
	char* countryName;
	int data;
}CountryData;

//specific country, date, and data:
typedef struct infoDB
{
	int typeOfData;
	CountryData** countriesArr;
	int sizeOfData;
	char* date;
	struct infoDB* next;
}InfoDB;

//data base for all data, one country: (part 3)
typedef struct countryWholeData
{
	char* countryName;
	char* data;
}CountryWD;



//data base for specific country, specific data, all dates: (part 2)
typedef struct countryDB
{
	char* countryName;
	int typeOfData;
	int* dataArr;
	int maxData;
	int maxDataIndex;
	int sizeOfData;
	struct countryDB* next;
} CountryDB;

//Part 1:ListInfoDB; 
typedef struct listInfoDB
{
	InfoDB* head;
	InfoDB* tail;
	int size;
}ListInfoDB;

typedef struct listCountryDB
{
	CountryDB* head;
	CountryDB* tail;
	int size;
}ListCountryDB;


void freeInfoDBList(ListInfoDB *lst);
void freeCountryDBList(ListCountryDB *lst);
void insertCountryDBToEndList(ListCountryDB *lst, CountryDB *newTail);
void insertInfoDBToEndList(ListInfoDB *lst, InfoDB *newTail);


//user functions(public):
//general:
char* getPathToMainFolder();

//part one:
InfoDB* setNewInfoDatabase(char* i_date, int i_key);
void freePartOne(InfoDB* io_dataBase);
void freePartOne(InfoDB* io_dataBase);


//part two:
CountryDB* setNewCountry(char* i_country, int i_key);
void freePartTwo(CountryDB* io_country);

//part three:
CountryWD* setNewCountryWholeData(char* i_country);
void createCSVFile(CountryWD* i_country);
void freePartThree(CountryWD* i_country);


//library functions(private):

//general:
char* findDircOfFile(char* i_nameOfCountry);
int getDataFromString(char* str, int dataPlace, char* del);
BOOL checkIFInputInFile(char* i_input, char* i_file);
int GetLengthOfFile(FILE* i_file);
int getLineInFileByInput(char* i_file, char* i_data);
int setTypeOfData(char* i_key);


//part one:
int getDataFromFileByTypeAndLine(char* i_file, char* i_line, int typeOfData);
void initDataBasePartOne(InfoDB* dataBase, char* pathToFolder);
CountryData** initCountriesArr(char* pathOfCountriesFile, int* size);
char* getTypeOfDataPartOne(int i_type);

//part two:
char** getCountryList(char* path);
BOOL checkIfCountryInList(char** list, char* country);
void freeCountryList(char** arr);
void setName(CountryDB* country, char* name);
void setMaxData(CountryDB* country);
void initDataBasePartTwo(CountryDB* country, char* i_file);

//part three:
CountryWD* setNewCountryWholeData(char* i_country);
char* initDataBasePartThree(char* pathToFile);
void setHeadLineCsv(FILE* csvFile);
void setDataCsv(FILE* csvFile, char* i_country);
char* setCSVName(char* name);

#endif
