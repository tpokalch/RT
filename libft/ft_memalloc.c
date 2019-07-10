/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memalloc.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpokalch <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/28 13:02:24 by tpokalch          #+#    #+#             */
/*   Updated: 2018/11/20 20:31:03 by tpokalch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>
#include <stdlib.h>

void	*ft_memalloc(size_t size)
{
	void	*arr;
	size_t	i;

	i = 0;
	if (!(arr = (void *)(malloc(size))))
		return (NULL);
	while (i < size)
	{
		*((int *)(arr + i)) = 0;
		i++;
	}
	return (arr);
}
