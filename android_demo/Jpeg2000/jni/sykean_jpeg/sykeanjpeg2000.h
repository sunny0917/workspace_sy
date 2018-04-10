
#ifndef _SYKEANJPEG2000_H_
#define _SYKEANJPEG2000_H_

#include <openjpeg.h>
#include "format_defs.h"

#include <stdlib.h>
#include <string.h>

typedef struct membuffer
{
	OPJ_INT8* mem_buffer;
	OPJ_INT8* cur;
}stream_buffer;

//int DebugBufferChar(const char* pszSvBufPath, unsigned char* pBuffer, int iWidth, int iHeight);

opj_image_t* buffertoimage(unsigned char* pBuffer, int iWidth, int iHeight, opj_cparameters_t *parameters);

opj_stream_t* OPJ_CALLCONV opj_stream_create_default_buffer_stream(stream_buffer* p_membuffer,
	OPJ_BOOL p_is_read_stream);

opj_stream_t* OPJ_CALLCONV opj_stream_create_buffer_stream(stream_buffer* p_membuffer,
	OPJ_SIZE_T p_size, OPJ_BOOL p_is_read_stream);

int OPJ_CALLCONV SykeanJPEG2000Compress(unsigned char* pBufferIn, int iWidth, int iHeight,
	stream_buffer* pBufferOut, int* iBufferOutLength, int iDestBytes);


#endif
