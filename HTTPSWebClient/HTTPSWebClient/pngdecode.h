#pragma once

#include "png.h"
#include "pnginfo.h"

#define MAX_PNG_SIZE (1024*1024*1)
#define SMALL_PNG_SIZE	(1024*300)
//unsigned int bmpCompGet(unsigned char* jpeg_buf, unsigned long jpeg_buf_size);
//unsigned int bmpHeightGet(unsigned char* jpeg_buf, unsigned long jpeg_buf_size);
//unsigned int bmpWidthGet(unsigned char* jpeg_buf, unsigned long jpeg_buf_size);
int bmpFromPng(unsigned char* png_buf, unsigned long png_buf_size, 
			   unsigned char* bmp_buf, unsigned long* bmp_buf_size,
			   unsigned int* height, unsigned int* width);