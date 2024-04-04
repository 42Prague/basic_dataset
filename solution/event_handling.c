/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   event_handling.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljiriste <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/04 10:31:51 by ljiriste          #+#    #+#             */
/*   Updated: 2024/04/04 10:38:04 by ljiriste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "digiteq.h"
#include <mlx.h>
#include <X11/keysym.h>

int	mlx_handle_key_press(int keycode, t_mlx_session *s)
{
	if (keycode == XK_Escape)
		mlx_close_win(s);
	return (0);
}

static void	print_green_frames(t_state *state)
{
	t_position	pos;
	size_t		i;

	i = 0;
	while (i < state->found.size)
	{
		pos = *(t_position *)ft_vec_access(&state->found, i);
		display_trans_img(&state->graph.mlx_ses,
			&state->graph.green_frame, pos.x, pos.y);
		++i;
	}
	return ;
}

static void	record_found(t_vec *found, t_position pos)
{
	ft_vec_append(found, &pos);
	ft_printf("Target at (%4i, %4i).\n", pos.x, pos.y);
	return ;
}

int	no_event_handle(t_state *state)
{
	t_graphics *const	g = &state->graph;

	if (!g->mlx_ses.mlx_win)
		return (0);
	if (state->pos.x + g->emoji.width > g->background.width)
	{
		state->pos.x = 0;
		++state->pos.y;
	}
	if (state->pos.y + g->emoji.height <= g->background.height)
	{
		display_trans_img(&g->mlx_ses, &g->background, 0, 0);
		print_green_frames(state);
		display_trans_img(&g->mlx_ses, &g->red_frame,
			state->pos.x, state->pos.y);
		if (emoji_encountered(&g->background, &g->emoji,
				state->pos.x, state->pos.y))
			record_found(&state->found, state->pos);
		++state->pos.x;
	}
	else
		mlx_close_win(&g->mlx_ses);
	return (0);
}
