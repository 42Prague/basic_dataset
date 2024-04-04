/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   jpeg_handling.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljiriste <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/04 10:21:40 by ljiriste          #+#    #+#             */
/*   Updated: 2024/04/04 10:36:49 by ljiriste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "digiteq.h"
#include <mlx.h>
#include <unistd.h>

#include <stdio.h>
#include <jpeglib.h>

static void	decompress_to_image(struct jpeg_decompress_struct *cinfo,
			t_mlx_data *img, void *mlx_ptr)
{
	char	*dst;

	img->img = mlx_new_image(mlx_ptr, cinfo->output_width,
			cinfo->output_height);
	img->addr = mlx_get_data_addr(img->img, &img->bits_per_pixel,
			&img->line_length, &img->endian);
	img->width = cinfo->output_width;
	img->height = cinfo->output_height;
	dst = img->addr;
	while (cinfo->output_scanline < cinfo->output_height)
	{
		jpeg_read_scanlines(cinfo, (JSAMPROW *)&dst, 1);
		dst += img->line_length;
	}
	return ;
}

static void	flip_alpha(t_mlx_data *img)
{
	unsigned int	*pixel;
	int				x;
	int				y;
	t_argb			color;

	x = 0;
	while (x < img->width)
	{
		y = 0;
		while (y < img->height)
		{
			pixel = get_mlx_pixel(img, x, y);
			color = uint_to_argb(*pixel);
			color.a = 255 - color.a;
			*pixel = argb_to_uint(color);
			++y;
		}
		++x;
	}
	return ;
}

//	This function is written as to have
//	the same signature as mlx_png_file_to_image
void	*mlx_jpeg_file_to_image(void *mlx_ptr, char *filename,
			int *width, int *height)
{
	struct jpeg_decompress_struct	cinfo;
	struct jpeg_error_mgr			jerr;
	FILE							*source;
	t_mlx_data						img;

	source = fopen(filename, "rb");
	if (!source)
	{
		ft_dprintf(STDERR_FILENO, "Can't open %s\n", filename);
		return (NULL);
	}
	cinfo.err = jpeg_std_error(&jerr);
	jpeg_create_decompress(&cinfo);
	jpeg_stdio_src(&cinfo, source);
	jpeg_read_header(&cinfo, TRUE);
	cinfo.out_color_space = JCS_EXT_BGRA;
	jpeg_start_decompress(&cinfo);
	*width = cinfo.output_width;
	*height = cinfo.output_height;
	decompress_to_image(&cinfo, &img, mlx_ptr);
	jpeg_finish_decompress(&cinfo);
	jpeg_destroy_decompress(&cinfo);
	fclose(source);
	flip_alpha(&img);
	return (img.img);
}
