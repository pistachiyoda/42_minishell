#include "minishell.h"

bool	is_valid_words(t_list *head)
{
	t_list	*words;

	if (head == NULL)
		return (false);
	words = head->next;
	if (ft_strncmp(words->content, "|", 1) == 0)
	{
		syntax_error("|");
		ft_lstclear(&head, free);
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
		*tokens = xlstnew(cmd, "parser");
	else
		ft_lstadd_back(tokens, xlstnew(cmd, "parser"));
}

t_list	*parser(t_list *words)
{
	t_list		*tokens;
	t_cmd_block	*cmd;

	tokens = NULL;
	if (!is_valid_words(words))
		return (NULL);
	words = words->next;
	while (words != NULL)
	{
		cmd = xmalloc(sizeof(t_cmd_block), "parser");
		set_cmd_block(&words, cmd);
		set_tokens(&tokens, cmd);
		if (words == NULL)
			break ;
		if (words->next == NULL && ft_strncmp(words->content, "|", 1) == 0)
		{
			set_tokens(&tokens, NULL);
			break ;
		}
		words = words->next;
	}
	return (tokens);
}
