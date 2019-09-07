/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   central.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpokalch <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/03 20:10:51 by tpokalch          #+#    #+#             */
/*   Updated: 2019/07/03 20:36:09 by tpokalch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

int	obstructed(t_vector hit, t_object obj, t_global *g)
{
//	int i;
//	int	objn;
	int	iobjn[2];
	t_dstpst	t;
	t_vector ray;

	iobjn[0] = 0;
	iobjn[1] = 0/*g->prim*/;
	ray = diff(*g->li, hit);
		if (con(g))
	{
			printf("hit is at %f,%f,%f\n", hit.x, hit.y, hit.z);
			printf("li is at %f,%f,%f\n", g->li->x, g->li->y, g->li->z);
	}
	while (++iobjn[0] < g->argc + 1 /*&& (iobjn[1] = (iobjn[1] + 1) % g->argc + 1)*/)
	{

		if (iobjn[1] == 0)
			iobjn[1] = (iobjn[1] + 1) % (g->argc + 1);
		if (obj.id != g->obj[iobjn[1]].id)
		{
			t = g->obj[iobjn[1]].hit(hit, *g->li, ray, g->obj[iobjn[1]], g);
			if (t.dst < 0.000001)
				return (0);
			if (ft_strequ(g->obj[iobjn[1]].name, "cone")
				&& inside_cone(*g->li, g->obj[iobjn[1]], g)
				&& !inside_cone(*g->cam_pos, g->obj[iobjn[1]], g))
				return (1);
			if (t.dst < 1)
			{
				g->prim = iobjn[1];
				if (con(g))
				{
				printf("obstructed by %d %s\n",g->obj[iobjn[1]].id ,g->obj[iobjn[1]].name); 
				printf("dst is %f\n", t.dst);
				}
				return (1);
			}
		}
		iobjn[1] = (iobjn[1] + 1) % (g->argc + 1);
	}
	return (0);
}

void	objecthit(t_dstpst *ret, t_vector st, t_vector end, t_object *obj, int objc, t_global *g)
{
	int i;
	int legal_hit;
	t_dstpst t_crt;
	double closest_tmp;
	t_vector ray;

	i = 0;
	legal_hit = 0;
	ray = diff(end, st);
//	printf("starting objhit\n");
	if (con(g))
	{
		printf("_____start OBJECTHIT function______\n");
		printf("objc is %d\n", objc);
		printf("cam pos is %f,%f,%f\n", g->cam_pos->x, g->cam_pos->y, g->cam_pos->z);
		printf("cam angle is %f,%f,%f\n", g->angle->x, g->angle->y, g->angle->z);
	}
	while (++i < objc)
	{
		if (con(g))
		{
//			printf("i is %d\n", i);
//			printf("calling on %d %s hit\n", obj[i].id, obj[i].name);
		}
//		printf("searching obj %d\n", i);
		t_crt = obj[i].hit(st, end, ray, obj[i], g);
//		printf("hit got \n");
		if ((t_crt.dst >= 0.0000001) && (!legal_hit || t_crt.dst < closest_tmp) /*&& t_crt.dst > 0.000000*/)
		{
			if (con(g))
			{
				printf("is distance >0? %d, dist is %f\n", t_crt.dst > 0, t_crt.dst);
			}
			if (con(g))
				printf("there is legal hit\n");
			legal_hit = 1;
			closest_tmp = t_crt.dst;
			ret->obj = t_crt.obj;
			ret->dst = t_crt.dst;
			if (con(g))
				printf("we hit %d %s\n", ret->obj.id, ret->obj.name); 
			ret->obj.cam_pos = t_crt.pst;
 		}
	}
	if (!legal_hit)
	{
		if (con(g))
			printf("returning no lega hit\n");
//		ret->dst = NAN;
		ret->obj.name = NULL;
	}
	else
	{
//		ret->dst = closest_tmp;
//		ret->obj = t_crt.obj;
		if (con(g))
			printf("there was legal hit\n");
//		ret->obj = t_crt.obj;
//		ret->hit = sum(scale(closest_tmp, ray), st);
	}
	if (con(g))
	{
//		printf("hit %d %s dst is %f\n", ret->obj.id, ret->obj.name, closest_tmp);
		printf("______end OBJECTHIT function\n");
	}
}

void		*toimg(void *tcp)
{
	int j;
	int	i;
 	t_colbri	bright;
	t_global *g;
	int end;
	int jheight;

	g = tcp;
	end = (g->core + 1) * HEIGHT / CORES;
	j = -1;
	if (CORES > 0)
		j = g->core * HEIGHT / CORES - 1;

	jheight = j * HEIGHT;

	while (++j < end && (i = -1))
	{
		jheight += HEIGHT; 
		while (++i < WIDTH)
			if (g->hits[j][i]->obj.name != NULL)
			{
				bright = (g->hits[j][i])->obj.
				simple_bright(*g->cam_pos, (g->hits[j][i])->hit, (g->hits)[j][i]->obj, g);
				g->data_ptr[jheight + i] = color(bright.bri, bright.col);
			}
	}
	return (NULL);
}

void		*move(void *p)
{
	t_global *g;
	int i;
	int j;
	int end;
	t_colbri bright;
	t_dstpst ret;

	g = (t_global *)p;
	end = (g->core + 1) * HEIGHT / CORES;
	j = g->core * HEIGHT / CORES - 1;
	while (++j < end && (i = -1))
		while (++i < WIDTH)
		{
			objecthit(&ret, *g->cam_pos,
			sum(*g->rays[j][i], *g->cam_pos), g->obj, g->argc + 1,
			g);
			g->hits[j][i]->hit = sum(scale(ret.dst, *g->rays[j][i]), *g->cam_pos);
			g->hits[j][i]->obj = ret.obj;
			if (g->hits[j][i]->obj.name != NULL)
			{
				bright = (g->hits[j][i])->obj.bright(*g->cam_pos, (g->hits[j][i])->hit, (g->hits)[j][i]->obj, g);
				g->hits[j][i]->obj.bright = g->hits[j][i]->obj.simple_bright;
				g->hits[j][i]->obj.color = bright.col;
				g->data_ptr[j * HEIGHT + i] = color(bright.bri, bright.col);
			}
		}
	return (NULL);
}

void		*recalc(void *p)
{
	t_global *g;
	int i;
	int j;
	t_vector ray;
	int end;
	t_colbri bright;
	t_dstpst ret;

	g = (t_global *)p;
	end = (g->core + 1) * HEIGHT / CORES;
	j = g->core * HEIGHT / CORES - 1;
//	printf("recalcing\n");
	while (++j < end && (i = -1))
	{
		if (g->core == 0)
			printf("%fp\n", j / (double)(HEIGHT / (double)CORES));
		while (++i < WIDTH)
		{

//for debug
			init_vector(g->ray, i - WIDTH / 2, HEIGHT / 2 - j, g->ray->z);
//
			init_vector(&ray, i - WIDTH / 2, HEIGHT / 2 - j, g->ray->z);
			ray = rotate(ray, *g->angle);
			*g->rays[j][i] = ray;

			objecthit(&ret, *g->cam_pos, sum(ray, *g->cam_pos), g->obj, g->argc + 1, g);
			g->hits[j][i]->obj = ret.obj;
			g->hits[j][i]->hit = sum(scale(ret.dst, *g->rays[j][i]), *g->cam_pos);
			if (g->hits[j][i]->obj.name != NULL)
			{
//				printf("calculating bright\n");
				bright = g->hits[j][i]->obj.
				bright(*g->cam_pos, g->hits[j][i]->hit, (g->hits)[j][i]->obj, g);
//				printf("bright is %d\n", bright.bri);
				g->hits[j][i]->obj.bright = g->hits[j][i]->obj.simple_bright;
				g->hits[j][i]->obj.color = bright.col;
				g->data_ptr[j * HEIGHT + i] = color(bright.bri, bright.col);
			}
		}
	}
	return (NULL);
}
