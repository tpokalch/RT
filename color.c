/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   color.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpokalch <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/03 12:40:58 by tpokalch          #+#    #+#             */
/*   Updated: 2019/07/17 20:17:40 by tpokalch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

int			brg(t_vector a)
{
//	int			ret;

//	ret = a.z + a.y * 256 + a.x * 65536;
	return (a.z + a.y * 256 + a.x * 65536);
//	return (ret);
}

t_vector	rgb(int c)
{
	t_vector	ret;

	ret.x = c / (65536);
	c = c - ret.x * 65536;
	ret.y = c / (256);
	c = c - ret.y * 256;
	ret.z = c;
	return (ret);
}

t_vector	base255(t_vector dir)
{
	dir.x = dir.x / (double)255;
	dir.y = dir.y / (double)255;
	dir.z = dir.z / (double)255;
	return (dir);
}

t_vector	base(t_vector dir)
{
	double		max;

	max = fmax(dir.x, fmax(dir.y, dir.z));
	dir.x = dir.x / (double)max;
	dir.y = dir.y / (double)max;
	dir.z = dir.z / (double)max;
//	dir.x = lround(dir.x);
//	dir.y = lround(dir.y);
//	dir.z = lround(dir.z);
	return (dir);
}

int			color(int b, t_vector v)
{
//	v = base(c);
//	if (con(g))
	v.x = lround(b * v.x);
	v.y = lround(b * v.y);
	v.z = lround(b * v.z);
	return(v.z + v.y * 256 + v.x * 65536);
}
