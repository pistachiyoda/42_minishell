/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wrapper2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmai      <fmai@student.42tokyo.jp>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/23 01:15:20 by fmai              #+#    #+#             */
/*   Updated: 2022/03/23 01:15:20 by fmai             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_strjoin_wrapper(char const *s1, char const *s2)
{
	char	*ret;

	ret = ft_strjoin(s1, s2);
	if (ret == NULL)
	{
		perror("ft_strjoin()");
		exit(1);
	}
	return (ret);
}

void	execve_wrapper(
		const char *pathname, char *const argv[], char *const envp[])
{
	if (execve(pathname, argv, envp) == -1)
	{
		perror("execve()");
		exit(1);
	}
}

int	dup2_wrapper(int oldfd, int newfd)
{
	char	*ascii_oldfd;
	char	*ascii_newfd;

	if (oldfd > FD_MAX)
	{
		ascii_oldfd = ft_xitoa(oldfd, "dup2_wrapper");
		print_error(ascii_oldfd, EMESS_BADF);
		free(ascii_oldfd);
		return (1);
	}
	if (newfd > FD_MAX)
	{
		ascii_newfd = ft_xitoa(newfd, "dup2_wrapper");
		print_error(ascii_newfd, EMESS_BADF);
		free(ascii_newfd);
		return (1);
	}
	if (dup2(oldfd, newfd) == -1)
	{
		perror("dup2()");
		exit(1);
	}
	return (0);
}
