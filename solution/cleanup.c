/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljiriste <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/04 10:24:04 by ljiriste          #+#    #+#             */
/*   Updated: 2024/04/04 10:26:04 by ljiriste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "digiteq.h"
#include "libft.h"
#include <mlx.h>
#include <stdlib.h>

int	mlx_close_win(t_mlx_session *s)
{
	mlx_destroy_window(s->mlx, s->mlx_win);
	s->mlx_win = NULL;
	return (0);
}

static void	free_session(t_mlx_session *s)
{
	mlx_destroy_display(s->mlx);
	free(s->mlx);
	return ;
}

void	cleanup(t_state *state)
{
	if (state->graph.background.img)
		mlx_destroy_image(state->graph.mlx_ses.mlx,
			state->graph.background.img);
	if (state->graph.red_frame.img)
		mlx_destroy_image(state->graph.mlx_ses.mlx,
			state->graph.red_frame.img);
	if (state->graph.green_frame.img)
		mlx_destroy_image(state->graph.mlx_ses.mlx,
			state->graph.green_frame.img);
	if (state->graph.emoji.img)
		mlx_destroy_image(state->graph.mlx_ses.mlx, state->graph.emoji.img);
	display_trans_img(&state->graph.mlx_ses, NULL, 0, 0);
	ft_vec_free(&state->found, NULL);
	free_session(&state->graph.mlx_ses);
	return ;
}
