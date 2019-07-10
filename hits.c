/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hits.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpokalch <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/03 20:35:53 by tpokalch          #+#    #+#             */
/*   Updated: 2019/07/10 21:11:57 by tpokalch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

t_dstpst		hit_plane(t_vector st, t_vector end, t_object obj, t_global *g)
{
	t_dstpst t;
	t_global p;

	p = *g;
	t.dst = -dot(diff(st, *obj.ctr), obj.nr) / dot(diff(end, st), obj.nr);
	if (t.dst < 0)
		return (*nani(&t));
	return (t);
}

t_dstpst		hit_sphere(t_vector st, t_vector end, t_object obj, t_global *g)
{
	t_vector	dx[2];
	t_vector	abc;
	double		det;
	t_dstpst	t;
	t_global	p;

	p = *g;
	t.pst = 0;
	dx[0] = (diff(end, st));
	dx[1] = diff(st, *obj.ctr);
	abc.x = dot(dx[0], dx[0]);
	abc.y = 2 * dot(dx[1], dx[0]);
	abc.z = dot(dx[1], dx[1]) - obj.rd2;
	det = abc.y * abc.y - 4 * abc.x * abc.z;
	if (det < 0)
		return (*(nani(&t)));
	t.dst = (-abc.y - sqrt(det)) / (2 * abc.x);
	if (t.dst < 0 && (t.pst = 1))
		t.dst = (-abc.y + sqrt(det)) / (2 * abc.x);
	if (t.dst < 0)
		return (*nani(&t));
	return (t);
}

t_dstpst		hit_cylinder(t_vector st, t_vector end, t_object obj,
		t_global *g)
{
	t_vector d;
	t_vector po[4];
	t_dstpst t;
	t_global p;

	p = *g;
	t.pst = 0;
	po[0] = (diff(end, st));
	po[3] = diff(st, *obj.ctr);
	d.y = dot(po[0], obj.nr);
	d.x = dot(po[3], obj.nr);
	po[2].x = dot(po[0], po[0]) - d.y * d.y;
	po[2].y = 2 * (dot(po[0], po[3]) - d.y * d.x);
	po[2].z = dot(po[3], po[3]) - d.x * d.x - obj.rd2;
	d.z = po[2].y * po[2].y - 4 * po[2].x * po[2].z;
	if (d.z < 0)
		return (*nani(&t));
	t.dst = (-po[2].y - sqrt(d.z)) / (2 * po[2].x);
	if (t.dst < 0 && (t.pst = 1))
		t.dst = (-po[2].y + sqrt(d.z)) / (2 * po[2].x);
	if (t.dst < 0)
		return (*(nani(&t)));
	return (t);
}

t_dstpst		hit_cone(t_vector st, t_vector end, t_object obj, t_global *g)
{
	t_vector	dx[2];
	t_vector	dvxvdet;
	t_vector	abc;
	t_global	p;
	double		ret;

	p = *g;
	dx[0] = diff(st, *obj.ctr);
	dx[1] = diff(end, st);
	dvxvdet.x = dot(dx[1], obj.nr);
	dvxvdet.y = dot(dx[0], obj.nr);
	abc.x = dot(dx[1], dx[1]) - (1 + obj.rd2) * dvxvdet.x * dvxvdet.x;
	abc.y = 2 * (dot(dx[1], dx[0]) - (1 + obj.rd2) * dvxvdet.x * dvxvdet.y);
	abc.z = dot(dx[0], dx[0]) - (1 + obj.rd2) * dvxvdet.y * dvxvdet.y;
	dvxvdet.z = abc.y * abc.y - 4 * abc.x * abc.z;
	if (dvxvdet.z < 0)
		return (*nani(&g->cone[0]));
	g->cone[0].dst = (-abc.y - sqrt(dvxvdet.z)) / (2 * abc.x);
	g->cone[1].dst = (-abc.y + sqrt(dvxvdet.z)) / (2 * abc.x);
	ret = fmin(g->cone[0].dst, g->cone[1].dst);
	if (ret < 0)
	{
		g->cone[0].dst = fmax(g->cone[1].dst, g->cone[0].dst);
		if (g->cone[0].dst < 0)
			return (*nani(&g->cone[0]));
		else
			return (g->cone[0]);
	}
	g->cone[0].dst = ret;
	return (g->cone[0]);
}
