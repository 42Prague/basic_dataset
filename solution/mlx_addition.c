/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mlx_addition.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljiriste <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/04 10:27:01 by ljiriste          #+#    #+#             */
/*   Updated: 2024/04/04 10:48:19 by ljiriste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "digiteq.h"
#include <mlx.h>
#include <limits.h>

unsigned int	*get_mlx_pixel(t_mlx_data *img, int x, int y)
{
	char	*res;

	res = img->addr + y * img->line_length + x * img->bits_per_pixel / CHAR_BIT;
	return ((unsigned int *)res);
}

static void	copy_image(t_mlx_data *dest, t_mlx_data *src, int x, int y)
{
	unsigned int	*dest_pix;
	unsigned int	*src_pix;
	int				i;
	int				j;

	i = 0;
	while (i + x < dest->width && i < src->width)
	{
		j = 0;
		while (j + y < dest->height && j < src->height)
		{
			dest_pix = get_mlx_pixel(dest, x + i, y + j);
			src_pix = get_mlx_pixel(src, i, j);
			*dest_pix = mix_colors(*dest_pix,
					*src_pix);
			++j;
		}
		++i;
	}
	return ;
}

//	First use has to be with the largest picture, as that initializes the image
int	display_trans_img(t_mlx_session *s, t_mlx_data *img,
		int x, int y)
{
	static t_mlx_data	to_print;

	if (!s->mlx_win)
	{
		if (to_print.img)
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
