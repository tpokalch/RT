/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_obj.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpokalch <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/03 20:20:33 by tpokalch          #+#    #+#             */
/*   Updated: 2019/07/06 16:01:01 by tpokalch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

int		obj_traver(char **argv, char *c)
{
	if (ft_strequ(c, "or"))
	{
		return (ft_strequ(*(argv), "plane") || ft_strequ(*(argv), "sphere")
		|| ft_strequ(*(argv), "cone") || ft_strequ(*(argv), "cylinder"));
	}
	return (!(ft_strequ(*(argv), "plane")) &&
		!(ft_strequ(*(argv), "sphere")) &&
		!(ft_strequ(*(argv), "cylinder")) &&
		!(ft_strequ(*(argv), "cone")));
}

int		init_objects(t_vector *ctr, char **argv, t_global *g)
{
	int			i;
	t_vector	o10;

	init_vector(&o10, 0, 1, 0);
	i = -1;
	g->obj = (t_object *)malloc(sizeof(t_object) * (g->argc + 2));
	while (*(argv + ++i))
	{
		if (ft_strequ(*(argv + i), "plane"))
			init_plane(ctr, i + 1, g);
		else if (ft_strequ(*(argv + i), "sphere"))
			init_sphere(ctr, i + 1, g);
		else if (ft_strequ(*(argv + i), "cylinder"))
			init_cylinder(ctr, i + 1, g);
		else if (ft_strequ(*(argv + i), "cone"))
			init_cone(ctr, i + 1, g);
		if (obj_traver((argv + i), "or"))
			g->obj[i + 1].nr = rotate(o10, g->obj[i + 1].ang);
		else
			return (0);
	}
	return (1);
}

int		arg_valid(char **argv)
{
	int i;
	int ret;
	int coords;

	i = 0;
	ret = 0;
	while ((coords = 0) == 0 && *(argv + i))
	{
		if (obj_traver(argv + i, "and"))
			return (putstr("unknown object\n", 0));
		else
		{
			ret++;
			i++;
		}
		while (is_coords(*(argv + i)))
		{
			coords++;
			i++;
		}
		if (coords > 4)
			return (putstr("too many coords\n", 0));
	}
	return (ret);
}

int		usage(void)
{
	ft_putstr("Usage: ./RTv1 obj1 [x,y,z] [angx,angy,angz]");
	ft_putstr(" [color] [radius] [obj2]...\n");
	ft_putstr("              sphere\n");
	ft_putstr("              plane\n");
	ft_putstr("              cone\n");
	ft_putstr("              cylinder\n");
	return (0);
}

int		check_arg(char **argv, int argc, t_global *g, t_vector *ctr)
{
	if (argc < 2)
		return (usage());
	g->argc = argc - 1;
	argv = argv + 1;
	{
		if (!(g->argc = (arg_valid(argv))))
			return (putstr("Arg invalid\n", 0));
		g->obj = (t_object *)malloc(sizeof(t_object) * (argc + 1));
		if (!fill_objects(ctr, argv, g))
			return (putstr("Init error\n", 0));
	}
	return (1);
}
