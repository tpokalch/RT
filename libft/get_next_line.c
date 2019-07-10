/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpokalch <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/21 13:01:16 by tpokalch          #+#    #+#             */
/*   Updated: 2019/02/20 22:05:34 by tpokalch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int			strndst(char *s, int size)
{
	int		i;

	i = 0;
	if (s == NULL)
		return (-1);
	while (i < size)
	{
		if (*(s + i) == '\n')
			return (i);
		i++;
	}
	return (-1);
}

void		read_connect(int fd, t_line *l, t_rest *c, int *rd)
{
	int		i;
	char	*temp;

	i = 0;
	if (!(*(l->n) = ft_strnew(BUFF_SIZE + 1)))
		return ;
	*rd = read(fd, *(l->n), BUFF_SIZE);
	l->sz = l->sz + *rd;
	while (ft_strchr(*(l->n), '\n') == NULL && *rd == BUFF_SIZE)
	{
		temp = ft_memcpy(ft_strnew(BUFF_SIZE * (i++ + 1) + 1), *(l->n), l->sz);
		free(*(l->n));
		*(l->n) = ft_memcpy(ft_strnew(BUFF_SIZE * (i + 1) + 1), temp, l->sz);
		*rd = read(fd, *(l->n) + (i) * BUFF_SIZE, BUFF_SIZE);
		l->sz = l->sz + *rd;
		free(temp);
	}
	if (c->t[fd] != NULL && c->sz[fd] != 0)
	{
		temp = ft_memcpy(ft_strnew(c->sz[fd] + l->sz + 1), c->t[fd], c->sz[fd]);
		ft_memcpy(temp + c->sz[fd], *(l->n), l->sz);
		free(*(l->n));
		*(l->n) = temp;
		(l->sz) = (l->sz) + (c->sz)[fd];
	}
}

void		cutt(int fd, t_line *l, t_rest *c)
{
	char	*temp;

	if (c->sz[fd] > 0)
		free(c->t[fd]);
	(c->sz)[fd] = l->sz - N(*(l->n), l->sz) - 1;
	if (N(*l->n, l->sz) != -1 && c->sz[fd] > 0)
	{
		if (l->sz > 0)
		{
			l->sz = l->sz - c->sz[fd];
			c->t[fd] = ft_memcpy(ft_strnew(c->sz[fd] + 1),
				*(l->n) + N(*(l->n), l->sz) + 1, c->sz[fd]);
			temp = ft_memcpy(ft_strnew(l->sz + 1), *(l->n), l->sz);
			free(*(l->n));
			*(l->n) = temp;
		}
	}
	else
		c->t[fd] = NULL;
	if (N(*(l->n), l->sz) != -1)
		*(*l->n + N(*(l->n), l->sz)) = '\0';
}

void		use_rest(t_line *l, t_rest *c, int fd)
{
	char	*temp;
	int		n;

	n = N(c->t[fd], c->sz[fd]);
	l->sz = n + 1;
	if (!(*(l->n) = ft_strnew(l->sz + 1)))
		return ;
	*(l->n) = ft_memcpy(*(l->n), c->t[fd], l->sz);
	c->sz[fd] = c->sz[fd] - n - 1;
	temp = ft_memcpy(ft_strnew(c->sz[fd] + 1), c->t[fd] + n + 1, c->sz[fd]);
	free(c->t[fd]);
	c->t[fd] = temp;
	if (c->sz[fd] == 0)
		ft_memdel((void **)&c->t[fd]);
	if (N(*(l->n), l->sz) != -1)
		*(*l->n + N(*(l->n), l->sz)) = '\0';
}

int			get_next_line(const int fd, char **line)
{
	static	t_rest	c;
	int				rd;
	t_line			l;

	if (BUFF_SIZE <= 0 || line == NULL || fd < 0)
		return (-1);
	l.n = line;
	l.sz = 0;
	if (c.t[fd] == NULL || N(c.t[fd], c.sz[fd]) == -1)
	{
		read_connect(fd, &l, &c, &rd);
		if ((rd == 0 && l.sz == 0) || rd < 0 || !(*(l.n)))
		{
			if (c.sz[fd] > 0)
				free(c.t[fd]);
			c.sz[fd] = 0;
			return ((rd < 0 || !(*(l.n))) ? -1 : 0);
		}
		cutt(fd, &l, &c);
	}
	else if (N(c.t[fd], c.sz[fd]) != -1)
		use_rest(&l, &c, fd);
	if (!(*(l.n)))
		return (-1);
	return (1);
}
