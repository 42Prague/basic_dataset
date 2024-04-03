/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljiriste <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/02 08:44:53 by ljiriste          #+#    #+#             */
/*   Updated: 2024/04/03 19:36:07 by ljiriste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <mlx.h>
#include <unistd.h>
#include <stdlib.h>
#include <X11/X.h>
#include <X11/keysym.h>

#include <stdio.h>
#include <jpeglib.h>

typedef struct s_mlx_session
{
	void	*mlx;
	void	*mlx_win;
}				t_mlx_session;

typedef struct s_mlx_data
{
	void	*img;
	char	*addr;
	int		bits_per_pixel;
	int		line_length;
	int		endian;
	int		width;
	int		height;
}				t_mlx_data;

void	decompress_to_image(struct jpeg_decompress_struct *cinfo,
			t_mlx_data *img, void *mlx_ptr)
{
	char	*dst;

	img->img = mlx_new_image(mlx_ptr, cinfo->output_width,
			cinfo->output_height);
	img->addr = mlx_get_data_addr(img->img, &img->bits_per_pixel,
			&img->line_length, &img->endian);
	dst = img->addr;
	while (cinfo->output_scanline < cinfo->output_height)
	{
		jpeg_read_scanlines(cinfo, (JSAMPROW *)&dst, 1);
		dst += img->line_length;
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
	return (img.img);
}

int	mlx_close_win(t_mlx_session *s)
{
	mlx_destroy_window(s->mlx, s->mlx_win);
	s->mlx_win = NULL;
	return (0);
}

void	free_session(t_mlx_session *s)
{
	mlx_destroy_display(s->mlx);
	free(s->mlx);
	return ;
}

void	cleanup(t_mlx_session *s, t_mlx_data *image)
{
	mlx_destroy_image(s->mlx, image->img);
	free_session(s);
	return ;
}

int	mlx_handle_key_press(int keycode, t_mlx_session *s)
{
	if (keycode == XK_Escape)
		mlx_close_win(s);
	return (0);
}

int	no_event_handle(__attribute__((unused)) t_mlx_session *s)
{
/*
	static size_t	x = 0;
	static size_t	y = 0;

	if (x + 1 == width)
	{
		x = 0;
		++y;
	}
	if (y < height)
	{
		mlx_put_image_to_window(s->mlx, s->mlx_win, red_frame, x, y);
		if (emoji_encountered(image, emoji, x, y))
		{
			mlx_put_image_to_window(s->mlx, s->mlx_win, green_frame, x, y);
			record_pos(state, x, y);
		}
		++x;
	}
*/
	return (0);
}

void	display(char **argv)
{
	t_mlx_session	s;
	t_mlx_data		image;

	s.mlx = mlx_init();
	image.img = mlx_jpeg_file_to_image(s.mlx, argv[1],
			&image.width, &image.height);
	if (!image.img)
		return ;
	image.addr = mlx_get_data_addr(image.img, &image.bits_per_pixel,
			&image.line_length, &image.endian);
	s.mlx_win = mlx_new_window(s.mlx, image.width, image.height, argv[1]);
	mlx_hook(s.mlx_win, KeyPress, KeyPressMask, mlx_handle_key_press, &s);
	mlx_hook(s.mlx_win, DestroyNotify, NoEventMask, mlx_close_win, &s);
	mlx_loop_hook(s.mlx, no_event_handle, &s);
	mlx_put_image_to_window(s.mlx, s.mlx_win, image.img, 0, 0);
	mlx_loop(s.mlx);
	cleanup(&s, &image);
	return ;
}

int	main(int argc, char **argv)
{
	if (argc != 2)
		return (1);
	display(argv);
	return (0);
}
