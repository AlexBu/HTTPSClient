#pragma once

#define MAX_JPG_SIZE (1024*1024*1)
#define SMALL_JPG_SIZE	(1024*300)
unsigned int bmpCompGet(unsigned char* jpeg_buf, unsigned long jpeg_buf_size);
unsigned int bmpHeightGet(unsigned char* jpeg_buf, unsigned long jpeg_buf_size);
unsigned int bmpWidthGet(unsigned char* jpeg_buf, unsigned long jpeg_buf_size);
int bmpFromJpeg(unsigned char* jpeg_buf, unsigned long jpeg_buf_size, unsigned char* bmp_buf, unsigned long* bmp_buf_size);