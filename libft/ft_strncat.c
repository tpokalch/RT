/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpokalch <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/09 15:53:46 by tpokalch          #+#    #+#             */
/*   Updated: 2018/11/19 23:10:13 by tpokalch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>
#include "libft.h"

char	*ft_strncat(char *restrict s1, const char *restrict s2, size_t n)
{
	size_t	i;
	int		a;

	i = 0;
	a = ft_strlen(s1);
	while (i < n && *(s2 + i) != '\0' && s1 + a)
	{
		*(s1 + a) = *(s2 + i);
		i++;
		a++;
	}
	*(s1 + a) = '\0';
	return (s1);
}
