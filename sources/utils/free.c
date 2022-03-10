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

void	free_tokens(t_list *tokens)
{
	t_cmd_block	*cmd;
	t_redirects	*redirects;

	while (tokens != NULL)
	{
		cmd = tokens->content;
		if (cmd != NULL)
		{
			free_2d_array(cmd->args);
			while (cmd->redirects != NULL)
			{
				redirects = cmd->redirects->content;
				free(redirects->target);
				cmd->redirects = cmd->redirects->next;
			}
			ft_lstclear(&cmd->redirects, free);
		}
		tokens = tokens->next;
	}
	ft_lstclear(&tokens, free);
}
