/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpokalch <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/09 18:38:35 by tpokalch          #+#    #+#             */
/*   Updated: 2018/11/10 23:08:38 by tpokalch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>

char	*ft_strnstr(const char *haystack, const char *needle, size_t len)
{
	size_t	k;
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	k = 0;
	if (*needle == '\0')
		return ((char *)(haystack));
	while (*(haystack + i) != '\0' && k < len)
	{
		if (*needle == *(haystack + i))
		{
			j = 0;
			while (*(haystack + i + j) == *(needle + j) && (i + j < len))
			{
				if (*(needle + j + 1) == '\0')
					return ((char *)(haystack + i));
				j++;
			}
		}
		i++;
		k++;
	}
	return (NULL);
}
