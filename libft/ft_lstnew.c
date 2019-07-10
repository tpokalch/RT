/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstnew.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpokalch <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/03 15:07:21 by tpokalch          #+#    #+#             */
/*   Updated: 2018/11/11 18:39:40 by tpokalch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"
#include <stdio.h>
#include <string.h>

t_list		*ft_lstnew(void const *content, size_t content_size)
{
	t_list	*list;
	void	*content2;

	if (!(content2 = (void *)malloc(content_size)))
		return (NULL);
	if (!(list = (t_list *)malloc(sizeof(t_list))))
		return (NULL);
	if (content == NULL)
	{
		(*list).content = NULL;
		(*list).content_size = 0;
		(*list).next = NULL;
		return (list);
	}
	(*list).content = ft_memcpy(content2, (void *)content, content_size);
	(*list).content_size = content_size;
	(*list).next = NULL;
	return (list);
}
