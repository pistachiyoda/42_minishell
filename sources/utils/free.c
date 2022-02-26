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

void	free_tokens(t_list *tokens)
{
	t_cmd_block	*cmd;
	t_redirects	*redirects;

	while (tokens != NULL)
	{
		cmd = tokens->content;
		if (cmd != NULL)
		{
			free(cmd->command);
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
