/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   brights.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpokalch <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/03 20:37:08 by tpokalch          #+#    #+#             */
/*   Updated: 2019/09/30 06:46:49 by tpokalch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

/*
int	i;

i = -1;
while (++i < 3)
{
	if (obj->prop[i]

	{
		obj->prop[i](a, b, c, d);
		if (i == 0)
		asdasd
	}
}
		*/

t_colbri		refl(t_vector refl, t_vector hit, t_object obj, t_global *g)
{
	t_dstpst	temp;
	t_objecthit	reflobj;
	t_colbri	ret;

	objecthit(&temp, hit, sum(refl, hit), g->obj, g->argc + 1, g);
	reflobj.hit = sum(scale(temp.dst, refl), hit);
	reflobj.obj = temp.obj;
	if (reflobj.obj.name == NULL)
	{
		init_vector(&ret.col, 0, 0, 0);
		ret.bri = 0;
		return (ret);
	}
	ret = reflobj.obj.bright(hit, reflobj.hit, &reflobj.obj, g);
	ret.col = scale(ret.bri / (double)255, ret.col);
	return (ret);
}

void		do_re(t_vector reflrayv, t_vector hit, 
		t_vector *reocol, t_object obj, t_global *g)
{
	t_colbri reflcolbri;
	t_vector tileocolo;

	tileocolo = *reocol;
	if (g->recursion[obj.id] < MAX_REC)
	{
		reflcolbri = refl(reflrayv, hit, obj, g);
		*reocol = reflcolbri.col;
		*reocol = sum(scale(1 - obj.re, tileocolo),
			scale(obj.re, reflcolbri.col));
	}
}


t_vector		reflray(t_vector st, t_vector end, t_vector nrm, t_global *g)
{
	t_vector	ray;
	t_vector	refl;
	t_vector	rayx;

	ray = diff(end, st);
	rayx = diff(ray, scale(dot(ray, nrm), nrm));
	refl = diff(scale(2, rayx), ray);
	return (refl);
}



t_colbri		trans(t_vector st, t_vector hit, t_object obj, t_global *g)
{
	t_dstpst	temp;
	t_colbri	ret;
	t_objecthit	transobj;
	t_vector	ray;

	ray = diff(hit, st);
	objecthit(&temp, hit, sum(ray, hit), g->obj, g->argc + 1, g);
	transobj.hit = sum(scale(temp.dst, ray), hit);
	transobj.obj = temp.obj;
	if (transobj.obj.name == NULL)
	{
		init_vector(&ret.col, 0, 0, 0);
		ret.bri = g->ambient;
		return (ret);
	}
	ret = transobj.obj.bright(hit, transobj.hit, &transobj.obj, g);
	return (ret);
}

void		do_trans(t_vector st, t_vector hit, t_colbri *ret,
	t_object obj, t_global *g)
{
	t_colbri transo;
	int		origbri;

	origbri = ret->bri;
	if (g->recursion[obj.id] > MAX_REC)
		return ;
	transo = trans(st, hit, obj, g);
	transo.col = sum(scale(1 - obj.trans, ret->col),
		scale(obj.trans, transo.col));
	ret->col = transo.col;
	ret->bri = transo.bri * obj.trans + origbri * (1 - obj.trans);
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
	while (i < g->lights)
	{
		*bri += fmax(round(255 * dot(norm(hitli[i]), nrm)), g->ambient);
		i++;
	}
	*bri = round(*bri / (double)g->lights);
}

int		inside_cone(t_vector p, t_object obj, t_global *g)
{
	double		axdst;
	double		cirad;
	t_vector	ptoaxproj;
	t_vector	ptoaxperp;
	t_vector	op;
	t_global	*a;

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

t_colbri	simple_bright_cone(t_vector st, t_vector hit, t_object *obj,
	t_global *g)
{
	t_colbri	ret;
	t_vector	reflrayv;
	t_vector	hitli[g->lights];

	init_hitli(hitli, hit, g);
	if (inside_cone(*g->cam_pos, *obj, g))
	{
		if (!inside_cone(*g->li, *obj, g))
			ret.bri = g->ambient;
		else
			init_bri(&ret.bri, hitli, obj->nr, g);
	}
	else if (inside_cone(*g->li, *obj, g))
		ret.bri = g->ambient;
	else
		init_bri(&ret.bri, hitli, obj->nr, g);
	ret.col = obj->color;
	if (obj->spec || obj->re)
		reflrayv = reflray(st, hit, obj->nr, g);
	if (obj->re)
		do_re(reflrayv, hit, &ret.col, *obj, g);
	if (obj->trans)
		do_trans(st, hit, &ret, *obj, g);
	obstructed(&ret, hit, hitli, reflrayv, *obj, g);
	return (ret);
}

void		do_tile_cone(t_vector ctrhit,
	t_vector hit, t_object *obj, t_global *g)
{
	double		x;
	double		y;
	t_vector	proj;
	t_tile		*objtile;

	objtile = obj->tile;
	proj = diff(ctrhit, scale(dot(obj->base[1], ctrhit), obj->base[1]));
	proj = norm(proj);
	x = objtile[0].w2 * (1 * M_1_PI *
		myacos(proj, obj->base[2], obj->base[1], g));
	y = mymod(1 - dot(obj->base[1], diff(hit, *obj->ctr)), objtile[0].h);
	obj->color = *(objtile[0].vectile
		+ lround(y) * objtile[0].w + lround(x));
}

t_colbri	bright_cone(t_vector st, t_vector hit, t_object *obj, t_global *g)
{
	t_vector	hit0;
	t_colbri	ret;
	t_vector	hitli[g->lights];
	t_vector	reflrayv;

	g->recursion[obj->id]++;
	hit0 = diff(hit, *obj->ctr);
	obj->nr = norm(diff(hit0, scale(dot(hit0, obj->base[1]) *
		(1 + obj->rd2), obj->base[1])));
	init_hitli(hitli, hit, g);
	if (inside_cone(*g->cam_pos, *obj, g))
	{
		obj->nr = scale(-1, obj->nr);
		if (!inside_cone(*g->li, *obj, g))
			ret.bri = g->ambient;
		else
			init_bri(&ret.bri, hitli, obj->nr, g);
	}
	else if (inside_cone(*g->li, *obj, g))
		ret.bri = g->ambient;
	else
		init_bri(&ret.bri, hitli, obj->nr, g);
	if (obj->re || obj->spec)
		reflrayv = reflray(st, hit, obj->nr, g);
	if (obj->tile[0].data_ptr)
		do_tile_cone(hit0, hit, obj, g);
	ret.col = obj->color;
	if (obj->re)
		do_re(reflrayv, hit, &ret.col, *obj, g);
	if (obj->trans)
		do_trans(st, hit, &ret, *obj, g);
	obstructed(&ret, hit, hitli, reflrayv, *obj, g);
	g->recursion[obj->id] = 0;
	return (ret);
}

t_colbri	simple_bright_cylinder(t_vector st, t_vector hit,
	t_object *obj, t_global *g)
{
	t_colbri	ret;
	t_vector	hitli[g->lights];
	t_vector	reflrayv;

	init_hitli(hitli, hit, g);
	if (obj->cam_pos)
	{
		if (obj->hit(*g->cam_pos, *g->li,
			diff(*g->li, *g->cam_pos), *obj, g).dst < 1)
			ret.bri = g->ambient;
		else
			init_bri(&ret.bri, hitli, obj->nr, g);	
	}
	else
		init_bri(&ret.bri, hitli, obj->nr, g);
	ret.col = obj->color;
	if (obj->spec || obj->re)
		reflrayv = reflray(st, hit, obj->nr, g);
	if (obj->re)
		do_re(reflrayv, hit, &ret.col, *obj, g);
	if (obj->trans)
		do_trans(st, hit, &ret, *obj, g);
	obstructed(&ret, hit, hitli, reflrayv, *obj, g);
	return (ret);
}

void	do_tile_cyl(t_vector hit, t_vector ctrhit,
	 t_object *obj, t_global *g)
{
	double		x;
	double		y;
	t_vector	proj;
	double		xdst;

	proj = diff(ctrhit, scale(dot(obj->base[1], ctrhit), obj->base[1]));
	proj = norm(proj);
	x = obj->tile[0].w2
	* (1 - (1 - 2 * (det(proj, obj->base[0]) < 0))
	* M_1_PI * acos(dot(proj, obj->base[0])));
	xdst = dot(obj->base[1], diff(hit, *obj->ctr));
	x = obj->tile[0].w2 * M_1_PI
		* myacos(proj, obj->base[0], obj->base[1], g);
	y = myintmod(xdst, obj->tile[0].h);
	obj->color = *(obj->tile[0].vectile +
		lround(y) * obj->tile[0].w + lround(x));
}

t_colbri	bright_cylinder(t_vector st, t_vector hit,
	t_object *obj, t_global *g)
{
	t_vector	ctrhit;
	t_colbri	ret;
	t_vector	hitli[g->lights];
	t_vector	reflrayv;

	g->recursion[obj->id]++;	
	ctrhit = diff(hit, *obj->ctr);
	obj->nr = scale(dot(ctrhit, obj->base[1]), obj->base[1]);
	obj->nr = norm(diff(ctrhit, obj->nr));
	init_hitli(hitli, hit, g);
	if (obj->cam_pos)
	{
		obj->nr = scale(-1, obj->nr);
		if (obj->hit(*g->cam_pos, *g->li,
			diff(*g->li, *g->cam_pos), *obj, g).dst < 1)
			ret.bri = g->ambient;
		else
			init_bri(&ret.bri, hitli, obj->nr, g);
	}
	else
		init_bri(&ret.bri, hitli, obj->nr, g);
	if (obj->tile[0].data_ptr)
		do_tile_cyl(hit, ctrhit, obj, g);
	ret.col = obj->color;
	if (obj->spec || obj->re)
		reflrayv = reflray(st, hit, obj->nr, g);
	if (obj->re)
		do_re(reflrayv, hit, &ret.col, *obj, g);
	if (obj->trans)
		do_trans(st, hit, &ret, *obj, g);
	obstructed(&ret, hit, hitli, reflrayv, *obj, g);
	g->recursion[obj->id] = 0;
	return (ret);
}

void		do_1_spec(t_colbri *tmp, t_colbri *ret, t_vector *hitli,
	t_vector reflrayv, t_object obj, int i, t_global *g)
{
	g->cosa[i] = dot(norm(hitli[i]), norm(reflrayv));
	if (g->cosa[i] > 0)
	{
		g->cosa[i] = tothe2(g->cosa[i], obj.spec);
		tmp->col = sum(tmp->col, sum(scale(g->cosa[i], g->white),
					scale((1 - g->cosa[i]), ret->col)));
	}
	else
		tmp->col = sum(tmp->col, ret->col);
}

void		do_spec(t_colbri *ret, t_vector hit, t_vector nrm,
	t_vector reflrayv, t_object obj, t_global *g)
{
	double		cosa;
	double		cosa3;
	int			i;
	t_colbri	tmp;
	t_vector	hitli[g->lights];

	i = -1;
	while (++i < g->lights)
		hitli[i] = diff(g->li[i], hit);
	init_vector(&tmp.col, 0, 0, 0);
	i = -1;
	while (++i < g->lights)
		do_1_spec(&tmp, ret, hitli, reflrayv, obj, i, g);
	tmp.col = scale(1 / (double)g->lights, tmp.col);
	ret->col = tmp.col;
}

t_colbri	simple_bright_sphere(t_vector st, t_vector hit,
	t_object *obj, t_global *g)
{
	t_colbri		ret;
	t_vector		reflrayv;
	t_vector		hitli[g->lights];
	t_vector		ctrli;

	init_hitli(hitli, hit, g);
	if (obj->cam_pos)
	{
		ctrli = diff(*g->li, *obj->ctr);
		if ((dot(ctrli, ctrli) > obj->rd2) && (ret.bri = g->ambient))
			ret.col = obj->color;
		else
			init_bri(&ret.bri, hitli, obj->nr, g);	
	}
	else
		init_bri(&ret.bri, hitli, obj->nr, g);
	ret.col = obj->color;
	if (obj->spec || obj->re)
		reflrayv = reflray(st, hit, obj->nr, g);
	if (obj->re)
		do_re(reflrayv, hit, &ret.col, *obj, g);
	if (obj->trans)
		do_trans(st, hit, &ret, *obj, g);
	obstructed(&ret, hit, hitli, reflrayv, *obj, g);
	return (ret);
}

void		do_tile_sphere(t_vector hit, t_object *obj, t_global *g)
{
	t_vector	ctrhit;
	t_vector	proj;
	double		x;
	double		y;
	t_tile		*tile;

	tile = obj->tile;
	ctrhit = diff(hit, *obj->ctr);
	proj = diff(ctrhit, scale(dot(obj->base[1], ctrhit), obj->base[1]));
	proj = norm(proj);
	y = tile[0].h * M_1_PI * acos(dot(obj->nr, obj->base[1]));
	x = tile[0].w2 * M_1_PI * myacos(proj, obj->base[2], obj->base[1], g);
	if (round(x) >= tile[0].w2)
		x--;
	obj->color = *(tile[0].vectile
		+ lround(y) * tile[0].w + lround(x));;
}

t_colbri	bright_sphere(t_vector st, t_vector hit, t_object *obj, t_global *g)
{
	t_colbri	ret;
	t_vector	reflrayv;
	t_vector	hitli[g->lights];

	g->recursion[obj->id]++;
	init_hitli(hitli, hit, g);
	obj->nr = scale(1 / (double)obj->rd, diff(hit, *obj->ctr));
	if (obj->cam_pos)
	{
		obj->nr = scale(-1, obj->nr);
		if (len2(diff(*g->li, *obj->ctr)) > obj->rd2)
			ret.bri = g->ambient;
		else
			init_bri(&ret.bri, hitli, obj->nr, g);	
	}
	else
		init_bri(&ret.bri, hitli, obj->nr, g);
	if (obj->spec || obj->re)
		reflrayv = reflray(st, hit, obj->nr, g);
	if (obj->tile[0].data_ptr)
		do_tile_sphere(hit, obj, g);
	ret.col = obj->color;
	if (obj->re)
		do_re(reflrayv, hit, &ret.col, *obj, g);
	if (obj->trans)
		do_trans(st, hit, &ret, *obj, g);
	obstructed(&ret, hit, hitli, reflrayv, *obj, g);
	g->recursion[obj->id] = 0;
	return (ret);
}

t_colbri		simple_bright_plane(t_vector st, t_vector hit,
	t_object *obj, t_global *g)
{
	t_colbri	ret;
	t_vector	reflrayv;
	t_vector	hitli[g->lights];

	init_hitli(hitli, hit, g);
	init_bri(&ret.bri, hitli, obj->base[1], g);
	ret.col = obj->color;
	if (obj->spec || obj->re)
		reflrayv = reflray(st, hit, obj->base[1], g);
	if (obj->re)
		do_re(reflrayv, hit, &ret.col, *obj, g);
	if (obj->trans)
		do_trans(st, hit, &ret, *obj, g);
	obj->nr = obj->base[1];
	obstructed(&ret, hit, hitli, reflrayv, *obj, g);
	return (ret);
}

void			do_tile_plane(t_colbri *retorig, t_vector hit,
	t_object *obj, t_global *g)
{
	t_vector	ctrhit;
	int			x;
	int			y;

	ctrhit = diff(hit, *obj->ctr);
	x = mymod(ctrhit.x, obj->tile[0].w);
	y = mymod(ctrhit.z, obj->tile[0].h);
	retorig->col = base(rgb(*(obj->tile[0].data_ptr +
	lround(y) * obj->tile[0].w + lround(x))));
	obj->color = retorig->col;
}

t_colbri		bright_plane(t_vector st, t_vector hit,
	t_object *obj, t_global *g)
{
	t_colbri	ret;
	t_vector	reflrayv;
	t_vector	hitli[g->lights];

	g->recursion[obj->id]++;
	init_hitli(hitli, hit, g);
	if (obj->cam_pos)
		obj->base[1] = scale(-1, obj->base[1]);
	init_bri(&ret.bri, hitli, obj->base[1], g);
	if (obj->spec || obj->re)
		reflrayv = reflray(st, hit, obj->base[1], g);
	if (obj->tile[0].data_ptr)
		do_tile_plane(&ret, hit, obj, g);
	else if (lround(fabs(hit.x) / (double)80) % 2
		== lround(fabs(hit.z) / (double)80) % 2)
		init_vector(&obj->color, 1, 0, 0.5);
	ret.col = obj->color;
	if (obj->re)
		do_re(reflrayv, hit, &ret.col, *obj, g);
	if (obj->trans)
		do_trans(st, hit, &ret, *obj, g);	
	obstructed(&ret, hit, hitli, reflrayv, *obj, g);
	g->recursion[obj->id] = 0;
	return (ret);
}

t_colbri		simple_bright_tri(t_vector st, t_vector hit,
	t_object *obj, t_global *g)
{
	t_colbri	ret;
	t_vector	hitli[g->lights];
	t_vector	reflrayv;
	t_colbri	retorig;

	init_hitli(hitli, hit, g);
	if (0 && dot(diff(hit, st), obj->base[1]) > 0)
	{
		obj->base[1].x = -obj->base[1].x;
		obj->base[1].y = -obj->base[1].y;
		obj->base[1].z = -obj->base[1].z;
	}
	init_bri(&retorig.bri, hitli, obj->base[1], g);
	ret.col = obj->color;
	ret.bri = retorig.bri;
	if (obj->spec || obj->re)
		reflrayv = reflray(st, hit, obj->base[1], g);
	if (obj->re)
		do_re(reflrayv, hit, &ret.col, *obj, g);
	ret.bri = retorig.bri;
	if (obj->trans)
		do_trans(st, hit, &ret, *obj, g);
	if (obj->spec)
		do_spec(&ret, hit, obj->base[1], reflrayv, *obj, g);
	return (ret);
}

void			do_tile_tri(t_vector *retcol, t_object *objp, t_vector hit)
{
	t_vector	v;
	double		x;
	double		y;
	t_object	obj;

	obj = *objp;
	v = diff(hit, *obj.ctr);
	x = mymod(v.x, obj.tile[0].w);
	y = mymod(v.z, obj.tile[0].h);
//	*retcol = *(obj.tile[0].vectile + lround(y) * obj.tile[0].w + lround(x));
	obj.color = *(obj.tile[0].vectile + lround(y) * obj.tile[0].w + lround(x));
}

t_colbri		bright_tri(t_vector st, t_vector hit, t_object *obj, t_global *g)
{
	t_colbri	ret;
	t_vector	hitli[g->lights];
	t_vector	reflrayv;
	t_colbri	retorig;

	init_hitli(hitli, hit, g);
	if (dot(diff(hit, *g->cam_pos), obj->base[1]) > 0)
		obj->base[1] = scale(-1, obj->base[1]);
	init_bri(&retorig.bri, hitli, obj->base[1], g);
	if (obj->tile[0].data_ptr)
		do_tile_tri(&ret.col, obj, hit);
	ret.col = obj->color;
//	ret.colself = ret.col;
	if (obj->re || obj->spec)
		reflrayv = reflray(st, hit, obj->base[1], g);
	if (obj->re)
		do_re(reflrayv, hit, &ret.col, *obj, g);
	if (obj->trans)
		do_trans(st, hit, &ret, *obj, g);
	ret.bri = retorig.bri;
	if (obj->spec)
		do_spec(&ret, hit, obj->base[1], reflrayv, *obj, g);
	return (ret);
}
