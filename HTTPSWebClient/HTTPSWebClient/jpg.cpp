#include "stdafx.h"

#include "jpeglib.h"

unsigned int bmpHeightGet(unsigned char* jpeg_buf, unsigned long jpeg_buf_size)
{
	struct jpeg_decompress_struct cinfo;
	struct jpeg_error_mgr jerr;
	cinfo.err = jpeg_std_error(&jerr);
	jpeg_create_decompress(&cinfo);

	jpeg_mem_src(&cinfo, jpeg_buf, jpeg_buf_size);

	jpeg_read_header(&cinfo, true);

	unsigned int height = cinfo.image_height;

	jpeg_destroy_decompress(&cinfo);

	return height;
}

unsigned int bmpWidthGet(unsigned char* jpeg_buf, unsigned long jpeg_buf_size)
{
	struct jpeg_decompress_struct cinfo;
	struct jpeg_error_mgr jerr;
	cinfo.err = jpeg_std_error(&jerr);
	jpeg_create_decompress(&cinfo);

	jpeg_mem_src(&cinfo, jpeg_buf, jpeg_buf_size);

	jpeg_read_header(&cinfo, true);

	unsigned int width = cinfo.image_width;

	jpeg_destroy_decompress(&cinfo);

	return width;
}

int bmpFromJpeg(unsigned char* jpeg_buf, unsigned long jpeg_buf_size, unsigned char* bmp_buf, unsigned long* bmp_buf_size)
{
	struct jpeg_decompress_struct cinfo;
	struct jpeg_error_mgr jerr;
	cinfo.err = jpeg_std_error(&jerr);
	jpeg_create_decompress(&cinfo);

	jpeg_mem_src(&cinfo, jpeg_buf, jpeg_buf_size);

	jpeg_read_header(&cinfo, true);

	*bmp_buf_size = cinfo.image_width*cinfo.image_height*cinfo.num_components;

	bmp_buf = new unsigned char[*bmp_buf_size];

	if(bmp_buf == NULL)
	{
		return -1;
	}

	jpeg_start_decompress(&cinfo);
	 
	JSAMPROW row_pointer[1];
	while (cinfo.output_scanline < cinfo.output_height)
	{
		row_pointer[0] = &bmp_buf[(cinfo.output_height - cinfo.output_scanline-1)*cinfo.image_width*cinfo.num_components];
		jpeg_read_scanlines(&cinfo,row_pointer, 1);
	}
	jpeg_finish_decompress(&cinfo);

	jpeg_destroy_decompress(&cinfo);
	return 0;
}