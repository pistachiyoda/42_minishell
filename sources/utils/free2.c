/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmai      <fmai@student.42tokyo.jp>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/21 23:03:08 by fmai              #+#    #+#             */
/*   Updated: 2022/03/21 23:03:08 by fmai             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_redirects(t_list *redirects)
{
	t_list	*current_node;
	t_list	*next_node;

	current_node = redirects;
	while (current_node)
	{
		free(((t_redirects *)current_node->content)->target);
		free(current_node->content);
		next_node = current_node->next;
		free(current_node);
		current_node = next_node;
	}
}

void	free_cmd_block_after_exec(t_cmd_block *cmd_block)
{
	if (cmd_block->args)
		free_2d_array(cmd_block->args);
	if (cmd_block->redirects)
		free_redirects(cmd_block->redirects);
	free(cmd_block);
}

void	free_cmd_list_after_exec(t_list *cmd_list)
{
	t_list	*current_node;
	t_list	*next_node;

	current_node = cmd_list;
	while (current_node)
	{
		free_cmd_block_after_exec(current_node->content);
		next_node = current_node->next;
		free(current_node);
		current_node = next_node;
	}
}
