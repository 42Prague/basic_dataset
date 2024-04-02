/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljiriste <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/02 08:44:53 by ljiriste          #+#    #+#             */
/*   Updated: 2024/04/02 11:29:04 by ljiriste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <unistd.h>
#include <setjmp.h>

#include <stdio.h>
#include <jpeglib.h>

typedef struct s_rgb
{
	unsigned char	r;
	unsigned char	g;
	unsigned char	b;
}					t_rgb;

//	Assumes 3 chanels
int	read_jpeg(char *filename, t_mat *image)
{
	struct jpeg_decompress_struct	cinfo;
	struct jpeg_error_mgr			jerr;
	JSAMPROW						buffer;
	FILE							*source;

	source = fopen(filename, "rb");
	if (!source)
	{
		ft_dprintf(STDERR_FILENO, "Can't open %s\n", filename);
		return (1);
	}
	cinfo.err = jpeg_std_error(&jerr);
	jpeg_create_decompress(&cinfo);
	jpeg_stdio_src(&cinfo, source);
	jpeg_read_header(&cinfo, TRUE);
	jpeg_start_decompress(&cinfo);
	ft_mat_zeros(image, cinfo.output_height, cinfo.output_width);
	while (cinfo.output_scanline < cinfo.output_height)
	{
		buffer = ft_mat_access(image, cinfo.output_scanline, 0);
		jpeg_read_scanlines(&cinfo, &buffer, 1);
	}
	jpeg_finish_decompress(&cinfo);
	jpeg_destroy_decompress(&cinfo);
	fclose(source);
	return (0);
}

int	main(int argc, char **argv)
{
	t_mat	image;

	if (argc != 2)
		return (1);
	ft_mat_init(&image, sizeof(t_rgb));
	read_jpeg(argv[1], &image);
	ft_mat_free(&image, NULL);
	return (0);
}
