//-------------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "fb.h"
#include "bmp.h"

int file_to_fb(const char * srcpath)
{
	int ret = -1;
	BMP_READ * bmp = NULL;
	struct FB * fb = NULL;
	int sw, sh;
	int srcbpp, dstbpp;
	void * pdata = NULL, * bmpdata = NULL;
	RGB_CONVERT_FUN convert_func = NULL;

	do
	{
		bmp = bmp_open(srcpath);
		if (!bmp) {
			break;
		}
		fb = fb_create(0);
		if (!fb) {
			break;
		}

		sw = bmp_width(bmp);
		sh = bmp_height(bmp);
		bmpdata = bmp_data(bmp);
		srcbpp = bmp_bpp(bmp);
		dstbpp = fb_bpp(fb);

		printf("bmp to fb, %dx%d,%d=>%d\n", sw,sh,srcbpp,dstbpp);
		if (srcbpp==32 && dstbpp==32) {
			convert_func = abgr8888_to_bgra8888_buffer;
		} else if (srcbpp==24 && dstbpp==32) {
			convert_func = bgr888_to_bgra8888_buffer;
		} else if (srcbpp==16 && dstbpp==32) {
			convert_func = rgb565_to_bgra8888_buffer;
		} else {
			convert_func = get_convert_func(srcbpp, dstbpp);
		}

		if (convert_func) {
			pdata = convert_func(bmpdata, sw, sh);
			bmpdata = pdata;
		}

		if (!bmp_forward(bmp)) {
			line_reversal(bmpdata, sw, sh, dstbpp);
		}

		rgb_copy(bmpdata, fb_bits(fb), sw, sh, fb_width(fb), fb_height(fb), dstbpp);
		ret = 0;
	} while (0);

	fb_destory(fb);
	bmp_close(bmp);
	if (pdata) {
		free(pdata);
	}
	return ret;
}
