/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   convolution.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljiriste <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/04 10:19:26 by ljiriste          #+#    #+#             */
/*   Updated: 2024/04/04 10:38:58 by ljiriste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "digiteq.h"

static unsigned int	sqr_diff(unsigned char a, unsigned char b)
{
	if (a > b)
		return ((a - b) * (a - b));
	else
		return ((b - a) * (b - a));
}

static unsigned int	uint_color_sqr_diff(unsigned int uint_color1,
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
	unsigned int	*pix_emoji;
	unsigned int	*pix_back;
	unsigned int	sqr_diff;

	sqr_diff = 0;
	i = 0;
	while (i < emoji->width)
	{
		j = 0;
		while (j < emoji->height)
		{
			pix_emoji = get_mlx_pixel(emoji, i, j);
			pix_back = get_mlx_pixel(background, x + i, y + j);
			sqr_diff += uint_color_sqr_diff(*pix_emoji, *pix_back);
			++j;
		}
		++i;
	}
	sqr_diff /= emoji->width * emoji->height;
	return (sqr_diff < EMOJI_TRESHOLD);
}
