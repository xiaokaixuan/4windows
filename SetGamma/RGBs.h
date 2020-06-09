#ifndef __RGBS_H__
#define __RGBS_H__

typedef struct tagRGB
{
	char *name;
	unsigned char rValue;
	unsigned char gValue;
	unsigned char bValue;
} TEMP_RGB;

extern const TEMP_RGB RGBs[782];

#endif // __RGBS_H__