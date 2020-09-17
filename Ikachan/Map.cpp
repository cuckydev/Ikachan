#include "Map.h"
#include <stdio.h>

BOOL LoadMapData(LPCTSTR path, MAP *map)
{
	//Open file
	FILE *fp = fopen(path, "rb");
	if (fp == NULL)
		return FALSE;
	
	//Read file
	BITMAPFILEHEADER bitmap_file_header;
	BITMAPINFOHEADER bitmap_info_header;
	DWORD bitmap_pal_data[0x100];

	fread(&bitmap_file_header, sizeof(BITMAPFILEHEADER), 1, fp);
	fread(&bitmap_info_header, sizeof(BITMAPINFOHEADER), 1, fp);
	fread(bitmap_pal_data, 4, 0x100, fp);

	//Verify file
	if (bitmap_file_header.bfType != 0x4D42) //'BM' little endian
		return FALSE;
	if (bitmap_info_header.biBitCount != 8) //8bpp
		return FALSE;

	//Get width and height and allocate buffer
	map->width = bitmap_info_header.biWidth;
	map->length = bitmap_info_header.biHeight;
	BYTE *final_buf = (BYTE*)malloc(bitmap_info_header.biWidth * bitmap_info_header.biHeight);
	
	//Read data from file
	BYTE *pre_buf = (BYTE*)malloc(bitmap_info_header.biWidth * bitmap_info_header.biHeight);
	fread(pre_buf, 1, bitmap_info_header.biWidth * bitmap_info_header.biHeight, fp);
	fclose(fp);
	
	//Copy data flipped vertically
	int v13 = 0;
	int v3 = map->width * (map->length - 1);
	while (v13 < (map->width * map->length))
	{
		memcpy(&final_buf[v3], &pre_buf[v13], map->width);
		v13 += map->width;
		v3 -= map->width;
	}
	free(pre_buf);

	//Use data
	map->data = final_buf;
	return TRUE;
}
