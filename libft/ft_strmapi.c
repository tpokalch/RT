/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpokalch <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/30 15:59:44 by tpokalch          #+#    #+#             */
/*   Updated: 2018/11/20 20:44:37 by tpokalch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	int		i;
	char	*ret;

	i = 0;
	if (s == NULL || f == NULL)
		return (NULL);
	if (!(ret = (char *)malloc(sizeof(char) * ft_strlen(s) + 1)))
		return (NULL);
	while (*(s + i) != '\0')
	{
		*(ret + i) = (*f)(i, *(s + i));
		i++;
	}
	*(ret + i) = '\0';
	return (ret);
}
