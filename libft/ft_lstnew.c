/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstnew.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obritany <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/07 16:54:56 by obritany          #+#    #+#             */
/*   Updated: 2020/11/07 16:55:41 by obritany         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstnew(void *content)
{
	t_list *rslt;

	if (!content)
		return (0);
	rslt = malloc(sizeof(t_list));
	if (rslt == 0)
		return (0);
	rslt->content = content;
	rslt->next = 0;
	return (rslt);
}
