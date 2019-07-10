/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   brights.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpokalch <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/03 20:37:08 by tpokalch          #+#    #+#             */
/*   Updated: 2019/07/06 14:49:06 by tpokalch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

int		inside_cone(t_vector p, t_object obj)
{
	double		axdst;
	double		cirad;
	t_vector	ptoaxproj;
	t_vector	ptoaxperp;
	t_vector	op;

	op = diff(p, *obj.ctr);
	if (dot(op, op) < 0.001)
		return (0);
	axdst = dot(op, obj.nr);
	cirad = axdst * obj.rd2;
	ptoaxproj = scale(axdst, obj.nr);
	ptoaxperp = diff(op, ptoaxproj);
	return (dot(ptoaxperp, ptoaxperp) - cirad * cirad <= 0);
}

int		bright_cone(t_vector hit, t_object obj, t_global *g)
{
	t_vector	nrm;
	t_vector	camforw;
	t_vector	hit0;
	int			ret;
	t_vector	ax;

	if (obstructed(hit, obj, g))
		return (0);
	hit0 = diff(hit, *obj.ctr);
	ax = scale(dot(hit0, obj.nr) * (1 + obj.rd2), obj.nr);
	nrm = norm(diff(hit0, ax));
	camforw = diff(sum(*g->cam_pos, *g->normal), *g->cam_pos);
	if (inside_cone(*g->cam_pos, obj))
	{
		if (!inside_cone(*g->li, obj))
			return (0);
		else
			nrm = scale(-1, nrm);
	}
	else if (inside_cone(*g->li, obj))
		return (0);
	ret = 255 * dot(norm(diff(diff(*g->li, *obj.ctr), hit0)), nrm);
	if (ret < 0)
		return (0);
	return (ret);
}

int		bright_cylinder(t_vector hit, t_object obj, t_global *g)
{
	t_vector	nrm;
	t_vector	vrt;
	t_vector	hit0;
	int			ret;
	t_dstpst	t;

	if (obstructed(hit, obj, g))
		return (0);
	hit0 = diff(hit, *obj.ctr);
	vrt = obj.nr;
	vrt = scale(dot(hit0, vrt), vrt);
	nrm = norm(diff(hit0, vrt));
	if (obj.cam_pos)
	{
		t = obj.hit(*g->cam_pos, *g->li, obj, g);
		if (t.dst < 1)
			return (0);
		nrm = scale(-1, nrm);
	}
	ret = 255 * dot(norm(diff(diff(*g->li, *obj.ctr), hit0)), nrm);
	if (ret < 0)
		return (0);
	return (ret);
}

int		bright_sphere(t_vector hit, t_object obj, t_global *g)
{
	int			ret;
	t_vector	nrm;
	t_vector	ctrli;

	if (obstructed(hit, obj, g))
		return (0);
	nrm = norm(diff(hit, *obj.ctr));
	ctrli = diff(*g->li, *obj.ctr);
	if (obj.cam_pos)
	{
		if (dot(ctrli, ctrli) > obj.rd2)
			return (0);
		nrm = scale(-1, nrm);
	}
	ret = (round(255 * dot(norm(diff(*g->li, hit)), nrm)));
	if (ret < 0)
		return (0);
	return (ret);
}

int		bright_plane(t_vector hit, t_object obj, t_global *g)
{
	int			ret;
	t_object	a;
	int			i;

	i = 0;
	a = obj;
	if (obstructed(hit, obj, g))
		return (0);
	if (dot(diff(hit, *g->cam_pos), obj.nr) > 0)
	{
		obj.nr.x = -obj.nr.x;
		obj.nr.y = -obj.nr.y;
		obj.nr.z = -obj.nr.z;
	}
	ret = round(255 * dot(norm(diff(*g->li, hit)), norm(obj.nr)));
	if (lround(fabs(hit.x) / 30) % 2 == lround(fabs(hit.z) / 30) % 2)
		ret = ret / (double)2;
	if (ret < 0)
		return (0);
	return (ret);
}
