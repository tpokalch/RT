/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstdelone.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpokalch <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/03 15:32:11 by tpokalch          #+#    #+#             */
/*   Updated: 2018/11/11 17:10:44 by tpokalch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>
#include "libft.h"

void	ft_lstdelone(t_list **aslt, void (*del)(void *, size_t))
{
	(*del)((**aslt).content, (**aslt).content_size);
	free(*(aslt));
	*(aslt) = NULL;
}
