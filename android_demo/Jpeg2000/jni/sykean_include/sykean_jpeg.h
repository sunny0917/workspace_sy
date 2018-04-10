/*
 * Copyright (c) 2017 SYKEAN Limited.
 *
 * All rights are reserved.
 * Proprietary and confidential.
 * Unauthorized copying of this file, via any medium is strictly prohibited.
 * Any use is subject to an appropriate license granted by SYKEAN Company.
 */

#ifndef __SYKEAN_JPEG2000__
#define __SYKEAN_JPEG2000__

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define JNIEXPORT __attribute__ ((visibility ("default")))

#ifdef __cplusplus
extern "C" {
#endif


JNIEXPORT int32_t sykean_jpeg_alloc_size(int32_t width, int32_t height, int32_t cmprsSizeByte);
JNIEXPORT int32_t sykean_jpeg_compress(uint8_t *pRawBuf, int32_t width, int32_t height,
                                       int8_t *pOutBuf, int32_t *pOutSize, int32_t cmprsSizeByte);


#ifdef __cplusplus
}
#endif

#endif