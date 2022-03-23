/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atol.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmai      <fmai@student.42tokyo.jp>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/12 13:36:58 by fmai              #+#    #+#             */
/*   Updated: 2022/03/12 13:36:58 by fmai             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_space_for_atol(char c)
{
	if (c == '\t')
		return (1);
	if (c == '\n')
		return (1);
	if (c == '\v')
		return (1);
	if (c == '\f')
		return (1);
	if (c == '\r')
		return (1);
	if (c == ' ')
		return (1);
	return (0);
}

int	return_invalid(bool *is_invalid)
{
	*is_invalid = true;
	return (0);
}

// unsigned long long 18446744073709551615
long long	ft_atol(const char *str, bool *is_invalid)
{
	long long			sign;
	unsigned long long	n;

	while (is_space_for_atol(*str))
		str++;
	if (*str == '-')
		sign = -1LL;
	else
		sign = 1LL;
	if (*str == '-' || *str == '+')
		str++;
	n = 0;
	while (ft_isdigit(*str))
	{
		n = (unsigned long long)10 * n + (unsigned long long)(*str - '0');
		if (sign == 1LL && n > (unsigned long long)LLONG_MAX)
			return (return_invalid(is_invalid));
		if (sign == -1LL
			&& (n > ((unsigned long long)LLONG_MAX + 1LLU)))
			return (return_invalid(is_invalid));
		str++;
	}
	*is_invalid = false;
	return (sign * (long long)n);
}
