/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   entex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpokalch <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/06 15:04:15 by tpokalch          #+#    #+#             */
/*   Updated: 2019/07/06 15:04:57 by tpokalch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

void	debug(t_global *g)
{
	int i;
	int j;

	i = 1;
	j = 1;
	while (j < HEIGHT - 1)
	{
		i = 1;
		while (i < WIDTH - 1)
		{
			if ((g->data_ptr[j * HEIGHT + i + 1] == 0
			&& g->data_ptr[j * HEIGHT + i - 1] == 0)
			&& g->data_ptr[(j - 1) * HEIGHT + i] == 0
			&& g->data_ptr[(j + 1) * HEIGHT + i] == 0
			&& g->data_ptr[(j + 1) * HEIGHT + i + 1] == 0
			&& g->data_ptr[(j - 1) * HEIGHT + i - 1] == 0
			&& g->data_ptr[(j + 1) * HEIGHT + i - 1] == 0
			&& g->data_ptr[(j - 1) * HEIGHT + i + 1] == 0)
				g->data_ptr[j * HEIGHT + i] = 0;
			i++;
		}
		j++;
	}
}

int		start_threads(void *f, t_global *g)
{
	int i;

	i = -1;
	while (++i < CORES)
		pthread_create(&g->tid[i], NULL, f, g->tcps[i]);
	i = -1;
	while (++i < CORES)
		pthread_join(g->tid[i], NULL);
	debug(g);
	mlx_put_image_to_window(g->mlx_ptr, g->win_ptr, g->img_ptr, 0, 0);
	return (1);
}
