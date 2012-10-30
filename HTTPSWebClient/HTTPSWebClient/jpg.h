#pragma once

unsigned int bmpHeightGet(unsigned char* jpeg_buf, unsigned long jpeg_buf_size);
unsigned int bmpWidthGet(unsigned char* jpeg_buf, unsigned long jpeg_buf_size);
int bmpFromJpeg(unsigned char* jpeg_buf, unsigned long jpeg_buf_size, unsigned char* bmp_buf, unsigned long* bmp_buf_size);