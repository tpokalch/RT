/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   brights.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpokalch <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/03 20:37:08 by tpokalch          #+#    #+#             */
/*   Updated: 2019/09/30 04:48:37 by tpokalch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

t_colbri		refl(t_vector refl, t_vector hit, t_vector nrm, t_object obj, t_global *g)
{
	t_dstpst temp;
	t_objecthit reflobj;
	t_colbri ret;

	objecthit(&temp, hit, sum(refl, hit), g->obj, g->argc + 1, g);
	reflobj.hit = sum(scale(temp.dst, refl), hit);
	reflobj.obj = temp.obj;
	if (reflobj.obj.name == NULL)
	{
		init_vector(&ret.col, 0, 0, 0);
		ret.bri = 0;
		return (ret);
	}
	ret = reflobj.obj.bright(hit, reflobj.hit, reflobj.obj, g);
	ret.col = scale(ret.bri / (double)255, ret.col);
	return (ret);
}

void		do_re(t_vector reflrayv, t_vector *reocol, t_vector tileocol, t_vector hit, t_vector nrm, t_object obj, t_global *g)
{
		t_vector reflrayv1;
		t_colbri reflcolbri;

		reflrayv1 = reflrayv;
		if (g->recursion[obj.id] < MAX_REC)
		{
			reflcolbri = refl(reflrayv1, hit, nrm, obj, g);
			*reocol = reflcolbri.col;
			*reocol = sum(scale(1 - obj.re, tileocol), scale(obj.re, reflcolbri.col/*base255(scale(reflcolbri.bri, reflcolbri.col))*/));
		}
		else
			*reocol = tileocol;
}

t_colbri		trans(t_vector st, t_vector hit, t_vector nrm, t_object obj, t_global *g)
{
	t_dstpst temp;
	t_colbri ret;
	t_objecthit	transobj;
	t_vector ray;

	ray = diff(hit, st);
	objecthit(&temp, hit, sum(ray, hit), g->obj, g->argc + 1, g);
	transobj.hit = sum(scale(temp.dst, ray), hit);
	transobj.obj = temp.obj;
//
	transobj.obj.cam_pos = 0;
//??
	if (transobj.obj.name == NULL)
	{
		init_vector(&ret.col, 0, 0, 0);
		ret.bri = 0;
		return (ret);
	}
	ret = transobj.obj.bright(hit, transobj.hit, transobj.obj, g);
	return (ret);
}

t_vector		reflray(t_vector st, t_vector end, t_vector nrm, t_global *g)
{
	t_vector ray;
	t_vector refl;
	t_vector rayx;

	ray = diff(end, st);
	rayx = diff(ray, scale(dot(ray, nrm), nrm));
	refl = diff(scale(2, rayx), ray);
	return (refl);
}

void		do_trans(t_vector st, t_vector hit, t_colbri *ret, t_colbri reo, t_vector nrm, t_object obj, t_global *g)
{
		t_colbri transo;

		if (g->recursion[obj.id] > MAX_REC)
			return ;
		transo = trans(st, hit, nrm, obj, g);
		transo.col = base255(scale(transo.bri, transo.col));
		transo.col = sum(scale(1 - obj.trans, reo.col), scale(obj.trans, transo.col));
		ret->col = transo.col;

		ret->bri = transo.bri;
}




t_vector		mip_col(double x, double y, double dst2, t_object obj, t_global *g)
{
	int		tilelow;
	int		tilehi;
	t_tile		*ti;
	double		newres;
	double		tilen;
	t_vector		ret;

	newres = g->ray->z / (double)sqrt(dst2) * obj.tile[0].w;
	ti = obj.tile;

	tilen = log2(obj.tile[0].w / newres) + 13;
	if (con(g))
	{
		printf("square distange to plane is %f\n", dst2);
		printf("distance to plane is %f\n", sqrt(dst2));

		printf("tile is %f\n", tilen);
		printf("newrsi is %f\n", newres);
	}	

	tilelow = floor(tilen);
	tilehi = ceil(tilen);
	double weight[2];

	weight[0] = 1 - (tilen - tilelow);
	weight[1] = 1 - (tilehi - tilen);

	if (tilehi > ti[0].mipq || tilelow > ti[0].mipq)
	{
		tilen = ti[0].mipq;
		tilehi = ti[0].mipq;
		tilelow = ti[0].mipq;
	}

	if (tilehi < 0 || tilelow < 0)
	{
		tilen = 0;
		tilehi = 0;
		tilelow = 0;
	}


	if (tilehi > 6 || tilelow > 6)
	{
		tilehi = 6;
		tilen = 6;
		weight[0] = 1;
		weight[1] = 0;
	}
	if (tilehi < 0 || tilelow < 0)
	{
		tilehi = 0;
		tilen = 0;
		weight[1] = 1;
		weight[0] = 0;
	}

	tilen = floor(tilen);


	if (tilen < 0)
		tilen = 0;
	else if (tilen > ti[0].mipq)
		tilen = ti[0].mipq;


//	printf("x y is %d,%d\n", x, y);

//	printf("weight low %f\n", (1 - (tilen - tilelow)));


	if (con(g))
	{
//		printf("low weight is %f\n", weight[0]);
//		printf("high weight is %f\n", weight[1]);
	}

/*
	ret.col = sum(scale(weight[0], base255(rgb(*(ti[tilelow].data_ptr + ti[tilelow].w * x + y)))),
		scale(weight[1], base255(rgb(*(ti[tilehi].data_ptr + ti[tilehi].w * x + y)))));
*/

	t_vector col[4];
	t_vector colhi[4];
	double	colweight[4];
	double colhiweight[4];
//the next two colours are colours of the pixel hit int the behind tile and the forward tile	
	col[0] = base255(rgb(*(ti[tilelow].data_ptr + ti[tilelow].w * (int)floor(x) + (int)floor(y))));
	col[1] = base255(rgb(*(ti[tilehi].data_ptr + ti[tilehi].w * (int)floor(x) + (int)floor(y))));

//the next four colours are colours of the four closest pixels to a given hit into a lower tile

/*
	col[0] = base255(rgb(*(ti[tilelow].data_ptr + ti[tilelow].w * (int)floor(x) + (int)floor(y))));
	col[1] = base255(rgb(*(ti[tilelow].data_ptr + ti[tilelow].w * (int)ceil(x) + (int)ceil(y))));
	col[2] = base255(rgb(*(ti[tilelow].data_ptr + ti[tilelow].w * (int)ceil(x) + (int)floor(y))));
	col[3] = base255(rgb(*(ti[tilelow].data_ptr + ti[tilelow].w * (int)floor(x) + (int)ceil(y))));
*/

//the next 4 are the coresponding weights depending on the distance of hit to each pixel

/*
	colweight[0] = sqrt((1 - (x - floor(x))) * (1 -(x - floor(x))) + (1 - (y - floor(y))) * (1 - (y - floor(y)))) / (double)8;
	colweight[1] = sqrt(1 - ((ceil(x) - x)) * (1 - (ceil(x) - x)) + (1 - (ceil(y) - y)) * (1 - (ceil(y) - y))) / (double)8;
	colweight[2] = sqrt(1 - ((ceil(x) - x)) * (1 - (ceil(x) - x)) + (1 - (y - floor(y))) * (1 - (y - floor(y)))) / (double)8;
	colweight[3] = sqrt(1 - ((floor(x) - x)) * (1 - (x - floor(x))) + (1 - (ceil(y) - y)) * (1 - (ceil(y) - y))) / (double)8;
*/

//the same thing for a higher tile

/*
	colhi[0] = base255(rgb(*(ti[tilehi].data_ptr + ti[tilehi].w * (int)floor(x) + (int)floor(y))));
	colhi[1] = base255(rgb(*(ti[tilehi].data_ptr + ti[tilehi].w * (int)ceil(x) + (int)ceil(y))));
	colhi[2] = base255(rgb(*(ti[tilehi].data_ptr + ti[tilehi].w * (int)ceil(x) + (int)floor(y))));
	colhi[3] = base255(rgb(*(ti[tilehi].data_ptr + ti[tilehi].w * (int)floor(x) + (int)ceil(y))));

	colhiweight[0] = sqrt((1 - (x - floor(x))) * (1 -(x - floor(x))) + (1 - (y - floor(y))) * (1 - (y - floor(y)))) / (double)8;
	colhiweight[1] = sqrt(1 - ((ceil(x) - x)) * (1 - (ceil(x) - x)) + (1 - (ceil(y) - y)) * (1 - (ceil(y) - y))) / (double)8;
	colhiweight[3] = sqrt(1 - (x - (floor(x))) * (1 - (x - floor(x))) + (1 - (ceil(y) - y)) * (1 - (ceil(y) - y))) / (double)8;
	colhiweight[2] = sqrt(1 - ((ceil(x) - x)) * (1 - (ceil(x) - x)) + (1 - (y - floor(y))) * (1 - (y - floor(y)))) / (double)8;

*/
//here are 2 retcols for each tile - back tile and forward tile
/*
	t_vector retcol[2];
	retcol[0] = sum(scale(colweight[0],col[0]), sum(scale(colweight[1],col[1]), sum(scale(colweight[2],col[2]), scale(colweight[3], col[3]))));


	retcol[1] = sum(scale(colhiweight[0],colhi[0]), sum(scale(colhiweight[1],colhi[1]), sum(scale(colhiweight[2],colhi[2]), scale(colhiweight[3], colhi[3]))));

	ret = sum(scale(weight[0], retcol[0]), scale(weight[1], retcol[1]));
	return (ret);
*/
	if (con(g))
		printf("colweight 0 %f\ncolweight 1 %f\n", colweight[0], colweight[1]);


//	ret.col = base255(rgb(*(ti[0].data_ptr + ti[0].w * x + y)));
//	ret.col = base255(rgb(*(ti[(int)tilen].data_ptr + ti[(int)tilen].w * x + y)));


//	ret.col = sum(scale(colweight[0], col[0]), scale(colweight[1], col[1]));

	ret = sum(scale(weight[0], col[0]), scale(weight[1], col[1]));



	if (con(g))
		printf("color is %f,%f,%f\n", ret.x, ret.y, ret.z);

	return (ret);	
}

void		init_hitli(t_vector *hitli, t_vector hit, t_global *g)
{
	int i;

	i = 0;
	while (i < g->lights)
	{
		hitli[i] = diff(g->li[i], hit);
		i++;
	}
}

void		init_bri(int *bri, t_vector *hitli, t_vector nrm, t_global *g)
{
	int i;

	i = 0;
	*bri = 0;
	while(i < g->lights)
	{
		*bri += fmax(round(255 * dot(norm(hitli[i]), nrm)), g->ambient);
		i++;
	}
	*bri = round(*bri / (double)g->lights);
}

int		inside_cone(t_vector p, t_object obj, t_global *g)
{
	double axdst;
	double cirad;
	t_vector ptoaxproj;
	t_vector ptoaxperp;
	t_vector op;
	t_global *a;

	a = g;
	op = diff(p, *obj.ctr);
	if (dot(op, op) < 0.001)
		return (0);
	axdst = dot(op, obj.base[1]);
	cirad = axdst * obj.rd2;
	ptoaxproj = scale(axdst, obj.base[1]);
	ptoaxperp = diff(op, ptoaxproj);
	return (dot(ptoaxperp, ptoaxperp) - cirad * cirad <= 0);
}


t_colbri	simple_bright_cone(t_vector st, t_vector hit, t_object obj, t_global *g)
{
	t_vector	nrm;
//	t_vector	camforw;
//	t_vector	hit0;
	t_colbri			ret;
//	t_vector	ax;
	t_vector	reflrayv;
	int		retobs;
	t_vector	hitli[g->lights];
	int		i;
	t_colbri	retorig;

//	printf("inside cone\n");
//	hit0 = diff(hit, *obj.ctr);
//	ax = scale(dot(hit0, obj.base[1]) * (1 + obj.rd2), obj.base[1]);

	nrm = obj.nr;

//	printf("obj color is %f,%f,%f\n", obj.color.x, obj.color.y, obj.color.z);
	init_hitli(hitli, hit, g);
	if (inside_cone(*g->cam_pos, obj, g))
	{
		if (!inside_cone(*g->li, obj, g))
		{
			ret.bri = g->ambient;
//			return (ret);
		}
	}
	else if (inside_cone(*g->li, obj, g))
	{
		ret.bri = g->ambient;
//		return (ret);
	}
	else
		init_bri(&ret.bri, hitli, nrm, g);
	ret.col = obj.color;	
	if (obj.spec || obj.re)
		reflrayv = reflray(st, hit, nrm, g);
	if (obj.re)
		do_re(reflrayv, &ret.col, ret.col, hit, nrm, obj, g);
	if (obj.trans)
		do_trans(st, hit, &ret, ret, nrm, obj, g);
	obstructed(&ret, hit, hitli, reflrayv, obj, g);
//	printf("returning %f,%f,%f bri %d\n", ret.col.x, ret.col.y, ret.col.z, ret.bri);
	return (ret);
}

void		do_tile_cone(t_colbri *ret, t_vector ctrhit, t_vector hit, t_object obj, t_global *g)
{
		double x;
		double y;

		t_vector proj;


//	t_vector ctrhit;

//	printf("hi\n");
//	ctrhit = diff(hit, *obj.ctr);

	proj = diff(ctrhit, scale(dot(obj.base[1], ctrhit),obj.base[1]));
	proj = norm(proj);

	if (con(g))
	{
		printf("proj is %f,%f,%f\n", proj.x, proj.y, proj.z);
		printf("width is %d\n", obj.tile[0].w);
		printf("acos is %f\n", acos(dot(obj.base[0], proj)));
	}
//	x = lround(obj.tile[0].w * M_2_PI * (M_PI + (1 - 2 * (det(proj, obj.base[0]) < 0)) * acos(dot(obj.base[0], proj))));
//	x = obj.tile[0].w2 * /*(1 - (1 - 2 * (det(proj, obj.base[0]) < 0)) **/ M_1_PI * myacos(dot(proj, obj.base[0]));
	x = obj.tile[0].w2 * (/*1 - (1 - 2 * (det(proj, obj.base[2]) < 0))*/ 1 * M_1_PI * myacos(proj, obj.base[2], obj.base[1], g));




	y = mymod(1 - dot(obj.base[1], diff(hit, *obj.ctr)), obj.tile[0].h);
	if (con(g))
	{
//		printf("x is %d\n", x);
//		printf("y is %d\n", y);
	}	
	ret->col = *(obj.tile[0].vectile + lround(y) * obj.tile[0].w + lround(x));
}

t_colbri	bright_cone(t_vector st, t_vector hit, t_object obj, t_global *g)
{
	t_vector	hit0;
	t_colbri	ret;
	t_vector	 hitli[g->lights];
	t_vector	reflrayv;

	hit0 = diff(hit, *obj.ctr);
	obj.nr = norm(diff(hit0, scale(dot(hit0, obj.base[1]) * (1 + obj.rd2), obj.base[1])));
	init_hitli(hitli, hit, g);
	if (inside_cone(*g->cam_pos, obj, g))
	{
		obj.nr = scale(-1, obj.nr);
		if (!inside_cone(*g->li, obj, g))
			ret.bri = g->ambient;
		else
			init_bri(&ret.bri, hitli, obj.nr, g);
	}
	else
		init_bri(&ret.bri, hitli, obj.nr, g);
	ret.nrm = obj.nr;
	if (obj.tile[0].data_ptr)
		do_tile_cone(&ret, hit0, hit, obj, g);
	else
		ret.col = obj.color;
	ret.colself = ret.col;
	if (obj.re || obj.spec)
		reflrayv = reflray(st, hit, obj.nr, g);
	if (obj.re)
		do_re(reflrayv, &ret.col, ret.col, hit, obj.nr, obj, g);
	if (obj.trans)
		do_trans(st, hit, &ret, ret, obj.nr, obj, g);
	obstructed(&ret, hit, hitli, reflrayv, obj, g);
	return (ret);
}

t_colbri	simple_bright_cylinder(t_vector st, t_vector hit, t_object obj, t_global *g)
{
	t_vector	nrm;
	t_colbri	ret;
	t_dstpst	t;
	t_vector	hitli[g->lights];
	t_vector	reflrayv;

	nrm = obj.nr;
	init_hitli(hitli, hit, g);
	if (obj.cam_pos && (t = obj.hit(*g->cam_pos, *g->li, diff(*g->li, *g->cam_pos),obj, g)).dst < 1)
		ret.bri = g->ambient;
	else
		init_bri(&ret.bri, hitli, nrm, g);
	ret.col = obj.color;
	if (obj.spec || obj.re)
		reflrayv = reflray(st, hit, nrm, g);
	if (obj.re)
		do_re(reflrayv, &ret.col, ret.col, hit, nrm, obj, g);
	if (obj.trans)
		do_trans(st, hit, &ret, ret, nrm, obj, g);
	obstructed(&ret, hit, hitli, reflrayv, obj, g);
	return (ret);
}

void	do_tile_cyl(t_vector *ret, t_vector hit, t_vector ctrhit, t_object *obj, t_global *g)
{
		double x;
		double y;
		t_vector proj;
		double xdst;

		proj = diff(ctrhit, scale(dot(obj->base[1], ctrhit),obj->base[1]));
		proj = norm(proj);
		x = obj->tile[0].w2 * (1 - (1 - 2 * (det(proj, obj->base[0]) < 0)) * M_1_PI * acos(dot(proj, obj->base[0])));
		xdst = dot(obj->base[1], diff(hit, *obj->ctr));
			x = obj->tile[0].w2 * /* (1 - (1 - 2 * (det(proj, obj->base[0]) < 0)) */ M_1_PI * myacos(proj, obj->base[0], obj->base[1], g);
			y = myintmod(xdst, obj->tile[0].h);
			obj->color = *(obj->tile[0].vectile + lround(y) * obj->tile[0].w + lround(x));
			*ret = obj->color;
}


t_colbri	bright_cylinder(t_vector st, t_vector hit, t_object obj, t_global *g)
{
	t_vector vrt;
	t_dstpst t;
	t_vector	ctrhit;
	t_colbri		ret;
	t_vector	hitli[g->lights];
	t_vector	reflrayv;

	ctrhit = diff(hit, *obj.ctr);
	vrt = scale(dot(ctrhit, obj.base[1]), obj.base[1]);
	obj.nr = norm(diff(ctrhit, obj.base[1]));

	g->recursion[obj.id]++;
	init_hitli(hitli, hit, g);
	if (obj.cam_pos)
	{
		t = obj.hit(*g->cam_pos, *g->li, diff(*g->li, *g->cam_pos), obj, g);
		obj.nr = scale(-1, obj.nr);
		if (t.dst < 1)
			ret.bri = g->ambient;
		else
			init_bri(&ret.bri, hitli, obj.nr, g);
	}
	else
		init_bri(&ret.bri, hitli, obj.nr, g);
	ret.nrm = obj.nr;
	if (obj.tile[0].data_ptr)
		do_tile_cyl(&ret.col, hit, ctrhit, &obj, g);
	else
		ret.col = obj.color;
	ret.colself = obj.color;
	if (obj.spec || obj.re)
		reflrayv = reflray(st, hit, obj.nr, g);
	if (obj.re)
		do_re(reflrayv, &ret.col, ret.col, hit, obj.nr, obj, g);
	if (obj.trans)
		do_trans(st, hit, &ret, ret, obj.nr, obj, g);
	obstructed(&ret, hit, hitli, reflrayv, obj, g);
	g->recursion[obj.id] = 0;
	return (ret);
}

void		do_1_spec(t_colbri *tmp, t_colbri *ret, t_vector hit, t_vector *hitli, t_vector nrm, t_vector reflrayv, t_object obj, int i, t_global *g)
{
	if (con(g))
	{
		printf("doing spec\n");
		printf("tmp is %f,%f,%f\n", tmp->col.x, tmp->col.y, tmp->col.z);
	}
	g->cosa[i] = dot(norm(hitli[i]),norm(reflrayv));
	if (g->cosa[i] > 0)
	{
		g->cosa[i] = tothe2(g->cosa[i], obj.spec);
		tmp->col = sum(tmp->col, sum(scale(g->cosa[i], g->white), scale((1 - g->cosa[i]), ret->col)));
	}
	else
		tmp->col = sum(tmp->col, ret->col);
}

void		do_spec(t_colbri *ret, t_vector hit, t_vector nrm, t_vector reflrayv, t_object obj, t_global *g)
{
//		t_vector reflrayv;
		double	cosa;
		double cosa3;
		int	i;
		t_colbri tmp;
		t_vector hitli[g->lights];

		i = -1;
		while (++i < g->lights)
			hitli[i] = diff(g->li[i], hit);
//		reflrayv = reflray(st, hit, nrm, g);
		init_vector(&tmp.col, 0, 0, 0);
		i = -1;
		while (++i < g->lights)
			do_1_spec(&tmp, ret, hit, hitli, nrm, reflrayv, obj, i, g);
		tmp.col = scale(1 / (double)g->lights, tmp.col);
		ret->col = tmp.col;	
}

t_colbri	simple_bright_sphere(t_vector st, t_vector hit, t_object obj, t_global *g)
{
	t_colbri		ret;
	t_vector reflrayv;
	t_vector hitli[g->lights];
	t_vector ctrli;

	init_hitli(hitli, hit, g);
	if (obj.cam_pos)
	{
		ctrli = diff(*g->li, *obj.ctr);
		if ((dot(ctrli, ctrli) > obj.rd2) && (ret.bri = g->ambient))
			ret.col = obj.color;
	}
	else
		init_bri(&ret.bri, hitli, obj.nr, g);
	ret.col = obj.color;
	if (obj.spec || obj.re)
		reflrayv = reflray(st, hit, obj.nr, g);
	if (obj.re)
		do_re(reflrayv, &ret.col, ret.col, hit, obj.nr, obj, g);
	if (obj.trans)
		do_trans(st, hit, &ret, ret, obj.nr, obj, g);
	obstructed(&ret, hit, hitli, reflrayv, obj, g);
	return (ret);
}


t_vector		do_tile_sphere(t_vector *tileocol, t_vector st, t_vector hit, t_object *obj, t_global *g)
{
		t_vector ctrhit;
		t_vector proj;
		double x;
		double y;

		ctrhit = diff(hit, *obj->ctr);
		proj = diff(ctrhit, scale(dot(obj->base[1], ctrhit),obj->base[1]));
		proj = norm(proj);
		y = obj->tile[0].h * (1/*1 - 2 * (det(nrm, obj->base[1]) < 0)*/) * M_1_PI * acos(dot(obj->nr, obj->base[1]));
		x = obj->tile[0].w2 * (/*1 - (1 - 2 * (det(proj, obj->base[2]) < 0))*/ 1 * M_1_PI * myacos(proj, obj->base[2], obj->base[1], g));

//	if (round(x) >= obj->tile[0].w2)
//		x--;
	*tileocol = *(obj->tile[0].vectile + lround(y)* obj->tile[0].w + lround(x));
	obj->color = *tileocol;
//	*tileocol = base255(rgb(*(obj.tile[0].data_ptr + obj.tile[0].w * lround(y) + lround(x))));
	return (*tileocol);
}

//this function takes the refl ray, the color it has to rewrite, the color without reflection, all else needs no exp

t_colbri	bright_sphere(t_vector st, t_vector hit, t_object obj, t_global *g)
{
	t_colbri	ret;
	t_vector	reflrayv;
	t_vector	hitli[g->lights];

	g->recursion[obj.id]++;
	init_hitli(hitli, hit, g);
	obj.nr = scale(1 / (double)obj.rd, diff(hit, *obj.ctr));
	if (obj.cam_pos)
	{
			if (len2(diff(*g->li, *obj.ctr)) > obj.rd2)
				ret.bri = g->ambient;
			obj.nr = scale(-1, obj.nr);
	}
	ret.nrm = obj.nr;
	if (!obj.trans)
		init_bri(&ret.bri, hitli, obj.nr, g);
	if (obj.tile[0].data_ptr)
		ret.colself = do_tile_sphere(&ret.col, st, hit, &obj, g);
	else
		ret.col = obj.color;
	ret.colself = obj.color;
	if (obj.spec || obj.re)
		reflrayv = reflray(st, hit, obj.nr, g);
	if (obj.re)
		do_re(reflrayv, &ret.col, ret.col, hit, obj.nr, obj, g);
	if (obj.trans)
		do_trans(st, hit, &ret, ret, obj.nr, obj, g);
	obstructed(&ret, hit, hitli, reflrayv, obj, g);
	g->recursion[obj.id] = 0;
	return (ret);
}

t_colbri		simple_bright_spheror(t_vector st, t_vector hit, t_object obj, t_global *g)
{
	t_colbri	ret;
	t_vector	nrm;
	t_vector	ctrli;
	t_vector	refl;
	t_objecthit	reflobj;
	t_vector	camhit;
	t_vector	combcolor;
	t_vector	reflcolor;
	int		retorig;
	double		re;
	t_vector hitli = diff(*g->li, hit);


	re = 0.5;
	if (con(g))
		printf("______start HIT SPHERE func______\n");
	nrm = norm(diff(hit, *obj.ctr));

	retorig = (round(255 * dot(norm(hitli), nrm)));
	if (retorig < g->ambient)
		retorig = g->ambient;
	camhit = diff(hit, st);
	t_vector camhitx = diff(camhit, scale(dot(camhit, nrm), nrm));
	refl = (diff(scale(2, camhitx), camhit));
	t_dstpst temp;

	objecthit(&temp, hit, sum(refl, hit), g->obj, g->argc + 1, g);

	reflobj.hit = sum(scale(temp.dst, refl), hit);
	reflobj.obj = temp.obj;

	if(con(g))
	{
		printf("reflected object is %s\n", reflobj.obj.name);
		printf("_____end HIT SPHERE func_____\n");
		printf("retorig is %d\n", retorig);
	}
	if (reflobj.obj.name == NULL)
	{
		if (con(g))
			printf("returning retorig\n");
		combcolor = scale(1 - re, obj.color);

		ret.col = combcolor;

//		ret.col = combcolor;
		ret.bri = ((1 - re) * retorig);

		return (ret);
	}
	ret = reflobj.obj.bright(hit, reflobj.hit, reflobj.obj, g);
//	combcolor = scale(1, sum(scale(ret.bri, ret.col), scale(retorig, obj.color)));

//	combcolor = sum(scale(ret.bri * 0.5, ret.col), scale(0.5 * retorig, obj.color));

//this cpmb is the best i could do


	combcolor = sum(scale(re, ret.col), scale(1 - re, obj.color));

//	combcolor = scale(retorig, obj.color);
	if (con(g))
		printf("reflcolor is %f,%f,%f\ncombcolor is %f, %f, %f\nret is %d\reflobj name is %s\n", ret.col.x,
						ret.col.y,
						ret.col.z,
			combcolor.x, combcolor.y, combcolor.z,
			ret.bri, reflobj.obj.name);

	ret.bri = ((1 - re) * retorig + re * ret.bri);

	ret.col = combcolor;
//	ret.bri = 1;

/*	if (obstructed(hit, obj, g))
	{
		ret.bri = ret.bri / (double)2;
		ret.bri = fmin(ret.bri, 70);
	}*/

//	obstructed(&ret, hit, g->hitli, obj, g);
	if (con(g))
		printf("ret brig is %d\n", ret.bri);
	return (ret);
}

t_colbri		simple_bright_plane(t_vector st, t_vector hit, t_object obj, t_global *g)
{
	t_colbri			ret;
	t_vector	reflrayv;
	t_vector	hitli[g->lights];

	init_hitli(hitli, hit, g);
	if (obj.cam_pos)
		obj.base[1] = scale(-1, obj.base[1]);
	init_bri(&ret.bri, hitli, obj.base[1], g);
	ret.col = obj.color;
	if (obj.spec || obj.re)
		reflrayv = reflray(st, hit, obj.base[1], g);	
	if (obj.re)
		do_re(reflrayv, &ret.col, ret.col, hit, obj.base[1], obj, g);
	obj.nr = obj.base[1];
	obstructed(&ret, hit, hitli, reflrayv, obj, g);
	return (ret);
}

void			do_tile_plane(t_colbri *retorig, t_vector hit, t_object *obj, t_global *g)
{
	t_vector ctrhit;
	int x;
	int y;

	ctrhit = diff(hit, *obj->ctr);
	x = mymod(ctrhit.x, obj->tile[0].w);
	y = mymod(ctrhit.z, obj->tile[0].h);

	retorig->col = base(rgb(*(obj->tile[0].data_ptr + lround(y) * obj->tile[0].w + lround(x))));
	obj->color = retorig->col;
}

t_colbri		bright_plane(t_vector st, t_vector hit, t_object obj, t_global *g)
{
	t_colbri	ret;
	t_vector	reflrayv;
	t_vector	hitli[g->lights];

	g->recursion[obj.id]++;
	init_hitli(hitli, hit, g);
	if (obj.cam_pos)
		obj.base[1] = scale(-1, obj.base[1]);
	init_bri(&ret.bri, hitli, obj.base[1], g);
	if (obj.tile[0].data_ptr)
		do_tile_plane(&ret, hit, &obj, g);
	else if (lround(fabs(hit.x) / (double)80) % 2 == lround(fabs(hit.z) / (double)80) % 2)
			init_vector(&obj.color, 1, 0, 0.5);
	ret.col = obj.color;
	ret.colself = ret.col;
	if (obj.spec || obj.re)
		reflrayv = reflray(st, hit, obj.base[1], g);
	if (obj.re)
		do_re(reflrayv, &ret.col, ret.col, hit, obj.base[1], obj, g);
	obstructed(&ret, hit, hitli, reflrayv, obj, g);
	g->recursion[obj.id] = 0;
	return (ret);
}

t_colbri		simple_bright_tri(t_vector st, t_vector hit, t_object obj, t_global *g)
{
	t_colbri			ret;
	t_object	a;
	int			i;
	int			retobs;
	int			tempbri;
	t_colbri		retorig;
	t_vector		hitli[g->lights];
	t_vector		reflrayv;

	init_hitli(hitli, hit, g);
	if (0 && dot(diff(hit, st), obj.base[1]) > 0)
	{
		obj.base[1].x = -obj.base[1].x;
		obj.base[1].y = -obj.base[1].y;
		obj.base[1].z = -obj.base[1].z;
	}
	init_bri(&retorig.bri, hitli, obj.base[1], g);

	ret.col = obj.color;
	ret.bri = retorig.bri;

	if (obj.spec || obj.re)
		reflrayv = reflray(st, hit, obj.base[1], g);
	if (obj.re)
		do_re(reflrayv, &ret.col, ret.col, hit, obj.base[1], obj, g);
	ret.bri = retorig.bri;
	if (obj.trans)
		do_trans(st, hit, &ret, ret, obj.base[1], obj, g);
	if (obj.spec)
		do_spec(&ret, hit, obj.base[1], reflrayv, obj, g);
	return (ret);
}

void			do_tile_tri(t_vector *retcol, t_object obj, t_vector hit)
{
	t_vector v;
	double x;
	double y;

	v = diff(hit, *obj.ctr);
	x = mymod(v.x, obj.tile[0].w);
	y = mymod(v.z, obj.tile[0].h);
	*retcol = *(obj.tile[0].vectile + lround(y)* obj.tile[0].w + lround(x));
}

t_colbri		bright_tri(t_vector st, t_vector hit, t_object obj, t_global *g)
{
	t_colbri	ret;
	t_vector	hitli[g->lights];
	t_vector	reflrayv;
	t_colbri	retorig;

	init_hitli(hitli, hit, g);
	if (dot(diff(hit, *g->cam_pos), obj.base[1]) > 0)
		obj.base[1] = scale(-1, obj.base[1]);
	init_bri(&retorig.bri, hitli, obj.base[1], g);
	if (obj.tile[0].data_ptr)
		do_tile_tri(&ret.col, obj, hit);
	else
		ret.col = obj.color;
	ret.colself = ret.col;
	if (obj.re || obj.spec)
		reflrayv = reflray(st, hit, obj.base[1], g);
	if (obj.re)
		do_re(reflrayv, &ret.col, ret.col, hit, obj.base[1], obj, g);
	if (obj.trans)
		do_trans(st, hit, &ret, ret, obj.base[1], obj, g);
	ret.bri = retorig.bri;
	if (obj.spec)
		do_spec(&ret, hit, obj.base[1], reflrayv, obj, g);
	return (ret);
}
