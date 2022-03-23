/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc_check2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmai      <fmai@student.42tokyo.jp>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/17 14:15:00 by fmai              #+#    #+#             */
/*   Updated: 2022/03/18 11:04:56 by fmai             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_xstrjoin(char const *s1, char const *s2, char *target)
{
	char	*ret;

	ret = ft_strjoin(s1, s2);
	if (ret == NULL)
	{
		print_error(target, EMESS_MALLOC_FAIL);
		exit(EXIT_FAILURE);
	}
	return (ret);
}

char	*ft_xstrjoin_with_free(char *s1, char *s2, char *target)
{
	char	*ret;

	ret = ft_strjoin(s1, s2);
	if (ret == NULL)
	{
		print_error(target, EMESS_MALLOC_FAIL);
		exit(EXIT_FAILURE);
	}
	free(s1);
	s1 = NULL;
	free(s2);
	s2 = NULL;
	return (ret);
}

char	*ft_xstrjoin2_with_free(char *s1, char *s2, char *target)
{
	size_t	len;
	char	*str;
	char	*ret;
	char	*s1_start;
	char	*s2_start;

	len = ft_strlen((char *)s1) + ft_strlen((char *)s2);
	str = ft_xmalloc(sizeof(char) * (len + 2), target);
	s1_start = s1;
	s2_start = s2;
	ret = str;
	while (*s1 != '\0')
		*str++ = *s1++;
	while (*s2 != '\0')
		*str++ = *s2++;
	*str++ = '\n';
	*str = '\0';
	free(s1_start);
	s1 = NULL;
	free(s2_start);
	s2 = NULL;
	return (ret);
}

char	*ft_xitoa(int val, char *target)
{
	char	*ret;

	ret = ft_itoa(val);
	if (ret == NULL)
	{
		print_error(target, EMESS_MALLOC_FAIL);
		exit(EXIT_FAILURE);
	}
	return (ret);
}
