#include "minishell.h"

void	assign_expanded_cmd_args(t_cmd_block *cmd, t_list *words)
{
	int	i;

	i = 0;
	if (words != NULL)
	{
		free_2d_array(cmd->args);
		cmd->command = words->content;
		cmd->args = xmalloc(sizeof(char *) * (ft_lstsize(words) + 1),
				"expansion");
		while (words != NULL)
		{
			cmd->args[i] = words->content;
			words = words->next;
			i++;
		}
		cmd->args[i] = NULL;
		ft_lstclear(&words, NULL);
	}
}

void	expand_cmd_args(t_cmd_block *cmd, t_environ *env, t_list *words)
{
	int	i;

	i = 1;
	if (cmd->command != NULL)
	{
		set_expanded_to_words(env, cmd->command, &words);
		while (cmd->args[i] != NULL)
		{
			set_expanded_to_words(env, cmd->args[i], &words);
			i++;
		}
		assign_expanded_cmd_args(cmd, words);
	}
}

void	assign_expanded_target(t_cmd_block *cmd, t_list *words, bool error)
{
	t_redirects	*redirects;

	redirects = cmd->redirects->content;
	if (error)
	{
		print_error("expansion", EMESS_REDIRECT);
		exit(EXIT_FAILURE);
	}
	free(redirects->target);
	redirects->target = ft_lstlast(words)->content;
	ft_lstclear(&words, NULL);
}

void	expand_redirects(t_cmd_block *cmd, t_environ *env, t_list *words)
{
	t_list		*head;
	t_redirects	*redirect;
	bool		error;

	head = cmd->redirects;
	while (cmd->redirects != NULL)
	{
		redirect = cmd->redirects->content;
		error = set_expanded_to_words(env, redirect->target, &words);
		assign_expanded_target(cmd, words, error);
		cmd->redirects = cmd->redirects->next;
	}
	cmd->redirects = head;
}

t_list	*expansion(t_list *tokens, t_environ *env)
{
	t_list		*words;
	t_list		*head;

	words = NULL;
	head = tokens;
	while (tokens != NULL)
	{
		if (tokens->content != NULL)
		{
			expand_cmd_args((t_cmd_block *)tokens->content, env, words);
			expand_redirects((t_cmd_block *)tokens->content, env, words);
		}
		tokens = tokens->next;
	}
	return (head);
}
