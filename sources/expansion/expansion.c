#include "minishell.h"

void	assign_expanded_cmd_args(t_cmd_block *cmd, t_list *words)
{
	int	i;

	i = 0;
	if (words != NULL)
	{
		cmd->command = words->content;
		// 必要であればcmd->argsフリー
		cmd->args = malloc(sizeof(char *) * (ft_lstsize(words) + 1));
		while (words != NULL)
		{
			cmd->args[i] = words->content;
			words = words->next;
			i++;
		}
		cmd->args[i] = NULL;
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

void	assign_expanded_target(t_cmd_block *cmd, t_list **words, int status)
{
	t_redirects	*redirects;

	redirects = cmd->redirects->content;
	if (status == 1)
	{
		print_error("expansion", EMESS_REDIRECT);
		exit(EXIT_FAILURE);
	}
	// free(redirects->target); //wordsの中身になりうる場合、free NG
	redirects->target = ft_lstlast(*words)->content;
}

void	expand_redirects(t_environ *env, t_cmd_block *cmd, t_list *words)
{
	t_list		*head;
	t_redirects	*redirect;
	int			status;

	head = cmd->redirects;
	while (cmd->redirects != NULL)
	{
		redirect = cmd->redirects->content;
		status = set_expanded_to_words(env, redirect->target, &words);
		assign_expanded_target(cmd, &words, status);
		cmd->redirects = cmd->redirects->next;
	}
	cmd->redirects = head;
}

// a$TE$TE'b'$TE | $USER$TE
// echo $USER > $FF
t_list	*expansion(t_list *tokens, t_environ *env)
{
	t_cmd_block	*cmd;
	t_list		*words;
	t_list		*head;

	words = NULL;
	head = tokens;
	while (tokens != NULL)
	{
		cmd = tokens->content;
		if (cmd != NULL)
		{
			expand_cmd_args(cmd, env, words);
			expand_redirects(env, cmd, words);
		}
		tokens = tokens->next;
	}
	return (head);
}
