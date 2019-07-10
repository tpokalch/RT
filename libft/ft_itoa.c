/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpokalch <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/02 21:35:28 by tpokalch          #+#    #+#             */
/*   Updated: 2018/11/20 15:27:41 by tpokalch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include "libft.h"

static int		intlen(int nb)
{
	int i;

	i = 0;
	if (nb == -2147483648)
		return (11);
	else if (nb == 0)
		return (1);
	else if (nb < 0 && nb > -214748368)
		return (intlen(-nb) + 1);
	while (nb != 0)
	{
		nb = nb / 10;
		i++;
	}
	return (i);
}

static char		*max(char *s, int nb)
{
	int i;
	int a;

	i = 1;
	*s = '8';
	nb = nb / 10;
	nb = -nb;
	a = intlen(nb);
	while (nb != 0)
	{
		*(s + i) = (nb) % 10 + '0';
		nb = nb / 10;
		i++;
	}
	*(s + i++) = '-';
	*(s + i) = '\0';
	ft_strrev(s);
	return (s);
}

char			*ft_itoa(int nbr)
{
	char	*s;
	int		i;
	int		b;

	b = nbr;
	if (!(s = (char *)malloc(sizeof(char) * (intlen(nbr) + 1))))
		return (NULL);
	i = (nbr == 0);
	if (nbr == -2147483648)
		return (max(s, nbr));
	else if (nbr == 0)
		*s = '0';
	if (nbr < 0)
		nbr = -(nbr);
	while (nbr != 0)
	{
		*(s + i) = (nbr) % 10 + '0';
		nbr = nbr / 10;
		i++;
	}
	if (b < 0)
		*(s + i++) = '-';
	*(s + i) = '\0';
	return (ft_strrev(s));
}
