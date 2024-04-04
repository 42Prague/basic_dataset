/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   images.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljiriste <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/04 10:29:22 by ljiriste          #+#    #+#             */
/*   Updated: 2024/04/04 10:31:33 by ljiriste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "digiteq.h"
#include <mlx.h>

static void	set_addresses(t_graphics *graphics)
{
	graphics->background.addr = mlx_get_data_addr(graphics->background.img,
			&graphics->background.bits_per_pixel,
			&graphics->background.line_length, &graphics->background.endian);
	graphics->emoji.addr = mlx_get_data_addr(graphics->emoji.img,
			&graphics->emoji.bits_per_pixel,
			&graphics->emoji.line_length, &graphics->emoji.endian);
	return ;
}

static void	fill_frame(t_mlx_data *frame, unsigned int color)
{
	unsigned int	*pix;
	int				x;
	int				y;

	x = 0;
	while (x < frame->width)
	{
		y = 0;
		while (y < frame->height)
		{
			pix = get_mlx_pixel(frame, x, y);
			if (x < FRAME_THICKNESS || y < FRAME_THICKNESS
				|| frame->width - x <= FRAME_THICKNESS
				|| frame->height - y <= FRAME_THICKNESS)
				*pix = color;
			else
				*pix = 0xFFFFFFFF;
			++y;
		}
		++x;
	}
	return ;
}

static t_mlx_data	create_frame_img(void *mlx_ptr,
		int width, int height, unsigned int color)
{
	t_mlx_data		frame;

	frame.img = mlx_new_image(mlx_ptr, width, height);
	if (!frame.img)
		return (frame);
	frame.width = width;
	frame.height = height;
	frame.addr = mlx_get_data_addr(frame.img, &frame.bits_per_pixel,
			&frame.line_length, &frame.endian);
	fill_frame(&frame, color);
	return (frame);
}

int	open_images(t_graphics *graphics, char **argv)
{
	int	res;

	graphics->background.img = mlx_jpeg_file_to_image(graphics->mlx_ses.mlx,
			argv[1], &graphics->background.width,
			&graphics->background.height);
	graphics->emoji.img = mlx_xpm_file_to_image(graphics->mlx_ses.mlx,
			"emoji.xpm", &graphics->emoji.width, &graphics->emoji.height);
	graphics->red_frame = create_frame_img(graphics->mlx_ses.mlx,
			graphics->emoji.width, graphics->emoji.height, 0x80FF0000);
	graphics->green_frame = create_frame_img(graphics->mlx_ses.mlx,
			graphics->emoji.width, graphics->emoji.height, 0x8000FF00);
	res = graphics->background.img && graphics->red_frame.img
		&& graphics->green_frame.img && graphics->emoji.img;
	if (res)
		set_addresses(graphics);
	return (!res);
}
