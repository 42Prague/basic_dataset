/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljiriste <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/02 08:44:53 by ljiriste          #+#    #+#             */
/*   Updated: 2024/04/03 22:18:06 by ljiriste         ###   ########.fr       */
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

typedef struct s_graphics
{
	t_mlx_session	mlx_ses;
	t_mlx_data		background;
	t_mlx_data		red_frame;
}					t_graphics;

void	decompress_to_image(struct jpeg_decompress_struct *cinfo,
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

void	cleanup(t_graphics *graphics)
{
	if (graphics->background.img)
		mlx_destroy_image(graphics->mlx_ses.mlx, graphics->background.img);
	if (graphics->red_frame.img)
		mlx_destroy_image(graphics->mlx_ses.mlx, graphics->red_frame.img);
	free_session(&graphics->mlx_ses);
	return ;
}

int	mlx_handle_key_press(int keycode, t_mlx_session *s)
{
	if (keycode == XK_Escape)
		mlx_close_win(s);
	return (0);
}

int	no_event_handle(t_graphics *g)
{
	static int	x = 0;
	static int	y = 0;

	if (!g->mlx_ses.mlx_win)
		return (0);
	if (x + g->red_frame.width + 1 == g->background.width)
	{
		x = 0;
		++y;
	}
	if (y + g->red_frame.height < g->background.height)
	{
		mlx_put_image_to_window(g->mlx_ses.mlx, g->mlx_ses.mlx_win, g->background.img, 0, 0);
		mlx_put_image_to_window(g->mlx_ses.mlx, g->mlx_ses.mlx_win, g->red_frame.img, x, y);
		/*
		if (emoji_encountered(image, emoji, x, y))
		{
			mlx_put_image_to_window(s->mlx, s->mlx_win, green_frame, x, y);
			record_pos(state, x, y);
		}
		*/
		++x;
	}
	else
		mlx_close_win(&g->mlx_ses);
	return (0);
}

void	display(char **argv)
{
	t_graphics		graphics;

	graphics.mlx_ses.mlx = mlx_init();
	graphics.background.img = mlx_jpeg_file_to_image(graphics.mlx_ses.mlx,
			argv[1], &graphics.background.width, &graphics.background.height);
	graphics.red_frame.img = mlx_xpm_file_to_image(graphics.mlx_ses.mlx,
			"red_frame.xpm", &graphics.red_frame.width, &graphics.red_frame.height);
	graphics.background.addr = mlx_get_data_addr(graphics.background.img,
			&graphics.background.bits_per_pixel,
			&graphics.background.line_length, &graphics.background.endian);
	if (graphics.background.img && graphics.red_frame.img)
	{
		graphics.red_frame.addr = mlx_get_data_addr(graphics.red_frame.img,
				&graphics.red_frame.bits_per_pixel,
				&graphics.red_frame.line_length, &graphics.red_frame.endian);
		graphics.mlx_ses.mlx_win = mlx_new_window(graphics.mlx_ses.mlx,
				graphics.background.width, graphics.background.height, argv[1]);
		mlx_hook(graphics.mlx_ses.mlx_win, KeyPress, KeyPressMask,
				mlx_handle_key_press, &graphics.mlx_ses);
		mlx_hook(graphics.mlx_ses.mlx_win, DestroyNotify, NoEventMask,
				mlx_close_win, &graphics.mlx_ses);
		mlx_loop_hook(graphics.mlx_ses.mlx, no_event_handle, &graphics);
		mlx_put_image_to_window(graphics.mlx_ses.mlx, graphics.mlx_ses.mlx_win, graphics.background.img, 0, 0);
		mlx_loop(graphics.mlx_ses.mlx);
	}
	cleanup(&graphics);
	return ;
}

int	main(int argc, char **argv)
{
	if (argc != 2)
		return (1);
	display(argv);
	return (0);
}
