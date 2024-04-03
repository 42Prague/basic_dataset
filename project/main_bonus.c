/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okraus <okraus@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/01 15:57:24 by okraus            #+#    #+#             */
/*   Updated: 2024/04/03 18:45:14 by okraus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cv42.h"

t_solver g_solver;

time_t	ft_get_time_in_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

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

// White
// Black
// Green
// Blue
// red
// cyan
// yellow
// magenta


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
		y = rand() % g_solver.emoji.width;
		g_solver.samples[i].x = x;
		g_solver.samples[i].y = y;
		r[0][i] = g_solver.emoji.pixels[g_solver.samples[i].y * g_solver.emoji.line + (g_solver.samples[i].x * g_solver.emoji.channels) + 0];
		g[0][i] = g_solver.emoji.pixels[g_solver.samples[i].y * g_solver.emoji.line + (g_solver.samples[i].x * g_solver.emoji.channels) + 1];
		b[0][i] = g_solver.emoji.pixels[g_solver.samples[i].y * g_solver.emoji.line + (g_solver.samples[i].x * g_solver.emoji.channels) + 2];
		for (int j = 0; j < 1024; j++)
		{
			x = rand() % g_solver.emoji.width;
			y = rand() % g_solver.emoji.width;
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
			//find the leaast black pixel in emoji
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

int	init_data(char *imgpath, char *emojipath)
{
	g_solver.img = read_jpeg(imgpath);
	g_solver.emoji = read_jpeg(emojipath);
	//printf("%u %u\n", g_solver.img.size, g_solver.emoji.size);
	g_solver.current.x = 0;
	g_solver.current.y = 0;
	g_solver.finished = 0;
	g_solver.newms = ft_get_time_in_ms();
	g_solver.time = 0;
	g_solver.bestmatchsofar.x = 0;
	g_solver.bestmatchsofar.y = 0;
	g_solver.bestmatchsofar.value = 0xFFFFFFFFFFFFFFFF;
	init_samples();
	return (0);
}

int	abs_difference(int a, int b)
{
	if (b > a)
		return (b - a);
	return (a - b);
}

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



unsigned long	calculatevalue(int xi, int yi)
{
	unsigned long	value;

	value = calculatesamplevalue(xi, yi);
	if (value > THRESHHOLD)
		return (value * g_solver.emoji.size);
	value = 0;
	for (int y = 0; y < 50; y++)
	{
		for (int x = 0; x < 50; x++)
		{
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

void	solve(void)
{
	int	w;
	int	h;
	int	x;
	int	y;
	//break to show current result

	w = g_solver.img.width - g_solver.emoji.width;
	h = g_solver.img.height - g_solver.emoji.height;
	for (y = g_solver.current.y; y <= h; y++)
	{
		for (x = g_solver.current.x; x <= w; x++)
		{
			g_solver.oldms = g_solver.newms;
			g_solver.newms = ft_get_time_in_ms();
			g_solver.time += g_solver.newms - g_solver.oldms;
			if (g_solver.time > 50)
			{
				g_solver.time = 0;
				return ;
			}
			g_solver.current.x = x;
			g_solver.current.y = y;
			g_solver.current.value = calculatevalue(x, y);
			if (g_solver.current.value < g_solver.bestmatchsofar.value)
			{
				g_solver.bestmatchsofar = g_solver.current;
			}
			if (x % 50 == 0 && y % 50 == 0)
			{
				g_solver.time = 0;
				++g_solver.current.x;
				return ;
			}
		}
		g_solver.current.x = 0;
		//printf("line %3i/%3i scanned \n", y, h);
	}
	if (! g_solver.finished)
	{
		printf("Image: %s | Emoji: %s\n", g_solver.imgpath, g_solver.emojipath);
		printf("Coordinates: (%u, %u)\n", g_solver.bestmatchsofar.x, g_solver.bestmatchsofar.y);
		g_solver.finished = 1;
	}
}

void	draw_rectangle(int	state)
{
	int	x;
	int	y;

	x = g_solver.bestmatchsofar.x;
	y = g_solver.bestmatchsofar.y;
	if (state == RED)
	{
		glColor3f(1, 0, 0);
		x = g_solver.current.x;
		y = g_solver.current.y;
	}
	else if (state == YELLOW)
		glColor3f(1, 1, 0);
	else if (state == GREEN)
		glColor3f(0, 1, 0);
	glPointSize(8);
	glLineWidth(4);
	glBegin(GL_LINES); 
	glVertex2i(x, g_solver.img.height - y); 
	glVertex2i(x + g_solver.emoji.width, g_solver.img.height - y);
	glEnd();
	glBegin(GL_LINES); 
	glVertex2i(x + g_solver.emoji.width, g_solver.img.height - y);
	glVertex2i(x + g_solver.emoji.width, g_solver.img.height - y - g_solver.emoji.height);
	glEnd();
	glBegin(GL_LINES); 
	glVertex2i(x, g_solver.img.height - y); 
	glVertex2i(x, g_solver.img.height - y - g_solver.emoji.height);
	glEnd();
	glBegin(GL_LINES); 
	glVertex2i(x, g_solver.img.height - y - g_solver.emoji.height);
	glVertex2i(x + g_solver.emoji.width, g_solver.img.height - y - g_solver.emoji.height);
	glEnd();
}

void display()
{
	//calculate for 100 ms
	//display current position
	int	r;
	int	g;
	int	b;
	int	a;
	solve();
	if (g_solver.finished < 2)
	{
		glutPostRedisplay();
		if (g_solver.finished)
			++g_solver.finished;
	}
	glClear(GL_COLOR_BUFFER_BIT);
	for(int y = g_solver.img.height; y >= 0; y--)
	{
		for(unsigned int x = 0; x < g_solver.img.width; x++)
		{
			r = g_solver.img.pixels[(g_solver.img.height - y) * (g_solver.img.line) + (x * g_solver.img.channels) + 0];
			g = g_solver.img.pixels[(g_solver.img.height - y) * (g_solver.img.line) + (x * g_solver.img.channels) + 1];
			b = g_solver.img.pixels[(g_solver.img.height - y) * (g_solver.img.line) + (x * g_solver.img.channels) + 2];
			(void)a;
			glColor3f(r / 255., g / 255., b / 255.);
			glPointSize(1);
			glBegin(GL_POINTS);
			glVertex2i(x, y);
			glEnd();
		}
	}
	if (g_solver.finished)
		draw_rectangle(GREEN);
	else
	{
		draw_rectangle(YELLOW);
		draw_rectangle(RED);
	}
	glFlush();
	glutSwapBuffers();
}

int	main(int argc, char** argv)
{
	if (argc != 3)
		return (1);
	if (init_data(argv[1], argv[2])) //we need two arguments
		return (2);
	if (g_solver.img.width < g_solver.emoji.width || g_solver.img.height < g_solver.emoji.height) // image cannot be smaller than the emoji we look for
		return (3);
	// solve();
	snprintf(g_solver.imgpath, 4095, "%s", argv[1]);
	snprintf(g_solver.emojipath, 4095, "%s", argv[2]);
	glutInit(&argc,	argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(g_solver.img.width, g_solver.img.height);
	glutInitWindowPosition(200,200);
	glutCreateWindow("OpenGL");
	glutDisplayFunc(display);
	gluOrtho2D(0,g_solver.img.width,0,g_solver.img.height);
	glClearColor(0.5,0.7,0.5,0);
	glutMainLoop();
	free(g_solver.img.pixels);
	free(g_solver.emoji.pixels);
	return 0;
}