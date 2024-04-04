/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   digiteq.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljiriste <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/04 10:14:39 by ljiriste          #+#    #+#             */
/*   Updated: 2024/04/04 11:40:32 by ljiriste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DIGITEQ_H
# define DIGITEQ_H

# include "libft.h"
# include <setjmp.h>

# include <stdio.h>
# include <jpeglib.h>

# define EMOJI_TRESHOLD 255
# define FRAME_THICKNESS 2

typedef struct s_jpeg_error
{
	struct jpeg_error_mgr	common;
	jmp_buf					setjmp_buffer;
}							t_jpeg_error;

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

typedef struct s_argb
{
	unsigned char	a;
	unsigned char	r;
	unsigned char	g;
	unsigned char	b;
}			t_argb;

t_argb			uint_to_argb(unsigned int uint_color);
unsigned int	argb_to_uint(t_argb color);
unsigned int	mix_colors(unsigned int back_uint, unsigned int front_uint);

int				emoji_encountered(t_mlx_data *background, t_mlx_data *emoji,
					int x, int y);

void			*mlx_jpeg_file_to_image(void *mlx_ptr, char *filename,
					int *width, int *height);

int				mlx_close_win(t_mlx_session *s);
void			cleanup(t_state *state);

unsigned int	*get_mlx_pixel(t_mlx_data *img, int x, int y);
int				display_trans_img(t_mlx_session *s, t_mlx_data *img,
					int x, int y);

int				open_images(t_graphics *graphics, char **argv);

int				mlx_handle_key_press(int keycode, t_mlx_session *s);
int				no_event_handle(t_state *state);

#endif //DIGITEQ_H
