/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_points.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpokalch <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/20 21:19:59 by tpokalch          #+#    #+#             */
/*   Updated: 2019/08/28 21:31:58 by tpokalch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

int				next_num(char *s)
{
	int i;

	i = 0;
	while (*(s + i) == ' ')
		i++;
	while (*(s + i) != ' ' && *(s + i) != '\0')
	{
		if ((*(s + i) < '0' || *(s + i) > '9') && *(s + i) != ' '
				&& *(s + i) != '-' && *(s + i) != '+')
			return (-1);
		i++;
	}
	while (*(s + i) == ' ')
		i++;
	return (i);
}

int				file_height(char *filename)
{
	char	*line;
	int		height;
	int		fd;
	int		gnl;

	height = 0;
	fd = open(filename, O_RDONLY);
	if (fd < 0)
		return (-1);
	while ((gnl = get_next_line(fd, &line)))
	{
		if (gnl == -1)
			return (-1);
		height++;
		free(line);
	}
	free(line);
	close(fd);
	return (height);
}

int				o_putstr(char *put, char *clean, t_vector **del)
{
	int i;

	i = 0;
	if (clean)
		free(clean);
	if (del)
		free_points(del);
	while (*(put + i) != '\0')
	{
		write(1, put + i, 1);
		i++;
	}
	return (0);
}

int				parse_line(t_vector **a, char *s, int j, t_vector *ptdim)
{
	int		i;
	char	*save;

	save = s;
	i = 0;
	*(a + j) = (t_vector *)malloc(sizeof(t_vector) * (ft_strlen(s) + 1));
	while (*s != '\0' && i != -1)
	{
		(*(a + j) + (i))->x = i * 20;
		(*(a + j) + i)->y = j * 20;
		(*(a + j) + i)->z = ft_atoi(s) * 20;
		if (a[j][i].z > ptdim->z)
			ptdim->z = a[j][i].z;
		if (next_num(s) == -1)
		{
			return (o_putstr("File must contain only spaces and numbers\n",
			save, a));
		}
		s = s + next_num(s);
		i++;
	}
	printf("initing len %d\n", i);
	if (j > 0 && (i * 20 != ptdim->x))
		return (o_putstr("File must contain rectangular map\n", save, a));
	ptdim->x = i * 20;
	return (1);
}

void			shift_center(t_vector **pts, t_vector *ptdim, t_global *g)
{
	int i;
	int j;
	int imax;
	
	t_vector	bas[3];  
	t_vector rc;
	i = 0;
	j = 0;

	init_vector(&g->base[0], 1, 0, 0);
        init_vector(&g->base[1], 0, 1, 0);
        init_vector(&g->base[2], 0, 0, 1);
 
       
	printf("g base[0] is %f,%f,%f\n", g->base[0].x, g->base[0].y, g->base[0].z);
		bas[0] = scale(ptdim->x, g->base[0]);
		bas[1] = scale(ptdim->y, g->base[1]);
        bas[2] = scale(ptdim->z, g->base[2]);
	imax = ptdim->x / 20;
	rc = scale(-0.5, sum(sum(bas[0], bas[1]), bas[2]));
	printf("shift is %f,%f,%f\n%f,%f,%f\n%f,%f,%f\n", bas[0].x, bas[0].y, bas[0].z, bas[1].x, bas[1].y, bas[1].z, bas[2].x, bas[2].y, bas[2].z);
	while (*(pts + j))
	{
		i = 0;
		while (i < imax)
		{
			pts[j][i] = sum(pts[j][i], rc);
			i++;
		}
		j++;
	}
}

t_vector		**create_points(char *filename, t_vector *ptdim, t_global *g)
{
	t_vector	**ret;
	char		*line;
	int			height;
	int			fd;
	int			j;

	j = 0;
	height = file_height(filename);
	printf("creating points\n");
	if (height == -1)
	{
		ft_putstr("Invalid file or buffer\n");
		return (NULL);
	}
	ret = initialize_points(height + 1);
	fd = open(filename, O_RDONLY);
	ptdim->z = 0;
	while (j < height)
	{
		if (get_next_line(fd, &line) == -1)
			return (NULL);
		if (!(parse_line(ret, line, j, ptdim)))
			return (NULL);
		free(line);
		j++;
	}
//	printf("line len int create is %d\n", (**ret).len);
	ptdim->y = height * 20;
	printf("frame dimensions are %f,%f,%f\n", ptdim->x, ptdim->y, ptdim->z);
	shift_center(ret, ptdim, g);
	return (ret);
}
