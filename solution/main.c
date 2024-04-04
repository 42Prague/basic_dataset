/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljiriste <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/02 08:44:53 by ljiriste          #+#    #+#             */
/*   Updated: 2024/04/04 10:38:20 by ljiriste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "digiteq.h"
#include "libft.h"
#include <mlx.h>
#include <X11/X.h>

int	init_state(t_state *state)
{
	state->graph.mlx_ses.mlx = mlx_init();
	state->pos.x = 0;
	state->pos.y = 0;
	return (ft_vec_init(&state->found, sizeof(t_position)) != success
		|| !state->graph.mlx_ses.mlx);
}

void	display(char **argv)
{
	t_state				state;

	if (!init_state(&state) && !open_images(&state.graph, argv))
	{
		state.graph.mlx_ses.mlx_win = mlx_new_window(state.graph.mlx_ses.mlx,
				state.graph.background.width,
				state.graph.background.height, argv[1]);
		mlx_hook(state.graph.mlx_ses.mlx_win, KeyPress, KeyPressMask,
			mlx_handle_key_press, &state.graph.mlx_ses);
		mlx_hook(state.graph.mlx_ses.mlx_win, DestroyNotify, NoEventMask,
			mlx_close_win, &state.graph.mlx_ses);
		mlx_loop_hook(state.graph.mlx_ses.mlx, no_event_handle, &state);
		display_trans_img(&state.graph.mlx_ses, &state.graph.background, 0, 0);
		mlx_loop(state.graph.mlx_ses.mlx);
	}
	cleanup(&state);
	return ;
}

int	main(int argc, char **argv)
{
	if (argc != 2)
		return (1);
	display(argv);
	return (0);
}
