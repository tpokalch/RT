/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpokalch <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/08 18:27:25 by tpokalch          #+#    #+#             */
/*   Updated: 2018/11/20 22:04:49 by tpokalch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "libft.h"

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	size_t i;

	i = 0;
	if (dest > src)
	{
		while (i < n)
		{
			*((unsigned char *)dest + n - 1 - i) =
			*((unsigned char *)src + n - 1 - i);
			i++;
		}
		return (dest);
	}
	else if (src > dest)
	{
		while (i < n)
		{
			*((unsigned char *)dest + i) = *((unsigned char *)src + i);
			i++;
		}
		return (dest);
	}
	return (dest);
}
