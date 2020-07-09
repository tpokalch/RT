/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   central.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpokalch <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/03 20:10:51 by tpokalch          #+#    #+#             */
/*   Updated: 2019/09/30 06:26:19 by tpokalch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

void	obstructed(t_colbri *cur, t_vector hit, t_vector *hitli, t_vector reflrayv, /*t_vector nrm,*/ t_object obj, t_global *g)
{
	//gamma correct
	cur->bri = 255.0 * pow(cur->bri/255.0, 0.66);



	int i;
//	int	objn;
	t_vector nrm;
	int	iobjn[2];
	t_dstpst	t;
	t_vector ray;
	int	obsc;
	t_colbri tmp;
	int obss[g->lights]; // used in do 1 spec. obss[i] == 1 if that obect is obstructed from light[i]
	int	specscal;
	double soft[g->lights];
	int	darken[g->lights];
	t_vector obstructed;
	ft_bzero(obss, 4 * g->lights);
	init_vector(&tmp.col, 0, 0, 0);
	obsc = 0;
	i = 0;
	obj.nr = nrm;
	while(i < g->lights)
	{
		ray = hitli[i];
		iobjn[0] = 0;
		iobjn[1] = g->prim;
		while (++iobjn[0] < g->argc + 1)
		{
			if (iobjn[1] == 0)
				iobjn[1] = (iobjn[1] + 1) % (g->argc + 1);
			if (obj.id != g->obj[iobjn[1]].id || 0)
			{
				t = g->obj[iobjn[1]].hit(hit, g->li[i], ray, g->obj[iobjn[1]], g);
				if (t.dst /*/ sqrt(dot(ray, ray))*/ < 0.000001)
				{
					i++;
					break;
				}
				if (t.dst < 1)
				{
					if (obj.soft)
					{
						obstructed = sum(scale(t.dst, ray), hit);
						t_vector ctrhit = diff(obstructed,  *g->obj[iobjn[1]].ctr); 
						soft[i] = dot(norm(g->obj[iobjn[1]].get_normal(obstructed, &g->obj[iobjn[1]])), norm(ray));
						soft[i] = tothe2(soft[i], obj.soft);
//						double soft = normal_to_the_obhect * hitli, so between 0 and 1
					}
					g->prim = iobjn[1];
					obsc++;
					obss[i] = 1;
					break;
				}
			}
			iobjn[1] = (iobjn[1] + 1) % (g->argc + 1);
		}
		i++;
	}

	if (obsc > 0) //if it is obscured from at least 1 of the lights
	{
		//bug: when plane takes soft
		//shadows from sphere and the
		//light is inside sphere
		//the plane isn't obstructed fully from
		//light by sphere
		if (obj.soft) // DRAWS SOFT SHADOWS
		{
			int briscale;
			briscale = (cur->bri - g->ambient) / g->lights;
			i = -1;
			while (++i < g->lights)
			{
				cur->bri = cur->bri - briscale * soft[i];
//				when soft == 1, bri is the darkest
			}
		}//		DRAWS REGUALR SHADOWS
		else
			cur->bri = g->ambient + ((g->lights - obsc) * (cur->bri - g->ambient) / (double)g->lights);
	}


	i = -1;
	if (obj.spec)
	{
		while (++i < g->lights)
			if (obss[i] == 0)
			{
//				do_1_spec(&tmp, cur, hitli, reflrayv, obj, i, g);
		g->cosa[i] = dot(norm(hitli[i]), norm(reflrayv));
//		g->cosa[i] = g->cosa[i] - soft[i]; 
		if (g->cosa[i] > 0)
		{
			g->cosa[i] = tothe2(g->cosa[i], obj.spec);
						//dirty trick to make it look like
//						the specular white light is mixed
//						with the color after the color is mixed
//						with brightness. here it is divided by bri
//						so that it cancels out when bri * col
//						in recalc
			tmp.col = sum(tmp.col, sum(scale(255 * g->cosa[i] / cur->bri, g->white),
			scale((1 - g->cosa[i]), cur->col)));
		}
		else
//			do same thing as if cosa == 0
       		         tmp.col = sum(tmp.col, cur->col);
		}
		if (obsc < g->lights)
		{
			specscal = g->lights - obsc;
			tmp.col = scale(1 / (double)specscal, tmp.col);
			cur->col = tmp.col;
		}
	}
	if (con(g))
	{
	printf("final color is %f,%f,%f\n", cur->col.x, cur->col.y, cur->col.z);
	printf("final brigh is %d\n", cur->bri);
	}
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
	while (++i < objc)
	{
		t_crt = obj[i].hit(st, end, ray, obj[i], g);
		if ((t_crt.dst >= 0.0000001) && (!legal_hit || t_crt.dst < closest_tmp))
		{
			legal_hit = 1;
			closest_tmp = t_crt.dst;
			*ret = t_crt;
			ret->obj.cam_pos = t_crt.pst;
 		}
	}
	if (!legal_hit)
		ret->obj.name = NULL;
}

void		*toimg(void *tcp)
{
	int j;
	int	i;
 	t_colbri	bright;
	t_global *g;
	int end;
	int jwidth;

	g = tcp;
	end = (g->core + 1) * HEIGHT / CORES;
	j = g->core * HEIGHT / CORES - 1;
	jwidth = j * WIDTH;
	while ((++j < end) && (i = -1))
	{
		jwidth += WIDTH; 
		while (++i < WIDTH)
		{
//for debug
			init_vector(g->ray, i - WIDTH_2, HEIGHT_2 - j, g->ray->z);
//
			if (g->hits[j][i]->obj.name != NULL)
			{
				bright = (g->hits[j][i])->obj.
				simple_bright(*g->cam_pos, (g->hits[j][i])->hit, &(g->hits)[j][i]->obj, g);
				g->data_ptr[jwidth + i] = color(bright.bri, bright.col);
			}
		}
	}
	return (NULL);
}

void		move_row(int j, int jwidth, t_global *g)
{
		int i;
		t_dstpst ret;
		t_colbri bright;

		i = -1;
		while (++i < WIDTH)
		{
			objecthit(&ret, *g->cam_pos,
			sum(*g->rays[j][i], *g->cam_pos), g->obj, g->argc + 1,
			g);
			g->hits[j][i]->hit = sum(scale(ret.dst, *g->rays[j][i]), *g->cam_pos);
			g->hits[j][i]->obj = ret.obj;
			if (g->hits[j][i]->obj.name != NULL)
			{
				bright = ret.obj.bright(*g->cam_pos, (g->hits[j][i])->hit, &(g->hits)[j][i]->obj, g);
				g->data_ptr[jwidth + i] = color(bright.bri, bright.col);
			}
			else
				g->data_ptr[jwidth + i] = 0;			
		}
}

void		*move(void *p)
{
	t_global *g;
	int j;
	int end;
	t_dstpst ret;
	int	jwidth;

	g = (t_global *)p;
	end = (g->core + 1) * HEIGHT / CORES;
	j = g->core * HEIGHT / CORES - 1;
	jwidth = j * WIDTH;
	while (++j < end)
	{
		jwidth += WIDTH;
		move_row(j, jwidth, g);
	}
	return (NULL);
}

void		do_load(int j, t_global *g)
{

		if (g->core == 0)
			printf("core 1 = %fp\n", j / (double)(HEIGHT / (double)CORES) + 1 - g->core);
		else if (g->core == 1)
			printf("core 2 = %fp\n", j / (double)(HEIGHT / (double)CORES) + 1 - g->core);
		else if (g->core == 2)
			printf("core 3 = %fp\n", j / (double)(HEIGHT / (double)CORES) + 1 - g->core);
		else if (g->core == 3)
			printf("core 4 = %fp\n", j / (double)(HEIGHT / (double)CORES) + 1 - g->core);
		else if (g->core == 4)
			printf("core 5 = %fp\n", j / (double)(HEIGHT / (double)CORES) + 1 - g->core);
		else if (g->core == 5)
			printf("core 6 = %fp\n", j / (double)(HEIGHT / (double)CORES) + 1 - g->core);
		else if (g->core == 6)
			printf("core 7 = %fp\n", j / (double)(HEIGHT / (double)CORES) + 1 - g->core);
		else if (g->core == 7)
			printf("core 8 = %fp\n", j / (double)(HEIGHT / (double)CORES) + 1 - g->core);
}

void		recalc_row(int jwidth, int j, t_global *g)
{
		t_vector ray;
		t_dstpst ret;
		int i;
		t_colbri bright;

		i = -1;
		if (WIDTH > 2000)
			do_load(j, g);
		while (++i < WIDTH)
		{
//for debug
			init_vector(g->ray, i - WIDTH_2, HEIGHT_2 - j, g->ray->z);
//
			init_vector(&ray, i - WIDTH_2, HEIGHT_2 - j, g->ray->z);
			ray = rotate(ray, *g->angle);
			*g->rays[j][i] = ray;
			objecthit(&ret, *g->cam_pos, sum(ray, *g->cam_pos), g->obj, g->argc + 1, g);
			g->hits[j][i]->obj = ret.obj;
			g->hits[j][i]->hit = sum(scale(ret.dst, *g->rays[j][i]), *g->cam_pos);
			if (g->hits[j][i]->obj.name != NULL)
			{
				bright = g->hits[j][i]->obj.
				bright(*g->cam_pos, g->hits[j][i]->hit, &(g->hits)[j][i]->obj, g);
				g->data_ptr[jwidth + i] = color(bright.bri, bright.col);
			}
			else
				g->data_ptr[jwidth + i] = 0;
		}
}	

void		*recalc(void *p)
{
	t_global *g;
	int j;
	int end;
	int jwidth;

	g = (t_global *)p;
	end = (g->core + 1) * HEIGHT / CORES;
	j = g->core * HEIGHT / CORES - 1;
	jwidth = j * WIDTH;
	while (++j < end)
	{
		jwidth += WIDTH;
		recalc_row(jwidth, j, g);
	}
	return (NULL);
}
