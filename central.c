/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   central.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpokalch <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/03 20:10:51 by tpokalch          #+#    #+#             */
/*   Updated: 2019/07/10 20:52:53 by tpokalch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

int			obstructed(t_vector hit, t_object obj, t_global *g)
{
	int			iobjn[2];
	t_dstpst	t;

	iobjn[0] = 0;
	iobjn[1] = g->prim;
	while (++iobjn[0] < g->argc + 1)
	{
		if (iobjn[1] == 0)
			iobjn[1] = (iobjn[1] + 1) % (g->argc + 1);
		if (obj.id != g->obj[iobjn[1]].id)
		{
			t = g->obj[iobjn[1]].hit(hit, *g->li, g->obj[iobjn[1]], g);
			if (t.dst < 0.000001)
				return (0);
			if (ft_strequ(g->obj[iobjn[1]].name, "cone") &&
					inside_cone(*g->li, g->obj[iobjn[1]]) &&
					!inside_cone(*g->cam_pos, g->obj[iobjn[1]]))
			{
				if (con(g))
					printf("obstructed in cone!\n");
				return (1);
			}
			if (t.dst < 1 && (g->prim = iobjn[1]))
			{
				if (con(g))
				{
					printf("obstructed by %d %s\n", g->obj[iobjn[1]].id, g->obj[iobjn[1]].name);
				}
				return (1);
			}
		}
		iobjn[1] = (iobjn[1] + 1) % (g->argc + 1);
	}
	if (con(g))
		printf("not obstructed\n");
	return (0);
}

void		objecthit(t_objecthit *ret, t_vector ray, t_global *g)
{
	int			i;
	int			legal_hit;
	t_dstpst	t_crt;
	double		closest_tmp;

	i = 0;
	legal_hit = 0;
	while (++i < g->argc + 1)
	{
		t_crt = g->obj[i].hit(*g->cam_pos, ray, g->obj[i], g);
		if ((t_crt.dst >= 0 || t_crt.dst < 0) &&
			(!legal_hit || t_crt.dst < closest_tmp))
		{
			legal_hit = 1;
			closest_tmp = t_crt.dst;
			ret->obj = g->obj[i];
			ret->obj.cam_pos = t_crt.pst;
		}
	}
	if (!legal_hit)
		ret->obj.name = NULL;
	else
		ret->hit = sum(scale(closest_tmp, diff(ray, *g->cam_pos)), *g->cam_pos);
}

void		*toimg(void *tcp)
{
	int			j;
	int			i;
	double		bright;
	t_global	*g;
	int			end;

	g = tcp;
	end = (g->core + 1) * HEIGHT / CORES;
	j = -1;
	if (CORES > 0)
		j = g->core * HEIGHT / CORES - 1;
	while (++j < end && (i = -1))
		while (++i < WIDTH)
			if (g->hits[j][i]->obj.name != NULL)
			{
				bright = (g->hits[j][i])->obj.
				bright((g->hits[j][i])->hit, (g->hits)[j][i]->obj, g);
				g->data_ptr[j * HEIGHT + i] =
				color(bright, g->hits[j][i]->obj.color);
			}
	return (NULL);
}

void		*move(void *p)
{
	t_global	*g;
	int			i;
	int			j;
	int			end;
	int			bright;

	g = (t_global *)p;
	end = (g->core + 1) * HEIGHT / CORES;
	j = g->core * HEIGHT / CORES - 1;
	while (++j < end && (i = -1))
		while (++i < WIDTH)
		{
			objecthit(g->hits[j][i],
			sum(*g->rays[j][i], *g->cam_pos),
			g);
			if (g->hits[j][i]->obj.name != NULL)
			{
				bright = (g->hits[j][i])->obj.
				bright((g->hits[j][i])->hit, (g->hits)[j][i]->obj, g);
				g->data_ptr[j * HEIGHT + i] =
				color(bright, g->hits[j][i]->obj.color);
			}
		}
	return (NULL);
}

void		*recalc(void *p)
{
	t_global	*g;
	int			i;
	int			j;
	t_vector	ray;
	int			end;

	g = (t_global *)p;
	end = (g->core + 1) * HEIGHT / CORES;
	j = g->core * HEIGHT / CORES - 1;
	while (++j < end && (i = -1))
		while (++i < WIDTH)
		{
			init_vector(g->ray, i - WIDTH / 2, HEIGHT / 2 - j, g->ray->z);
			init_vector(&ray, i - WIDTH / 2, HEIGHT / 2 - j, g->ray->z);
			ray = rotate(ray, *g->angle);
			*g->rays[j][i] = ray;
			objecthit(g->hits[j][i], sum(ray, *g->cam_pos), g);
			if (g->hits[j][i]->obj.name != NULL)
			{
				g->data_ptr[j * HEIGHT + i] =
				color((g->hits[j][i])->obj.
				bright(g->hits[j][i]->hit, (g->hits)[j][i]->obj, g),
				g->hits[j][i]->obj.color);
			}
		}
	return (NULL);
}
