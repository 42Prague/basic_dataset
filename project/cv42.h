/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cv42.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okraus <okraus@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/03 15:08:04 by okraus            #+#    #+#             */
/*   Updated: 2024/04/03 19:36:29 by okraus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CV42_H

# define CV42_H
# include <stdlib.h>
# include <stdbool.h>
# include <stdio.h>
# include <strings.h>
# include <jpeglib.h>
# include <jerror.h>
# include <GL/glut.h>
# include <time.h>
# include <sys/time.h>

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
	time_t			time;
	time_t			oldms;
	time_t			newms;
	t_point			current;
	t_point			bestmatchsofar;
	t_point			perfectmatch;
	t_point			samples[16];
	int				finished;
	char			imgpath[4096];
	char			emojipath[4096];
} t_solver;

extern t_solver	g_solver;

# define THRESHHOLD 64

# define RED 0
# define YELLOW 1
# define GREEN 2

time_t	ft_get_time_in_ms(void);
t_image read_jpeg(const char *path);
void	init_samples(void);
int		init_data(char *imgpath, char *emojipath);
int		abs_difference(int a, int b);
int		errors(int value);

#endif