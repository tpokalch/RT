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
//	NOTE: GET RID OF ALL THE NORM(HITLI), failed the first time
//	gamma correct
//	cur->bri = 255.0 * pow(cur->bri/255.0, 0.66);


	int i = 0;
//	int	objn;
	t_vector nrm;
	int	iobjn[2];
	double cosa[g->lights];
	t_dstpst	t;
	t_vector ray;
	t_colbri tmp;
	int	obsc = 0; // how many ones there are in obss. from how many lights it is shielded.
	int obss[g->lights]; // used in do 1 spec. obss[i] == 1 if that obect is obstructed from light[i]
	int	specscal;
	double soft[g->lights];
//	int	darken[g->lights];
	t_colbri bright;
	t_vector color;

	t_vector obstructed;
	ft_bzero(obss, sizeof(int) * g->lights);
	ft_bzero(soft, sizeof(double) * g->lights);
//	printf("bzero cosa (g-<ligts)\n");
	ft_bzero(cosa, sizeof(double) * g->lights);


	init_vector(&tmp.col, 0, 0, 0);
	tmp.bri = 0;
	cosa[0] = 0;
	obj.nr = nrm;
//	printf("loop\n");
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
//				printf("inside obstructed\n");
//				printf("hit %d %s\n", g->obj[iobjn[1]].id, g->obj[iobjn[1]].name);
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
//						t_vector ctrhit = diff(obstructed,  *g->obj[iobjn[1]].ctr); 
//						printf("calcaulating soft\n");
//						segfailt here if object is complex. object must be a triangle
						soft[i] = fmax(soft[i], fmax(0, -dot(norm(g->obj[iobjn[1]].get_normal(obstructed, &g->obj[iobjn[1]])), norm(ray))));
			//		if (obj.soft)
						soft[i] = tothe2(soft[i], obj.soft);
/*						if (g->obj[iobjn[1]].trans)
						{
						bright = g->obj[iobjn[1]].bright(hit, obstructed, &g->obj[iobjn[1]], g);
						color = scale(bright.bri, bright.col);


//							cur->col = scale(1 / (float)cur->bri, sum(scale(soft[i], color), scale(1 - soft[i], scale(cur->bri, cur->col))));
							tmp.col = sum(tmp.col, color);
//							printf("cur col is %f,%f,%f\n", cur->col.x, cur->col.y, cur->col.z); 
						}
*/
//						printf("done\n");
//						soft[i] = fmax(0, -dot(norm(g->obj[iobjn[1]].get_normal(obstructed, &g->obj[iobjn[1]])), norm(ray)));

//						soft[i] = dot(norm(g->obj[iobjn[1]].get_normal(obstructed, &g->obj[iobjn[1]])), norm(ray));
//						soft[i] = tothe2(soft[i], obj.soft);
//						soft[i] = sqrt(soft[i]);
//						soft[i] = pow(soft[i], 1.5);

//						double soft = normal_to_the_obhect * hitli, so between 0 and 1
					}
					g->prim = iobjn[1];
					obsc++;
					obss[i] = 1;
					if (!obj.soft)// if soft shadwos need to look on all obstructions
					{
#if 0
						if (g->obj[iobjn[1]].trans)
						{
//							printf("obstructed\n");
							obstructed = sum(scale(t.dst, ray), hit);
							bright = g->obj[iobjn[1]].bright(hit, obstructed, &g->obj[iobjn[1]], g);
							color = scale(g->ambient/*bright.bri*/, bright.col);
								tmp.col = sum(tmp.col, color);
						}
#endif
						if (con(g))
							printf("tmp col is %f,%f,%f\n", tmp.col.x, tmp.col.y, tmp.col.z); 

//						cur->col = scale(1 / (float)cur->bri, sum(scale(g->obj[iobjn[1]].trans, color), scale(1 - g->obj[iobjn[1]].trans, scale(cur->bri, cur->col))));
						break;
					}
				}
			}
			iobjn[1] = (iobjn[1] + 1) % (g->argc + 1);
		}
//		if (obj.soft)
//			soft[i] = tothe2(soft[i], obj.soft);

		i++;
	}
//	delete if no trans
/*
	if (obsc > 0)
	{
	tmp.col = scale(1 / (float)obsc, tmp.col);
	if (con(g))
		printf("tmp col is %f,%f,%f\n", tmp.col.x, tmp.col.y, tmp.col.z); 



//	tmp.col is object color
//	cur col is plane color
	float coef = ((obsc) / (float)g->lights) * (1 - g->obj[iobjn[1]].trans);
	cur->col = scale(1 / (float)cur->bri, sum(scale(1 - coef, scale(cur->bri, cur->col)), scale(coef, tmp.col)));
	init_vector(&tmp.col, 0, 0, 0);
	return ;
}
*/
////////////
	i = -1;
	if (obj.spec)
	{
		while (++i < g->lights)
//			if (obss[i] == 0)
		{
//				do_1_spec(&tmp, cur, hitli, reflrayv, obj, i, g);
			cosa[i] = dot(norm(hitli[i]),/* norm(*/reflrayv/*)*/);

//						soft ^ n for more difussion
			cosa[i] = fmax(0, cosa[i] - soft[i] * soft[i]);
			if (con(g)) {
				printf("cosa[0] is %f\n", cosa[0]);


				printf("soft[i] is %f\n", soft[i]);
				}
			if (cosa[i] > 0)
			{
/*			double cosai2 = cosa[i] * cosa[i];
			double cosai4 = cosai2 * cosai2;
*///			double m = 0.2;
//			double a = acos(cosa[i]);

//			cosa[i] = exp(-a * a/ (m * m));
//			cosa[i] =  1/(M_PI * m * m * cosai4) * exp(-1/(m * m) * (1/cosai2 - 1));
				cosa[i] = tothe2(cosa[i], obj.spec);


			if (con(g))
			{
				printf("cosa[%d] > 0\n", i);
				printf("cosa[%d] is %f\n", i, cosa[i]);
			}
				//dirty trick to make it look like
//				the specular white light is mixed
//				with the color after the color is mixed
//				with brightness. here it is divided by bri
//				so that it cancels out when bri * col
//				in recalc
//				doesn't work when multipple hard shadows
//			tmp.col = sum(tmp.col, sum(scale(255 * cosa[i] / (cur->bri * g->lights), g->white),
//				if (obss[i] != 1)
//				tmp.col is added to itself each iteration (we are inside a loop now)
				if (obss[i] != 1 || obj.soft)
/*					tmp.col = sum(tmp.col,
						sum(
							scale(cosa[i], g->white),
							scale(1 - cosa[i], cur->col)
											));
*/
//#if 0
				tmp.col = sum(tmp.col,
						 sum(
							scale(cosa[i] / (cur->bri / (255.0 )), g->white), /* + */
							scale((1 - cosa[i]), cur->col)
									)
					);

//#endif
	
//				tmp.col = sum(tmp.col, sum(scale(/*255 * */cosa[i] /*/ (cur->bri)*/, g->white),
//					scale((1 - cosa[i]), cur->col)));
	
			}
			else
			{
//				do same thing as if cosa == 0
				if (con(g))
					printf("cosa < 0, no light spots\n");
				if (obss[i] != 1 || obj.soft)
					tmp.col = sum(tmp.col, cur->col);
			}
		}//end loop
//		printf("end loop\n");
//		tmp.col = scale(1 / (double)g->lights, tmp.col);
		if (con(g))
			printf("tmp col is %f,%f,%f\n", tmp.col.x, tmp.col.y, tmp.col.z);
		if (obsc < g->lights || obj.soft)
		{
			if (!obj.soft)
				specscal = g->lights - obsc;
			else
				specscal = g->lights;
			tmp.col = scale(1 / (double)specscal, tmp.col);
			cur->col = tmp.col;
		}
	}
	if (obsc > 0 /*&& 0*/) //if it is obscured from at least 1 of the lights
	{
		// bug: when plane takes soft
		// shadows from sphere and the
		// light is inside sphere
		// the plane isn't obstructed fully from
		// light by sphere
		if (obj.soft) // DRAWS SOFT SHADOWS
		{
			i = -1;
			while (++i < g->lights)
			{
				//must stay here, don't move
				int briscale;
				briscale = (cur->bri - g->ambient) / g->lights;
				cur->bri = cur->bri - briscale * soft[i];
//				when soft == 1, bri is the darkest
			}
		}//		DRAWS REGUALR SHADOWS
		else
			cur->bri = g->ambient + ((g->lights - obsc) * (cur->bri - g->ambient) / (double)g->lights);
	}

	if (con(g))
	{
		printf("obstruction counter is %d\n", obsc);
		printf("cosa[0] is %f\n", cosa[0]);
		printf("final color is %f,%f,%f\n", cur->col.x, cur->col.y, cur->col.z);
		printf("final brigh is %d\n", cur->bri);
	}
//	gamma correct
//	cur->bri = 255.0 * pow(cur->bri/255.0, 0.66);


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
//		printf("tryitn hit\n");
		t_crt = obj[i].hit(st, end, ray, obj[i], g);
		if ((t_crt.dst >= 0.0000001) && (!legal_hit || t_crt.dst < closest_tmp))
		{
//			printf("legal hit\n");
			legal_hit = 1;
			closest_tmp = t_crt.dst;
			*ret = t_crt;
			ret->obj.cam_pos = t_crt.pst;
 		}
	}
	if (!legal_hit)
		ret->obj.name = nothing;
//	printf("hit object %d %s\n", ret->obj.id, ret->obj.name);
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
//	if (con(g))
//		printf("li is %f,%f,%f\n", g->li->x, g->li->x, g->li->z);

	while ((++j < end) && (i = -1))
	{
		jwidth += WIDTH; 
		while (++i < WIDTH)
		{
//for debug
			init_vector(g->ray, i - WIDTH_2, HEIGHT_2 - j, g->ray->z);
//
			if (g->hits[j][i]->obj.name != nothing)
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
//for debug
			init_vector(g->ray, i - WIDTH_2, HEIGHT_2 - j, g->ray->z);
//
			objecthit(&ret, *g->cam_pos,
			sum(*g->rays[j][i], *g->cam_pos), g->obj, g->argc + 1,
			g);
			g->hits[j][i]->hit = sum(scale(ret.dst, *g->rays[j][i]), *g->cam_pos);
			g->hits[j][i]->obj = ret.obj;
			if (g->hits[j][i]->obj.name != nothing)
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
			printf("core 1 = %fp\n", j / (double)(HEIGHT / (double)CORES)  - g->core);
		else if (g->core == 1)
			printf("core 2 = %fp\n", j / (double)(HEIGHT / (double)CORES)  - g->core);
		else if (g->core == 2)
			printf("core 3 = %fp\n", j / (double)(HEIGHT / (double)CORES)  - g->core);
		else if (g->core == 3)
			printf("core 4 = %fp\n", j / (double)(HEIGHT / (double)CORES)  - g->core);
		else if (g->core == 4)
			printf("core 5 = %fp\n", j / (double)(HEIGHT / (double)CORES)  - g->core);
		else if (g->core == 5)
			printf("core 6 = %fp\n", j / (double)(HEIGHT / (double)CORES)  - g->core);
		else if (g->core == 6)
			printf("core 7 = %fp\n", j / (double)(HEIGHT / (double)CORES)  - g->core);
		else if (g->core == 7)
			printf("core 8 = %fp\n", j / (double)(HEIGHT / (double)CORES)  - g->core);
}

void		recalc_row(int jwidth, int j, t_global *g)
{
		t_vector ray;
		t_dstpst ret;
		int i;
		t_colbri bright;

		i = -1;
//		if (WIDTH > 2000)
			do_load(j, g);
		while (++i < WIDTH)
		{
//for debug
			init_vector(g->ray, i - WIDTH_2, HEIGHT_2 - j, g->ray->z);
//
			if (con(g))
				printf("li at %f,%f,%f\n", g->li->x, g->li->y, g->li->z); 


			init_vector(&ray, i - WIDTH_2, HEIGHT_2 - j, g->ray->z);
			ray = rotate(ray, *g->angle);
			*g->rays[j][i] = ray;
//			printf("objecthit from recalc\n");

			objecthit(&ret, *g->cam_pos, sum(ray, *g->cam_pos), g->obj, g->argc + 1, g);
//			printf("assign\n");
			g->hits[j][i]->obj = ret.obj;
			g->hits[j][i]->hit = sum(scale(ret.dst, *g->rays[j][i]), *g->cam_pos);
//			printf("checking hit\n");
			if (g->hits[j][i]->obj.name != nothing)
			{
//				printf("doing pixel i, j %d,%d\n", i, j);
//				if (i == 61 && j == 14)
//					printf("hit obj %d %d\n", ret.obj.name, ret.obj.id);
				bright = g->hits[j][i]->obj.
				bright(*g->cam_pos, g->hits[j][i]->hit, &(g->hits)[j][i]->obj, g);

				if (con(g))
					printf("brightness in recalc is %d\n", bright.bri);
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
