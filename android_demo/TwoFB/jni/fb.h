/********************************************************************
	created:	2012/02/07
	filename: 	myfb.h
	author:

	purpose:
*********************************************************************/

#ifndef _fb_h__
#define _fb_h__
//-------------------------------------------------------------------

#include <linux/fb.h>
struct FB {
    unsigned short *bits;
    int fd;
    struct fb_fix_screeninfo fi;
    struct fb_var_screeninfo vi;
};

int fb_bpp(struct FB *fb);
int fb_width(struct FB *fb);
int fb_height(struct FB *fb);
int fb_size(struct FB *fb);
int fb_virtual_size(struct FB *fb);
void * fb_bits(struct FB *fb);
void fb_update(struct FB *fb);

struct FB * fb_create(int readonly);
void fb_destory(struct FB *fb);

typedef struct FBinfo {
	int bpp;
	int width;
	int height;
}FBinfo;
int fb_info(FBinfo * fbinfo);

//-------------------------------------------------------------------
#endif // #ifndef _fb_h__
