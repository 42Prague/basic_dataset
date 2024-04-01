/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readjpg.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okraus <okraus@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/01 11:50:05 by okraus            #+#    #+#             */
/*   Updated: 2024/04/01 15:58:57 by okraus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//test program to read jpg into array

// -ljpeg

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <jpeglib.h>
#include <jerror.h>

typedef struct s_image
{
	unsigned int	width;
	unsigned int	height;
	unsigned int	line;
	unsigned int	size;
	unsigned int	alpha;
	unsigned int	channels;
	unsigned char	*pixels;
}	t_image;


t_image read_jpeg(const char *path)
{
	t_image	image;
	FILE	*file = fopen( path, "rb" );
	if ( file == NULL )
	{
		return ((t_image){0, 0, 0, 0, 0, 0, NULL});
	}
	struct jpeg_decompress_struct info; //for our jpeg info
	struct jpeg_error_mgr err; //the error handler
	info.err = jpeg_std_error(&err);
	jpeg_create_decompress(&info ); //fills info structure
	jpeg_stdio_src(&info, file );
	jpeg_read_header(&info, true);
	jpeg_start_decompress(&info);
	image.width = info.output_width;
	image.height = info.output_height;
	image.channels = info.num_components; // 3 = RGB, 4 = RGBA
	image.line = image.height * image.channels;
	if (image.channels == 4)
		image.alpha = 1;
	else
		image.alpha = 0;
	image.size = image.width * image.height * image.channels;
	image.pixels = (unsigned char *)malloc( image.size );
	unsigned char* rowptr;
	while ( info.output_scanline < image.height )
	{
		rowptr = image.pixels  + info.output_scanline * image.width * image.channels;
		jpeg_read_scanlines( &info, &rowptr, 1 );
	}
	jpeg_finish_decompress( &info );
	jpeg_destroy_decompress( &info );   
	fclose( file );
	return image;
}

int	main (void)
{
	t_image	img;

	img = read_jpeg("test.jpg");
	int	y = 0;
	int	x = 0;
	while (y < img.height)
	{
		x = 0;
		while (x < img.width * img.channels)
		{
			printf("%02x", img.pixels[y * img.width * img.channels + x]);
			++x;
		}
		++y;
		printf("\n");
	}
	return (0);
}

//read image
//read emoji
//scan the image and find the emoji