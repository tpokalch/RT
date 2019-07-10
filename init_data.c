/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_data.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpokalch <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/03 15:51:47 by tpokalch          #+#    #+#             */
/*   Updated: 2019/07/06 16:01:39 by tpokalch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

void		ginit(t_global *g)
{
	int				i;
	t_objecthit		***hits;

	g->ray->z = 400;
	i = -1;
	init_vector(g->li, 102, 0, g->ray->z);
	g->liz = g->li->z;
	init_vector(&g->o01, 0, 0, 15);
	init_vector(&g->o10, 0, 1, 0);
	init_vector(g->angle, 0, 0, 0);
	init_vector(g->normal, 0, 0, 20);
	init_vector(g->cam_pos, 0, 0, 0);
	g->light_switch = 1;
	g->objn = 0;
	g->prim = 0;
	(hits) = (t_objecthit ***)malloc(sizeof(t_objecthit **) * HEIGHT + 1);
	init_hits(hits);
	g->hits = hits;
	init_rays(&g->rays);
	while (++i < CORES)
		g->tcps[i] = (t_global *)malloc(sizeof(t_global));
	copy_tcps(g);
}

void		init_plane(t_vector *ctr, int i, t_global *g)
{
	g->obj[i].name = "plane";
	g->obj[i].id = i;
	g->obj[i].hit = &hit_plane;
	g->obj[i].bright = &bright_plane;
	g->obj[i].ctr = &ctr[i];
	g->obj[i].ctr->x = 2 * i;
	g->obj[i].ctr->y = -HEIGHT / 2;
	g->obj[i].ctr->z = 0;
	g->obj[i].rd = 10;
	g->obj[i].color = 0x010100;
	g->obj[i].nr.x = 0;
	g->obj[i].nr.y = 1;
	g->obj[i].nr.z = 0;
	g->obj[i].ang.x = 0;
	g->obj[i].ang.y = 0;
	g->obj[i].ang.z = 0;
}

void		init_cylinder(t_vector *ctr, int i, t_global *g)
{
	g->obj[i].name = "cylinder";
	g->obj[i].id = i;
	g->obj[i].hit = &hit_cylinder;
	g->obj[i].bright = &bright_cylinder;
	g->obj[i].ctr = &ctr[i];
	g->obj[i].ctr->x = 0;
	g->obj[i].ctr->y = 0;
	g->obj[i].ctr->z = 600;
	g->obj[i].rd = 70;
	g->obj[i].rd2 = g->obj[i].rd * g->obj[i].rd;
	g->obj[i].color = 0x010001;
	g->obj[i].hitnow.x = 0;
	g->obj[i].ang.x = 0;
	g->obj[i].ang.y = 0;
	g->obj[i].ang.z = 0;
}

void		init_sphere(t_vector *ctr, int i, t_global *g)
{
	g->obj[i].name = "sphere";
	g->obj[i].id = i;
	g->obj[i].hit = &hit_sphere;
	g->obj[i].bright = &bright_sphere;
	g->obj[i].ctr = &ctr[i];
	g->obj[i].ctr->x = 0;
	g->obj[i].ctr->y = 0;
	g->obj[i].ctr->z = 600;
	g->obj[i].rd = 100;
	g->obj[i].rd2 = g->obj[i].rd * g->obj[i].rd;
	g->obj[i].color = 0x000101;
	g->obj[i].hitnow.x = 0;
	g->obj[i].ang.x = 0;
	g->obj[i].ang.y = 0;
	g->obj[i].ang.z = 0;
}

void		init_cone(t_vector *ctr, int i, t_global *g)
{
	g->obj[i].name = "cone";
	g->obj[i].id = i;
	g->obj[i].hit = &hit_cone;
	g->obj[i].bright = &bright_cone;
	g->obj[i].ctr = &ctr[i];
	g->obj[i].ctr->x = 0;
	g->obj[i].ctr->y = 0;
	g->obj[i].ctr->z = 600;
	g->obj[i].rd = 1;
	g->obj[i].rd2 = g->obj[i].rd * g->obj[i].rd;
	g->obj[i].color = 0x000100;
	g->obj[i].hitnow.x = 0;
	g->obj[i].ang.x = 0;
	g->obj[i].ang.y = 0;
	g->obj[i].ang.z = 0;
}
