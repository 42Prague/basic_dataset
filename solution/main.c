/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljiriste <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/02 08:44:53 by ljiriste          #+#    #+#             */
/*   Updated: 2024/04/04 00:24:08 by ljiriste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <mlx.h>
#include <unistd.h>
#include <stdlib.h>
#include <X11/X.h>
#include <X11/keysym.h>
#include <limits.h>

#include <stdio.h>
#include <jpeglib.h>

#define EMOJI_TRESHOLD 255

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
	t_mlx_data		green_frame;
	t_mlx_data		emoji;
}					t_graphics;

typedef struct s_position
{
	int	x;
	int	y;
}		t_position;

typedef struct s_state
{
	t_graphics	graph;
	t_position	pos;
	t_vec		found;
}				t_state;

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

typedef struct s_argb
{
	unsigned char	a;
	unsigned char	r;
	unsigned char	g;
	unsigned char	b;
}			t_argb;

t_argb	uint_to_argb(unsigned int uint_color)
{
	t_argb	res;

	res.a = uint_color >> (8 * 3);
	res.r = uint_color >> (8 * 2);
	res.g = uint_color >> (8 * 1);
	res.b = uint_color >> (8 * 0);
	return (res);
}

unsigned int	argb_to_uint(t_argb color)
{
	unsigned int	res;

	res = 0;
	res |= color.a << (8 * 3);
	res |= color.r << (8 * 2);
	res |= color.g << (8 * 1);
	res |= color.b << (8 * 0);
	return (res);
}

void	flip_alpha(t_mlx_data *img)
{
	char	*pixel;
	int		x;
	int		y;
	t_argb	color;

	x = 0;
	while (x < img->width)
	{
		y = 0;
		while (y < img->height)
		{
			pixel = (char *)img->addr
				+ y * img->line_length + x * img->bits_per_pixel / CHAR_BIT;
			color = uint_to_argb(*(unsigned int *)pixel);
			color.a = 255 - color.a;
			*(unsigned int *)pixel = argb_to_uint(color);
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

//	This function probably could do the arithmetic in uint form
//	but this way it is more legible I think
unsigned int	mix_colors(unsigned int back_uint, unsigned int front_uint)
{
	t_argb	back;
	t_argb	front;
	t_argb	res;

	back = uint_to_argb(back_uint);
	front = uint_to_argb(front_uint);
	res.a = front.a - (255 - back.a) * front.a / 255;
	res.r = (front.r * (255 - front.a)
			+ back.r * (255 - back.a) * front.a / 255) / (255 - res.a);
	res.g = (front.g * (255 - front.a)
			+ back.g * (255 - back.a) * front.a / 255) / (255 - res.a);
	res.b = (front.b * (255 - front.a)
			+ back.b * (255 - back.a) * front.a / 255) / (255 - res.a);
	return (argb_to_uint(res));
}

void	copy_image(t_mlx_data *dest, t_mlx_data *src, int x, int y)
{
	char	*dest_pix;
	char	*src_pix;
	int		i;
	int		j;

	i = 0;
	while (i + x < dest->width && i < src->width)
	{
		j = 0;
		while (j + y < dest->height && j < src->height)
		{
			dest_pix = (char *)dest->addr + ((j + y) * dest->line_length
					+ (i + x) * dest->bits_per_pixel / CHAR_BIT);
			src_pix = (char *)src->addr + (j * src->line_length
					+ i * src->bits_per_pixel / CHAR_BIT);
			*(unsigned int *)dest_pix = mix_colors(*(unsigned int *)dest_pix,
					*(unsigned int *)src_pix);
			++j;
		}
		++i;
	}
	return ;
}

//	First use has to be with the largest picture, as that initializes the image
int	mlx_put_image_to_window_transparency(t_mlx_session *s, t_mlx_data *img,
		int x, int y)
{
	static t_mlx_data	to_print;

	if (!s->mlx_win)
	{
		mlx_destroy_image(s->mlx, to_print.img);
		return (0);
	}
	if (!to_print.img)
	{
		to_print.img = mlx_new_image(s->mlx, img->width, img->height);
		if (!to_print.img)
			return (1);
		to_print.addr = mlx_get_data_addr(to_print.img,
				&to_print.bits_per_pixel, &to_print.line_length,
				&to_print.endian);
		to_print.width = img->width;
		to_print.height = img->height;
	}
	copy_image(&to_print, img, x, y);
	mlx_put_image_to_window(s->mlx, s->mlx_win, to_print.img, 0, 0);
	return (0);
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

void	cleanup(t_state *state)
{
	if (state->graph.background.img)
		mlx_destroy_image(state->graph.mlx_ses.mlx,
			state->graph.background.img);
	if (state->graph.red_frame.img)
		mlx_destroy_image(state->graph.mlx_ses.mlx,
			state->graph.red_frame.img);
	if (state->graph.green_frame.img)
		mlx_destroy_image(state->graph.mlx_ses.mlx,
			state->graph.green_frame.img);
	if (state->graph.emoji.img)
		mlx_destroy_image(state->graph.mlx_ses.mlx, state->graph.emoji.img);
	mlx_put_image_to_window_transparency(&state->graph.mlx_ses, NULL, 0, 0);
	ft_vec_free(&state->found, NULL);
	free_session(&state->graph.mlx_ses);
	return ;
}

int	mlx_handle_key_press(int keycode, t_mlx_session *s)
{
	if (keycode == XK_Escape)
		mlx_close_win(s);
	return (0);
}

unsigned int	sqr_diff(unsigned char a, unsigned char b)
{
	if (a > b)
		return ((a - b) * (a - b));
	else
		return ((b - a) * (b - a));
}

unsigned int	uint_color_sqr_diff(unsigned int uint_color1,
		unsigned int uint_color2)
{
	t_argb			color1;
	t_argb			color2;
	unsigned int	res;

	color1 = uint_to_argb(uint_color1);
	color2 = uint_to_argb(uint_color2);
	res = 0;
	res += sqr_diff(color1.r, color2.r) / 255
		* (255 - color1.a) * (255 - color2.a) / 255;
	res += sqr_diff(color1.g, color2.g) / 255
		* (255 - color1.a) * (255 - color2.a) / 255;
	res += sqr_diff(color1.b, color2.b) / 255
		* (255 - color1.a) * (255 - color2.a) / 255;
	return (res);
}

int	emoji_encountered(t_mlx_data *background, t_mlx_data *emoji, int x, int y)
{
	int				i;
	int				j;
	char			*pix_emoji;
	char			*pix_back;
	unsigned int	sqr_diff;

	sqr_diff = 0;
	i = 0;
	while (i < emoji->width)
	{
		j = 0;
		while (j < emoji->height)
		{
			pix_emoji = (char *)emoji->addr
				+ j * emoji->line_length
				+ i * emoji->bits_per_pixel / CHAR_BIT;
			pix_back = (char *)background->addr
				+ (j + y) * background->line_length
				+ (i + x) * background->bits_per_pixel / CHAR_BIT;
			sqr_diff += uint_color_sqr_diff(*(unsigned int *)pix_emoji,
					*(unsigned int *)pix_back);
			++j;
		}
		++i;
	}
	sqr_diff /= emoji->width * emoji->height;
	return (sqr_diff < EMOJI_TRESHOLD);
}

void	print_green_frames(t_state *state)
{
	t_position	pos;
	size_t		i;

	i = 0;
	while (i < state->found.size)
	{
		pos = *(t_position *)ft_vec_access(&state->found, i);
		mlx_put_image_to_window_transparency(&state->graph.mlx_ses,
			&state->graph.green_frame, pos.x, pos.y);
		++i;
	}
	return ;
}

int	no_event_handle(t_state *state)
{
	if (!state->graph.mlx_ses.mlx_win)
		return (0);
	if (state->pos.x + state->graph.emoji.width > state->graph.background.width)
	{
		state->pos.x = 0;
		++state->pos.y;
	}
	if (state->pos.y + state->graph.emoji.height
		<= state->graph.background.height)
	{
		mlx_put_image_to_window_transparency(&state->graph.mlx_ses,
			&state->graph.background, 0, 0);
		print_green_frames(state);
		mlx_put_image_to_window_transparency(&state->graph.mlx_ses,
			&state->graph.red_frame, state->pos.x, state->pos.y);
		if (emoji_encountered(&state->graph.background,
				&state->graph.emoji, state->pos.x, state->pos.y))
			ft_vec_append(&state->found, &state->pos);
		++state->pos.x;
	}
	else
		mlx_close_win(&state->graph.mlx_ses);
	return (0);
}

void	set_addresses(t_graphics *graphics)
{
	graphics->background.addr = mlx_get_data_addr(graphics->background.img,
			&graphics->background.bits_per_pixel,
			&graphics->background.line_length, &graphics->background.endian);
	graphics->red_frame.addr = mlx_get_data_addr(graphics->red_frame.img,
			&graphics->red_frame.bits_per_pixel,
			&graphics->red_frame.line_length, &graphics->red_frame.endian);
	graphics->green_frame.addr = mlx_get_data_addr(graphics->green_frame.img,
			&graphics->green_frame.bits_per_pixel,
			&graphics->green_frame.line_length, &graphics->green_frame.endian);
	graphics->emoji.addr = mlx_get_data_addr(graphics->emoji.img,
			&graphics->emoji.bits_per_pixel,
			&graphics->emoji.line_length, &graphics->emoji.endian);
	return ;
}

int	open_images(t_graphics *graphics, char **argv)
{
	int	res;

	graphics->background.img = mlx_jpeg_file_to_image(graphics->mlx_ses.mlx,
			argv[1], &graphics->background.width,
			&graphics->background.height);
	graphics->red_frame.img = mlx_xpm_file_to_image(graphics->mlx_ses.mlx,
			"red_frame.xpm", &graphics->red_frame.width,
			&graphics->red_frame.height);
	graphics->green_frame.img = mlx_xpm_file_to_image(graphics->mlx_ses.mlx,
			"green_frame.xpm", &graphics->green_frame.width,
			&graphics->green_frame.height);
	graphics->emoji.img = mlx_xpm_file_to_image(graphics->mlx_ses.mlx,
			"emoji.xpm", &graphics->emoji.width, &graphics->emoji.height);
	res = graphics->background.img && graphics->red_frame.img
		&& graphics->green_frame.img && graphics->emoji.img;
	if (res)
		set_addresses(graphics);
	return (!res);
}

int	init_state(t_state *state)
{
	state->graph.mlx_ses.mlx = mlx_init();
	state->pos.x = 0;
	state->pos.y = 0;
	return (ft_vec_init(&state->found, sizeof(t_position)) != success
		|| !state->graph.mlx_ses.mlx);
}

void	display(char **argv)
{
	t_state	state;

	if (!init_state(&state) && !open_images(&state.graph, argv))
	{
		state.graph.mlx_ses.mlx_win = mlx_new_window(state.graph.mlx_ses.mlx,
				state.graph.background.width,
				state.graph.background.height, argv[1]);
		mlx_hook(state.graph.mlx_ses.mlx_win, KeyPress, KeyPressMask,
			mlx_handle_key_press, &state.graph.mlx_ses);
		mlx_hook(state.graph.mlx_ses.mlx_win, DestroyNotify, NoEventMask,
			mlx_close_win, &state.graph.mlx_ses);
		mlx_loop_hook(state.graph.mlx_ses.mlx, no_event_handle, &state);
		mlx_put_image_to_window_transparency(&state.graph.mlx_ses,
			&state.graph.background, 0, 0);
		mlx_loop(state.graph.mlx_ses.mlx);
	}
	cleanup(&state);
	return ;
}

int	main(int argc, char **argv)
{
	if (argc != 2)
		return (1);
	display(argv);
	return (0);
}
