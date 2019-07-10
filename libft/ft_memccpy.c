/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memccpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpokalch <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/08 21:52:50 by tpokalch          #+#    #+#             */
/*   Updated: 2018/11/20 22:03:58 by tpokalch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>

void	*ft_memccpy(void *restrict dst, const void *restrict src,
	int c, size_t n)
{
	size_t i;

	i = 0;
	while (i < n)
	{
		if (*((unsigned char *)src + i) == (unsigned char)c)
		{
			*((unsigned char *)dst + i) = *((unsigned char *)src + i);
			return (dst + i + 1);
		}
		*((unsigned char *)dst + i) = *((unsigned char *)src + i);
		i++;
	}
	return (NULL);
}
