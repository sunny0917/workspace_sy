#include <assert.h>
#include <getopt.h>             /* getopt_long() */
#include <unistd.h>
#include <malloc.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>
#include <asm/types.h>          /* for videodev2.h */
#include <pthread.h> 
#include <A20V4l2.h>

#define CAMERA_PORTING_DEBUG
#ifdef CAMERA_PORTING_DEBUG
#define CAMERA_DEBUG IRIS_PRINTF
#else
#define CAMERA_DEBUG
#endif
//#define VIDIOC_S_CTRLV2	0xc00c561c  //VIDIOC_S_CTRL 0xc008561c 
//#define VIDIOC_G_CTRLV2	0xc00c561b
#define V4l2_ClearMemory(buff) memset (&(buff), 0, sizeof (buff))

#define V4L2_PIX_FMT_SGRBG10 v4l2_fourcc('B', 'A', '1', '0') /* 10  GRGR.. BGBG.. */

#define MAX_SHUTTER	5
#define MIN_SHUTTER	-5

#define MAX_GAIN	4 
#define MIN_GAIN	1

struct FrameSpace {
    void * start;
    size_t length;
};
struct V4L2_Handle{
	int fd;
	int frameid;
	int gain;
	int shutter;
	int hflip;
	int vflip;
    int status;
	char name[24];
    pthread_mutex_t mutex;
	struct FrameSpace* buffers;
};


static int V4l2Ioctl(int fd, int request, void * arg)
{
    return ioctl(fd, request, arg);
}


int V4l2_SetParams(void* CamHandle,int index,int value)
{
	int ret = 0;
	int ret2 = 0;
	
	struct v4l2_control ctrl = {0};
	if(CamHandle == NULL)
	{
		return V4L2_DEV_SUCCESS;
	}
	struct V4L2_Handle* handle = CamHandle;
	if(handle->fd == V4L2_DEV_FAIL)
	{
		return V4L2_DEV_SUCCESS;
	}
	switch(index)
	{
		case 0:
			if(value == handle->gain||value<1||value>8)//increase
			{
				return 2;
			}
			
			ctrl.id = V4L2_CID_GAIN;
			ctrl.value = value;
			
			ret = V4l2Ioctl(handle->fd, VIDIOC_S_CTRL, &ctrl);
			
			if (V4L2_DEV_SUCCESS != ret)
			{
				CAMERA_DEBUG(IRIS_PF_ERROR,"VIDIOC_S_CTRL 0x%x\n",ret);
				return -1;
			}
			handle->gain = value;
			break;
		case 1:
			if(value > 0)//increase
			{
				if(handle->shutter == MAX_SHUTTER)
				{
					return 2;
				}
				value = handle->shutter+1;
			}
			else//decrease
			{
				if(handle->shutter == MIN_SHUTTER)
				{
					return 2;
				}
				value = handle->shutter-1;
			}
			ctrl.id = V4L2_CID_EXPOSURE;
			ctrl.value = value;
			
			ret = V4l2Ioctl(handle->fd, VIDIOC_S_CTRL, &ctrl);
			//CAMERA_DEBUG(IRIS_PF_DEBUG,"set shutter %d\n",value);
			if (V4L2_DEV_SUCCESS != ret)
			{
				CAMERA_DEBUG(IRIS_PF_ERROR,"VIDIOC_S_CTRL 0x%x\n",ret);
				return -1;
			}
			handle->shutter = value;
			break;
		case 2:
			if(handle->hflip == 0)
			{
				value = 1;
			}
			else
			{
				value = 0;
			}
			ctrl.id = V4L2_CID_HFLIP;
			ctrl.value = value;
			
			ret = V4l2Ioctl(handle->fd, VIDIOC_S_CTRL, &ctrl);
			
			if (V4L2_DEV_SUCCESS != ret)
			{
				CAMERA_DEBUG(IRIS_PF_ERROR,"VIDIOC_S_CTRL 0x%x\n",ret);
				return -1;
			}
			handle->hflip = value;
			break;
		case 3:
			if(handle->vflip == 0)
			{
				value = 1;
			}
			else
			{
				value = 0;
			}
			ctrl.id = V4L2_CID_VFLIP;
			ctrl.value = value;
			
			ret = V4l2Ioctl(handle->fd, VIDIOC_S_CTRL, &ctrl);
			
			if (V4L2_DEV_SUCCESS != ret)
			{
				CAMERA_DEBUG(IRIS_PF_ERROR,"VIDIOC_S_CTRL 0x%x\n",ret);
				return -1;
			}
			handle->vflip = value;
			break;
		case 4:			
			ctrl.id = V4L2_CID_EXPOSURE;
			ctrl.value = value;
			
			ret = V4l2Ioctl(handle->fd, VIDIOC_S_CTRL, &ctrl);
			//CAMERA_DEBUG(IRIS_PF_DEBUG,"set shutter %d\n",value);
			if (V4L2_DEV_SUCCESS != ret)
			{
				CAMERA_DEBUG(IRIS_PF_ERROR,"VIDIOC_S_CTRL 0x%x\n",ret);
				return -1;
			}
			break;
        case 5:			
			ctrl.id = V4L2_CID_EXPOSURE;
			ctrl.value = 0;
			ret = V4l2Ioctl(handle->fd, VIDIOC_G_CTRL, &ctrl);
			CAMERA_DEBUG(IRIS_PF_DEBUG,"get shutter %d\n",ctrl.value);
			if (V4L2_DEV_SUCCESS != ret)
			{
				CAMERA_DEBUG(IRIS_PF_ERROR,"VIDIOC_G_CTRL 0x%x\n",ret);
				return -1;
			}

            ctrl.id = V4L2_CID_EXPOSURE;
			ctrl.value += value;
			
			ret = V4l2Ioctl(handle->fd, VIDIOC_S_CTRL, &ctrl);
			CAMERA_DEBUG(IRIS_PF_DEBUG,"set shutter %d\n",ctrl.value);
			if (V4L2_DEV_SUCCESS != ret)
			{
				CAMERA_DEBUG(IRIS_PF_ERROR,"VIDIOC_S_CTRL 0x%x\n",ret);
				return -1;
			}
			break;    
		default:
			break;
	}
	return 0;
}


static int V4l2_Mmap(void* CamHandle)
{
	struct v4l2_requestbuffers req;
	struct v4l2_buffer buf;
	int buffers_num = 0;
	struct V4L2_Handle* handle = CamHandle;
	V4l2_ClearMemory(req);
	req.count		 	= MMP_BUF_MAX;
	req.type 			= V4L2_BUF_TYPE_VIDEO_CAPTURE;
	req.memory 			= V4L2_MEMORY_MMAP;
	if (V4L2_DEV_SUCCESS != V4l2Ioctl(handle->fd, VIDIOC_REQBUFS, &req))
	{
		CAMERA_DEBUG(IRIS_PF_ERROR,"requit memory on %s fail\n", handle->name);
		return V4L2_DEV_MMFAIL;
	}
	if (req.count < MMP_BUF_MAX)
	{
		CAMERA_DEBUG(IRIS_PF_ERROR,"Insufficient buffer memory on %s\n", handle->name);
		return V4L2_DEV_MMFAIL;
	}
	handle->buffers = calloc(req.count, sizeof(struct FrameSpace));
	if (handle->buffers == NULL)
	{
		CAMERA_DEBUG(IRIS_PF_ERROR,"out of memoryl\n");
		return V4L2_DEV_MMFAIL;
	}
    for (buffers_num = 0; buffers_num < req.count; ++buffers_num)
	{
	        V4l2_ClearMemory(buf);
	        buf.type 		= V4L2_BUF_TYPE_VIDEO_CAPTURE;
	        buf.memory 		= V4L2_MEMORY_MMAP;
	        buf.index 		= buffers_num;
	        if (V4L2_DEV_SUCCESS != V4l2Ioctl(handle->fd, VIDIOC_QUERYBUF, &buf))
	        {
	        	CAMERA_DEBUG(IRIS_PF_ERROR,"get mmap addr fail\n");
				if(handle->buffers!=NULL){
					free(handle->buffers);
					handle->buffers = NULL;
				}
				return V4L2_DEV_MMFAIL;
	        }
	        handle->buffers[buffers_num].length = buf.length;
	        handle->buffers[buffers_num].start = mmap(NULL, buf.length,PROT_READ | PROT_WRITE,MAP_SHARED, handle->fd, buf.m.offset);
	        if (MAP_FAILED == handle->buffers[buffers_num].start)
	        {
	        	CAMERA_DEBUG(IRIS_PF_ERROR,"mmap fail\n");
				return V4L2_DEV_MMFAIL;
	        }
    }
	return V4L2_DEV_SUCCESS;
}

int V4l2_StopCapture(struct V4L2_Handle* handle)
{
	enum v4l2_buf_type type;
    if(handle->status == 0)
    {
        return V4L2_DEV_SUCCESS;
    }
    
	type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	if (V4L2_DEV_SUCCESS != V4l2Ioctl(handle->fd, VIDIOC_STREAMOFF, &type))
	{
		CAMERA_DEBUG(IRIS_PF_ERROR,"VIDIOC_STREAMON failure!\n");
		return V4L2_DEV_FAIL;
	}
    handle->status = 0;
	return V4L2_DEV_SUCCESS;
}

int V4l2_StartCapture(struct V4L2_Handle* handle)
{
	unsigned int i;
	enum v4l2_buf_type type;
    if(handle->status == 1)
    {
        return V4L2_DEV_SUCCESS;
    }
    struct v4l2_buffer buf;
	for (i = 0; i < MMP_BUF_MAX; ++i)
	{
		V4l2_ClearMemory (buf);
		buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		buf.memory = V4L2_MEMORY_MMAP;
		buf.index = i;
		if (V4L2_DEV_SUCCESS != V4l2Ioctl(handle->fd, VIDIOC_QBUF, &buf))
		{
			CAMERA_DEBUG(IRIS_PF_ERROR,"VIDIOC_QBUF failure!\n");
			return V4L2_DEV_FAIL;
		}
	}
	type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	if (V4L2_DEV_SUCCESS != V4l2Ioctl(handle->fd, VIDIOC_STREAMON, &type))
	{
		CAMERA_DEBUG(IRIS_PF_ERROR,"VIDIOC_STREAMON failure!\n");
		return V4L2_DEV_FAIL;
	}
    handle->status = 1;
	return V4L2_DEV_SUCCESS;
}

int V4l2_OpenDev(unsigned char* name,int cm, void** CamHandle, int with, int high, int rawfmt)
{
	int i;
	int ret;
	struct stat st;
	v4l2_std_id std;
	unsigned int min;
	struct v4l2_capability cap;
	struct v4l2_cropcap cropcap;
	struct v4l2_crop crop;
	struct v4l2_format fmt;
	struct v4l2_control ctrl = {0};
	struct V4L2_Handle* handle = NULL;
	handle = malloc(sizeof(struct V4L2_Handle));
	//handle = (struct V4L2_Handle*)CamHandle;
	switch(cm)
	{
		case IRISCAMERA:
				handle->shutter 	= 4;
				handle->gain		= 2;
				handle->hflip		= 0;
				handle->vflip		= 0;
			break;
		case FACECAMERA:
				handle->shutter 	= 4;
				handle->gain		= 2;
				handle->hflip		= 0;
				handle->vflip		= 0;
			break;
		case USBCAMERA:
			break;
		default:
			break;
	}
	*CamHandle = handle;

	handle->fd = open(name, O_RDWR, 0);
	if (V4L2_DEV_FAIL == handle->fd)
	{
	        if(*CamHandle!=NULL){
        		free(*CamHandle);
        		*CamHandle = NULL;
               }
		CAMERA_DEBUG(IRIS_PF_ERROR,"Open camera failure!\n");
		return V4L2_DEV_NOFIX;
	}
    pthread_mutex_init(&handle->mutex,NULL);
	if (V4L2_DEV_SUCCESS != V4l2Ioctl(handle->fd, VIDIOC_QUERYCAP, &cap))
	{
		CAMERA_DEBUG(IRIS_PF_ERROR,"%s is no V4L2 device\n", handle->name);
		close(handle->fd);
        pthread_mutex_destroy(&handle->mutex);
		if(*CamHandle!=NULL){
        		free(*CamHandle);
        		*CamHandle = NULL;
               }
		return V4L2_DEV_NOFIX;
	}

	if (!(cap.capabilities & V4L2_CAP_VIDEO_CAPTURE))
	{
		CAMERA_DEBUG(IRIS_PF_ERROR,"%s is no video capture device\n", handle->name);
		close(handle->fd);
		handle->fd = V4L2_DEV_FAIL;
        pthread_mutex_destroy(&handle->mutex);
		if(*CamHandle!=NULL){
        		free(*CamHandle);
        		*CamHandle = NULL;
               }
		return V4L2_DEV_FAIL;
	}
	if (!(cap.capabilities & V4L2_CAP_STREAMING))
	{
		CAMERA_DEBUG(IRIS_PF_ERROR,"%s does not support streaming i/o\n", handle->name);
		close(handle->fd);
		handle->fd = V4L2_DEV_FAIL;
        pthread_mutex_destroy(&handle->mutex);
		if(*CamHandle!=NULL){
        		free(*CamHandle);
        		*CamHandle = NULL;
               }
		return V4L2_DEV_FAIL;
	}
	CAMERA_DEBUG(IRIS_PF_INFO,"%s set input\n", handle->name);
	int index;
	if (V4l2Ioctl(handle->fd, VIDIOC_G_INPUT, &index) < 0)
	{
		CAMERA_DEBUG(IRIS_PF_ERROR,"VIDIOC_G_INPUT 0 error!\n");
		close(handle->fd);
		handle->fd = V4L2_DEV_FAIL;
        pthread_mutex_destroy(&handle->mutex);
		if(*CamHandle!=NULL){
        		free(*CamHandle);
        		*CamHandle = NULL;
               }
		return V4L2_DEV_FAIL;
	}
	else {
		CAMERA_DEBUG(IRIS_PF_ERROR,"VIDIOC_G_INPUT %d success!\n", index);
	}
	index = 0;
	if (V4L2_DEV_SUCCESS != V4l2Ioctl(handle->fd, VIDIOC_S_INPUT, &index))
	{
		//CAMERA_DEBUG(IRIS_PF_ERROR,"%s set input 0 fail\n", handle->name);
			//close(handle->fd);
			//handle->fd = V4L2_DEV_FAIL;
			//free(CamHandle);
			//CamHandle = NULL;
			//return V4L2_DEV_FAIL;
	}

	//////not all capture support crop!!!!!!!
	CAMERA_DEBUG(IRIS_PF_DEBUG,"-#-#-#-#-#-#-#-#-#-#-#-#-#-set cropcap\n");
	V4l2_ClearMemory (fmt);
	fmt.type 					= V4L2_BUF_TYPE_VIDEO_CAPTURE;
	fmt.fmt.pix.width 			= with;
	fmt.fmt.pix.height 			= high;
	switch(rawfmt)
	{
		case 1:
			fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_YUYV;
			break;
		case 2:
			fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_SBGGR8;
			break;
        case 3:
            fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_SGRBG10;
            break;
		default:
//			fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_SBGGR8;
			break;
	}
	fmt.fmt.pix.field 			= V4L2_FIELD_NONE;
	CAMERA_DEBUG(IRIS_PF_DEBUG,"-#-#-#-#-#-#-#-#-#-#-#-#-#-\n");
	CAMERA_DEBUG(IRIS_PF_DEBUG,"=====will set fmt to (%d, %d)--", fmt.fmt.pix.width, fmt.fmt.pix.height);

	ret = V4l2Ioctl(handle->fd, VIDIOC_S_FMT, &fmt);
	if (V4L2_DEV_SUCCESS != ret)
	{
		CAMERA_DEBUG(IRIS_PF_ERROR,"VIDIOC_S_FMT 0x%x\n",ret);
		close(handle->fd);
		handle->fd = V4L2_DEV_FAIL;
        pthread_mutex_destroy(&handle->mutex);
		if(*CamHandle!=NULL){
        		free(*CamHandle);
        		*CamHandle = NULL;
               }
		return V4L2_DEV_FAIL;
	}

	if(V4l2_Mmap(handle) != V4L2_DEV_SUCCESS)
	{
		for (i = 0; i < MMP_BUF_MAX; ++i)
		{
			if (V4L2_DEV_FAIL== munmap(handle->buffers[i].start, handle->buffers[i].length))
			{
				CAMERA_DEBUG(IRIS_PF_ERROR,"free map failure!\n");
			}
		}
        if(handle->buffers!=NULL){
            free(handle->buffers);
			handle->buffers = NULL;
        }
		close(handle->fd);
		handle->fd = V4L2_DEV_FAIL;
        pthread_mutex_destroy(&handle->mutex);
		if(*CamHandle!=NULL){
    		free(*CamHandle);
    		*CamHandle = NULL;
           }
		return V4L2_DEV_MMFAIL;
	}
	if(V4l2_StartCapture(handle) != V4L2_DEV_SUCCESS)
	{
		for (i = 0; i < MMP_BUF_MAX; ++i)
		{
			if (V4L2_DEV_FAIL== munmap(handle->buffers[i].start, handle->buffers[i].length))
			{
				CAMERA_DEBUG(IRIS_PF_ERROR,"free map failure!\n");
			}
		}
		 if(handle->buffers!=NULL){
            free(handle->buffers);
        }
		close(handle->fd);
        pthread_mutex_destroy(&handle->mutex);
		handle->fd = V4L2_DEV_FAIL;
		if(*CamHandle!=NULL){
        		free(*CamHandle);
        		*CamHandle = NULL;
               }
		return V4L2_DEV_STREAMONFAIL;
	}
	return V4L2_DEV_SUCCESS;
}

int V4l2_CloseDev(void* CamHandle)
{
	unsigned int i;
	if(CamHandle == NULL)
	{
		return V4L2_DEV_SUCCESS;
	}
	struct V4L2_Handle* handle = CamHandle;
	if(handle->fd == V4L2_DEV_FAIL)
	{
		return V4L2_DEV_SUCCESS;
	}
	V4l2_StopCapture(handle);
	for (i = 0; i < MMP_BUF_MAX; ++i)
	{
		if (V4L2_DEV_FAIL== munmap(handle->buffers[i].start, handle->buffers[i].length))
		{
			CAMERA_DEBUG(IRIS_PF_ERROR,"free map failure!\n");
		}
	}
	if(handle->buffers!=NULL){
            free(handle->buffers);
			handle->buffers = NULL;
        }
	close(handle->fd);
    pthread_mutex_destroy(&handle->mutex);
	if(CamHandle!=NULL){
    		free(CamHandle);
    		CamHandle = NULL;
        }
	return V4L2_DEV_SUCCESS;
}

int V4l2_ReadFrame(void* CamHandle,void** buff,int* lenght)
{
	struct v4l2_buffer buf;
	struct V4L2_Handle* handle = CamHandle;
    pthread_mutex_lock(&handle->mutex);
    handle->frameid = 0xff;
	if(handle->fd == V4L2_DEV_FAIL)
	{
        pthread_mutex_unlock(&handle->mutex);
		return V4L2_DEV_READERROR;
	}
    if(handle->status == 0)
    {
        pthread_mutex_unlock(&handle->mutex);
        return -2;
    }
	V4l2_ClearMemory (buf);
	buf.type 			= V4L2_BUF_TYPE_VIDEO_CAPTURE;
	buf.memory 			= V4L2_MEMORY_MMAP;
	if (V4L2_DEV_FAIL== V4l2Ioctl(handle->fd, VIDIOC_DQBUF, &buf))
	{
		//perror("de-queue buffers failed");
		//CAMERA_DEBUG(IRIS_PF_ERROR,"VIDIOC_DQBUF FAIL!\n");
        pthread_mutex_unlock(&handle->mutex);
		return V4L2_DEV_READERROR;
	}
	handle->frameid= buf.index;
	*buff = handle->buffers[handle->frameid].start;
	*lenght =  handle->buffers[handle->frameid].length;
	//CAMERA_DEBUG(IRIS_PF_INFO,"get frame %d\n",*lenght);
    pthread_mutex_unlock(&handle->mutex);
	return V4L2_DEV_SUCCESS;
}

int V4l2_FinishFrame(void* CamHandle)
{
	struct v4l2_buffer buf;
	struct V4L2_Handle* handle = CamHandle;
    pthread_mutex_lock(&handle->mutex);
    if(handle->frameid == 0xff)
    {
        pthread_mutex_unlock(&handle->mutex);
        return V4L2_DEV_READFINISHFAIL;
    }
	V4l2_ClearMemory (buf);
	buf.type 			= V4L2_BUF_TYPE_VIDEO_CAPTURE;
	buf.memory 			= V4L2_MEMORY_MMAP;
	buf.index = handle->frameid;
	if (V4L2_DEV_FAIL == V4l2Ioctl(handle->fd, VIDIOC_QBUF, &buf))
	{
		//CAMERA_DEBUG(IRIS_PF_ERROR,"VIDIOC_QBUF FAIL!\n");
        pthread_mutex_unlock(&handle->mutex);
		return V4L2_DEV_READFINISHFAIL;
	}
	//CAMERA_DEBUG(IRIS_PF_INFO,"VIDIOC_QBUF SUCCESS!\n");
    pthread_mutex_unlock(&handle->mutex);
	return V4L2_DEV_SUCCESS;

}

int 
covertoraw(unsigned char* src,unsigned char* dst,int w,int h,int stride)
{

	unsigned char *lineBuf;
	int hi,wi;
	int si = w/4;
	int li = w%4;
	unsigned char byte0; 
	unsigned char byte1; 
	unsigned char byte2; 
	unsigned char byte3; 
	unsigned char byte4; 
	unsigned char byte5; 
	for(hi=0;hi<h;hi++)
    {
        lineBuf = src + hi * stride;
        for(wi=0;wi<si;wi++) //(byte0 + ((byte1 & 0x3) << 8)
        {
            byte0 = *(lineBuf++); 
            byte1 = *(lineBuf++);
            byte2 = *(lineBuf++);
            byte3 = *(lineBuf++);
            byte4 = *(lineBuf++); 
            *(dst++) = (byte0 +((byte1 & 0x3) << 8))>>2; 
            *(dst++) = (((byte1 & 0xFC) >> 2)+(((byte2 & 0xf) << 6)))>>2; 
            *(dst++) = (((byte2 & 0xf0) >> 4)+((byte3 & 0x3f) << 4))>>2; 
            *(dst++) = (((byte3 & 0xc0) >> 6)+(byte4 << 2))>>2; 
        }
        if (li != 0) 
        {
            switch(li)
            {
                case 1:
                    byte0 = *(lineBuf++); 
                    byte1 = *(lineBuf++);
                    *(dst++) = (byte0 +((byte1 & 0x3) << 8))>>2; 
                     break;
                case 2:
                    byte0 = *(lineBuf++); 
                    byte1 = *(lineBuf++);
                    byte2 = *(lineBuf++);
                    *(dst++) = (byte0 +((byte1 & 0x3) << 8))>>2; 
                    *(dst++) = ((byte1 & 0xFC) >> 2)+((byte2 & 0xf) << 6)>>2;  
                    break;
                case 3:
                    byte0 = *(lineBuf++); 
                    byte1 = *(lineBuf++);
                    byte2 = *(lineBuf++);
                    byte3 = *(lineBuf++);
                    *(dst++) = (byte0 +((byte1 & 0x3) << 8))>>2; 
                    *(dst++) = (((byte1 & 0xFC) >> 2)+((byte2 & 0xf) << 6))>>2; 
                    *(dst++) = (((byte2 & 0xf0) >> 4)+((byte3 & 0x3f) << 4))>>2; 
                    break;
            }
        }
                   
    }
	return 0;
}

int 
cover10toraw8(unsigned char* src,unsigned char* dst,int w,int h,int stride)
{

	unsigned char *lineBuf;
	int hi,wi;
	int si = w/4;
	int li = w%4;
	unsigned char byte0; 
	unsigned char byte1; 
	unsigned char byte2; 
	unsigned char byte3; 
	unsigned char byte4; 
	unsigned char byte5; 
	unsigned char byte6; 
	unsigned char byte7; 
	for(hi=h-1;hi>=0;hi--)
    {
        lineBuf = src + hi * stride;
        for(wi=0;wi<si;wi++)
        {
            byte0 = *(lineBuf++); 
            byte1 = *(lineBuf++);
            byte2 = *(lineBuf++);
            byte3 = *(lineBuf++);
            byte4 = *(lineBuf++); 
            byte5 = *(lineBuf++);
            byte6 = *(lineBuf++);
            byte7 = *(lineBuf++);
            *(dst++) = ((byte0>>2)&0x3F) + ((byte1<<6) & 0xC0);
            *(dst++) = ((byte2>>2)&0x3F) + ((byte3<<6) & 0xC0);
            *(dst++) = ((byte4>>2)&0x3F) + ((byte5<<6) & 0xC0);
            *(dst++) = ((byte6>>2)&0x3F) + ((byte7<<6) & 0xC0);
        }
        switch(li)
        {
            case 1:
                byte0 = *(lineBuf++); 
                byte1 = *(lineBuf++);
                ((byte0>>2)&0x3F) + ((byte1<<6) & 0xC0);
                 break;
            case 2:
                byte0 = *(lineBuf++); 
                byte1 = *(lineBuf++);
                byte2 = *(lineBuf++); 
                byte3 = *(lineBuf++);
                *(dst++) = ((byte0>>2)&0x3F) + ((byte1<<6) & 0xC0);
                *(dst++) = ((byte2>>2)&0x3F) + ((byte3<<6) & 0xC0);
                break;
            case 3:
                byte0 = *(lineBuf++); 
                byte1 = *(lineBuf++);
                byte2 = *(lineBuf++); 
                byte3 = *(lineBuf++);
                byte4 = *(lineBuf++); 
                byte5 = *(lineBuf++);
                *(dst++) = ((byte0>>2)&0x3F) + ((byte1<<6) & 0xC0);
                *(dst++) = ((byte2>>2)&0x3F) + ((byte3<<6) & 0xC0);
                *(dst++) = ((byte4>>2)&0x3F) + ((byte5<<6) & 0xC0);
                break;
            default:
                break;
        }                   
    }
	return 0;
}

int
cover10toraw8_2(unsigned char* src,unsigned char* dst,int w,int h,int stride)
{

	unsigned char *lineBuf;
	int hi,wi;
	unsigned char byte0;
	for(hi=h*2 - 2;hi>=0;hi-=2)
    {
        lineBuf = src + hi * stride;

        for(wi = 0;wi < w;wi++)
        {
        	*(dst++) = *(lineBuf++);
        }
    }
	return 0;
}

int SaveFile(char* filename,unsigned char* bmp,int w ,int h,int position)
{
   int i = 0;
   char color = 0;
   char end[2] = {0,0};
   char patte[1024] = {0};
   int pos = 0;
   char file[400] = {0};
   time_t   now;
   struct timeval	tv;
   struct   tm     *timenow;
   FILE *outfile;
   char heard[54] = {0x42,0x4d,0x30,0x0C,0x01,0,0,0,0,0,0x36,4,0,0,0x28,0,\
                 0,0,0xF5,0,0,0,0x46,0,0,0,0x01,0,8,0,0,0,\
                 0,0,0xF8,0x0b,0x01,0,0x12,0x0b,0,0,0x12,0x0b,0,0,0,0,\
                 0,0,0,0,0,0};
   int size = w*h;
   int allsize = size +1080;

   heard[2] = allsize&0xFF;
   heard[3] = (allsize>>8)&0xFF;
   heard[4] = (allsize>>16)&0xFF;
   heard[5] = (allsize>>24)&0xFF;

   heard[18] = w&0xFF;
   heard[19] = (w>>8)&0xFF;
   heard[20] = (w>>16)&0xFF;
   heard[21] = (w>>24)&0xFF;

   heard[22] = h&0xFF;
   heard[23] = (h>>8)&0xFF;
   heard[24] = (h>>16)&0xFF;
   heard[25] = (h>>24)&0xFF;

   allsize -=1078;
   heard[34] = allsize&0xFF;
   heard[35] = (allsize>>8)&0xFF;
   heard[36] = (allsize>>16)&0xFF;
   heard[37] = (allsize>>24)&0xFF;

   for(i=0;i<1024;i+=4)
   {
       patte[pos++] = color;
       patte[pos++] = color;
       patte[pos++] = color;
       patte[pos++] = 0;
       color++;
   }
   time(&now);
   timenow = localtime(&now);
   gettimeofday(&tv, NULL);
   sprintf(file,"%s/%d__%04d-%02d-%02d_%02d-%02d-%02d_%06d.raw",filename,position,timenow->tm_year+1900,timenow->tm_mon+1,timenow->tm_mday,timenow->tm_hour,timenow->tm_min,timenow->tm_sec,tv.tv_usec);
   outfile = fopen( file, "w" );
   if(outfile!=NULL){
       fwrite(bmp,size,1,outfile);
       fclose(outfile);
   }
}

int GetPrivewFrame(void* CamHandle,unsigned char* yuv,int CamFmt)
{
    unsigned char* Frame;
    int LenL;
	int in = 0;
    int timout = 0;
    static int tmd = 0;
	if(CamHandle == NULL)
	{
		return -1;
	}
    while(timout < 4)
	{
        if(V4l2_ReadFrame(CamHandle,(void**)&Frame,&LenL) == V4L2_DEV_SUCCESS)
        {
			
			switch(CamFmt)
			{
				case 1:
					memcpy(yuv,Frame,LenL);
					break;
				case 2:
					 memcpy(yuv,Frame,LenL);
					break;
                case 3:
                    CAMERA_DEBUG(IRIS_PF_INFO,"get frame %d!\n",LenL);
                    //cover10toraw8(Frame,yuv,1920,1080,3840);
                    cover10toraw8_2(Frame,yuv,1920,1080,1920);
                    tmd++;
                    if(tmd == 50)
                    {
                        //SaveFile("/sdcard/sykean",Frame,3840 ,1080,3);
                        SaveFile("/sdcard/sykean",Frame,3840 ,1080,3);
                    }
                    break;
			}
            V4l2_FinishFrame(CamHandle);
            return 0;
        }
        else
        {
            timout++;
            usleep(20000);
            continue;
        }
    }
    return -1;
}

int ClearAllFrame(void* CamHandle)
{
	unsigned char* Frame;
	int Lenght;
	int i = 0;
	if(CamHandle == NULL)
	{
		return 0;
	}
	for(i =0;i<MMP_BUF_MAX;)
	{
		if(V4l2_ReadFrame(CamHandle,(void**)&Frame,&Lenght) == V4L2_DEV_SUCCESS)
		{
			V4l2_FinishFrame(CamHandle);
			i++;
		}
		else
		{
			usleep(30000);
		}
	}
 	return 0;
}




