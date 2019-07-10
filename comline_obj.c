/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_obj.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpokalch <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/03 20:17:05 by tpokalch          #+#    #+#             */
/*   Updated: 2019/07/06 16:08:32 by tpokalch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

int			is_coords(char *argv)
{
	int i;
	int coma_count;

	if (!argv)
		return (0);
	i = 0;
	coma_count = 0;
	while (*(argv + i) != '\0')
	{
		if (!(ft_isdigit(*(argv + i)))
		&& (*(argv + i) != ',' && (*(argv + i) != '-')))
			return (0);
		if (*(argv + i) == ',')
			coma_count++;
		i++;
	}
	if (coma_count != 2 && coma_count != 0)
		return (putstr("wrong number of comas\n", 0));
	return (1);
}

void		vectorify(void *obj_coord, char **argv)
{
	t_vector	ret;
	char		**split;
	int			i;

	i = 0;
	split = ft_strsplit(*argv, ',');
	while (*(split + i))
		i++;
	if (i == 3)
	{
		ret.x = ft_atoi(*(split + 0));
		ret.y = ft_atoi(*(split + 1));
		ret.z = ft_atoi(*(split + 2));
		*(t_vector *)obj_coord = ret;
	}
	else if (i == 1)
	{
		*((int *)obj_coord) = ft_atoi(*(split));
	}
	free_arr(split);
	free(split);
}

void		fill_obj(char **argv, int n, t_global *g)
{
	int i;

	i = 0;
	while (*(argv + i) && is_coords(*(argv + i)))
	{
		if (i == 0)
			vectorify((g->obj[n].ctr), (argv + 0));
		else if (i == 1)
			vectorify(&(g->obj[n].ang), (argv + 1));
		else if (i == 2)
			vectorify(&(g->obj[n].color), (argv + 2));
		else if (i == 3)
		{
			vectorify(&(g->obj[n].rd), (argv + 3));
			g->obj[n].rd2 = g->obj[n].rd * g->obj[n].rd;
		}
		i++;
	}
}

int			fill_objects(t_vector *ctr, char **argv, t_global *g)
{
	int			iobjc[2];
	t_vector	o10;

	init_vector(&o10, 0, 1, 0);
	iobjc[0] = -1;
	iobjc[1] = 0;
	while (*(argv + ++iobjc[0]))
	{
		if (obj_traver(argv + iobjc[0], "or") && (++iobjc[1]))
		{
			if (ft_strequ(*(argv + iobjc[0]), "plane"))
				init_plane(ctr, iobjc[1], g);
			else if (ft_strequ(*(argv + iobjc[0]), "sphere"))
				init_sphere(ctr, iobjc[1], g);
			else if (ft_strequ(*(argv + iobjc[0]), "cylinder"))
				init_cylinder(ctr, iobjc[1], g);
			else if (ft_strequ(*(argv + iobjc[0]), "cone"))
				init_cone(ctr, iobjc[1], g);
			fill_obj(argv + iobjc[0] + 1, iobjc[1], g);
			g->obj[iobjc[1]].nr = rotate(o10, g->obj[iobjc[1]].ang);
		}
		else if (!is_coords(*(argv + iobjc[0])))
			return (0);
	}
	return (1);
}
