#include <jni.h>
#include <sys/select.h>
#include <sys/time.h>
#include <dlfcn.h>

#include <stdio.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <linux/videodev2.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <stdarg.h>
#include <fcntl.h>

#include <time.h>
#include <math.h>
#include <dirent.h>
#include <pthread.h>
#include <android/log.h>
#include <android/bitmap.h>

#define IRIS_OFFSET 0
#define CAMERA_FRAME_WITH   (1920)
#define CAMERA_FRAME_HIGH   (1080)
#define ERROR			-2
#define IRIS_TOP		159						//border-top
#define IRIS_BOM		CAMERA_FRAME_HIGH-IRIS_TOP			//border-top
#define IRIS_LEFT		IRIS_OFFSET+160			//border-bottom
#define IRIS_RIGHT		IRIS_OFFSET+CAMERA_FRAME_WITH-160	//border-right
#define IRIS_MIDDLE		IRIS_OFFSET+960			//border-mid

static int offset_of 		= IRIS_OFFSET;
static int left_of 	 	    = IRIS_LEFT;
static int right_of  		= IRIS_RIGHT;
static int middle_l_of 	    = IRIS_MIDDLE-20;
static int middle_r_of 	    = IRIS_MIDDLE+20;

#define u8 unsigned char
#define  LOGD(...)  do {printf(__VA_ARGS__);printf("\n");} while (0)
#define DBG(fmt, args...) LOGD("%s:%d, " fmt, __FUNCTION__, __LINE__, ##args);
#define ASSERT(b) \
do \
{ \
    if (!(b)) \
    { \
        LOGD("error on %s:%d", __FUNCTION__, __LINE__); \
        return 0; \
    } \
} while (0)

/*  Four-character-code (FOURCC) */
//#define v4l2_fourcc(a, b, c, d)\
//    ((__u32)(a) | ((__u32)(b) << 8) | ((__u32)(c) << 16) | ((__u32)(d) << 24))

#define V4L2_PIX_FMT_SGRBG10 v4l2_fourcc('B', 'A', '1', '0') /* 10  GRGR.. BGBG.. */

#define VIDEO_DEVICE "/dev/video1"
#define IMAGE_WIDTH 1280
#define IMAGE_HEIGHT 720
#define IMAGE_SIZE (IMAGE_WIDTH * IMAGE_HEIGHT * 2)
#define BUFFER_COUNT 5

int cam_fd = -1;
struct v4l2_buffer video_buffer[BUFFER_COUNT];
u8* video_buffer_ptr[BUFFER_COUNT];
u8 buf[IMAGE_SIZE];
u8 raw8buf[IMAGE_SIZE/2];

int raw10toraw8(u8 *pSrc,u8 *pDst,int w,int h)
{
        int i,j;
        u8 *p = NULL;
        u8 byte0,byte1;
        for(i = 0;i < h;i++)
        {
                p = pSrc + i * w * 2;
                for(j = 0;j < w;j++)
                {
                        byte0 = *(p++);
                        byte1 = *(p++);
                        *(pDst++) = (byte0 + ((byte1 & 0x03) << 8)) >> 2;
                }
        }
        return 0;
}

int saveRawToBmp(uint8_t *pBuf, int32_t width, int32_t height, const char *pFileName)
{
    char heard[54] = { \
            0x42,0x4d,0x30,0x0C,0x01,0x00,0x00,0x00,0x00,0x00, \
            0x36,0x04,0x00,0x00,0x28,0x00,0x00,0x00,0xF5,0x00, \
            0x00,0x00,0x46,0x00,0x00,0x00,0x01,0x00,0x08,0x00, \
            0x00,0x00,0x00,0x00,0xF8,0x0b,0x01,0x00,0x12,0x0b, \
            0x00,0x00,0x12,0x0b,0x00,0x00,0x00,0x00,0x00,0x00, \
            0x00,0x00,0x00,0x00 };
    char patte[1024] = {0};
    char end[2] = {0, 0};
    char color = 0;
    int32_t w = width;
    int32_t h = height;
    int32_t size = w * h;
    int32_t allsize = size + 1080;
    int32_t pos = 0;
    int32_t idx = 0;
    int32_t ret = 0;
    FILE *pFp = NULL;

    do
    {
        if ((NULL == pBuf) || (NULL == pFileName))
        {
            ret = -1;
            break;
        }
        if ((width < 0) || (height < 0))
        {
            ret = -2;
            break;
        }

        heard[2] = allsize & 0xFF;
        heard[3] = (allsize >> 8) & 0xFF;
        heard[4] = (allsize >> 16) & 0xFF;
        heard[5] = (allsize >> 24) & 0xFF;

        heard[18] = w & 0xFF;
        heard[19] = (w >> 8) & 0xFF;
        heard[20] = (w >> 16) & 0xFF;
        heard[21] = (w >> 24) & 0xFF;

        heard[22] = h & 0xFF;
        heard[23] = (h >> 8) & 0xFF;
        heard[24] = (h >> 16)  & 0xFF;
        heard[25] = (h >> 24) & 0xFF;

        allsize -= 1078;
        heard[34] = allsize & 0xFF;
        heard[35] = (allsize >> 8) & 0xFF;
        heard[36] = (allsize >>  16) & 0xFF;
        heard[37] = (allsize >> 24) &  0xFF;

        for (idx = 0; idx < 1024; idx += 4)
        {
            patte[pos++] = color;
            patte[pos++] = color;
            patte[pos++] = color;
            patte[pos++] = 0;
            color++;
        }

        //====== Write raw data ======
        pFp = fopen(pFileName, "wb");
        if (NULL == pFp)
        {
            ret = -3;
            break;
        }

        fwrite(heard, 1, sizeof(heard), pFp);
        fwrite(patte, 1, sizeof(patte), pFp);
#if 1
        for (idx = height - 1; idx >= 0; idx--)
        {
            fwrite(&pBuf[idx * width], sizeof(uint8_t) * width, 1, pFp);
        }
#else
        fwrite(buf, 1, size, pFp);
#endif
        fwrite(end, 1, sizeof(end), pFp);

        fflush(pFp);
        if (0 != fsync(fileno(pFp)))
        {
            ret = -4;
            break;
        }
    } while (0);

    if (NULL != pFp)
    {
        fclose(pFp);
        pFp = NULL;
    }

    return ret;
}

int cam_open()
{
    cam_fd = open(VIDEO_DEVICE, O_RDWR);

    if (cam_fd >= 0) return 0;
    else return -1;
}

int cam_close()
{
    close(cam_fd);

    return 0;
}

int cam_select(int index)
{
    int ret;

    int input = index;
    ret = ioctl(cam_fd, VIDIOC_S_INPUT, &input);
    return ret;
}

int cam_init()
{
    int i;
    int ret;
    struct v4l2_format format;

    memset(&format, 0, sizeof(format));
    format.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    //format.fmt.pix.pixelformat = V4L2_PIX_FMT_SBGGR8;
	format.fmt.pix.pixelformat = V4L2_PIX_FMT_SGRBG10;
	//format.fmt.pix.pixelformat = V4L2_PIX_FMT_SGRBG8;
    format.fmt.pix.width = IMAGE_WIDTH;
    format.fmt.pix.height = IMAGE_HEIGHT;
    ret = ioctl(cam_fd, VIDIOC_TRY_FMT, &format);
    if (ret != 0)
    {
        DBG("ioctl(VIDIOC_TRY_FMT) failed %d(%s)", errno, strerror(errno));
        return ret;
    }

    format.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    ret = ioctl(cam_fd, VIDIOC_S_FMT, &format);
    if (ret != 0)
    {
        DBG("ioctl(VIDIOC_S_FMT) failed %d(%s)", errno, strerror(errno));
        return ret;
    }

    struct v4l2_requestbuffers req;
    req.count = BUFFER_COUNT;
    req.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    req.memory = V4L2_MEMORY_MMAP;
    ret = ioctl(cam_fd, VIDIOC_REQBUFS, &req);
    if (ret != 0)
    {
        DBG("ioctl(VIDIOC_REQBUFS) failed %d(%s)", errno, strerror(errno));
        return ret;
    }
    DBG("req.count: %d", req.count);
    if (req.count < BUFFER_COUNT)
    {
        DBG("request buffer failed");
        return ret;
    }

    struct v4l2_buffer buffer;
    memset(&buffer, 0, sizeof(buffer));
    buffer.type = req.type;
    buffer.memory = V4L2_MEMORY_MMAP;
    for (i=0; i<req.count; i++)
    {
        buffer.index = i;
        ret = ioctl (cam_fd, VIDIOC_QUERYBUF, &buffer);
        if (ret != 0)
        {
            DBG("ioctl(VIDIOC_QUERYBUF) failed %d(%s)", errno, strerror(errno));
            return ret;
        }
        DBG("buffer.length: %d", buffer.length);
        DBG("buffer.m.offset: %d", buffer.m.offset);
        video_buffer_ptr[i] = (u8*) mmap(NULL, buffer.length, PROT_READ|PROT_WRITE, MAP_SHARED, cam_fd, buffer.m.offset);
        if (video_buffer_ptr[i] == MAP_FAILED)
        {
            DBG("mmap() failed %d(%s)", errno, strerror(errno));
            return -1;
        }

        buffer.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        buffer.memory = V4L2_MEMORY_MMAP;
        buffer.index = i;
        ret = ioctl(cam_fd, VIDIOC_QBUF, &buffer);
        if (ret != 0)
        {
            DBG("ioctl(VIDIOC_QBUF) failed %d(%s)", errno, strerror(errno));
            return ret;
        }
    }

    int buffer_type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    ret = ioctl(cam_fd, VIDIOC_STREAMON, &buffer_type);
    if (ret != 0)
    {
        DBG("ioctl(VIDIOC_STREAMON) failed %d(%s)", errno, strerror(errno));
        return ret;
    }

    DBG("cam init done.");

    return 0;
}

int cam_get_image(u8* out_buffer, int out_buffer_size)
{
    int ret;
    struct v4l2_buffer buffer;

    memset(&buffer, 0, sizeof(buffer));
    buffer.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    buffer.memory = V4L2_MEMORY_MMAP;
    buffer.index = BUFFER_COUNT;
    ret = ioctl(cam_fd, VIDIOC_DQBUF, &buffer);
    if (ret != 0)
    {
        DBG("ioctl(VIDIOC_DQBUF) failed %d(%s)", errno, strerror(errno));
        return ret;
    }

    if (buffer.index < 0 || buffer.index >= BUFFER_COUNT)
    {
        DBG("invalid buffer index: %d", buffer.index);
        return ret;
    }

    DBG("dequeue done, index: %d", buffer.index);
    memcpy(out_buffer, video_buffer_ptr[buffer.index], IMAGE_SIZE);
    DBG("copy done.");

    ret = ioctl(cam_fd, VIDIOC_QBUF, &buffer);
    if (ret != 0)
    {
        DBG("ioctl(VIDIOC_QBUF) failed %d(%s)", errno, strerror(errno));
        return ret;
    }
    DBG("enqueue done.");

    return 0;
}

JNIEXPORT jint JNICALL Java_com_sykean_imageproc_init(JNIEnv *env, jobject obj)
{
	jint ret = -1;

	ret = cam_open();
	ASSERT(ret==0);

	ret = cam_select(0);
	ASSERT(ret==0);

	ret = cam_init();
	ASSERT(ret==0);

	return ret;
}

JNIEXPORT jint JNICALL Java_com_sykean_imageproc_getRawData(JNIEnv *env, jobject obj,jbyteArray Frame)
{
	jint ret = -1;

	unsigned char* frame_tmp = NULL;
	frame_tmp = (*env)->GetByteArrayElements(env, Frame, 0);

	ret = cam_get_image(buf, IMAGE_SIZE);
	raw10toraw8(buf,frame_tmp,1920,1080);
	(*env)->ReleaseByteArrayElements(env, Frame, frame_tmp, 0);

	return ret;
}
int main()
{
    int i;
    int ret;

    ret = cam_open();
    ASSERT(ret==0);

    ret = cam_select(0);
    ASSERT(ret==0);

    ret = cam_init();
    ASSERT(ret==0);

    int count = 0;
    while (1)
    {
        ret = cam_get_image(buf, IMAGE_SIZE);
        ASSERT(ret==0);

        char tmp[64] = {"---\n"};
        for (i=0; i<16; i++)
            sprintf(&tmp[strlen(tmp)], "%02x ", buf[i]);
        LOGD("%s", tmp);

        char filename[32];
        sprintf(filename, "/sdcard/raw/%05d.bmp", count++);
        char filename1[32];
        sprintf(filename1, "/sdcard/raw/1%05d.raw", count);
		raw10toraw8(buf,raw8buf,1920,1080);
		saveRawToBmp(raw8buf,1920,1080,filename);
        /*int fd = open(filename,O_WRONLY|O_CREAT,00700);
        if (fd >= 0)
        {
			raw10toraw8(buf,raw8buf,1920,1080);
            //write(fd, buf, IMAGE_SIZE);
            write(fd, raw8buf, IMAGE_SIZE/2);
            close(fd);
        }
        else
        {
            LOGD("open() failed: %d(%s)", errno, strerror(errno));
        }*/
        /*int fd1 = open(filename1,O_WRONLY|O_CREAT,00700);
        if (fd1 >= 0)
        {
		raw10toraw8(buf,raw8buf,1920,1080);
            write(fd1, raw8buf, IMAGE_SIZE/2);
            close(fd1);
        }
        else
        {
            LOGD("open() failed: %d(%s)", errno, strerror(errno));
        }*/
    }

    ret = cam_close();
    ASSERT(ret==0);

    return 0;
}
