/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lin_alg.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpokalch <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/03 12:30:35 by tpokalch          #+#    #+#             */
/*   Updated: 2019/07/03 12:30:40 by tpokalch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

t_vector	scale(double a, t_vector b)
{
	b.x *= a;
	b.y *= a;
	b.z *= a;
	return (b);
}

double		dot(t_vector a, t_vector b)
{
	return (a.x * b.x + a.y * b.y + a.z * b.z);
}

t_vector	norm(t_vector a)
{
	double leng;

	leng = sqrt(dot(a, a));
	a.x = a.x / (double)leng;
	a.y = a.y / (double)leng;
	a.z = a.z / (double)leng;
	return (a);
}

t_vector	sum(t_vector b, t_vector a)
{
	a.x = b.x + a.x;
	a.y = b.y + a.y;
	a.z = b.z + a.z;
	return (a);
}

t_vector	diff(t_vector a, t_vector b)
{
	a.x = a.x - b.x;
	a.y = a.y - b.y;
	a.z = a.z - b.z;
	return (a);
}
