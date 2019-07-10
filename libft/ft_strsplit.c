/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strsplit.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpokalch <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/01 13:55:05 by tpokalch          #+#    #+#             */
/*   Updated: 2018/11/11 20:49:34 by tpokalch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "libft.h"

static int		ft_words(char const *s, char c, int n)
{
	int	f;
	int	i;

	i = 0;
	f = 0;
	while (*(s + i) != '\0')
	{
		if (*(s + i) == c)
			f = 0;
		else
			f = 1;
		if (f == 1)
		{
			n++;
			while (*(s + i) != '\0' && *(s + i) != c)
				i++;
		}
		else
		{
			while (*(s + i) == c)
				i++;
		}
	}
	return (n);
}

static int		ft_full(char const *s, char **r, char c)
{
	int	i;
	int	k;
	int	j;

	k = 0;
	i = 0;
	j = -1;
	while (*(s + i) != '\0')
	{
		if (*(s + i) == c)
			i++;
		else
		{
			j++;
			k = 0;
			while (*(s + i) != c && *(s + i) != '\0')
			{
				*(*(r + j) + k) = *(s + i);
				i++;
				k++;
			}
			*(*(r + j) + k) = '\0';
		}
	}
	return (j);
}

char			**ft_strsplit(char const *s, char c)
{
	int		i;
	size_t	l;
	int		n;
	int		j;
	char	**r;

	j = 0;
	n = 0;
	i = 0;
	if (!s)
		return (NULL);
	l = ft_strlen(s);
	n = ft_words(s, c, n);
	r = (char**)malloc((n + 1) * sizeof(char *));
	if (r == 0)
		return (0);
	while (i <= n - 1)
	{
		if (!(*(r + i) = (char *)malloc((1 + l) * sizeof(char))))
			return (NULL);
		i++;
	}
	j = ft_full(s, r, c);
	r[j + 1] = NULL;
	return (r);
}
