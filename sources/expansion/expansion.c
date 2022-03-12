#include "minishell.h"

void	error_check(t_list **tokens, t_list **prev, t_list **head, bool error)
{
	if (error)
	{
		if (*head == *tokens)
			*head = (*tokens)->next;
		free_cmd_block(tokens);
		if (*prev)
			(*prev)->next = *tokens;
	}
	else
	{
		*prev = *tokens;
		*tokens = (*tokens)->next;
	}
}

void	expansion(t_list **tokens, t_environ *env)
{
	t_list		*words;
	t_list		*head;
	t_list		*prev;
	bool		error;

	words = NULL;
	head = *tokens;
	prev = NULL;
	while (*tokens != NULL)
	{
		error = false;
		if ((*tokens)->content != NULL)
		{
			expand_cmd_args((t_cmd_block *)(*tokens)->content, env, words);
			expand_redirects((t_cmd_block *)(*tokens)->content,
				env, words, &error);
		}
		error_check(tokens, &prev, &head, error);
	}
	*tokens = head;
}
