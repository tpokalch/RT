/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpokalch <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/09 16:57:03 by tpokalch          #+#    #+#             */
/*   Updated: 2018/11/19 23:09:49 by tpokalch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>
#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	char	*ret;
	int		i;

	i = 0;
	ret = NULL;
	while (*(s + i) != '\0')
	{
		if (*(s + i) == (const char)c)
			ret = (char *)(s + i);
		i++;
	}
	if (*(s + i) == (const char)c)
		ret = (char *)(s + i);
	return (ret);
}
