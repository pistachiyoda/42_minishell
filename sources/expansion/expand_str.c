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
		assign_expanded_cmd_args(cmd, &words);
	}
}

void	assign_expanded_target(char **str, t_list **words, bool error)
{
	if (error)
	{
		print_error("expansion", EMESS_REDIRECT);
		ft_lstclear(words, free);
	}
	else
	{
		free(*str);
		*str = (*words)->content;
		ft_lstclear2(words);
	}
}

void	expand_redirects(t_cmd_block *cmd, t_environ *env,
	t_list *words, bool *error)
{
	t_list		*head;
	t_redirects	*redirect;

	head = cmd->redirects;
	while (cmd->redirects != NULL && !*error)
	{
		redirect = cmd->redirects->content;
		*error = set_expanded_to_words(env, redirect->target, &words);
		assign_expanded_target(&redirect->target, &words, *error);
		cmd->redirects = cmd->redirects->next;
	}
	cmd->redirects = head;
}