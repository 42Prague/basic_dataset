/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okraus <okraus@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/01 15:57:24 by okraus            #+#    #+#             */
/*   Updated: 2024/04/03 20:02:22 by okraus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cv42.h"

t_solver	g_solver;

//calculates the difference of 16 sample pixels
unsigned long	calculatesamplevalue(int xi, int yi)
{
	int	value;

	value = 0;
	for (int i = 0; i < 16; i++)
	{
		value += abs_difference(g_solver.img.pixels[(yi + g_solver.samples[i].y) * (g_solver.img.line) + ((xi + g_solver.samples[i].x) * g_solver.img.channels) + 0], g_solver.emoji.pixels[g_solver.samples[i].y * g_solver.emoji.line + (g_solver.samples[i].x * g_solver.emoji.channels) + 0]);
		value += abs_difference(g_solver.img.pixels[(yi + g_solver.samples[i].y) * (g_solver.img.line) + ((xi + g_solver.samples[i].x) * g_solver.img.channels) + 1], g_solver.emoji.pixels[g_solver.samples[i].y * g_solver.emoji.line + (g_solver.samples[i].x * g_solver.emoji.channels) + 1]);
		value += abs_difference(g_solver.img.pixels[(yi + g_solver.samples[i].y) * (g_solver.img.line) + ((xi + g_solver.samples[i].x) * g_solver.img.channels) + 2], g_solver.emoji.pixels[g_solver.samples[i].y * g_solver.emoji.line + (g_solver.samples[i].x * g_solver.emoji.channels) + 2]);
	}
	return (value);
}

//simple difference of pixels calculation
unsigned long	calculatevalue(int xi, int yi)
{
	unsigned long	value;

	value = calculatesamplevalue(xi, yi);
	//if the differene of the 16 pixels is too big, there is no point calculating the difference for every pixel
	if (value > THRESHHOLD)
		return (value * g_solver.emoji.size);
	value = 0;
	//calculates the difference of all pixels, it is done for red, green and blue channel separately
	for (int y = 0; y < 50; y++)
	{
		for (int x = 0; x < 50; x++)
		{
			//skips the transparency pixels, I think:)
			if (g_solver.emoji.alpha && g_solver.emoji.pixels[y * g_solver.emoji.line + (x * g_solver.emoji.channels) + 3] < 255)
				continue ;
			else
			{
				value += abs_difference(g_solver.img.pixels[(yi + y) * (g_solver.img.line) + ((xi + x) * g_solver.img.channels) + 0], g_solver.emoji.pixels[y * g_solver.emoji.line + (x * g_solver.emoji.channels) + 0]);
				value += abs_difference(g_solver.img.pixels[(yi + y) * (g_solver.img.line) + ((xi + x) * g_solver.img.channels) + 1], g_solver.emoji.pixels[y * g_solver.emoji.line + (x * g_solver.emoji.channels) + 1]);
				value += abs_difference(g_solver.img.pixels[(yi + y) * (g_solver.img.line) + ((xi + x) * g_solver.img.channels) + 2], g_solver.emoji.pixels[y * g_solver.emoji.line + (x * g_solver.emoji.channels) + 2]);
			}
		}
	}
	return (value);
}

//main solving algorithm, goes through the image pixel by pixel
void	solve(void)
{
	int	w;
	int	h;
	int	x;
	int	y;

	w = g_solver.img.width - g_solver.emoji.width;
	h = g_solver.img.height - g_solver.emoji.height;
	for (y = g_solver.current.y; y <= h; y++)
	{
		for (x = g_solver.current.x; x <= w; x++)
		{
			g_solver.current.x = x;
			g_solver.current.y = y;
			//calculates the difference of the area
			g_solver.current.value = calculatevalue(x, y);
			//if the difference is smaller than before, sets the best match so far
			//as the current value
			if (g_solver.current.value < g_solver.bestmatchsofar.value)
			{
				g_solver.bestmatchsofar = g_solver.current;
			}
		}
		g_solver.current.x = 0;
	}
	//sets up the solving as finished and prints the results
	if (! g_solver.finished)
	{
		printf("Image: %s | Emoji: %s\n", g_solver.imgpath, g_solver.emojipath);
		printf("Coordinates: (%u, %u)\n", g_solver.bestmatchsofar.x, g_solver.bestmatchsofar.y);
		g_solver.finished = 1;
	}
}

int	main(int argc, char** argv)
{
	if (argc != 3)
		return (errors(1));
	if (init_data(argv[1], argv[2])) //we need two arguments
		return (errors(2));
	if (g_solver.img.width < g_solver.emoji.width || g_solver.img.height < g_solver.emoji.height) // image cannot be smaller than the emoji we look for
		return (errors(3));
	//save paths to images to print later
	snprintf(g_solver.imgpath, 4095, "%s", argv[1]);
	snprintf(g_solver.emojipath, 4095, "%s", argv[2]);
	//main solving algorithm
	solve();
	free(g_solver.img.pixels);
	free(g_solver.emoji.pixels);
	return 0;
}