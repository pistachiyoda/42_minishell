#include "minishell.h"

void	expansion(t_list *tokens)
{
	t_cmd_block	*cmd;
	t_redirects	*redirects;

	while (tokens != NULL)
	{
		cmd = tokens->content;
		tokens = tokens->next;
	}
}
