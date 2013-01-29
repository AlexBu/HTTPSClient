#include "stdafx.h"

#include "jpeglib.h"

static void safe_exit (j_common_ptr cinfo)
{
	/* Always display the message */
	(*cinfo->err->output_message) (cinfo);

	/* Let the memory manager delete any temp files before we die */
	jpeg_destroy(cinfo);

	throw(0);
}

unsigned int bmpHeightGet(unsigned char* jpeg_buf, unsigned long jpeg_buf_size)
{
	if(jpeg_buf_size == 0)
		return -1;
	struct jpeg_decompress_struct cinfo;
	struct jpeg_error_mgr jerr;
	unsigned int height = 0;
	cinfo.err = jpeg_std_error(&jerr);
	jerr.error_exit = safe_exit;
	try
	{
		jpeg_create_decompress(&cinfo);

		jpeg_mem_src(&cinfo, jpeg_buf, jpeg_buf_size);

		jpeg_read_header(&cinfo, true);

		height = cinfo.image_height;

		jpeg_destroy_decompress(&cinfo);

	}
	catch(int)
	{
		height = 0;
	}
	
	return height;
}

unsigned int bmpWidthGet(unsigned char* jpeg_buf, unsigned long jpeg_buf_size)
{
	if(jpeg_buf_size == 0)
		return -1;
	struct jpeg_decompress_struct cinfo;
	struct jpeg_error_mgr jerr;
	unsigned int width = 0;
	cinfo.err = jpeg_std_error(&jerr);
	jerr.error_exit = safe_exit;
	try
	{
		jpeg_create_decompress(&cinfo);

		jpeg_mem_src(&cinfo, jpeg_buf, jpeg_buf_size);

		jpeg_read_header(&cinfo, true);

		width = cinfo.image_width;

		jpeg_destroy_decompress(&cinfo);
	}
	catch(int)
	{
		width = 0;
	}
	return width;
}

unsigned int bmpCompGet(unsigned char* jpeg_buf, unsigned long jpeg_buf_size)
{
	if(jpeg_buf_size == 0)
		return -1;
	struct jpeg_decompress_struct cinfo;
	struct jpeg_error_mgr jerr;
	unsigned int comp = 0;
	cinfo.err = jpeg_std_error(&jerr);
	jerr.error_exit = safe_exit;

	try
	{
		jpeg_create_decompress(&cinfo);

		jpeg_mem_src(&cinfo, jpeg_buf, jpeg_buf_size);

		jpeg_read_header(&cinfo, true);

		comp = cinfo.num_components;

		jpeg_destroy_decompress(&cinfo);

	}
	catch(int)
	{
		comp = 0;
	}
	
	return comp;
}

int bmpFromJpeg(unsigned char* jpeg_buf, unsigned long jpeg_buf_size, unsigned char* bmp_buf, unsigned long* bmp_buf_size)
{
	if(bmp_buf == NULL)
		return -1;
	if(jpeg_buf_size == 0)
		return -1;

	struct jpeg_decompress_struct cinfo;
	struct jpeg_error_mgr jerr;
	cinfo.err = jpeg_std_error(&jerr);
	jerr.error_exit = safe_exit;

	try
	{
		jpeg_create_decompress(&cinfo);

		jpeg_mem_src(&cinfo, jpeg_buf, jpeg_buf_size);

		jpeg_read_header(&cinfo, true);

		*bmp_buf_size = cinfo.image_width*cinfo.image_height*cinfo.num_components;

		jpeg_start_decompress(&cinfo);

		JSAMPROW row_pointer[1];
		while (cinfo.output_scanline < cinfo.output_height)
		{
			row_pointer[0] = &bmp_buf[(cinfo.output_height - cinfo.output_scanline-1)*cinfo.image_width*cinfo.num_components];
			jpeg_read_scanlines(&cinfo,row_pointer, 1);
		}
		jpeg_finish_decompress(&cinfo);

		jpeg_destroy_decompress(&cinfo);

	}
	catch(int)
	{
		*bmp_buf_size = 0;
		return -1;
	}

	return 0;
}
