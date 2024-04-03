/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okraus <okraus@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/03 19:03:34 by okraus            #+#    #+#             */
/*   Updated: 2024/04/03 19:54:12 by okraus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cv42.h"

//t_solver	g_solver;

//write an error message and return the error value
int	errors(int value)
{
	if (value == 1)
	{
		printf("Usage:\n");
		printf("./42CV <path_to_jpg_image> <path_to_jpg_emoji>\n");
	}
	if (value == 2)
	{
		dprintf(2, "Error: Could not process the jpg image\n");
	}
	if (value == 3)
	{
		dprintf(2, "Error: image is not bigger than the emoji.\n");
	}
	return(value);
}


//gets time in miliseconds
time_t	ft_get_time_in_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}


//reads jpeg and stores important values in my global structure
t_image read_jpeg(const char *path)
{
	t_image	image;
	FILE	*file = fopen(path, "rb");
	if (file == NULL)
	{
		return ((t_image){0, 0, 0, 0, 0, 0, NULL});
	}
	struct jpeg_decompress_struct info; //for our jpeg info
	struct jpeg_error_mgr err; //the error handler
	info.err = jpeg_std_error(&err);
	jpeg_create_decompress(&info); //fills info structure
	jpeg_stdio_src(&info, file);
	jpeg_read_header(&info, true);
	jpeg_start_decompress(&info);
	image.width = info.output_width;
	image.height = info.output_height;
	image.channels = info.num_components; // 3 = RGB, 4 = RGBA
	image.line = image.width * image.channels;
	if (image.channels == 4)
		image.alpha = 1;
	else
		image.alpha = 0;
	image.size = image.width * image.height * image.channels;
	image.pixels = (unsigned char *)malloc(image.size);
	unsigned char* rowptr;
	while (info.output_scanline < image.height)
	{
		rowptr = image.pixels  + info.output_scanline * image.width * image.channels;
		jpeg_read_scanlines(&info, &rowptr, 1);
	}
	jpeg_finish_decompress(&info);
	jpeg_destroy_decompress(&info);   
	fclose(file);
	return (image);
}

//randomly takes 16 pixels that differ in colour
//(black and white ones in the dataset,
//the colour calcualtion is not done properly,
//because the difference between colour pixels is 0 in black and white image)
void	init_samples(void)
{
	int	r[2][16];
	int	g[2][16];
	int	b[2][16];
	int	x;
	int	y;

	for (int i = 0; i < 16; i++)
	{
		x = rand() % g_solver.emoji.width;
		y = rand() % g_solver.emoji.height;
		g_solver.samples[i].x = x;
		g_solver.samples[i].y = y;
		r[0][i] = g_solver.emoji.pixels[g_solver.samples[i].y * g_solver.emoji.line + (g_solver.samples[i].x * g_solver.emoji.channels) + 0];
		g[0][i] = g_solver.emoji.pixels[g_solver.samples[i].y * g_solver.emoji.line + (g_solver.samples[i].x * g_solver.emoji.channels) + 1];
		b[0][i] = g_solver.emoji.pixels[g_solver.samples[i].y * g_solver.emoji.line + (g_solver.samples[i].x * g_solver.emoji.channels) + 2];
		for (int j = 0; j < 1024; j++)
		{
			x = rand() % g_solver.emoji.width;
			y = rand() % g_solver.emoji.height;
			r[1][i] = g_solver.emoji.pixels[y * g_solver.emoji.line + (x * g_solver.emoji.channels) + 0];
			g[1][i] = g_solver.emoji.pixels[y * g_solver.emoji.line + (x * g_solver.emoji.channels) + 1];
			b[1][i] = g_solver.emoji.pixels[y * g_solver.emoji.line + (x * g_solver.emoji.channels) + 2];
			//find the blackest pixel in emoji
			if (i == 0)
			{
				if (r[1][i] + g[1][i] + b[1][i] <= r[0][i] + g[0][i] + b[0][i])
				{
					g_solver.samples[i].x = x;
					g_solver.samples[i].y = y;
					r[0][i] = r[1][i];
					g[0][i] = g[1][i];
					b[0][i] = b[1][i];
				}
			}
			//find the least black pixel in emoji
			else if (i == 1)
			{
				if (r[1][i] + g[1][i] + b[1][i] >= r[0][i] + g[0][i] + b[0][i])
				{
					g_solver.samples[i].x = x;
					g_solver.samples[i].y = y;
					r[0][i] = r[1][i];
					g[0][i] = g[1][i];
					b[0][i] = b[1][i];
				}
			}
			//find the whitest pixel in emoji
			else if (i == 2)
			{
				if (r[1][i] + g[1][i] + b[1][i] >= r[0][i] + g[0][i] + b[0][i])
				{
					g_solver.samples[i].x = x;
					g_solver.samples[i].y = y;
					r[0][i] = r[1][i];
					g[0][i] = g[1][i];
					b[0][i] = b[1][i];
				}
			}
			//find the least white pixel in emoji
			else if (i == 3)
			{
				if (r[1][i] + g[1][i] + b[1][i] <= r[0][i] + g[0][i] + b[0][i])
				{
					g_solver.samples[i].x = x;
					g_solver.samples[i].y = y;
					r[0][i] = r[1][i];
					g[0][i] = g[1][i];
					b[0][i] = b[1][i];
				}
			}
			//find the most red pixel in emoji
			else if (i == 4)
			{
				if (r[1][i] >= r[0][i])
				{
					g_solver.samples[i].x = x;
					g_solver.samples[i].y = y;
					r[0][i] = r[1][i];
					g[0][i] = g[1][i];
					b[0][i] = b[1][i];
				}
			}
			//find the least red pixel in emoji
			else if (i == 5)
			{
				if (r[1][i] <= r[0][i])
				{
					g_solver.samples[i].x = x;
					g_solver.samples[i].y = y;
					r[0][i] = r[1][i];
					g[0][i] = g[1][i];
					b[0][i] = b[1][i];
				}
			}
			//find the most green pixel in emoji
			else if (i == 6)
			{
				if (g[1][i] >= g[0][i])
				{
					g_solver.samples[i].x = x;
					g_solver.samples[i].y = y;
					r[0][i] = r[1][i];
					g[0][i] = g[1][i];
					b[0][i] = b[1][i];
				}
			}
			//find the least green pixel in emoji
			else if (i == 7)
			{
				if (g[1][i] <= g[0][i])
				{
					g_solver.samples[i].x = x;
					g_solver.samples[i].y = y;
					r[0][i] = r[1][i];
					g[0][i] = g[1][i];
					b[0][i] = b[1][i];
				}
			}
			//find the most blue pixel in emoji
			else if (i == 8)
			{
				if (b[1][i] >= b[0][i])
				{
					g_solver.samples[i].x = x;
					g_solver.samples[i].y = y;
					r[0][i] = r[1][i];
					g[0][i] = g[1][i];
					b[0][i] = b[1][i];
				}
			}
			//find the least blue pixel in emoji
			else if (i == 9)
			{
				if (b[1][i] <= b[0][i])
				{
					g_solver.samples[i].x = x;
					g_solver.samples[i].y = y;
					r[0][i] = r[1][i];
					g[0][i] = g[1][i];
					b[0][i] = b[1][i];
				}
			}
			//find the most cyan pixel in emoji
			else if (i == 10)
			{
				if (g[1][i] + b[1][i] >= g[0][i] + b[0][i])
				{
					g_solver.samples[i].x = x;
					g_solver.samples[i].y = y;
					r[0][i] = r[1][i];
					g[0][i] = g[1][i];
					b[0][i] = b[1][i];
				}
			}
			//find the least cyan pixel in emoji
			else if (i == 11)
			{
				if (g[1][i] + b[1][i] <= g[0][i] + b[0][i])
				{
					g_solver.samples[i].x = x;
					g_solver.samples[i].y = y;
					r[0][i] = r[1][i];
					g[0][i] = g[1][i];
					b[0][i] = b[1][i];
				}
			}
			//find the most magenta pixel in emoji
			else if (i == 12)
			{
				if (r[1][i] + b[1][i] >= r[0][i] + b[0][i])
				{
					g_solver.samples[i].x = x;
					g_solver.samples[i].y = y;
					r[0][i] = r[1][i];
					g[0][i] = g[1][i];
					b[0][i] = b[1][i];
				}
			}
			//find the least magenta pixel in emoji
			else if (i == 13)
			{
				if (r[1][i] + b[1][i] <= r[0][i] + b[0][i])
				{
					g_solver.samples[i].x = x;
					g_solver.samples[i].y = y;
					r[0][i] = r[1][i];
					g[0][i] = g[1][i];
					b[0][i] = b[1][i];
				}
			}
			//find the most yellow pixel in emoji
			else if (i == 14)
			{
				if (r[1][i] + g[1][i] >= r[0][i] + g[0][i])
				{
					g_solver.samples[i].x = x;
					g_solver.samples[i].y = y;
					r[0][i] = r[1][i];
					g[0][i] = g[1][i];
					b[0][i] = b[1][i];
				}
			}
			//find the least yellow pixel in emoji
			else if (i == 15)
			{
				if (r[1][i] + g[1][i] <= r[0][i] + g[0][i])
				{
					g_solver.samples[i].x = x;
					g_solver.samples[i].y = y;
					r[0][i] = r[1][i];
					g[0][i] = g[1][i];
					b[0][i] = b[1][i];
				}
			}
		}
		g_solver.samples[i].x = x;
		g_solver.samples[i].y = y;
	}
}

//this function initialises the global structure
int	init_data(char *imgpath, char *emojipath)
{
	g_solver.img = read_jpeg(imgpath);
	g_solver.emoji = read_jpeg(emojipath);
	//printf("%u %u\n", g_solver.img.size, g_solver.emoji.size);
	if (g_solver.img.size == 0 || g_solver.emoji.size == 0)
		return (1);
	g_solver.current.x = 0;
	g_solver.current.y = 0;
	g_solver.finished = 0;
	g_solver.newms = ft_get_time_in_ms(); //gets time for the bonus part to display where I am searching at certain intervals
	g_solver.time = 0;
	g_solver.bestmatchsofar.x = 0;
	g_solver.bestmatchsofar.y = 0;
	g_solver.bestmatchsofar.value = 0xFFFFFFFFFFFFFFFF;
	init_samples();
	return (0);
}

//just calculates the absolute value of difference of two integers
int	abs_difference(int a, int b)
{
	if (b > a)
		return (b - a);
	return (a - b);
}
