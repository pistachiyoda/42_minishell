#include "minishell.h"

void	free_2d_array(char **two_d_array)
{
	int	i;

	i = 0;
	while (two_d_array[i] != NULL)
	{
		free(two_d_array[i]);
		i++;
	}
	free(two_d_array);
}

void	ft_lstclear2(t_list **lst)
{
	t_list	*next_lst;

	if (lst == NULL)
		return ;
	while (*lst != NULL)
	{
		next_lst = (*lst)->next;
		free(*lst);
		*lst = next_lst;
	}
	*lst = NULL;
}

bool	free_words_str(t_list **words, char *str)
{
	ft_lstclear(words, free);
	free(str);
	return (false);
}

void	free_cmd_block(t_list **tokens)
{
	t_list		*next;
	t_cmd_block	*cmd;
	t_list		*head;

	next = (*tokens)->next;
	cmd = (*tokens)->content;
	if (cmd->command != NULL)
		free_2d_array(cmd->args);
	head = cmd->redirects;
	while (cmd->redirects != NULL)
	{
		free(((t_redirects *)cmd->redirects->content)->target);
		cmd->redirects = cmd->redirects->next;
	}
	ft_lstclear(&head, free);
	ft_lstdelone(*tokens, free);
	*tokens = next;
}

void	free_tokens(t_list **tokens)
{
	t_cmd_block	*cmd;
	t_redirects	*redirects;
	t_list		*head_t;
	t_list		*head_r;

	head_t = *tokens;
	while (*tokens != NULL)
	{
		cmd = (*tokens)->content;
		if (cmd != NULL)
		{
			if (cmd->command != NULL)
				free_2d_array(cmd->args);
			head_r = cmd->redirects;
			while (cmd->redirects != NULL)
			{
				redirects = cmd->redirects->content;
				free(redirects->target);
				cmd->redirects = cmd->redirects->next;
			}
			ft_lstclear(&head_r, free);
		}
		*tokens = (*tokens)->next;
	}
	ft_lstclear(&head_t, free);
}
