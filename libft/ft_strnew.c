/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnew.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpokalch <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/28 13:25:11 by tpokalch          #+#    #+#             */
/*   Updated: 2018/11/19 20:56:34 by tpokalch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <strings.h>

char	*ft_strnew(size_t size)
{
	char	*of_the_king;
	size_t	i;

	if (!(of_the_king = (char *)malloc(size + 1)))
		return (NULL);
	i = 0;
	while (i <= size)
	{
		*(of_the_king + i) = '\0';
		i++;
	}
	return (of_the_king);
}
