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
	if (ft_strncmp(words->content, ">", 1) == 0
		|| ft_strncmp(words->content, ">>", 2) == 0
		|| ft_strncmp(words->content, "<", 1) == 0
		|| ft_strncmp(words->content, "<<", 2) == 0)
		return (true);
	return (false);
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

	if (!is_valid_words(words, str))
		return (false);
	head = words;
	while (words != NULL)
	{
		cmd = ft_xmalloc(sizeof(t_cmd_block), "parser");
		set_cmd_block(&words, cmd);
		set_tokens(tokens, cmd);
		if (words == NULL)
			break ;
		if (words->next == NULL && ft_strncmp(words->content, "|", 1) == 0)
		{
			set_tokens(tokens, NULL);
			break ;
		}
		words = words->next;
	}
	ft_lstclear2(&head);
	return (true);
}
