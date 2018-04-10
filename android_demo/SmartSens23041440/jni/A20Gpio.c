#include <A20V4l2.h>
//#define GPIO_PORTING_DEBUG
#ifdef GPIO_PORTING_DEBUG
#define GPIO_DEBUG IRIS_PRINTF
#else
#define GPIO_DEBUG
#endif

void LedSetValue(const char* name,int value)
{
	char Low[] = "0";
	char Hig[] = "1";
	int gpio_fd=-1;
	gpio_fd = open(name,O_RDWR);
	GPIO_DEBUG(IRIS_PF_INFO,"set %s to %d\n",name,value);
	if(gpio_fd != -1)
	{
		if(value)
		{
			write(gpio_fd,Hig,strlen(Hig));
		}
		else
		{
			write(gpio_fd,Low,strlen(Low));
		}
		close(gpio_fd);
	}
	else
	{
		GPIO_DEBUG(IRIS_PF_INFO,"open %s fail\n",name);
	}
	return;
}
int OpenIndraredLight(int mode,int value)
 {
	 int index = mode;
	switch(index)
	{
		case RIGHTLIGHT:
			LedSetValue(RIGHTDEV,LIGHTON);
			break;
		case LEFTLIGHT:
			LedSetValue(LEFTDEV,LIGHTON);
			break;
		default:
			break;
	}
	 return 0;
 }

 int CloseIndraredLight(int mode,int value)
 {
	 int index = mode;
	switch(index)
	{
		case RIGHTLIGHT:
			LedSetValue(RIGHTDEV,LIGHTOFF);
			break;
		case LEFTLIGHT:
			LedSetValue(LEFTDEV,LIGHTOFF);
			break;
		default:
			break;
	}
	return 0;
 }

 int OpenIndicatLight(int mode)
 {
	switch(mode)
	{
		case REDLIGHT:
			LedSetValue(GREENDEV,LIGHTOFF);
			LedSetValue(BLUEDEV,LIGHTOFF);
			LedSetValue(REDDEV,LIGHTON);
			break;
		case GREENLIGHT:
			LedSetValue(BLUEDEV,LIGHTOFF);
			LedSetValue(REDDEV,LIGHTOFF);
			LedSetValue(GREENDEV,LIGHTON);
			break;
		case BLUELIGHT:
			LedSetValue(REDDEV,LIGHTOFF);
			LedSetValue(GREENDEV,LIGHTOFF);
			LedSetValue(BLUEDEV,LIGHTON);
			break;
		default:
			break;
	}
	return 0;
 }

 int CloseIndicatLight(int mode)
 {
	switch(mode)
	{
		case REDLIGHT:
			LedSetValue(REDDEV,LIGHTOFF);
			break;
		case GREENLIGHT:
			LedSetValue(GREENDEV,LIGHTOFF);
			break;
		case BLUELIGHT:
			LedSetValue(BLUEDEV,LIGHTOFF);
			break;
		default:
			break;
	}
	return 0;
 }

