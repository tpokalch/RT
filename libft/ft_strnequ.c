/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnequ.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpokalch <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/30 16:33:50 by tpokalch          #+#    #+#             */
/*   Updated: 2018/11/20 20:39:42 by tpokalch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int		ft_strnequ(char const *s1, char const *s2, size_t n)
{
	size_t i;

	i = 0;
	if (s1 == NULL || s2 == NULL)
		return (0);
	if (n == 0)
		return (1);
	while (*(s1 + i) != '\0' && *(s2 + i) != '\0' && i < n)
	{
		if (*(s1 + i) != *(s2 + i))
			return (0);
		i++;
	}
	if (*(s1 + i) != '\0' && *(s2 + i) == '\0')
		return (0);
	else if (*(s1 + i) == '\0' && *(s2 + i) != '\0')
		return (0);
	if (i == n || (*(s1 + i) == '\0' && *(s2 + i) == '\0'))
		return (1);
	return (1);
}
