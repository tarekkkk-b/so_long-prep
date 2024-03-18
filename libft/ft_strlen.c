/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlen.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tabadawi <tabadawi@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/01 10:17:48 by tabadawi          #+#    #+#             */
/*   Updated: 2024/03/18 13:37:34 by tabadawi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlen(const char *str, int flag)
{
	size_t	i;

	i = 0;
	if (!str)
		return (0);
	if (flag == 1)
	{
		while (str[i] != '\0' && str[i] != '\n')
			i++;
		return (i);
	}
	while (str[i] != '\0')
		i++;
	return (i);
}
