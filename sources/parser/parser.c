#include "minishell.h"

bool	is_valid_words(t_list *head, char *str)
{
	t_list	*tail;

	tail = ft_lstlast(head);
	if (ft_strncmp(head->content, "|", 1) == 0
		|| ft_strncmp(tail->content, "|", 1) == 0)
	{
		syntax_error("|");
		free_words_str(&head, str);
		return (false);
	}
	return (true);
}

bool	is_redirect(t_list *words)
{
	if (words != NULL
		&& (ft_strncmp(words->content, ">", 1) == 0
			|| ft_strncmp(words->content, ">>", 2) == 0
			|| ft_strncmp(words->content, "<", 1) == 0
			|| ft_strncmp(words->content, "<<", 2) == 0))
		return (true);
	return (false);
}

void	set_cmd_block(t_list **words, t_cmd_block *cmd, t_list **prev)
{
	cmd->command = NULL;
	cmd->args = NULL;
	cmd->redirects = NULL;
	if (!is_redirect((*words)->next) || (*words)->next == NULL)
		cmd->command = (*words)->content;
	if (cmd->command != NULL)
		set_args(words, cmd, prev);
	while (*words != NULL && is_redirect((*words)->next))
	{
		if (cmd->redirects == NULL)
			cmd->redirects = ft_xlstnew(set_redirect(words, prev), "parser");
		else
			ft_lstadd_back(&cmd->redirects,
				ft_xlstnew(set_redirect(words, prev), "parser"));
		if (*words != NULL && ft_strncmp((*words)->content, "|", 1) != 0
			&& (!is_redirect((*words)->next) || (*words)->next == NULL))
		{
			if (cmd->command == NULL)
				cmd->command = (*words)->content;
			set_args(words, cmd, prev);
		}
	}
}

void	set_tokens(t_list **tokens, t_cmd_block *cmd)
{
	if (tokens == NULL)
		*tokens = ft_xlstnew(cmd, "parser");
	else
		ft_lstadd_back(tokens, ft_xlstnew(cmd, "parser"));
}

bool	parser(t_list *words, t_list **tokens, char *str)
{
	t_cmd_block	*cmd;
	t_list		*head;
	t_list		*prev;
	t_list		*next;

	if (!is_valid_words(words, str))
	{
		g_status = 258;
		return (false);
	}
	head = words;
	while (words != NULL)
	{
		cmd = ft_xmalloc(sizeof(t_cmd_block), "parser");
		set_cmd_block(&words, cmd, &prev);
		set_tokens(tokens, cmd);
		if (words == NULL)
			break ;
		next = words->next;
		ft_lstdelone(words, free);
		prev->next = next;
		words = next;
	}
	ft_lstclear2(&head);
	return (true);
}
