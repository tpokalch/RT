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

t_colbri		refl(t_vector refl, t_vector hit, t_vector nrm, t_object obj, t_global *g)
{
//	t_vector camhit;
//	t_vector camhitx;
//	t_vector refl;
	t_dstpst temp;
	t_objecthit reflobj;
	t_colbri ret;

//	camhit = diff(hit, st);
//	camhitx = diff(camhit, scale(dot(camhit, nrm), nrm));
//	refl = diff(scale(2, camhitx), camhit);

	if (con(g))
		printf("doing reflection objecthit\n");
	objecthit(&temp, hit, sum(refl, hit), g->obj, g->argc + 1, g);

	reflobj.hit = sum(scale(temp.dst, refl), hit);
	reflobj.obj = temp.obj;
	if (reflobj.obj.name == NULL)
	{
		if (con(g))
		{
			printf("no object reflected\n");
			printf("returning color 0 0 0, bri 0\n");
		}
		init_vector(&ret.col, 0, 0, 0);
		ret.bri = 0;
		return (ret);
	}
	if (con(g))
		printf("there was reflected object is %d %s calculating it's color\n", reflobj.obj.id, reflobj.obj.name);
	ret = reflobj.obj.bright(hit, reflobj.hit, reflobj.obj, g);
	ret.col = scale(ret.bri / (double)255, ret.col);
	return (ret);
}

void		do_re(t_vector reflrayv, t_vector *reocol, t_vector tileocol, t_vector hit, t_vector nrm, t_object obj, t_global *g)
{
		t_vector reflrayv1;
		t_colbri reflcolbri;

		reflrayv1 = reflrayv;
		if (con(g))
			printf("recursion is %d\n", g->recursion);
		if (g->recursion < MAX_REC)
		{
//			g->recursion++;
			reflcolbri = refl(reflrayv1, hit, nrm, obj, g);
			*reocol = reflcolbri.col;
			*reocol = sum(scale(1 - obj.re, tileocol), scale(obj.re, /*reflcolbri.col*/base255(scale(reflcolbri.bri, reflcolbri.col))));
//			if (con(g))
//				printf("color after refl %f,%f,%f\n", reo->col.x, reo->col.y, reo->col.z);
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
	t_vector	end;
	t_vector	start;

//	g->recursion++;
//	if (g->recursion > 4)
//		return (ret);
//NEED TO MAKE ANOTJER RECURSION LIMIT
	g->recursion++;
//TEMPORARI SOLUTION. DONT KNOW WHY doesn't work WORKS YET
//no fix
	ray = diff(hit, st);
	if (con(g))
	{
		printf("_______inside trans______\n");
		printf("obj re is %f\n", obj.re);
		printf("calling objecthi\n");
	}
	objecthit(&temp, hit, sum(ray, hit), g->obj, g->argc + 1, g);
	if (con(g))
		printf("objecthit done\n");
	transobj.hit = sum(scale(temp.dst, ray), hit);
	transobj.obj = temp.obj;
	
//WJY THIS BUG? DOENST RETURN THE COMPLETE OBJECT SPECIFICATIONS
	transobj.obj.cam_pos = 0;
	if (con(g))
	{
		printf("obj trans is %f\n", obj.trans);
		printf("trans of transobj is %f\n", transobj.obj.trans);
		printf("trans hit %d %s\n", transobj.obj.id, transobj.obj.name);
		printf("hit %f,%f,%f\n", transobj.hit.x, transobj.hit.y, transobj.hit.z);
	}

	if (transobj.obj.name == NULL)
	{
		if (con(g))
			printf("returning retorig\n");
		init_vector(&ret.col, 0, 0, 0);
		ret.bri = g->ambient;
		return (ret);
	}
	if (con(g))
		printf("calling brihgt\n");
//will be infinite recursion if this is refl
	ret = transobj.obj.bright(hit, transobj.hit, transobj.obj, g);
	if (con(g))
		printf("trans returns color %f,%f,%f bri %d\n", ret.col.x, ret.col.y, ret.col.z, ret.bri);


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

		if (con(g))
			printf("______START DO TRANS FUNC_____\n");
		transo = trans(st, hit, nrm, obj, g);
		if (con(g))
			printf("ater trans is %f, %f,%f bri %d\n", transo.col.x, transo.col.y, transo.col.z, transo.bri);
		transo.col = base255(scale(transo.bri, transo.col));
//		transo.col = base(scale(transo.bri, transo.col));
		transo.col = sum(scale(1 - obj.trans, reo.col), scale(obj.trans, transo.col));
		ret->col = transo.col;

		ret->bri = transo.bri;
//		ret->bri = (1 - obj.trans) * reo.bri + (obj.trans * transo.bri);
		if (con(g))
			printf("______END DO TRANS FUNC_____\n");
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

t_colbri	bright_cone(t_vector st, t_vector hit, t_object obj, t_global *g)
{
	t_vector	nrm;
	t_vector	camforw;
	t_vector	hit0;
	t_colbri			ret;
	t_vector	ax;
	int		retobs;
	t_vector	 hitli[g->lights];
	t_vector	reflrayv;

	hit0 = diff(hit, *obj.ctr);
	ax = scale(dot(hit0, obj.base[1]) * (1 + obj.rd2), obj.base[1]);
	nrm = norm(diff(hit0, ax));

	init_hitli(hitli, hit, g);
	if (inside_cone(*g->cam_pos, obj, g))
	{
		printf("inside cone\n");
		nrm = scale(-1, nrm);
		if (!inside_cone(*g->li, obj, g))
			ret.bri = g->ambient;
		else
			init_bri(&ret.bri, hitli, nrm, g);
	}
	else
		init_bri(&ret.bri, hitli, nrm, g);
	if (con(g))
		printf("bri is after init %d\n", ret.bri);
	obj.nr = nrm;
	ret.nrm = nrm;
	if (obj.tile[0].data_ptr)
	{
		double x;
		double y;

		t_vector proj;


	t_vector ctrhit;

//	printf("hi\n");
	ctrhit = diff(hit, *obj.ctr);

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
	if (g->mip_map)
		ret.col = mip_col(y, x, dot(diff(hit, *g->cam_pos), diff(hit, *g->cam_pos)), obj, g);
	else
		ret.col = *(obj.tile[0].vectile + lround(y) * obj.tile[0].w + lround(x));
	}
	else
		ret.col = obj.color;
	ret.colself = ret.col;
	if (con(g))
		printf("bri now %d\n", ret.bri);	
	if (obj.re || obj.spec)
		reflrayv = reflray(st, hit, nrm, g);
	if (obj.re)
		do_re(reflrayv, &ret.col, ret.col, hit, nrm, obj, g);
	if (obj.trans)
		do_trans(st, hit, &ret, ret, nrm, obj, g);
	if (con(g))
		printf("bri is in the end %d\n", ret.bri);	
	obstructed(&ret, hit, hitli, reflrayv, obj, g);
	return (ret);
}

t_colbri	simple_bright_cylinder(t_vector st, t_vector hit, t_object obj, t_global *g)
{
	t_vector	nrm;
	t_colbri		ret;
	t_dstpst	t;
	int		retobs;
	t_vector	hitli[g->lights];
	int		i;
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

//	else
//		ret.col = ret.col;
///
	if (obj.trans)
		do_trans(st, hit, &ret, ret, nrm, obj, g);


//////
	obstructed(&ret, hit, hitli, reflrayv, obj, g);
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
	t_vector	hitli[g->lights];
	int		i;
	t_vector	reflrayv;

	hit0 = diff(hit, *obj.ctr);
	vrt = obj.base[1];
	vrt = scale(dot(hit0, vrt), vrt);
	nrm = norm(diff(hit0, vrt));

	init_hitli(hitli, hit, g);
	if (obj.cam_pos)
	{
		t = obj.hit(*g->cam_pos, *g->li, diff(*g->li, *g->cam_pos),obj, g);
		nrm = scale(-1, nrm);	
		if (t.dst < 1)
			ret.bri = g->ambient;
		else
			init_bri(&ret.bri, hitli, nrm, g);
	}
	else
		init_bri(&ret.bri, hitli, nrm, g);
	obj.nr = nrm;	
	ret.nrm = nrm;	
	if (obj.tile[0].data_ptr)
	{
		double x;
		double y;

		t_vector proj;
		t_vector ctrhit;
		ctrhit = diff(hit, *obj.ctr);
		proj = diff(ctrhit, scale(dot(obj.base[1], ctrhit),obj.base[1]));
		proj = norm(proj);
		if (con(g))
		{
			printf("proj is %f,%f,%f\n", proj.x, proj.y, proj.z);
			printf("width is %d\n", obj.tile[0].w);
			printf("acos is %f\n", acos(dot(obj.base[0], proj)));
		}
	x = obj.tile[0].w2 * (1 - (1 - 2 * (det(proj, obj.base[0]) < 0)) * M_1_PI * acos(dot(proj, obj.base[0])));

	double xdst;
	xdst = dot(obj.base[1], diff(hit, *obj.ctr));
//	if (xdst > 0)
//		y = lround(obj.tile[0].h - fmod(xdst, obj.tile[0].h));
//	else
//		y = lround(-fmod(xdst, obj.tile[0].h + 1));
	if (con(g))
	{
		printf("xdst is %f\n", xdst);
		printf("tile h is %d\n", obj.tile[0].h);
	}
//	y = mymod(xdst, obj.tile[0].h);

	if (con(g))
	{
		printf("x is %f\n", x);
		printf("y is %f\n", y);
	}
	if (g->mip_map)
	{
		x = obj.tile[0].w2 * (1 - (1 - 2 * (det(proj, obj.base[0]) < 0)) * M_1_PI * acos(dot(proj, obj.base[0])));
		y = mymod(xdst, obj.tile[0].h);
		ret.col = mip_col(y, x, dot(diff(hit, *g->cam_pos), diff(hit, *g->cam_pos)), obj, g);
	}
	else
	{
		x = obj.tile[0].w2 * /* (1 - (1 - 2 * (det(proj, obj.base[0]) < 0)) */ M_1_PI * myacos(proj, obj.base[0], obj.base[1], g);
//		x = obj.tile[0].w2 * (/*1 - (1 - 2 * (det(proj, obj.base[2]) < 0))*/ 1 * M_1_PI * myacos(proj, obj.base[2], obj.base[1], g));


		y = myintmod(xdst, obj.tile[0].h);
//		printf("x y is %f,%f\n", x, y);
		ret.col = *(obj.tile[0].vectile + lround(y) * obj.tile[0].w + lround(x));
		ret.colself = ret.col;
	}
	}
	else
	{
		ret.colself = obj.color;
	}
//	t_vector savecolself;

//	savecolself = ret.colself;
	if (obj.spec || obj.re)
		reflrayv = reflray(st, hit, nrm, g);
	if (obj.re)
		do_re(reflrayv, &ret.col, ret.col, hit, nrm, obj, g);
	if (obj.trans)
		do_trans(st, hit, &ret, ret, nrm, obj, g);
//	ret.colself = savecolself;
	obstructed(&ret, hit, hitli, reflrayv, /*nrm, */obj, g);

	if (ret.bri < g->ambient)
		ret.bri = g->ambient;
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
	t_vector	nrm;
	t_vector	ctrli;
	t_colbri	ret2;
	int		retobs;
	t_vector	proj;
	t_vector ctrhit;
	int i;
	t_colbri retorig;
	t_vector reflrayv;
	t_vector hitli[g->lights];

	init_hitli(hitli, hit, g);
	if (con(g))
		printf("______start BRI SPHERE func______\n");
	nrm = obj.nr;
	if (obj.cam_pos)
	{
//		printf("hi\n");
		ctrli = diff(*g->li, *obj.ctr);
		if (dot(ctrli, ctrli) > obj.rd2)
		{
			ret.col = obj.color;
			ret.bri = g->ambient;
			return (ret);
		}
//		nrm = scale(-1, nrm);
	}
	init_bri(&retorig.bri, hitli, nrm, g);

	ret.col = obj.color;
//recently commented don't know if creates bugs
//	ret.colself = obj.color;

	if (con(g))
	{
		printf("bri is %d\n", ret.bri);
	}

	if (obj.spec || obj.re)
		reflrayv = reflray(st, hit, nrm, g);
	if (obj.re)
		do_re(reflrayv, &ret.col, ret.col, hit, nrm, obj, g);
	ret.bri = retorig.bri;
	if (obj.trans)
		do_trans(st, hit, &ret, ret, nrm, obj, g);
	obstructed(&ret, hit, hitli, reflrayv, obj, g);
	if (ret.bri < g->ambient)
		ret.bri = g->ambient;
	return (ret);
}


t_vector		do_tile_sphere(t_vector *tileocol, t_vector st, t_vector hit, t_vector nrm, t_object obj, t_global *g)
{
		t_vector ctrhit;
		t_vector proj;
		double x;
		double y;

		ctrhit = diff(hit, *obj.ctr);
		proj = diff(ctrhit, scale(dot(obj.base[1], ctrhit),obj.base[1]));
		proj = norm(proj);
//		x = obj.tile[0].w2 * (/*1 - (1 - 2 * (det(proj, obj.base[2]) < 0))*/ 1 * M_1_PI * acos(dot(proj, obj.base[2])));
		y = obj.tile[0].h * (1/*1 - 2 * (det(nrm, obj.base[1]) < 0)*/) * M_1_PI * acos(dot(nrm, obj.base[1]));
		x = obj.tile[0].w2 * (/*1 - (1 - 2 * (det(proj, obj.base[2]) < 0))*/ 1 * M_1_PI * myacos(proj, obj.base[2], obj.base[1], g));

//		y = obj.tile[0].h * (1/*1 - 2 * (det(nrm, obj.base[1]) < 0)*/) * M_1_PI * myacos(nrm, obj.base[1], obj.base[2], g);
//	printf("x y is %f, %f\n", x, y);
		if (con(g))
			printf("%d\n", 1 - 2 * (det(nrm, obj.base[1]) > 0));
 	if (con(g))
	{
		printf("obj base if %f,%f,%f\n", obj.base[0].x, obj.base[1].y, obj.base[2].z);
		printf("sign det %f\n", det(proj, obj.base[2]));
		printf("proj is %f,%f,%f\n", proj.x, proj.y, proj.z);
	}
	if (con(g))
	{
		printf("x acos is %f\n", myacos(proj, obj.base[2], obj.base[1], g));
		printf("y acos is %f\n", myacos(nrm, obj.base[1], obj.base[0], g));

	}
//	y = lround(obj.tile[0].h2 * (1 - (dot(obj.base[1], nrm))));
	if (round(x) >= obj.tile[0].w2)
		x--;
	if (con(g))
		printf("x, y is %f, %f\n", x, y);
	if (g->mip_map)
		*tileocol = mip_col(y, x, dot(diff(hit, *g->cam_pos), diff(hit, *g->cam_pos)), obj, g);
	else
	{
		*tileocol = *(obj.tile[0].vectile + lround(y)* obj.tile[0].w + lround(x));
//		*tileocol = base255(rgb(*(obj.tile[0].data_ptr + obj.tile[0].w * lround(y) + lround(x))));
	}
	if (con(g))
		printf("color from tile is %f,%f,%f\n", tileocol->x, tileocol->y, tileocol->z);
	return (*tileocol);
}

//this function takes the refl ray, the color it has to rewrite, the color without reflection, all else needs no exp

t_colbri	bright_sphere(t_vector st, t_vector hit, t_object obj, t_global *g)
{
	t_colbri		ret;
	t_vector	nrm;
	t_vector	ctrli;
	int		retobs;
	t_vector	proj;
	t_vector	ctrhit;
	t_colbri	retorig;
	t_colbri	transo;
	t_colbri	reo;
	t_colbri	tileo;
	t_colbri	speco;
	int		i;
	t_vector	reflrayv;
	t_vector	hitli[g->lights];
	static int rec = 0;

	rec++;
	g->recursion++;
	if (con(g))
	{
		printf("______start BRI SPHERE func______\n");
		printf("rec is %d\n", rec);
	}
	init_hitli(hitli, hit, g);
	nrm = scale(1 / (double)obj.rd, diff(hit, *obj.ctr));
	if (obj.cam_pos)
	{
		if (!obj.trans)
		{
			ctrli = diff(*g->li, *obj.ctr);
			if (dot(ctrli, ctrli) > obj.rd2)
			{
//				ret.col = obj.color;
				ret.bri = g->ambient;
			}
		}
	//	printf("changing nr\n");
		nrm = scale(-1, nrm);
	}
	ret.nrm = nrm;
	obj.nr = nrm;
	init_bri(&reo.bri, hitli, nrm, g);
	if (con(g))
	{
		printf("brightnress is %d\n", retorig.bri);
		printf("ambient is %d\n", g->ambient);
	}
	if (obj.tile[0].data_ptr)
		ret.colself = do_tile_sphere(&reo.col, st, hit, nrm, obj, g);
	else
	{
		ret.colself = obj.color;
		reo.col = obj.color;		
	}
	if (con(g))
		printf("colbri after tile is %f,%f,%f bri %d\n", tileo.col.x, tileo.col.y, tileo.col.z, tileo.bri);
	if (obj.spec || obj.re)
		reflrayv = reflray(st, hit, nrm, g);
	if (obj.re)
		do_re(reflrayv, &reo.col, reo.col, hit, nrm, obj, g);
	if (con(g))
	{
		printf("colbri after re is %f,%f,%f bri %d\n", reo.col.x, reo.col.y, reo.col.z, reo.bri);
		printf("trans %f\n", obj.trans);
	}
//	int savereobri;

//	savereobri = reo.bri;
	if (obj.trans)
	{
		do_trans(st, hit, &ret, reo, nrm, obj, g);
//		ret.bri = reo.bri;
	}
	else
	{
		ret.col = reo.col;
		ret.bri = reo.bri;
	}
	if (con(g))
		printf("colbri after trans is %f,%f,%f bri %d\n", transo.col.x, transo.col.y, transo.col.z, transo.bri);
	if (con(g))
		printf("calling obstructed\n");
	obstructed(&ret, hit, hitli, reflrayv, obj, g);
	if (con(g))
		printf("returning col %f,%f,%f bri %d\n", ret.col.x, ret.col.y, ret.col.z, ret.bri);
	g->recursion = 0;
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



t_colbri		bright_spheror(t_vector st, t_vector hit, t_object obj, t_global *g)
{
	t_colbri	ret;
	t_vector	nrm;
	t_vector	ctrli;
	t_objecthit	reflobj;
	t_vector	camhit;
	t_vector	combcolor;
	t_vector	reflcolor;
	int		retorig;
	t_vector	proj;
	double		x;
	double		y;
	t_vector	ctrhit;
	t_vector	hitli = diff(*g->li, hit);

	if (con(g))
		printf("______start HIT SPHERE func______\n");
	nrm = norm(diff(hit, *obj.ctr));

	retorig = (round(255 * dot(norm(diff(*g->li, hit)), nrm)));
	if (retorig < g->ambient)
		retorig = g->ambient;

	if (obj.tile[0].data_ptr)
	{
		ctrhit = diff(hit, *obj.ctr);
		proj = diff(ctrhit, scale(dot(obj.base[1], ctrhit),obj.base[1]));
		proj = norm(proj);
		x = obj.tile[0].w2 * (1 - (1 - 2 * (det(proj, obj.base[2]) < 0)) * M_1_PI * acos(dot(proj, obj.base[2])));
		y = obj.tile[0].h * ( (1 - 2 * (det(nrm, obj.base[1]) < 0)) * M_1_PI * acos(dot(nrm, obj.base[1])));

	
		obj.color = mip_col(y, x, dot(diff(hit, *g->cam_pos), diff(hit, *g->cam_pos)), obj, g);
	}
	if(con(g))
	{
		printf("reflected object is %s\n", reflobj.obj.name);
		printf("_____end HIT SPHERE func_____\n");
		printf("retorig is %d\n", retorig);
	}

	ret = refl(st, hit, nrm, obj, g);
	ret.bri = ((1 - obj.re) * retorig + obj.re * ret.bri);
	ret.col = sum(scale(1 - obj.re, obj.color), scale(obj.re, ret.col));


	if (con(g))
		printf("reflcolor is %f,%f,%f\ncombcolor is %f, %f, %f\nret is %d\reflobj name is %s\n", ret.col.x,
						ret.col.y,
						ret.col.z,
			combcolor.x, combcolor.y, combcolor.z,
			ret.bri, reflobj.obj.name);

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

double			bell(double tile_dst, double crt_dst)
{
	double v;

	v = 5;	
	return (1 / sqrt(M_PI * 2 * v * v) * exp(-(crt_dst - tile_dst) * (crt_dst - tile_dst) / (2 * v * v)));
}


t_colbri		simple_bright_plane(t_vector st, t_vector hit, t_object obj, t_global *g)
{
	t_colbri			ret;
	int			i;
	int			retobs;
	int			chess;
	int			tempbri;
	t_vector	v;
	t_vector	reflrayv;
	t_vector	hitli[g->lights];
	
//	printf("object color is %f,%f,%f\n", obj.color.x, obj.color.y, obj.color.z);
	init_hitli(hitli, hit, g);
	if (/*dot(diff(hit, st), obj.base[1]) > 0*/ obj.cam_pos)
	{
		if (con(g))
			printf("changing normal\n");
		obj.base[1].x = -obj.base[1].x;
		obj.base[1].y = -obj.base[1].y;
		obj.base[1].z = -obj.base[1].z;
	}
	if (con(g))
		printf("orient > 0? %f, %d\n", dot(diff(hit, *g->cam_pos), obj.base[1]), obj.cam_pos);
	init_bri(&ret.bri, hitli, obj.base[1], g);
	ret.col = obj.color;
//recently comented
//	ret.colself = ret.col;
	if (obj.spec || obj.re)
		reflrayv = reflray(st, hit, obj.base[1], g);	
	if (obj.re)
		do_re(reflrayv, &ret.col, ret.col, hit, obj.base[1], obj, g);
	if (con(g))
		printf("color after re %f,%f,%f bri %d\n", ret.col.x, ret.col.y, ret.col.z, ret.bri);
	obj.nr = obj.base[1];
	obstructed(&ret, hit, hitli, reflrayv, obj, g);
	if (ret.bri < g->ambient)
	{
		ret.bri = g->ambient;
		if (con(g))
			printf("returning ambient bri\n");
		return (ret);
	}
	return (ret);
}

t_colbri		bright_plane(t_vector st, t_vector hit, t_object obj, t_global *g)
{
	t_colbri			ret;
	int			i;
	int			retobs;
	int			chess;
	int			tempbri;
	t_vector	v;
	double		x;
	double		y;
	t_colbri	retorig;
	t_vector	reflrayv;
	t_vector	hitli[g->lights];
	t_vector	colself;
//	printf("doing bright plane\n");
	init_hitli(hitli, hit, g);
	if (con(g))
		printf("doing bright plane\n");	
	if (/*dot(diff(hit, st), obj.base[1]) > 0*/ obj.cam_pos)
	{
		if (con(g))
			printf("changing normal\n");
//		printf("hi\n");
		obj.base[1].x = -obj.base[1].x;
		obj.base[1].y = -obj.base[1].y;
		obj.base[1].z = -obj.base[1].z;
	}
//	if (con(g))
//		printf("orient > 0? %f\n", dot(diff(hit, *g->cam_pos), obj.base[1]));
	init_bri(&retorig.bri, hitli, obj.base[1], g);
//	printf("color obj is %f,%f,%f, bri %d\n", obj.color.x, obj.color.y, obj.color.z, retorig.bri);
//	retorig.bri = round(255 * dot(norm(hitli), obj.base[1]));

	if (con(g))
		printf("bri is %d\n", ret.bri);
//	printf("x y is %d, %d\n", x, y);
//	screen(obj.tile[1].data_ptr, obj.tile[1].w, obj.tile[1].h, g);
//	tilen = 5;

	if (obj.tile[0].data_ptr)
	{
//		printf("there is tile\n");
		v = diff(hit, *obj.ctr);
		x = mymod(v.x, obj.tile[0].w);
		y = mymod(v.z, obj.tile[0].h);

		retorig.col = base(rgb(*(obj.tile[0].data_ptr + lround(y) * obj.tile[0].w + lround(x))));
//		ret.col = mip_col(y, x, dot(diff(hit, *g->cam_pos), diff(hit, *g->cam_pos)), obj, g);
		ret.colself = retorig.col;
	}
	else
	{
//		printf("calculating chess\n");
		chess = lround(fabs(hit.x) / (double)80) % 2 == lround(fabs(hit.z) / (double)80) % 2;
		if (chess)
			init_vector(&retorig.col, 1, 0, 0.5);
		else
			retorig.col = obj.color;
//		printf("writing %f,%f,%f\n", retorig.col.x, retorig.col.y, retorig.col.z);
		ret.colself = retorig.col;
		colself = retorig.col;
	}
//	printf("colself now %f,%f,%f\n", ret.colself.x, ret.colself.y, ret.colself.z);	
	if (obj.spec || obj.re)
		reflrayv = reflray(st, hit, obj.base[1], g);
	if (obj.re)
	{

		do_re(reflrayv, &ret.col, retorig.col, hit, obj.base[1], obj, g);

/*
		t_vector reflrayv1;

		reflrayv1 = reflrayv;
		if (g->recursion < 3)
		{
			g->recursion++;
			ret = refl(reflrayv1, hit, obj.base[1], obj, g);
			ret.col = sum(scale(1 - obj.re, retorig.col), scale(obj.re, base255(scale(ret.bri, ret.col))));
			ret.colself = colself;
		}
		else
		{
			ret.col = retorig.col;
			ret.bri = retorig.bri;
		}
		g->recursion = 0;
*/
	}
	else
		ret.col = retorig.col;
	ret.bri = retorig.bri;

/*	if (obstructed(hit, hitli, obj, g))
	{
		retobs = g->ambient;
		tempbri = ret.bri;
		ret.bri = fmin(tempbri, retobs);
	}
*/
	if (con(g))
	{
		printf("doing obstructed from bright plane\n");
		printf("hit is at %f,%f,%f\n", hit.x, hit.y, hit.z);
	}
//	printf("col now is %f,%f,%f, bri %d\n", ret.col.x, ret.col.y, ret.col.z, ret.bri);
//	printf("colself before obs %f,%f,%f\n", ret.colself.x, ret.colself.y, ret.colself.z);
	obj.nr = obj.base[0];
	obstructed(&ret, hit, hitli, reflrayv, obj, g);
//most likely not needed
/*
	if (ret.bri < g->ambient && (ret.bri = g->ambient))
	{
		if (con(g))
			printf("returning ambient bri\n");
		return (ret);
	}
*/
//	ret.colself = colself;	
//	printf("col after obstructed returning %f,%f,%f, bri %d\n",ret.col.x, ret.col.y, ret.col.z, ret.bri);	
//	printf("colself returning %f,%f,%f\n", ret.colself.x, ret.colself.y, ret.colself.z);
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
		if (con(g))
			printf("changing normal\n");
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
	
//
//
	if (obj.spec)
		do_spec(&ret, hit, obj.base[1], reflrayv, obj, g);
	return (ret);
}

t_colbri		bright_tri(t_vector st, t_vector hit, t_object obj, t_global *g)
{
	t_colbri			ret;
	int			i;
	int			retobs;
	int			chess;
	int			tempbri;
	t_vector	v;
	double		x;
	double		y;
	t_tile		*ti;
	double		newres;
	double		tilen;
	double	dst2;
	int tilelow;
	int tilehi;
	t_colbri retorig;
	t_vector	hitli[g->lights];
		t_vector reflrayv;


//	printf("doing bright tri\n");
//	printf("object color %f,%f,%f\n", obj.color.x, obj.color.y, obj.color.z);
	init_hitli(hitli, hit, g);
//	printf("first hitli is %f,%f,%f\n", g->hitli[0].x, g->hitli[0].y, g->hitli[0].z);	
	if (con(g))
		printf("base is %f,%f,%f\n", obj.base[1].x, obj.base[1].y, obj.base[1].z); 
	if (dot(diff(hit, *g->cam_pos), obj.base[1]) > 0)
	{
		if (con(g))
			printf("changing normal\n");
		obj.base[1].x = -obj.base[1].x;
		obj.base[1].y = -obj.base[1].y;
		obj.base[1].z = -obj.base[1].z;
	}

	if (con(g))
		printf("orient > 0? %f\n", dot(diff(hit, *g->cam_pos), obj.base[1]));
//	printf("orient calculated\n");
//	v = dot(obj.base[1], hit);

	init_bri(&retorig.bri, hitli, obj.base[1], g);
//	retorig = round(255 * dot(norm(hitli), obj.base[1]));
	if (con(g))
	{
		printf("bri is %d\n", retorig.bri);
		printf("normal is %f,%f,%f\n", obj.base[1].x, obj.base[1].y, obj.base[1].z);
	}
	if (obj.tile[0].data_ptr)
	{
//		printf("calculating tile\n");
		v = diff(hit, *obj.ctr);
//		printf("v is %f,%f,%f\n", v.x, v.y, v.z);
		x = mymod(v.x, obj.tile[0].w);
		y = mymod(v.z, obj.tile[0].h);

//		printf("x y is %d, %d\n", x, y);
		if (con(g))
		{
			printf("w h of tile is %d, %d\n", obj.tile[0].w, obj.tile[0].h);
//			printf("x y is %d, %d\n", x, y);
		}
//		ret.col = base(rgb(*(obj.tile[0].data_ptr + y * obj.tile[0].w + x)));
		if (g->mip_map)
			ret.col = mip_col(y, x, dot(diff(hit, *g->cam_pos), diff(hit, *g->cam_pos)), obj, g);
		else
			ret.col = *(obj.tile[0].vectile + lround(y)* obj.tile[0].w + lround(x));
	}
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
	if (con(g))
		printf("returning color %f,%f,%f bri %d\n", ret.col.x, ret.col.y, ret.col.z, ret.bri);
	return (ret);
}
