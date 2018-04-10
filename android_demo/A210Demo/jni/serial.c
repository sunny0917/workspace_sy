#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/ioctl.h>
#include<pthread.h>
#include <sys/mman.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <termios.h>
#include <sys/types.h>
#include <jni.h>

#include<assert.h>

#include "debug.h"
#include "serial.h"

int tim_subtract(struct timeval *result, struct timeval *x, struct timeval *y)
{
	if ( x->tv_sec > y->tv_sec )
	{
		return	-1;
	}

	if ((x->tv_sec == y->tv_sec) && (x->tv_usec > y->tv_usec))
	{
		return	-1;
	}

	result->tv_sec = ( y->tv_sec - x->tv_sec );
	result->tv_usec = ( y->tv_usec - x->tv_usec );

	if (result->tv_usec < 0)
	{
		result->tv_sec--;
		result->tv_usec += 1000000;
	}

	return	0;
}

void time_udelay(unsigned int us)
{
	struct timeval start,stop,diff;
	int gettimes = 0;

	gettimeofday(&start,0);

	while(1)
	{
		gettimeofday(&stop,0);
		tim_subtract(&diff,&start,&stop);
		if(diff.tv_usec >= us)
		{
			return;
		}
	}
	return;
}

int getBaudrate(int baudrate) {
	switch (baudrate) {
	case 0:
		return B0;
	case 50:
		return B50;
	case 75:
		return B75;
	case 110:
		return B110;
	case 134:
		return B134;
	case 150:
		return B150;
	case 200:
		return B200;
	case 300:
		return B300;
	case 600:
		return B600;
	case 1200:
		return B1200;
	case 1800:
		return B1800;
	case 2400:
		return B2400;
	case 4800:
		return B4800;
	case 9600:
		return B9600;
	case 19200:
		return B19200;
	case 38400:
		return B38400;
	case 57600:
		return B57600;
	case 115200:
		return B115200;
	case 230400:
		return B230400;
	case 460800:
		return B460800;
	case 500000:
		return B500000;
	case 576000:
		return B576000;
	case 921600:
		return B921600;
	case 1000000:
		return B1000000;
	case 1152000:
		return B1152000;
	case 1500000:
		return B1500000;
	case 2000000:
		return B2000000;
	case 2500000:
		return B2500000;
	case 3000000:
		return B3000000;
	case 3500000:
		return B3500000;
	case 4000000:
		return B4000000;
	default:
		return ERROR;
	}
}

int setPort(int NBits, unsigned char NEvent, int NSpeed, int NStop)
 {
	struct termios newtio;
	struct termios oldtio;

	if(tcgetattr(fd,&oldtio) != 0)
	{
		LOGE("SetupSerial 1");
		return ERROR;
	}

	bzero(&newtio,sizeof(newtio));
	newtio.c_cflag |= CLOCAL |CREAD;
	newtio.c_cflag &= ~CSIZE;

	newtio.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
	newtio.c_oflag &= ~OPOST;
	newtio.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);

	switch(NBits)
	{
		case 7:
			newtio.c_cflag |= CS7;
			break;
		case 8:
			newtio.c_cflag |= CS8;
			break;
	}

	switch(NEvent)
	{
		case 'O':
			newtio.c_cflag |= PARENB;
			newtio.c_cflag |= PARODD;
			newtio.c_iflag |= (INPCK | ISTRIP);
			break;
		case 'E':
			newtio.c_iflag |= (INPCK |ISTRIP);
			newtio.c_cflag |= PARENB;
			newtio.c_cflag &= ~PARODD;
			break;
		case 'N':
			newtio.c_cflag &= ~PARENB;
			break;
	}

	switch(NSpeed)
	{
		case 2400:
			cfsetispeed(&newtio,B2400);
			cfsetospeed(&newtio,B2400);
			break;
		case 4800:
			cfsetispeed(&newtio,B4800);
			cfsetospeed(&newtio,B4800);
			break;
		case 9600:
			cfsetispeed(&newtio,B9600);
			cfsetospeed(&newtio,B9600);
			break;
		case 19200:
			cfsetispeed(&newtio,B19200);
			cfsetospeed(&newtio,B19200);
			break;
		case 115200:
			cfsetispeed(&newtio,B115200);
			cfsetospeed(&newtio,B115200);
			break;
		case 460800:
			cfsetispeed(&newtio,B460800);
			cfsetospeed(&newtio,B460800);
			break;
		default:
			cfsetispeed(&newtio,B9600);
			cfsetospeed(&newtio,B9600);
			break;
	}

	if(NStop == 1)
	{
		newtio.c_cflag &= ~CSTOPB;
	}
	else if(NStop ==2)
	{
		newtio.c_cflag |= CSTOPB;
	}
	newtio.c_cc[VTIME] = 0;
	newtio.c_cc[VMIN] = 0;

	tcflush(fd,TCIOFLUSH);
	if((tcsetattr(fd,TCSANOW,&newtio)) != 0)
	{
		LOGE("com set error");
		return ERROR;
	}
	LOGI("[%s]:set done!\n",__func__);
	return SUCCESS;
}

int openPort(const char* name,int NBits, char NEvent,int NSpeed,int NStop) {
	int i;
	//fd = open(name,O_RDWR | O_NOCTTY | O_NDELAY);
	fd = open(name,O_RDWR | O_NOCTTY);
	if(fd < 0)
	{
		LOGE("[%s]:open %s fail\n",__func__,name);
		return ERROR;
	}
	//fcntl(-1, F_SETFL, 0);//
	if(setPort(NBits,NEvent,NSpeed,NStop) < 0)
	{
		LOGE("[%s]:set_opt error\n",__func__);
		return ERROR;
	}
	LOGI("[%s]:open %s successfull fd = %d",__func__,name,fd);
	return 0;
}

int closePort() {
	if(fd < 0)
	{
		LOGE("[%s]:closePort a invall fd\n",__func__);
		return ERROR;
	}

	close(fd);
	fd = -1;
	return 0;
}

int readPort(unsigned char* readb,int maxlen)
{
	int nread = 0;
	int time = 0;
	int len = 0;
	int i=0;
	struct timeval start,stop,diff;
	int gettimes = 0;

	LOGI("[%s]:readPort maxlen = %d",__func__,maxlen);
	gettimeofday(&start,0);

	if(fd < 0)
	{
		LOGE("[%s]:tty fd is invall\n",__func__);
		return ERROR;
	}
	while(1)
	{
		gettimeofday(&stop,0);
		tim_subtract(&diff,&start,&stop);

		if(diff.tv_sec*1000000+diff.tv_usec > 2000000)
		{
			LOGE("[%s]:read timeout\n",__func__);
			return TIMEOUT;
		}

		nread = read(fd,readb+len,maxlen-len);
		if(nread < 0)
		{
			LOGE("[%s]:device is error %d:%s",__func__,errno,strerror(errno));
			return ERROR;
		}
		else
		{
			//for(i = 0;i < nread;i++)
				//LOGE("readPort buf[%d]=0x%02x",i+len,readb[i+len]);
			if(nread == (maxlen-len))
			{
				LOGI("[%s]:get all number:%d:%d:%d\n",__func__,maxlen,nread,len);
				return SUCCESS;
			}
			else if(nread == 0)
			{
				usleep(20000);
				continue;
			}
			else
			{
				len += nread;
				LOGI("[%s]:get %d left:%d:%d\n",__func__,len,maxlen-len,nread);
				continue;
			}
		}
	}

	return ERROR;
}

int writePort(char *buf,int maxlen)
{
	int sendlen = 0;
	int len = 0;
	int i = 0;

	if(fd < 0)
	{
		LOGE("[%s]:writePort a invall fd\n",__func__);
		return fd;
	}

#ifdef DEBUG
	for(i = 0;i < maxlen;i++)
		LOGI("[%s]:writePort buf[%d]=0x%02x",__func__,i,buf[i]);
#endif

	tcflush(fd,TCIOFLUSH);

	while(1)
	{
		sendlen = write(fd,buf+len,maxlen-len);
		if(sendlen < 0)
			return sendlen;

		len +=sendlen;
		if(len == maxlen){
			break;
		}
	}
	LOGI("[%s]:writePort success len=%d",__func__,len);
	return len;
}
