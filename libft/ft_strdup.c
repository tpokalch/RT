/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpokalch <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/08 23:09:53 by tpokalch          #+#    #+#             */
/*   Updated: 2018/11/20 22:13:09 by tpokalch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "libft.h"

char			*ft_strdup(const char *s1)
{
	int		a;
	char	*s2;

	a = ft_strlen(s1);
	if (!(s2 = (char *)malloc(sizeof(char) * (a + 1))))
		return (NULL);
	ft_memcpy(s2, s1, a + 1);
	return (s2);
}
