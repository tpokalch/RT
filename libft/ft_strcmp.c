/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpokalch <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/09 18:52:22 by tpokalch          #+#    #+#             */
/*   Updated: 2018/11/20 22:13:52 by tpokalch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>

int		ft_strcmp(const char *s1, const char *s2)
{
	int i;

	i = 0;
	while (*((unsigned char *)(s1 + i)) != '\0' &&
			*((unsigned char *)(s2 + i)) != '\0' && s1 + i && s2 + i)
	{
		if (*((unsigned char *)(s1 + i)) < *((unsigned char *)(s2 + i)))
			return (-1);
		else if (*((unsigned char *)(s1 + i)) > *((unsigned char *)(s2 + i)))
			return (1);
		i++;
	}
	if (*((unsigned char *)(s1 + i)) == '\0' &&
			*((unsigned char *)(s2 + i)) != '\0')
		return (-1);
	else if (*((unsigned char *)(s1 + i)) != '\0' &&
			*((unsigned char *)(s2 + i)) == '\0')
		return (1);
	return (0);
}
