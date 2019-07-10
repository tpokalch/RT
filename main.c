/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpokalch <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/06 14:49:14 by tpokalch          #+#    #+#             */
/*   Updated: 2019/07/10 20:49:41 by tpokalch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

int			con(t_global *g)
{
	return (shot.x == g->ray->x && shot.y == g->ray->y);
}

void		free_arr(char **arr)
{
	int i;

	i = 0;
	while (*(arr + i) != NULL)
	{
		free(*(arr + i));
		i++;
	}
}

int			putstr(char *s, int ret)
{
	int i;

	i = 0;
	while (*(s + i) != '\0')
	{
		write(1, s + i, 1);
		i++;
	}
	return (ret);
}
int			mouse_press(int b, int x, int y, void *p)
{
	t_global *g;

	g = p;
	printf("\nshot is %d, %d\n", x, y);
	if (b == 1)
	{
		shot.x = -WIDTH / 2 + x;
		shot.y = (HEIGHT / 2 - y);
	}
	return (start_threads(recalc, g));
}

int			main(int argc, char **argv)
{
	t_global g;
	t_vector ctr[argc];
	t_vector kenobi[5];

	shot.x = -WIDTH;
	g.cam_pos = &kenobi[0];
	g.angle = &kenobi[1];
	g.ray = &kenobi[2];
	g.li = &kenobi[3];
	g.normal = &kenobi[4];
	if (!check_arg(argv, argc, &g, ctr))
		return (0);
	g.mlx_ptr = mlx_init();
	g.img_ptr = mlx_new_image(g.mlx_ptr, WIDTH, HEIGHT);
	g.data_ptr = (int *)mlx_get_data_addr(g.img_ptr, &g.bpp, &g.sz_l, &g.e);
	g.win_ptr = mlx_new_window(g.mlx_ptr, WIDTH, HEIGHT, "window1");
	ginit(&g);
	start_threads(recalc, &g);
	mlx_hook(g.win_ptr, 4, 4, mouse_press, &g);	
	mlx_hook(g.win_ptr, 2, 2, key_press, &g);
	mlx_hook(g.win_ptr, 6, 6, mouse_move, &g);
	mlx_loop(g.mlx_ptr);
}
