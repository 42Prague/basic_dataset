/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   color.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljiriste <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/04 10:16:25 by ljiriste          #+#    #+#             */
/*   Updated: 2024/04/04 10:18:07 by ljiriste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "digiteq.h"

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
