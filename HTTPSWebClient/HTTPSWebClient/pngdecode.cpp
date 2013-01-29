#include "stdafx.h"

#include "pngdecode.h"

typedef struct {
	unsigned char* data;
	unsigned int size;
	unsigned int offset;
}png_buf_mgr;

static void user_read_buf_callback(png_structp png_ptr, png_bytep data, png_size_t length)
{
	png_buf_mgr* mgr = (png_buf_mgr*)png_get_io_ptr(png_ptr);

	if(mgr->offset + length <= mgr->size)
	{
		memcpy(data, mgr->data + mgr->offset, length);
		mgr->offset += length;
	}
	else
	{
		png_error(png_ptr,"user_read_buf_callback failed");
	}
}

int bmpFromPng(unsigned char* png_buf, unsigned long png_buf_size, 
			   unsigned char* bmp_buf, unsigned long* bmp_buf_size,
			   unsigned int* bmp_height, unsigned int* bmp_width)
{
	if(bmp_buf == NULL)
		return -1;
	if(png_buf_size == 0)
		return -1;

	//if(png_sig_cmp(png_buf, (png_size_t)0, 8) != 0)
	//	return -1;

	png_structp png_ptr;
	png_infop info_ptr;
	png_ptr	= png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if(!png_ptr)
		return -1;

	info_ptr = png_create_info_struct(png_ptr);
	if(!info_ptr)
	{
		png_destroy_read_struct(&png_ptr, NULL, NULL);
		return -1;
	}

	if(setjmp(png_jmpbuf(png_ptr)))
	{
		png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
		return -1;
	}

	png_buf_mgr mgr;
	mgr.data = png_buf;
	mgr.size = png_buf_size;
	mgr.offset = 0;

	png_set_read_fn(png_ptr, &mgr, user_read_buf_callback);

	int png_transforms = PNG_TRANSFORM_STRIP_ALPHA;
	png_read_png(png_ptr, info_ptr, png_transforms, NULL);

	unsigned int height, width, perpixel;
	height = info_ptr->height;
	width = info_ptr->width;
	perpixel = (info_ptr->pixel_depth < 8)?(1):(info_ptr->pixel_depth/8);

	*bmp_height = height;
	*bmp_width = width;

	if(height == 0 || width == 0 || perpixel == 0)
	{
		png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
		return -1;
	}

	png_bytepp row_pointers = png_get_rows(png_ptr, info_ptr);

	// copy pixels to out buffer
	// upside down
	for(unsigned int i = 0; i < height; i++)
	{
		memcpy(bmp_buf + (height - i - 1) * width * perpixel, row_pointers[i], width * perpixel);
	}

	*bmp_buf_size = height * width * perpixel;
	png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
	return 0;
}