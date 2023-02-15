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

//#if 0
void	obstructed(t_colbri *cur, t_vector hit, t_vector *hitli, t_vector reflrayv, /*t_vector nrm,*/ t_object obj, t_global *g)
{
	if (con(g))
	{
		printf("entering obstructed\n");
		printf("cur bri is %d\n", cur->bri);
	}
//	NOTE: GET RID OF ALL THE NORM(HITLI), failed the first time
//	this creates a bug: shadow at distance is not ambient
//	cur->bri = 255.0 * pow(cur->bri/255.0, 0.66); // gamma correct


	int i = 0;
//	int	objn;
	t_vector nrm;
	int j = 0;
	int objn = 0;
//	int	iobjn[2]; //iobj[0] is a counter, //objn is the number of the obj that is checked to obstruct light
	double cosa[g->lights];
	t_dstpst	t;
	t_vector ray;
	t_colbri tmp;
	int	obsc = 0; // how many ones there are in obss. from how many lights the point is shielded.
	int obss[g->lights]; // used in do 1 spec. obss[i] == 1 if that obect is obstructed from light[i]
	int	specscal;
	double soft[g->lights];
	float spec_part = 0;
	float col_part = 0; // isn't changed if obstructed form all, changed to 0 inf starting caclulate shadow
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
		j = 0;
		objn = g->prim;
		while (++j < g->argc + 1)//check to be obstructed by all objects g->obj[0] is the camera
		{
			if (objn == 0)
				objn = (objn + 1) % (g->argc + 1);
			if (obj.id != g->obj[objn].id)
			{
				t = g->obj[objn].hit(hit, g->li[i], ray, g->obj[objn], g);
//				printf("inside obstructed\n");
//				printf("hit %d %s\n", g->obj[objn].id, g->obj[objn].name);
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

//						t_vector ctrhit = diff(obstructed,  *g->obj[objn].ctr); 
//						printf("calcaulating soft\n");
//						segfailt here if object is complex. object must be a triangle
						soft[i] = fmax(soft[i], fmax(0, -dot(/*norm(*/g->obj[objn].get_normal(obstructed, &g->obj[objn])/*)*/, norm(ray))));
			//		if (obj.soft)
//						soft is the gradient from to 0 (0 shadow) to 1 (100% shadow)
						soft[i] = tothe2(soft[i], obj.soft); // change power to change hardness
						if (con(g))
							printf("soft[%d] = %f\n", i, soft[i]);

/*
						if (g->obj[objn].trans)
						{
						bright = g->obj[objn].bright(hit, obstructed, &g->obj[objn], g);
						color = scale(bright.bri, bright.col);


//							cur->col = scale(1 / (float)cur->bri, sum(scale(soft[i], color), scale(1 - soft[i], scale(cur->bri, cur->col))));
							tmp.col = sum(tmp.col, color);
//							printf("cur col is %f,%f,%f\n", cur->col.x, cur->col.y, cur->col.z); 
						}
*/
//						printf("done\n");
//						soft[i] = fmax(0, -dot(norm(g->obj[objn].get_normal(obstructed, &g->obj[objn])), norm(ray)));

//						soft[i] = dot(norm(g->obj[objn].get_normal(obstructed, &g->obj[objn])), norm(ray));
//						soft[i] = tothe2(soft[i], obj.soft);
//						soft[i] = sqrt(soft[i]);
//						soft[i] = pow(soft[i], 1.5);

//						double soft = normal_to_the_obhect * hitli, so between 0 and 1

					}
					g->prim = objn;
					obsc++;
					obss[i] = 1;
					if (!obj.soft)// if no soft shadwos need to look only at first obstruction
					{
#if 0
						if (g->obj[objn].trans)
						{
//							printf("obstructed\n");
							obstructed = sum(scale(t.dst, ray), hit);
							bright = g->obj[objn].bright(hit, obstructed, &g->obj[objn], g);
							color = scale(g->ambient/*bright.bri*/, bright.col);
								tmp.col = sum(tmp.col, color);
						}
#endif
/*						if (con(g))
							printf("tmp col is %f,%f,%f\n", tmp.col.x, tmp.col.y, tmp.col.z); 
*/
//						cur->col = scale(1 / (float)cur->bri, sum(scale(g->obj[objn].trans, color), scale(1 - g->obj[objn].trans, scale(cur->bri, cur->col))));
						break;
					}
				}
			}
			objn = (objn + 1) % (g->argc + 1);
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
	float coef = ((obsc) / (float)g->lights) * (1 - g->obj[objn].trans);
	cur->col = scale(1 / (float)cur->bri, sum(scale(1 - coef, scale(cur->bri, cur->col)), scale(coef, tmp.col)));
	init_vector(&tmp.col, 0, 0, 0);
	return ;
}
*/
////////////

//	recently moved obsc > 0 section before

	if (obsc > 0 /*&& 0*/) //if it is obscured from at least 1 of the lights
	{
		// bug: when plane takes soft
		// shadows from sphere and the
		// light is inside sphere
		// the plane isn't obstructed fully from
		// light by sphere
		// this happens because the plane knows it is obstructed by sphere
		// but it is expecting soft shadows and calculates them by multip
		// normals and hitli ray, which doesn't always equall 1 
		// we should check if objects are obstructed completetely by being
		// on the oppoite side of light in space divided by an obstruction.
		// (check if light is inside sphere)
		// this is also a reason of plane having specular reflecion
		// even when it is obstructed.

		if (con(g))
		{
			printf("____OBSTRUCTED____ obsc is %d\n", obsc);
		}
		if (obj.soft) // DRAWS SOFT SHADOWS
		{
			i = -1;
			while (++i < g->lights)
			{/*
				if (soft[i] > 0.99)
					cur->bri = g->ambient;
				else
				*/
			//	{				//must stay here, don't move
				int briscale;
				briscale = (cur->bri - g->ambient) / g->lights;
				cur->bri = cur->bri - briscale * soft[i];
//				when soft == 1, bri is the darkest
			//	}
			}
		}//		DRAWS REGUALR SHADOWS
		else
		{
			cur->bri = g->ambient + (g->lights - obsc) * (cur->bri - g->ambient) / (double)g->lights;
			if (con(g))
				printf("cur bri now is %d\n", cur->bri);
		}
	}




	i = -1;
	if (obj.spec)
	{
		while (++i < g->lights)
		{
//			if (obss[i] == 0)
//				do_1_spec(&tmp, cur, hitli, reflrayv, obj, i, g);
//			cosine of angle between normal and reflection of specular light


//			if (obss[i] == 0 || obj.soft) //because of || obj.soft the trick is done even when obstructed
//			{
				cosa[i] = dot(norm(hitli[i]),/* norm(*/reflrayv/*)*/);
//						soft ^ n for more difussion
			//if (cosa[i] > 0)
//			{
//				dirty trick to make it look like
//				the specular white light is mixed
//				with the color after the color is mixed
//				with brightness. here it is divided by bri
//				so that it cancels out when bri * col
//				in recalc
//				doesn't work when multipple hard shadows
//				tmp.col is added to itself each iteration (we are inside a loop now)
//				moved cosa[i] inside, check for bug
//				without dirty trick set cur->bri = 255.0
				if (cosa[i] < 0)
					cosa[i] = 0;
				if (obj.soft)
					cosa[i] = fmax(0, cosa[i] - soft[i] * soft[i]);
				cosa[i] = tothe2(cosa[i], obj.spec);


				if (!obj.soft && obss[i] == 1)
					cosa[i] = 0;
				if (con(g))
				{
					printf("now cosa[%d] = %f\n", i, cosa[i]);
					printf("now soft[%d] = %f\n", i, soft[i]);

				}
				spec_part += 255.0 / (float)cur->bri * cosa[i];
//				col_part += (1 - cosa[i]) / (float)(g->lights - obsc);
				col_part += (1 - cosa[i]) / (float)(g->lights);
#if 0
				if (con(g))
					printf("mixing with light\n");
				tmp.col = sum(tmp.col,
						 sum(
							scale( cosa[i] / (cur->bri / 255.0 ), g->white), /* + */
							scale((1 - cosa[i]), cur->col)));
#endif
//				tmp.col = + cosa[i] / ( cur->bri / 255.0 ) * g->white + (1 - cosa[i]) * cur->col
//			}
		/*	else
			{
//				do same thing as if cosa == 0
				if (con(g))
					printf("cosa < 0, no light spots\n");
				if (obss[i] == 0 || obj.soft)
				{
					col_part += 1.0 / (g->lights - obsc);
					if (con(g))
						printf("col part is %f\n", col_part);
//					tmp.col = sum(tmp.col, cur->col);
				}
			}*/
//		tmp.col = sum(scale(spec_part, g->spec_con), scale(col_part, cur->col));
//		}//end of (obss[i] == 0)
/*		else if (g->lights != obsc)//enters here (g->lights - obscc) times
			col_part += (1.0) / (g->lights - obsc);
*/
//		(g->lights == obsc) => col_part = 1;
		}//end loop
//#endif

/////////////////////// end previous verion
//		obstructed zones should contrubute to col part as 1 / (g->lights - obsc
		tmp.col = sum(scale(spec_part, g->spec_con), scale(col_part, cur->col));
/*		if (obsc == g->lights && !obj.soft) // otherwise intersection is black
			tmp.col = cur->col;
*/
//		printf("end loop\n");
		if (con(g))
			printf("tmp col is %f,%f,%f\n", tmp.col.x, tmp.col.y, tmp.col.z);
		if (obsc < g->lights || obj.soft)
		{
			if (obj.soft)
				specscal = g->lights ;
			else
				specscal = g->lights - obsc;
//			tmp.col = scale(1 / (double)specscal, tmp.col); //divide by the number of cycles in obj.spec
//			cur->col = tmp.col;

		}
		cur->col = tmp.col;
//	end if(obj.spec)
	}
//	gamma correct
//	doesn't work here on the specular white light
//	cur->bri = 255.0 * pow(cur->bri/255.0, 0.66);
//	printf("obstructed ended\n");

}
//#endif

void	objecthit(t_dstpst *ret, t_vector st, t_vector end, t_object *obj, int objc, t_global *g)
{
	int i;
	int legal_hit;
	t_dstpst t_crt;
	double closest_tmp;
	t_vector ray;

	i = 1;
	legal_hit = 0;
	ray = diff(end, st);
//	object with id 0 is ignored. for this reason 1 triangle in 
	if (con(g))
		printf("objc in objecthit is %d\n", objc);
	while (i < objc)
	{
		if (con(g))
		{
//			printf("tryitn hit i = %d\n", i);
		}
		t_crt = obj[i].hit(st, end, ray, obj[i], g);
		if ((t_crt.dst >= 0.0000001) && (!legal_hit || t_crt.dst < closest_tmp))
		{
//			printf("legal hit\n");
			legal_hit = 1;
			closest_tmp = t_crt.dst;
			*ret = t_crt;
			ret->obj.cam_pos = t_crt.pst;
 		}
		i++;
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
//			printf("_________________-working on pixel %d,%d\n", i, j);
//for debug
			init_vector(g->ray, i - WIDTH_2, HEIGHT_2 - j, g->ray->z);
//
//			printf("objecthit\n");
			objecthit(&ret, *g->cam_pos,
			sum(*g->rays[j][i], *g->cam_pos), g->obj, g->argc + 1,
			g);
//			printf("objecthit success. ret.obj.name = %u\n", ret.obj.name);
			g->hits[j][i]->hit = sum(scale(ret.dst, *g->rays[j][i]), *g->cam_pos);
			g->hits[j][i]->obj = ret.obj;
//			printf("hit inited\n");
			if (g->hits[j][i]->obj.name != nothing)
			{
//				printf("calling bright %u\n", ret.obj.name);
//				printf("obj ctr.x is %f\n", g->hits[j][i]->obj.ctr->x);
											// probably segfolt because of this
				bright = ret.obj.bright(*g->cam_pos, (g->hits[j][i])->hit, &((g->hits)[j][i]->obj) /*&(g->hits)[j][i]->obj*/, g);
//				printf("writing to data_ptr\n");
				g->data_ptr[jwidth + i] = color(bright.bri, bright.col);
//				printf("success\n");
			}
			else
				g->data_ptr[jwidth + i] = 0;			
//			printf("pixel complete\n");
		}
}

//	this function can be called instead of recalc when the camera 
//	isn't rotating, for example moving the object, or moving the camera without rotation
//	for light movement call toimg


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
		printf("row moved\n");
	}
	printf("move complete\n");
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
//		printf("recalcing row %d\n", j);

		t_vector ray;
		t_dstpst ret;
		int i;
		t_colbri bright;

		i = -1;
		if (WIDTH > 1000 && CORES >= 4)
			do_load(j, g);
		while (++i < WIDTH)
		{
//for debug
			init_vector(g->ray, i - WIDTH_2, HEIGHT_2 - j, g->ray->z);
//
//			printf("recalc row\n");
			if (con(g))
				printf("li at %f,%f,%f\n", g->li->x, g->li->y, g->li->z); 


			init_vector(&ray, i - WIDTH_2, HEIGHT_2 - j, g->ray->z);
			ray = rotate(ray, *g->angle);
			*g->rays[j][i] = ray;
	//		printf("objecthit from recalc\n");
	//		printf("objecthiting\n");
			objecthit(&ret, *g->cam_pos, sum(ray, *g->cam_pos), g->obj, g->argc + 1, g);
	//		printf("assign\n");
			g->hits[j][i]->obj = ret.obj;
			g->hits[j][i]->hit = sum(scale(ret.dst, *g->rays[j][i]), *g->cam_pos);
	//		printf("checking hit i is %d\njwidth is %d\n", i, jwidth);
			if (g->hits[j][i]->obj.name != nothing)
			{
	//			printf("doing pixel i, j %d,%d\n", i, j);
//				if (i == 61 && j == 14)
//					printf("hit obj name = %d id = %d\n", ret.obj.name, ret.obj.id);
				bright = g->hits[j][i]->obj.
				bright(*g->cam_pos, g->hits[j][i]->hit, &(g->hits)[j][i]->obj, g);

//				if (con(g))
	//				printf("brightness in recalc is %d\n", bright.bri);
				// specular reflection shouldn't be afected here by the unmax brightness,
				// hence the dirty trick. the color is mixed in such a way that bri * col becomes
				// own color with brightness = 255
				g->data_ptr[jwidth + i] = color(bright.bri, bright.col);
			}
			else
			{
//				if (con(g))
	//				printf("hit nothing\n");
				g->data_ptr[jwidth + i] = 0;
			}
		}
}	

void		*recalc(void *p)
{
	t_global *g;
	int j;
	int end;
	int jwidth;

//	printf("in recalc\n");
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
