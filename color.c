/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   color.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpokalch <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/03 12:40:58 by tpokalch          #+#    #+#             */
/*   Updated: 2019/07/03 12:42:49 by tpokalch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

int			brg(t_vector a)
{
	int			ret;

	ret = a.x + a.y * 256 + a.z * 65536;
	return (ret);
}

t_vector	rgb(int c)
{
	t_vector	ret;

	ret.z = c / (65536);
	c = c - ret.z * 65536;
	ret.y = c / (256);
	c = c - ret.y * 256;
	ret.x = c;
	return (ret);
}

t_vector	base(int c)
{
	t_vector	dir;
	double		max;

	dir = rgb(c);
	max = fmax(dir.x, fmax(dir.y, dir.z));
	dir.x = dir.x / (double)max;
	dir.y = dir.y / (double)max;
	dir.z = dir.z / (double)max;
	return (dir);
}

int			color(int b, int c)
{
	t_vector	v;

	v = base(c);
	v.x = lround(b * v.x);
	v.y = lround(b * v.y);
	v.z = lround(b * v.z);
	return (brg(v));
}
