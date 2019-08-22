/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   brights.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpokalch <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/03 20:37:08 by tpokalch          #+#    #+#             */
/*   Updated: 2019/07/03 20:49:07 by tpokalch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

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
	axdst = dot(op, obj.nr);
	cirad = axdst * obj.rd2;
	ptoaxproj = scale(axdst, obj.nr);
	ptoaxperp = diff(op, ptoaxproj);
	return (dot(ptoaxperp, ptoaxperp) - cirad * cirad <= 0);
}

t_colbri	bright_cone(t_vector st, t_vector hit, t_object obj, t_global *g)
{
	t_vector	nrm;
	t_vector	camforw;
	t_vector	hit0;
	t_colbri			ret;
	t_vector	ax;
	int		retobs;

//	printf("inside cone\n");
	hit0 = diff(hit, *obj.ctr);
	ax = scale(dot(hit0, obj.nr) * (1 + obj.rd2), obj.nr);
	nrm = norm(diff(hit0, ax));
	camforw = diff(sum(*g->cam_pos, *g->normal), *g->cam_pos);
	if (inside_cone(*g->cam_pos, obj, g))
	{
		if (!inside_cone(*g->li, obj, g))
		{
			ret.bri = 0;
			return (ret);
		}
		else
		nrm = scale(-1, nrm);
	}
	else if (inside_cone(*g->li, obj, g))
	{
		ret.bri = 0;
		return (ret);
	}
	ret.bri = 255 * dot(norm(diff(diff(*g->li, *obj.ctr), hit0)), nrm);

	if (obj.tile[0].data_ptr)
	{
		int x;
		int y;
		t_vector _100;

		init_vector(&_100, 1, 0, 0);
		t_vector proj;


	t_vector ctrhit;

//	printf("hi\n");
	ctrhit = diff(hit, *obj.ctr);

	proj = diff(ctrhit, scale(dot(obj.nr, ctrhit),obj.nr));
	proj = norm(proj);

	if (con(g))
	{
		printf("proj is %f,%f,%f\n", proj.x, proj.y, proj.z);
		printf("width is %d\n", obj.tile[0].w);
		printf("acos is %f\n", acos(dot(_100, proj)));
	}
//	x = lround(obj.tile[0].w * M_2_PI * (M_PI + (1 - 2 * (det(proj, _100) < 0)) * acos(dot(_100, proj))));
	x = lround(obj.tile[0].w2 * (1 - (1 - 2 * (det(proj, _100) < 0)) * M_1_PI * acos(dot(proj, _100))));



	y = lround(fabs(1 - dot(obj.nr, diff(hit, *obj.ctr)))) % obj.tile[0].h;
	if (con(g))
	{
		printf("x is %d\n", x);
		printf("y is %d\n", y);
	}	
	ret.col = base255(rgb(*(obj.tile[0].data_ptr + y * obj.tile[0].w + x)));
	

	if (obstructed(hit, obj, g))
	{
		retobs = g->ambient;
//		ret.bri = ret.bri;
		ret.bri = fmin(ret.bri, retobs);
	}
	}
	else
		ret.col = obj.color;
	if (ret.bri < g->ambient)
	{
		ret.bri = g->ambient;
		return (ret);
	}
	
//	ret.col = obj.color;
	return (ret);
}

t_colbri	bright_cylinder(t_vector st, t_vector hit, t_object obj, t_global *g)
{
	t_vector	nrm;
	t_vector	vrt;
	t_vector	hit0;
	t_colbri		ret;
	t_dstpst	t;
	int		retobs;

	hit0 = diff(hit, *obj.ctr);
	vrt = obj.nr;
	vrt = scale(dot(hit0, vrt), vrt);
	nrm = norm(diff(hit0, vrt));
	if (obj.cam_pos)
	{
		t = obj.hit(*g->cam_pos, *g->li, diff(*g->li, *g->cam_pos),obj, g);
		if (t.dst < 1)
		{
			ret.bri = 0;
			return (ret);
		}
		nrm = scale(-1, nrm);
	}
	ret.bri = 255 * dot(norm(diff(diff(*g->li, *obj.ctr), hit0)), nrm);
//	else 
//		ret = color(ret, obj.color);	
//	ret.col = obj.color;

	if (obj.tile[0].data_ptr)
	{
		int x;
		int y;
		t_vector _100;

		init_vector(&_100, 1, 0, 0);
		t_vector proj;


		t_vector ctrhit;

		ctrhit = diff(hit, *obj.ctr);

		proj = diff(ctrhit, scale(dot(obj.nr, ctrhit),obj.nr));
		proj = norm(proj);
		if (con(g))
		{
			printf("proj is %f,%f,%f\n", proj.x, proj.y, proj.z);
			printf("width is %d\n", obj.tile[0].w);
			printf("acos is %f\n", acos(dot(_100, proj)));
		}
//	x = lround(obj.tile[0].w * M_2_PI * (M_PI + (1 - 2 * (det(proj, _100) < 0)) * acos(dot(_100, proj))));
	x = lround(obj.tile[0].w2 * (1 - (1 - 2 * (det(proj, _100) < 0)) * M_1_PI * acos(dot(proj, _100))));


	double xdst;

	xdst = dot(obj.nr, diff(hit, *obj.ctr));
	y = lround(/*1 - */fabs(fmod(xdst, obj.tile[0].h)));
	if (con(g))
	{
		printf("x is %d\n", x);
		printf("y is %d\n", y);
	}	
	ret.col = base255(rgb(*(obj.tile[0].data_ptr + y * obj.tile[0].w + x)));
	}
	else
		ret.col = obj.color;
	if (obstructed(hit, obj, g))
	{
		retobs = 70;
		ret.bri = ret.bri;
		ret.bri = fmin(ret.bri, retobs);
	}

	if (ret.bri < g->ambient)
	{
		ret.bri = g->ambient;
		return (ret);
	}


	return (ret);
}

t_vector		mip_col(int x, int y, double dst2, t_object obj, t_global *g)
{
	int		tilelow;
	int		tilehi;
	t_tile		*ti;
	double		newres;
	double		tilen;
	t_vector		ret;

	newres = g->ray->z / (double)sqrt(dst2) * obj.tile[0].w;

	ti = obj.tile;

	tilen = log2(obj.tile[0].w / newres);



	tilelow = floor(tilen);
	tilehi = ceil(tilen);
	float weight[2];

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

	t_vector col[4];
	double	colweight[4];	
	col[0] = base255(rgb(*(ti[tilelow].data_ptr + ti[tilelow].w * (int)floor(x) + (int)floor(y))));
	col[1] = base255(rgb(*(ti[tilehi].data_ptr + ti[tilehi].w * (int)floor(x) + (int)floor(y))));

	if (con(g))
		printf("colweight 0 %f\ncolweight 1 %f\n", colweight[0], colweight[1]);


//	ret.col = base255(rgb(*(ti[0].data_ptr + ti[0].w * x + y)));
//	ret.col = base255(rgb(*(ti[(int)tilen].data_ptr + ti[(int)tilen].w * x + y)));


//	ret.col = sum(scale(colweight[0], col[0]), scale(colweight[1], col[1]));

	ret = sum(scale(weight[0], col[0]), scale(weight[1], col[1]));
	return (ret);
}


t_colbri	bright_sphere(t_vector st, t_vector hit, t_object obj, t_global *g)
{
	t_colbri		ret;
	t_vector	nrm;
	t_vector	ctrli;
	t_colbri	ret2;
	int		retobs;
	int		x;
	int		y;
	t_vector	proj;

	if (con(g))
		printf("______start BRI SPHERE func______\n");
	nrm = norm(diff(hit, *obj.ctr));
	ctrli = diff(*g->li, *obj.ctr);
	if (obj.cam_pos)
	{
		if (dot(ctrli, ctrli) > obj.rd2)
		{
			ret.bri = 0;
			return (ret);
		}
		nrm = scale(-1, nrm);
	}
	ret.bri = (round(255 * dot(norm(diff(*g->li, hit)), nrm)));
	if (obstructed(hit, obj, g))
	{
		retobs = g->ambient;
//		ret.bri = ret.bri / (double)2;
		ret.bri = fmin(ret.bri, retobs);
	}	
//	else
//		ret = color(ret, obj.color);
	if (con(g))
		printf("ambient is %d\n", g->ambient);
	if (ret.bri < g->ambient)
		ret.bri = g->ambient;
	if (obj.tile[0].data_ptr)
	{
		proj = nrm;
		proj.y = 0;
		proj = norm(proj);

	t_vector _001;
	t_vector _100;
	init_vector(&_001, 0, 0, 1);
	init_vector(&_100, 1, 0, 0);
 	if (con(g))
	{
		printf("sign det %f\n", det(proj, _100));
		printf("proj is %f,%f,%f\n", proj.x, proj.y, proj.z);
	}
	x = lround(obj.tile[0].w2 * (1 - (1 - 2 * (det(proj, _001) < 0)) * M_1_PI * acos(dot(proj, _001))));

	y = lround(obj.tile[0].h * ( (1 - 2 * (det(nrm, obj.nr) < 0)) * M_1_PI * acos(dot(nrm, obj.nr))));

	if (con(g))
		printf("acos is %f\n", acos(dot(nrm, obj.nr)));
//	y = lround(obj.tile[0].h2 * (1 - (dot(obj.nr, nrm))));

	if (con(g))
		printf("x, y is %d, %d\n", x, y);
//	ret.col = base255(rgb(*(obj.tile[0].data_ptr + y * obj.tile[0].w + x)));
	ret.col = mip_col(y, x, dot(diff(hit, *g->cam_pos), diff(hit, *g->cam_pos)), obj, g);
	}
	else
		ret.col = obj.color;
	return (ret);
}



t_colbri		bright_spheror(t_vector st, t_vector hit, t_object obj, t_global *g)
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

	re = 0.5;
	if (con(g))
		printf("______start HIT SPHERE func______\n");
	nrm = norm(diff(hit, *obj.ctr));

	retorig = (round(255 * dot(norm(diff(*g->li, hit)), nrm)));
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
	if (con(g))
		printf("ret brig is %d\n", ret.bri);
	return (ret);
}

double			bell(double tile_dst, double crt_dst)
{
	double v;

	v = 5;	
	return (1 / sqrt(M_PI * 2 * v * v) * exp(-(crt_dst - tile_dst) * (crt_dst - tile_dst) / (2 * v * v)));
}


t_colbri		bright_plane(t_vector st, t_vector hit, t_object obj, t_global *g)
{
	t_colbri			ret;
	t_object	a;
	int			i;
	int			retobs;
	int			chess;
	int			tempbri;
	t_vector	v;
	int		x;
	int		y;
	t_tile		*ti;
	double		newres;
	double		tilen;
	double	dst2;
	int tilelow;
	int tilehi;

	i = 0;
	a = obj;
//	printf("inside bright plane\n");



	if (dot(diff(hit, *g->cam_pos), obj.nr) > 0)
	{
		if (con(g))
			printf("changing normal\n");
		obj.nr.x = -obj.nr.x;
		obj.nr.y = -obj.nr.y;
		obj.nr.z = -obj.nr.z;
	}
	if (con(g))
		printf("orient > 0? %f\n", dot(diff(hit, *g->cam_pos), obj.nr));
//	printf("orient calculated\n");
//	v = dot(obj.nr, hit);
	v = diff(hit, *obj.ctr);

	ret.bri = round(255 * dot(norm(diff(*g->li, hit)), obj.nr));
	if (con(g))
		printf("bri is %d\n", ret.bri);
//	printf("x y is %d, %d\n", x, y);
//	screen(obj.tile[1].data_ptr, obj.tile[1].w, obj.tile[1].h, g);
//	tilen = 5;

	if (obj.tile[0].data_ptr)
	{
		printf("there is tile\n");
		dst2 = dot(diff(hit, *g->cam_pos), diff(hit, *g->cam_pos));
		newres = g->ray->z / (double)sqrt(dst2) * obj.tile[0].w;

	ti = obj.tile;
	if (con(g))
	{
		printf("square distange to plane is %f\n", dot(diff(hit, *g->cam_pos), diff(hit, *g->cam_pos)));
		printf("distance to plane is %f\n", sqrt(dot(diff(hit, *g->cam_pos), diff(hit, *g->cam_pos))));

		printf("tile is %f\n", tilen);
		printf("newrsi is %f\n", newres);
		printf("potenital tile n %f\n",log2( obj.tile[0].w / newres));
	}	
	tilen = log2(obj.tile[0].w / newres);

	x = lround(fabs(v.x)) % (ti[0].w);
	y = lround(fabs(v.z)) % (ti[0].h);



	tilelow = floor(tilen);
	tilehi = ceil(tilen);
	float weight[2];

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
//	t_vector colhi[4];
	double	colweight[4];
//	double colhiweight[4];
	
	col[0] = base255(rgb(*(ti[tilelow].data_ptr + ti[tilelow].w * (int)floor(x) + (int)floor(y))));
	col[1] = base255(rgb(*(ti[tilehi].data_ptr + ti[tilehi].w * (int)floor(x) + (int)floor(y))));

/*	col[0] = base255(rgb(*(ti[tilelow].data_ptr + ti[tilelow].w * (int)floor(x) + (int)floor(y))));
	col[1] = base255(rgb(*(ti[tilelow].data_ptr + ti[tilelow].w * (int)ceil(x) + (int)ceil(y))));
	col[2] = base255(rgb(*(ti[tilelow].data_ptr + ti[tilelow].w * (int)ceil(x) + (int)floor(y))));
	col[3] = base255(rgb(*(ti[tilelow].data_ptr + ti[tilelow].w * (int)floor(x) + (int)ceil(y))));
*/

/*	colweight[0] = sqrt((1 - (x - floor(x))) * (1 -(x - floor(x))) + (1 - (y - floor(y))) * (1 - (y - floor(y)))) / (double)2;
	colweight[1] = sqrt(1 - ((ceil(x) - x)) * (1 - (ceil(x) - x)) + (1 - (ceil(y) - y)) * (1 - (ceil(y) - y))) / (double)2;
	colweight[3] = sqrt(1 - ((floor(x) - x)) * (1 - (floor(x) - x)) + (1 - (ceil(y) - y)) * (1 - (ceil(y) - y))) / (double)2;
	colweight[2] = sqrt(1 - ((ceil(x) - x)) * (1 - (ceil(x) - x)) + (1 - (floor(y) - y)) * (1 - (floor(y) - y))) / (double)2;
*/
/*	colhi[0] = base255(rgb(*(ti[tilehi].data_ptr + ti[tilehi].w * (int)floor(x) + (int)floor(y))));
	colhi[1] = base255(rgb(*(ti[tilehi].data_ptr + ti[tilehi].w * (int)ceil(x) + (int)ceil(y))));
	colhi[2] = base255(rgb(*(ti[tilehi].data_ptr + ti[tilehi].w * (int)ceil(x) + (int)floor(y))));
	colhi[3] = base255(rgb(*(ti[tilehi].data_ptr + ti[tilehi].w * (int)floor(x) + (int)ceil(y))));

	colhiweight[0] = sqrt((1 - (x - floor(x))) * (1 -(x - floor(x))) + (1 - (y - floor(y))) * (1 - (y - floor(y)))) / (double)2;
	colhiweight[1] = sqrt(1 - ((ceil(x) - x)) * (1 - (ceil(x) - x)) + (1 - (ceil(y) - y)) * (1 - (ceil(y) - y))) / (double)2;
	colhiweight[3] = sqrt(1 - ((floor(x) - x)) * (1 - (floor(x) - x)) + (1 - (ceil(y) - y)) * (1 - (ceil(y) - y))) / (double)2;
	colhiweight[2] = sqrt(1 - ((ceil(x) - x)) * (1 - (ceil(x) - x)) + (1 - (floor(y) - y)) * (1 - (floor(y) - y))) / (double)2;



	t_vector retcol[2];
	retcol[0] = sum(scale(colweight[0],col[0]), sum(scale(colweight[1],col[1]), sum(scale(colweight[2],col[2]), scale(colweight[3], col[3]))));


	retcol[1] = sum(scale(colhiweight[0],colhi[0]), sum(scale(colhiweight[1],colhi[1]), sum(scale(colhiweight[2],colhi[2]), scale(colhiweight[3], colhi[3]))));

	ret.col = sum(scale(weight[0], retcol[0]), scale(weight[1], retcol[1]));
*/

	if (con(g))
		printf("colweight 0 %f\ncolweight 1 %f\n", colweight[0], colweight[1]);


//	ret.col = base255(rgb(*(ti[0].data_ptr + ti[0].w * x + y)));
//	ret.col = base255(rgb(*(ti[(int)tilen].data_ptr + ti[(int)tilen].w * x + y)));


//	ret.col = sum(scale(colweight[0], col[0]), scale(colweight[1], col[1]));

	ret.col = sum(scale(weight[0], col[0]), scale(weight[1], col[1]));



	if (con(g))
		printf("color is %f,%f,%f\n", ret.col.x, ret.col.y, ret.col.z);

	}
	else
	{
//		printf("calculating chess\n");
		chess = lround(fabs(hit.x) / (double)80) % 2 == lround(fabs(hit.z) / (double)80) % 2;
		if (chess)
			init_vector(&ret.col, 0, 0, 0);
		else
			ret.col = obj.color;
	}
	if (obstructed(hit, obj, g))
	{
//		printf("now obstructed\n");
		retobs = g->ambient;
		if (chess)
		{
			if (con(g))
				printf("hit darker\n");
//			init_vector(&ret.col, 0, 0, 0);
		}
		else
//			ret.col = obj.color;
		tempbri = ret.bri /*/ (double)2*/;
		ret.bri = fmin(tempbri, retobs);
		if (con(g))
		{
			if (retobs == ret.bri)
				printf("returning constant bri\n");
			else if (tempbri == ret.bri)
				printf("returning darker bri\n");
			else
				printf("error\n");
		}
	}
	if (ret.bri < g->ambient)
	{
		ret.bri = g->ambient;
		if (con(g))
			printf("returning ambient bri\n");
		return (ret);
	}
	return (ret);
}

t_colbri		bright_tri(t_vector st, t_vector hit, t_object obj, t_global *g)
{
	t_colbri			ret;
	t_object	a;
	int			i;
	int			retobs;
	int			chess;
	int			tempbri;
	t_vector	v;
	int		x;
	int		y;
	t_tile		*ti;
	double		newres;
	double		tilen;
	double	dst2;
	int tilelow;
	int tilehi;

	i = 0;
	a = obj;
//	printf("inside bright plane\n");



	if (dot(diff(hit, *g->cam_pos), obj.nr) > 0)
	{
		if (con(g))
			printf("changing normal\n");
		obj.nr.x = -obj.nr.x;
		obj.nr.y = -obj.nr.y;
		obj.nr.z = -obj.nr.z;
	}
	if (con(g))
		printf("orient > 0? %f\n", dot(diff(hit, *g->cam_pos), obj.nr));
//	printf("orient calculated\n");
//	v = dot(obj.nr, hit);
//	v = diff(hit, *obj.ctr);

	ret.bri = round(255 * dot(norm(diff(*g->li, hit)), obj.nr));
	if (con(g))
		printf("bri is %d\n", ret.bri);
//	printf("x y is %d, %d\n", x, y);
//	screen(obj.tile[1].data_ptr, obj.tile[1].w, obj.tile[1].h, g);
//	tilen = 5;

	if (obj.tile[0].data_ptr)
	{
		printf("there is tile\n");
		dst2 = dot(diff(hit, *g->cam_pos), diff(hit, *g->cam_pos));
		newres = g->ray->z / (double)sqrt(dst2) * obj.tile[0].w;

	ti = obj.tile;
	if (con(g))
	{
		printf("square distange to plane is %f\n", dot(diff(hit, *g->cam_pos), diff(hit, *g->cam_pos)));
		printf("distance to plane is %f\n", sqrt(dot(diff(hit, *g->cam_pos), diff(hit, *g->cam_pos))));

		printf("tile is %f\n", tilen);
		printf("newrsi is %f\n", newres);
		printf("potenital tile n %f\n",log2( obj.tile[0].w / newres));
	}	
	tilen = log2(obj.tile[0].w / newres);

	x = lround(fabs(v.x)) % (ti[0].w);
	y = lround(fabs(v.z)) % (ti[0].h);



	tilelow = floor(tilen);
	tilehi = ceil(tilen);
	float weight[2];

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
//	t_vector colhi[4];
	double	colweight[4];
//	double colhiweight[4];
	
	col[0] = base255(rgb(*(ti[tilelow].data_ptr + ti[tilelow].w * (int)floor(x) + (int)floor(y))));
	col[1] = base255(rgb(*(ti[tilehi].data_ptr + ti[tilehi].w * (int)floor(x) + (int)floor(y))));

/*	col[0] = base255(rgb(*(ti[tilelow].data_ptr + ti[tilelow].w * (int)floor(x) + (int)floor(y))));
	col[1] = base255(rgb(*(ti[tilelow].data_ptr + ti[tilelow].w * (int)ceil(x) + (int)ceil(y))));
	col[2] = base255(rgb(*(ti[tilelow].data_ptr + ti[tilelow].w * (int)ceil(x) + (int)floor(y))));
	col[3] = base255(rgb(*(ti[tilelow].data_ptr + ti[tilelow].w * (int)floor(x) + (int)ceil(y))));
*/

/*	colweight[0] = sqrt((1 - (x - floor(x))) * (1 -(x - floor(x))) + (1 - (y - floor(y))) * (1 - (y - floor(y)))) / (double)2;
	colweight[1] = sqrt(1 - ((ceil(x) - x)) * (1 - (ceil(x) - x)) + (1 - (ceil(y) - y)) * (1 - (ceil(y) - y))) / (double)2;
	colweight[3] = sqrt(1 - ((floor(x) - x)) * (1 - (floor(x) - x)) + (1 - (ceil(y) - y)) * (1 - (ceil(y) - y))) / (double)2;
	colweight[2] = sqrt(1 - ((ceil(x) - x)) * (1 - (ceil(x) - x)) + (1 - (floor(y) - y)) * (1 - (floor(y) - y))) / (double)2;
*/
/*	colhi[0] = base255(rgb(*(ti[tilehi].data_ptr + ti[tilehi].w * (int)floor(x) + (int)floor(y))));
	colhi[1] = base255(rgb(*(ti[tilehi].data_ptr + ti[tilehi].w * (int)ceil(x) + (int)ceil(y))));
	colhi[2] = base255(rgb(*(ti[tilehi].data_ptr + ti[tilehi].w * (int)ceil(x) + (int)floor(y))));
	colhi[3] = base255(rgb(*(ti[tilehi].data_ptr + ti[tilehi].w * (int)floor(x) + (int)ceil(y))));

	colhiweight[0] = sqrt((1 - (x - floor(x))) * (1 -(x - floor(x))) + (1 - (y - floor(y))) * (1 - (y - floor(y)))) / (double)2;
	colhiweight[1] = sqrt(1 - ((ceil(x) - x)) * (1 - (ceil(x) - x)) + (1 - (ceil(y) - y)) * (1 - (ceil(y) - y))) / (double)2;
	colhiweight[3] = sqrt(1 - ((floor(x) - x)) * (1 - (floor(x) - x)) + (1 - (ceil(y) - y)) * (1 - (ceil(y) - y))) / (double)2;
	colhiweight[2] = sqrt(1 - ((ceil(x) - x)) * (1 - (ceil(x) - x)) + (1 - (floor(y) - y)) * (1 - (floor(y) - y))) / (double)2;



	t_vector retcol[2];
	retcol[0] = sum(scale(colweight[0],col[0]), sum(scale(colweight[1],col[1]), sum(scale(colweight[2],col[2]), scale(colweight[3], col[3]))));


	retcol[1] = sum(scale(colhiweight[0],colhi[0]), sum(scale(colhiweight[1],colhi[1]), sum(scale(colhiweight[2],colhi[2]), scale(colhiweight[3], colhi[3]))));

	ret.col = sum(scale(weight[0], retcol[0]), scale(weight[1], retcol[1]));
*/

	if (con(g))
		printf("colweight 0 %f\ncolweight 1 %f\n", colweight[0], colweight[1]);


//	ret.col = base255(rgb(*(ti[0].data_ptr + ti[0].w * x + y)));
//	ret.col = base255(rgb(*(ti[(int)tilen].data_ptr + ti[(int)tilen].w * x + y)));


//	ret.col = sum(scale(colweight[0], col[0]), scale(colweight[1], col[1]));

	ret.col = sum(scale(weight[0], col[0]), scale(weight[1], col[1]));



	if (con(g))
		printf("color is %f,%f,%f\n", ret.col.x, ret.col.y, ret.col.z);

//	hit.y = 0;
//	chess = lround(sqrt(dot(v, v)) / (double)80) % 2 == 1;
/*
	if (chess)
		init_vector(&ret.col, 0, 0, 0);

	else
		ret.col = obj.color;
	*/
	}
	else
	{
//		printf("calculating chess\n");
//		chess = lround(fabs(hit.x) / (double)80) % 2 == lround(fabs(hit.z) / (double)80) % 2;/
//		if (chess)
//			init_vector(&ret.col, 0, 0, 0);
//		else
			ret.col = obj.color;
	}
	if (0 == 1 && obstructed(hit, obj, g))
	{
	//	printf("now obstructed\n");
		retobs = g->ambient;
		if (chess)
		{
			if (con(g))
				printf("hit darker\n");
//			init_vector(&ret.col, 0, 0, 0);
		}
		else
//			ret.col = obj.color;
		tempbri = ret.bri /*/ (double)2*/;
		ret.bri = fmin(tempbri, retobs);
		if (con(g))
		{
			if (retobs == ret.bri)
				printf("returning constant bri\n");
			else if (tempbri == ret.bri)
				printf("returning darker bri\n");
			else
				printf("error\n");
		}
	}
	if (ret.bri < g->ambient)
	{
		ret.bri = g->ambient;
		if (con(g))
			printf("returning ambient bri\n");
		return (ret);
	}
	return (ret);
}
