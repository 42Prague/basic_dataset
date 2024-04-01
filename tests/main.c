/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okraus <okraus@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/01 15:57:24 by okraus            #+#    #+#             */
/*   Updated: 2024/04/01 17:26:54 by okraus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <jpeglib.h>
#include <jerror.h>
#include <GL/glut.h>

typedef struct s_image
{
	unsigned int	width;
	unsigned int	height;
	unsigned int	line;
	unsigned int	size;
	unsigned int	alpha;
	unsigned int	channels;
	unsigned char	*pixels;
}	t_image;

typedef struct s_point
{
	unsigned int	x;
	unsigned int	y;
	unsigned long	value;
}	t_point;

typedef struct s_solver
{
	t_image			img;
	t_image			emoji;
	unsigned long	time;
	t_point			current;
	t_point			bestmatchsofar;
	t_point			perfectmatch;
} t_solver;

t_solver g_solver;

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

int	init_data(char *imgpath, char *emojipath)
{
	g_solver.img = read_jpeg(imgpath);
	g_solver.emoji = read_jpeg(emojipath);
	printf("%u %u\n", g_solver.img.size, g_solver.emoji.size);
	g_solver.current.x = 0;
	g_solver.current.y = 0;
	g_solver.bestmatchsofar.value = 0xFFFFFFFFFFFFFFFF;
	return (0);
}

int	abs_difference(int a, int b)
{
	if (b > a)
		return (b - a);
	return (a - b);
}

unsigned long	calculatevalue(int xi, int yi)
{
	unsigned long	value;
	int				diff;

	value = 0;
	for (int y = 0; y < 50; y++)
	{
		for (int x = 0; x < 50; x++)
		{
			value += abs_difference(g_solver.img.pixels[(yi + y) * (g_solver.img.line) + ((xi + x) * 3) + 0], g_solver.emoji.pixels[y * g_solver.emoji.line + (x * 3) + 0]);
			value += abs_difference(g_solver.img.pixels[(yi + y) * (g_solver.img.line) + ((xi + x) * 3) + 1], g_solver.emoji.pixels[y * g_solver.emoji.line + (x * 3) + 1]);
			value += abs_difference(g_solver.img.pixels[(yi + y) * (g_solver.img.line) + ((xi + x) * 3) + 2], g_solver.emoji.pixels[y * g_solver.emoji.line + (x * 3) + 2]);
		}
	}
	return (value);
}

void	solve(void)
{
	int	w;
	int	h;

	w = g_solver.img.width - g_solver.emoji.width;
	h = g_solver.img.height - g_solver.emoji.height;
	for (int y = 0; y < h; y++)
	{
		for (int x = 0; x < h; x++)
		{
			g_solver.current.x = x;
			g_solver.current.y = y;
			g_solver.current.value = calculatevalue(x, y);
			if (g_solver.current.value < g_solver.bestmatchsofar.value)
			{
				g_solver.bestmatchsofar = g_solver.current;
			}
		}
		printf("line %3i/%3i scanned \n", y, h);
	}
	printf("x = %u, y = %u, dif = %lu\n", g_solver.bestmatchsofar.x, g_solver.bestmatchsofar.y, g_solver.bestmatchsofar.value);
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT);
	for(int y=639;y>=0;y--)
	{
		for(int x=0;x<640;x++)
		{
			glColor3f((x % 256) / 255., 0, ((640 - y) % 256) / 255.);
			glPointSize(8);
			glBegin(GL_POINTS);
			glVertex2i(x,y);
			glEnd();
		}
	}
	glFlush();
	glutSwapBuffers();
}

int	main(int argc, char** argv)
{
	if (argc != 3)
		return (1);
	if (init_data(argv[1], argv[2]))
		return (2);
	solve();
	glutInit(&argc,	argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(640,640);
	glutInitWindowPosition(200,200);
	glutCreateWindow("OpenGL");
	glutDisplayFunc(display);
	gluOrtho2D(0,640,0,640);
	glClearColor(0.5,0.7,0.5,0);
	glutMainLoop();
	return 0;
}