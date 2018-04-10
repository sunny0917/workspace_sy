
#include "sykean_jpeg.h"
#include "sykeanjpeg2000.h"

int32_t sykean_jpeg_alloc_size(int32_t width, int32_t height, int32_t cmprsSizeByte)
{
	if (width <= 0 || height <= 0 || cmprsSizeByte < 1)
	{
		return -1;
	}

	float inSize = width * height;
	float outSize = cmprsSizeByte;
	float fRatio = inSize / outSize;

	if (fRatio < 2)
	{
		return -2;
	}

	int iMemneed = (inSize / (fRatio - 1));

	return iMemneed;
}

int32_t sykean_jpeg_compress(uint8_t *pRawBuf, int32_t width, int32_t height,
	int8_t *pOutBuf, int32_t *pOutSize, int32_t cmprsSizeByte)
{
	if (width <= 0 || height <= 0 || cmprsSizeByte < 1)
	{
		return -5;
	}

	float inSize = width * height;
	float outSize = cmprsSizeByte;
	float fRatio = inSize / outSize;

	if (fRatio < 2)
	{
		return -6;
	}

	stream_buffer* p_membuffer = NULL;
	p_membuffer = (stream_buffer*)malloc(sizeof(stream_buffer));

	p_membuffer->mem_buffer = pOutBuf;	
	p_membuffer->cur = p_membuffer->mem_buffer;

	int iRet = SykeanJPEG2000Compress(pRawBuf, width, height,
		p_membuffer, pOutSize, cmprsSizeByte);

	if (p_membuffer != NULL)
	{
		if (p_membuffer->cur != NULL)
		{
			p_membuffer->cur = NULL;
		}

		free(p_membuffer);
		p_membuffer = NULL;
	}

	return iRet;
}