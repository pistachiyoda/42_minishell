#include "minishell.h"

void	assign_expanded_cmd_args(t_cmd_block *cmd, t_list **words)
{
	int	i;

	i = 0;
	if (*words != NULL)
	{
		free_2d_array(cmd->args);
		cmd->command = (*words)->content;
		cmd->args = ft_xmalloc(sizeof(char *) * (ft_lstsize(*words) + 1),
				"expansion");
		while (*words != NULL)
		{
			cmd->args[i] = (*words)->content;
			*words = (*words)->next;
			i++;
		}
		cmd->args[i] = NULL;
		ft_lstclear2(words);
	}
	else
	{
		free_2d_array(cmd->args);
		cmd->command = NULL;
		cmd->args = NULL;
	}
}

void	expand_cmd_args(t_cmd_block *cmd, t_environ *env, t_list *words)
{
	int		i;

	i = 1;
	if (cmd->command != NULL)
	{
		set_expanded_to_words(env, cmd->command, &words, -1);
		while (cmd->args[i] != NULL)
		{
			set_expanded_to_words(env, cmd->args[i], &words, -1);
			i++;
		}
		assign_expanded_cmd_args(cmd, &words);
	}
}

void	assign_expanded_target(t_redirects *redir, t_list **words)
{
	if (!redir->error)
	{
		free(redir->target);
		if (*words != NULL)
			redir->target = (*words)->content;
		else
			redir->target = NULL;
		ft_lstclear2(words);
	}
	else
		ft_lstclear(words, free);
}

void	expand_redirects(t_cmd_block *cmd, t_environ *env, t_list *words)
{
	t_list		*head;
	t_redirects	*redir;
	int			qhdoc;

	head = cmd->redirects;
	while (cmd->redirects != NULL)
	{
		redir = cmd->redirects->content;
		qhdoc = redir->redirect;
		if (redir->redirect != HEREDOC)
		{
			redir->error = set_expanded_to_words(
					env, redir->target, &words, qhdoc);
			assign_expanded_target(redir, &words);
		}
		cmd->redirects = cmd->redirects->next;
	}
	cmd->redirects = head;
}

void	expansion(t_list **tokens, t_environ *env)
{
	t_list		*words;
	t_list		*head;

	words = NULL;
	head = *tokens;
	while (*tokens != NULL)
	{
		if ((*tokens)->content != NULL)
		{
			expand_cmd_args((t_cmd_block *)(*tokens)->content, env, words);
			expand_redirects((t_cmd_block *)(*tokens)->content, env, words);
		}
		*tokens = (*tokens)->next;
	}
	*tokens = head;
}
