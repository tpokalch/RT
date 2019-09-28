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

void	obstructed(t_colbri *cur, t_vector hit, t_vector *hitli, t_vector reflrayv, /*t_vector nrm,*/ t_object obj, t_global *g)
{
	int i;
//	int	objn;
	t_vector nrm;
	int	iobjn[2];
	t_dstpst	t;
	t_vector ray;
	int	obsc;
	t_colbri tmp;
	int obss[g->lights];
	int	specscal;
	double soft[g->lights];
	t_vector obstructed;
//	printf("colself in the begining %f,%f,%f\n", cur->colself.x, cur->colself.y, cur->colself.z);
	ft_bzero(obss, 4 * g->lights);
	init_vector(&tmp.col, 0, 0, 0);
	obsc = 0;
	i = 0;
	obj.nr = nrm;
	if (con(g))
	{
			printf("starting bri is %d col %f,%f,%f\n", cur->bri, cur->col.x, cur->col.y, cur->col.z);
			printf("hit is at %f,%f,%f\n", hit.x, hit.y, hit.z);
			printf("li is at %f,%f,%f\n", g->li->x, g->li->y, g->li->z);
	}
//	printf("in obstructed\n");
	while(i < g->lights)
	{
		if (con(g))
			printf("i is %d\n", i);
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
					if (con(g))
						printf("doing 1 spec %d\n", i);
					i++;
					break;
				}
/*
				if (ft_strequ(g->obj[iobjn[1]].name, "cone")
					&& inside_cone(g->li[i], g->obj[iobjn[1]], g)
					&& !inside_cone(*g->cam_pos, g->obj[iobjn[1]], g))
					obsc++;
//					return (1);
*/
				if (t.dst < 1)
				{
		//added soft haphhazard;y
					if (obj.soft)
					{
						obstructed = sum(scale(t.dst, ray), hit);
						soft[i] = dot(norm(diff(obstructed, *g->obj[iobjn[1]].ctr))/*norm(diff(obstructed, hit))*/, norm(ray));
						soft[i] = tothe2(soft[i], obj.soft);
						if (con(g))
							printf("soft is %f i is %d\n", soft[i], i);
					}
					g->prim = iobjn[1];
					if (con(g))
					{
					printf("obstructed by %d %s\n",g->obj[iobjn[1]].id ,g->obj[iobjn[1]].name); 
					printf("dst is %f, ray length is %f\n", t.dst, dot(ray, ray));
					printf("dst / ray length is %f\n", t.dst / sqrt(dot(ray, ray)));
					}
					obsc++;
					obss[i] = 1;
					break;
				}
			}
			iobjn[1] = (iobjn[1] + 1) % (g->argc + 1);
		}
		i++;
	}
	i = -1;
	if (obj.spec)
	{
		while (++i < g->lights)
		{
			if (obss[i] == 0)
			{
				if (con(g))
					printf("doing %d spec\n", i);
				do_1_spec(&tmp, cur, hit, hitli, nrm, reflrayv, obj, i, g);
			}
		}
		specscal = g->lights - obsc;
		if (obsc < g->lights)
		{
			tmp.col = scale(1 / (double)specscal, tmp.col);
			cur->col = tmp.col;
		}
	}
	if (con(g))
		printf("obstructed %d times\n", obsc);
//recently changed added * (1 - soft) + g->amb
	if (obsc > 0)
	{
//			int a;
	
		if (obj.soft)
		{
//			a = round((fmax(g->ambient, cur->bri * (1 - soft[0])) + fmax(g->ambient, cur->bri * (1 - soft[1]))) / (g->lights - obsc));
		cur->bri = fmax(g->ambient, cur->bri * (1 - soft[0])) + ((g->lights - obsc) * (cur->bri - g->ambient) / (double)g->lights);
		}
		else
			cur->bri = g->ambient + ((g->lights - obsc) * (cur->bri - g->ambient) / (double)g->lights);
	}
	if (con(g))
		printf("returning bri is %d\n", cur->bri);
//	printf("colself in the end %f,%f,%f\n", cur->colself.x, cur->colself.y, cur->colself.z);
//	return (0);
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
		if ((t_crt.dst >= 0.0000001) && (!legal_hit || t_crt.dst < closest_tmp))
		{
			if (con(g))
			{
				printf("is distance >0? %d, dist is %f\n", t_crt.dst > 0, t_crt.dst);
			}
			if (con(g))
				printf("there is legal hit\n");
			legal_hit = 1;
			closest_tmp = t_crt.dst;
//			ret->obj = t_crt.obj;
//			ret->dst = t_crt.dst;
			*ret = t_crt;
			if (con(g))
			{
				t_vector hit;

				hit =  sum(scale(ret->dst, ray), st);
				printf("we hit %d %s at %f,%f,%f\n", ret->obj.id, ret->obj.name, hit.x, hit.y, hit.z); 
				printf("dst is %f\n", ret->dst);
			}
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

int		empty_line(int current, int *line_taken)
{
	int i;

	i = current;
	while (line_taken[i])
	{
//		printf("line taken %d is %d\n", i, line_taken[i]);
		i++;
	}
	if (i >= STRIPS)
		return (-1);
	return (i);
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
	j = g->core * HEIGHT / CORES - 1;
	jheight = j * HEIGHT;
	while ((++j < end) && (i = -1))
	{
		jheight += HEIGHT; 
		while (++i < WIDTH)
			if (g->hits[j][i]->obj.name != NULL)
			{
//				printf("now object %d %s\n", g->hits[j][i]->obj.id, g->hits[j][i]->obj.name);
//				printf("color is %f,%f,%f\n", g->hits[j][i]->obj.color.z, g->hits[j][i]->obj.color.y, g->hits[j][i]->obj.color.z);
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
	int	jheight;

	g = (t_global *)p;
	end = (g->core + 1) * HEIGHT / CORES;
	j = g->core * HEIGHT / CORES - 1;
//	j = g->my_line - 1;
//	end = j + TASK + 1;
	jheight = j * HEIGHT;
	while (++j < end && (i = -1))
	{
		jheight += HEIGHT;
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
				g->hits[j][i]->obj.color = bright.colself;
				g->hits[j][i]->obj.nr = bright.nrm;
				g->data_ptr[jheight + i] = color(bright.bri, bright.col);
			}
			else
				g->data_ptr[jheight + i] = 0;			
		}
	}
/*
	if ((g->my_line = empty_line(0, g->line_taken)) != -1 && (g->line_taken[g->my_line] = 1))
	{
//		printf("core %d is executing line %d\n", g->core, g->my_line);
		g->my_line *= TASK;
//		printf("recalcing\n");
//		printf("hi there\n");
		return(recalc(g));
	}
*/
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
	int jheight;
	
//	printf("now recalcong\n");
	g = (t_global *)p;

	end = (g->core + 1) * HEIGHT / CORES;
	j = g->core * HEIGHT / CORES - 1;

//	j = g->my_line - 1;
//	end = j + TASK + 1;

//	i = -1;
//	printf("doing work %d-%d\n", j, end);
	jheight = j * HEIGHT;
	while (++j < end && (i = -1))
	{
		if (WIDTH > 2000)
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
		jheight += HEIGHT;
		while (++i < WIDTH)
		{

//for debug
			init_vector(g->ray, i - WIDTH_2, HEIGHT_2 - j, g->ray->z);
//
			init_vector(&ray, i - WIDTH_2, HEIGHT_2 - j, g->ray->z);
//			printf("rotating vector\n");
			ray = rotate(ray, *g->angle);
			*g->rays[j][i] = ray;

			objecthit(&ret, *g->cam_pos, sum(ray, *g->cam_pos), g->obj, g->argc + 1, g);
//			printf("objecthit done\n");
			g->hits[j][i]->obj = ret.obj;
			g->hits[j][i]->hit = sum(scale(ret.dst, *g->rays[j][i]), *g->cam_pos);

//			printf("obj cam pos is %d\n", ret.obj.cam_pos);
			if (g->hits[j][i]->obj.name != NULL)
			{
//				printf("now object %d %s\n", g->hits[j][i]->obj.id, g->hits[j][i]->obj.name);
//				printf("color is %f,%f,%f\n", g->hits[j][i]->obj.color.z, g->hits[j][i]->obj.color.y, g->hits[j][i]->obj.color.z);	
//				printf("calculating bright\n");
				bright = g->hits[j][i]->obj.
				bright(*g->cam_pos, g->hits[j][i]->hit, (g->hits)[j][i]->obj, g);
//				printf("bright is %d\n", bright.bri);
//				g->hits[j][i]->obj.bright = g->hits[j][i]->obj.simple_bright;
//				printf("writing %f,%f,%f inot color\n", bright.colself.x, bright.colself.y, bright.colself.z);
				g->hits[j][i]->obj.color = bright.colself;
				g->hits[j][i]->obj.nr = bright.nrm;
//				if (i == 0)
//					printf("writing line\n");
				g->data_ptr[jheight + i] = color(bright.bri, bright.col);
			}
			else
				g->data_ptr[jheight + i] = 0;
		}
	}
//	printf("core %d is executing line %d - %d says empty line is %d\n", g->core, g->my_line - 1, end, empty_line(0, g->line_taken));

/*
	if ((g->my_line = empty_line(0, g->line_taken)) != -1 && (g->line_taken[g->my_line] = 1))
	{
//		printf("core %d is executing line %d\n", g->core, g->my_line);

		g->my_line *= TASK;
//		printf("recalcing\n");
//		printf("hi there\n");
		return(recalc(g));
	}
*/
	return (NULL);
//	pthread_exit(NULL);
}
