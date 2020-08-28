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
//	return (a.z + a.y * 256 + a.x * 65536);
	return (((unsigned int)a.x << 16) +
		((unsigned int)a.y << 8) +
		((unsigned int)a.z));
}

t_vector	rgb(int c)
{
	t_vector	ret;

	unsigned int a = c;

	ret.x = (a >> 16);
	a = (a & (0x00FFFF));
	ret.y = (a >> 8);
	a = (a & (0x0000FF));
	ret.z = a;

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
	v.x = lround(b * v.x);
	v.y = lround(b * v.y);
	v.z = lround(b * v.z);

	v.x = ((unsigned int)v.x << 16);
	v.y = ((unsigned int)v.y << 8);
	return(v.x + v.y + v.z);
}




