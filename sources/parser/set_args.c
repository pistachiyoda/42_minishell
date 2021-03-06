/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_args.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmai      <fmai@student.42tokyo.jp>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/21 23:03:08 by fmai              #+#    #+#             */
/*   Updated: 2022/03/21 23:03:08 by fmai             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	get_args_malloc(t_cmd_block *cmd, int arg_num)
{
	int		i;
	int		j;
	char	**old;

	i = 0;
	j = 0;
	old = cmd->args;
	while (old != NULL && old[j] != NULL)
		j++;
	cmd->args = ft_xmalloc(sizeof(char *) * (j + arg_num + 1), "parser");
	while (i < j)
	{
		cmd->args[i] = old[i];
		i++;
	}
	free(old);
	return (j);
}

void	add_args(t_list **words, t_cmd_block *cmd, int arg_num)
{
	int	i;
	int	j;

	i = 0;
	j = get_args_malloc(cmd, arg_num);
	while (i < arg_num)
	{
		cmd->args[j + i] = ft_xstrdup((*words)->content, "parser");
		if (cmd->command == NULL)
			cmd->command = cmd->args[j + i];
		*words = (*words)->next;
		i++;
	}
	cmd->args[j + i] = NULL;
}

void	set_args(t_list **words, t_cmd_block *cmd)
{
	t_list	*tmp;
	int		arg_num;

	arg_num = 0;
	tmp = *words;
	while (tmp != NULL && tmp->next != NULL && !is_redirect(tmp->next)
		&& ft_strncmp(tmp->next->content, "|", 1) != 0)
	{
		arg_num++;
		tmp = tmp->next;
	}
	if (tmp->next == NULL || ft_strncmp(tmp->next->content, "|", 1) == 0)
		arg_num++;
	add_args(words, cmd, arg_num);
}
