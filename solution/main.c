/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljiriste <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/02 08:44:53 by ljiriste          #+#    #+#             */
/*   Updated: 2024/04/02 22:44:46 by ljiriste         ###   ########.fr       */
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

typedef struct s_rgb
{
	unsigned char	r;
	unsigned char	g;
	unsigned char	b;
}					t_rgb;

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
}				t_mlx_data;

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

unsigned int	rgb_to_uint(t_rgb rgb)
{
	unsigned int	res;

	res = 0;
	res |= rgb.r << (8 * 2);
	res |= rgb.g << (8 * 1);
	res |= rgb.b << (8 * 0);
	return (res);
}

void	copy_mat_to_mlx_image(t_mat *image, t_mlx_data *mlx_image)
{
	char	*dest;
	size_t	x;
	size_t	y;

	x = 0;
	while (x < image->cols)
	{
		y = 0;
		while (y < image->rows)
		{
			dest = mlx_image->addr + (y * mlx_image->line_length + x * (mlx_image->bits_per_pixel / 8));
			*(unsigned int *)dest = rgb_to_uint(*(t_rgb *)ft_mat_access(image, y, x));
			++y;
		}
		++x;
	}
	return ;
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
	return (0);
}

void	display(t_mat *image, char *filename)
{
	t_mlx_session	s;
	t_mlx_data		mlx_image;

	s.mlx = mlx_init();
	s.mlx_win = mlx_new_window(s.mlx, image->cols, image->rows, filename);
	mlx_image.img = mlx_new_image(s.mlx, image->cols, image->rows);
	mlx_image.addr = mlx_get_data_addr(mlx_image.img, &mlx_image.bits_per_pixel, &mlx_image.line_length, &mlx_image.endian);
	copy_mat_to_mlx_image(image, &mlx_image);
	mlx_hook(s.mlx_win, KeyPress, KeyPressMask, mlx_handle_key_press, &s);
	mlx_hook(s.mlx_win, DestroyNotify, NoEventMask, mlx_close_win, &s);
	mlx_loop_hook(s.mlx, no_event_handle, &s);
	mlx_put_image_to_window(s.mlx, s.mlx_win, mlx_image.img, 0, 0);
	mlx_loop(s.mlx);
	cleanup(&s, &mlx_image);
	return ;
}

int	main(int argc, char **argv)
{
	t_mat			image;

	if (argc != 2)
		return (1);
	ft_mat_init(&image, sizeof(t_rgb));
	read_jpeg(argv[1], &image);
	display(&image, argv[1]);
	ft_mat_free(&image, NULL);
	return (0);
}
