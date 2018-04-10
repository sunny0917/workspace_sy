#ifndef __SERIAL_UTILS_H
#define __SERIAL_UTILS_H

static int fd = -1;

int setPort(int NBits, unsigned char NEvent, int NSpeed, int NStop);
int openPort(const char* name,int NBits, char NEvent,int NSpeed,int NStop);
int closePort();
int readPort(unsigned char* readb,int maxlen);
int writePort(char *buf,int maxlen);
#endif
