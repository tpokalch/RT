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
	if (reflobj.obj.name == nothing)
	{
		init_vector(&ret.col, 0, 0, 0);
		ret.bri = 0;
		return (ret);
	}
	ret = reflobj.obj.bright(hit, reflobj.hit, &reflobj.obj, g);
	ret.col = scale(ret.bri / (double)255, ret.col);
	return (ret);
}
/*
t_vector	vecmul(t_vector &a, t_vector &b)
{
	t_vector ret;

	init_vector(&ret, a->x * b->x, a->y * b->y, a->z * b->z); 
	return (ret);
}
*/
void		do_re(t_vector reflrayv, t_vector hit, 
		t_vector *reocol, t_object obj, t_global *g)
{
	t_colbri reflcolbri;
	t_vector tileocolo;

	tileocolo = *reocol;
//	t_vector _111;
//	init_vector(&_111, 1, 1, 1);
//	g->recursion = vecmul(g->recursion, &obj.col);
	if (g->recursion[obj.id] < MAX_REC)
//	if (g->recursion->x > 0 || g->recursion->y > 0 || g->recursion->z > 0)
	{
		reflcolbri = refl(reflrayv, hit, obj, g);
		*reocol = reflcolbri.col;
		*reocol = sum(scale(1 - obj.re, tileocolo),
			scale(obj.re, reflcolbri.col));

//		*reocol = sum(scale(diff(g->recursion, vecmul(obj.re, g->recursion)), tileocolo),
//			scale(obj.re, reflcolbri.col));
	}
}

//can be done faster
t_vector		reflray(t_vector st, t_vector end, t_vector nrm, t_global *g)
{
	t_vector	ray;
	t_vector	refl;
	t_vector	rayx;

	ray = diff(end, st);
	rayx = diff(ray, scale(dot(ray, nrm), nrm));
	refl = diff(scale(2, rayx), ray);
//	do i need this?
	refl = norm(refl);
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
	if (transobj.obj.name == nothing)
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
//		hitli[i] = norm(diff(g->li[i], hit));
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

void		do_tile_cone(/*t_vector ctrhit,*/
	t_vector hit, t_object *obj, t_global *g)
{
	double		x;
	double		y;
	t_vector	proj;
	t_tile		*objtile;

	objtile = obj->tile;
	proj = diff(obj->nr/*ctrhit*/, scale(dot(obj->base[1], obj->nr/*ctrhit*/), obj->base[1]));
	proj = norm(proj);
	x = objtile[0].w2 * (1 * M_1_PI *
		myacos(proj, obj->base[2], obj->base[1], g));
	y = mymod(1 - dot(obj->base[1], diff(hit, *obj->ctr)), objtile[0].h);
	obj->color = *(objtile[0].vectile
		+ lround(y) * objtile[0].w + lround(x));
}


t_vector	get_normal_cone(t_vector point, t_object *obj)
{
	t_vector hit0;
	t_vector ret;

	hit0 = diff(point, *obj->ctr);
	ret = norm(diff(hit0, scale(dot(hit0, obj->base[1]) *
		(1 + obj->rd2), obj->base[1])));
	return (ret);
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
		do_tile_cone(/*hit0, */hit, obj, g);
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

	proj = diff(obj->nr/*ctrhit*/, scale(dot(obj->base[1], obj->nr/*ctrhit*/), obj->base[1]));
//	proj = norm(proj);
//for correct straight

/*	x = obj->tile[0].w2
	* (1 - (1 - 2 * (det(proj, obj->base[0]) < 0))
	* M_1_PI * acos(dot(proj, obj->base[0])));
*/
//ctrhit used only here
	xdst = dot(obj->base[1], ctrhit);

//for all rotations

	x = obj->tile[0].w2 *(1 - M_1_PI
		 * myacos(proj, obj->base[0], obj->base[1], g));
	y = myintmod(xdst, obj->tile[0].h);
	if (con(g))
		printf("x, y is %f, %f\n", x, y);
	if (y == 0)
		y = 1;
	obj->color = *(obj->tile[0].vectile +
		lround(y) * obj->tile[0].w + lround(x));
}

t_vector	get_normal_cyl(t_vector point, t_object *obj)
{
	t_vector ctrhit;
	t_vector ret;

	ctrhit = diff(point, *obj->ctr);
	ret = scale(dot(ctrhit, obj->base[1]), obj->base[1]);
	ret = norm(diff(ctrhit, ret));	
	return ret;
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
	g->cosa[i] = dot(/*norm(*/hitli[i]/*)*/, /*norm(*/reflrayv/*)*/);
	if (g->cosa[i] > 0)
	{
		g->cosa[i] = tothe2(g->cosa[i], obj.spec);
//				dirty trick, see central for explenation of do_1_spec
		tmp->col = sum(tmp->col, sum(scale(255 * g->cosa[i] / ret->bri, g->white),
					scale((1 -  g->cosa[i]), ret->col)));
	}
	else
//		do same thing as if cosa == 0
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

	if (con(g))
		printf("normal is %f,%f,%f\n", obj->nr.x, obj->nr.y, obj->nr.z);
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
	ctrhit = diff(hit, *obj->ctr)/*obj->real_nr*/;
//	ctrhit = obj->nr; /real_nr!

	proj = diff(ctrhit, scale(dot(obj->base[1], ctrhit), obj->base[1]));
	proj = norm(proj);
	y = tile[0].h * M_1_PI * acos(dot(obj->real_nr, obj->base[1]));
	x = tile[0].w2 * M_1_PI * myacos(proj, obj->base[2], obj->base[1], g);
	if (round(x) >= tile[0].w2)
		x--;
	obj->color = *(tile[0].vectile
		+ lround(y) * tile[0].w + lround(x));
//	obj->color = *(tile[0].vectile
//		+ lround(y) * tile[0].w2 + lround(x) % tile[0].w2);
}

void	    do_normal_map_sphere(t_vector hit, t_object *obj, t_global *g)
{
	t_vector	ctrhit;
	t_vector	proj;
	t_vector angle;
	double	  x;
	double	  y;
	t_tile	  normalmap;
	normalmap = obj->normal_map;
//	ctrhit = diff(hit, *obj->ctr);
	ctrhit = obj->nr;
	proj = diff(ctrhit, scale(dot(obj->base[1], ctrhit), obj->base[1]));
	proj = norm(proj);

//	angle between y axis and the normal
	angle.x = acos(dot(obj->nr, obj->base[1]));
//	angle.y = 0;
//	angle.z = 0;
//	angle between projection of the normal on the x z axis
//	and z axis counting counter clockwise when looking from the top
		
	angle.y = -myacos(proj, obj->base[2], obj->base[1], g);
	angle.z = 0;


//	printf("angles are %f,%f,%f\n", angle.x, angle.y, angle.z);
	y = normalmap.h * M_1_PI * angle.x;
	x = normalmap.w2 * M_1_PI * angle.y;
	if (round(x) >= normalmap.w2)
		x--;
	obj->nr = /*norm(*/*(normalmap.vectile
		+ lround(y) * normalmap.w + lround(x));

	if (con(g))
		printf("normal before rotate %f,%f,%f\n",obj->nr.x, obj->nr.y, obj->nr.z);
//	obj->nr = rotate(obj->nr, angle);
//	obj->nr.x = -obj->nr.x;
//*retorig = *(obj->normal_map.vectile +
//	lround(y) * obj->normal_map.w + lround(x));
	if (con(g))
	{
		t_vector test = norm(ctrhit);
		printf("real normal  %f,%f,%f\n", test.x, test.y, test.z);
		printf("calculated   %f,%f,%f\n", obj->nr.x, obj->nr.y, obj->nr.z);
	}



//	printf("normal is %f,%f,%f\n", obj->nr.x, obj->nr.y, obj->nr.z);
}

t_vector	get_normal_sphere(t_vector hit, t_object *obj)
{
	return (scale(obj->rd_1, diff(hit, *obj->ctr)));
}

t_colbri	bright_sphere(t_vector st, t_vector hit, t_object *obj, t_global *g)
{
	t_colbri	ret;
	t_vector	reflrayv;
	t_vector	hitli[g->lights];

	g->recursion[obj->id]++;
	init_hitli(hitli, hit, g);
	obj->nr = obj->real_nr = scale(obj->rd_1, diff(hit, *obj->ctr));

//	normal must be inited before lighting calculation
//	this changes obj->nr
	if (obj->normal_map.data_ptr)
		do_normal_map_sphere(hit, obj, g);

//	if (con(g))
//		printf("cam: %f, %f, %f, li: %f, %f, %f angle: %f,%f,%f\n",g->cam_pos->x, g->cam_pos->y, g->cam_pos->z, g->li->x, g->li->y, g->li->z, g->angle->x, g->angle->y, g->angle->z);
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

void			do_normal_map_plane(t_vector *retorig, t_vector hit,
	t_object *obj, t_global *g)
{
	t_vector	ctrhit;
	int			x;
	int			y;

	ctrhit = diff(hit, *obj->ctr);
	x = mymod(ctrhit.x, obj->normal_map.w);
	y = mymod(ctrhit.z, obj->normal_map.h);
	*retorig = *(obj->normal_map.vectile +
	lround(y) * obj->normal_map.w + lround(x));

}



void			do_tile_plane(t_colbri *retorig, t_vector hit,
	t_object *obj, t_global *g)
{
	t_vector	ctrhit;
	int			x;
	int			y;

//	printf("doing tile plane\n");
	ctrhit = diff(hit, *obj->ctr);
//	printf("hit is %f,%f,%f\n", hit.x, hit.y, hit.z);
	x = mymod(ctrhit.x, obj->tile[0].w);
	y = mymod(ctrhit.z, obj->tile[0].h);
//	printf("x,y is %d,%d\n%f,%f\n", x, y, ctrhit.x, ctrhit.z);

	retorig->col = *(obj->tile[0].vectile +
	lround(y) * obj->tile[0].w + lround(x));

//	obj->color = *(tile[0].vectile
//		+ lround(y) * tile[0].w + lround(x));

//	printf("tile plane done\n");
	obj->color = retorig->col;
}

t_vector	get_normal_plane(t_vector point, t_object *obj)
{
//	printf("returning normal to plane\n");
	return (obj->base[1]);
}

t_colbri		bright_plane(t_vector st, t_vector hit,
	t_object *obj, t_global *g)
{
	t_colbri	ret;
	t_vector	reflrayv;
	t_vector	hitli[g->lights];

	g->recursion[obj->id]++;
	init_hitli(hitli, hit, g);


	if (obj->normal_map.data_ptr)
		do_normal_map_plane(&obj->nr, hit, obj, g);
	else
		obj->nr = obj->base[1];

//	changed campos to invert obj base //message не актуально
	if (obj->cam_pos)
	{
//		obj->base[1] = scale(-1, obj->base[1]);
		obj->nr = scale(-1, obj->nr);
	}
	init_bri(&ret.bri, hitli, obj->nr, g);
	if (obj->spec || obj->re)
		reflrayv = reflray(st, hit, obj->nr, g);
	if (obj->tile[0].data_ptr)
		do_tile_plane(&ret, hit, obj, g);
	else if (lround(fabs(hit.x) / (double)80) % 2
		== lround(fabs(hit.z) / (double)80) % 2)
		init_vector(&obj->color, 0,0.5, 0.5);
//		init_vector(&obj->color, 1,1, 1);

	ret.col = obj->color;
	if (obj->re)
		do_re(reflrayv, hit, &ret.col, *obj, g);
	if (obj->trans)
		do_trans(st, hit, &ret, *obj, g);	
	obstructed(&ret, hit, hitli, reflrayv, *obj, g);

	g->recursion[obj->id] = 0;

	if (con(g))
		printf("BRIGTNTESS %d\n", ret.bri);

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
