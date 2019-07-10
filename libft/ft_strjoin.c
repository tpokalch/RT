/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpokalch <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/30 18:03:51 by tpokalch          #+#    #+#             */
/*   Updated: 2018/12/09 17:58:56 by tpokalch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	int		j;
	int		i;
	char	*ret;

	if (s2 == NULL || s1 == NULL)
		return (NULL);
	if (!(ret = (char *)malloc(sizeof(char) *
	(ft_strlen(s1) + ft_strlen(s2) + 1))))
		return (NULL);
	i = 0;
	j = 0;
	while (i < ft_strlen(s1))
	{
		*(ret + i) = *(s1 + i);
		i++;
	}
	while (i < ft_strlen(s1) + ft_strlen(s2))
	{
		*(ret + i) = *(s2 + j);
		i++;
		j++;
	}
	*(ret + i) = '\0';
	return (ret);
}
