#ifndef HYIDCARD_H
#define HYIDCARD_H

struct idtext
{
	char name[31];
	char sex[3];
	char nation[5];
	char startdate[17];
	char addr[71];
	char number[37];
	char police[32];
	char enddate[33];
	char date[16];
	char other[37];
};

int IDCard_Init(char *ttyS, int baud);


int IDCard_Rlease();

int IDCard_GetLastError(char *szMessage);


int IDCard_get();

struct idtext* IDCard_getText();
char* IDCard_getPhoto();
#endif // HYIDCARD_H

