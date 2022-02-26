#include "minishell.h"

bool	is_redirect(t_list *words)
{
	if (ft_strncmp(words->content, ">", 1) == 0
		|| ft_strncmp(words->content, ">>", 2) == 0
		|| ft_strncmp(words->content, "<", 1) == 0
		|| ft_strncmp(words->content, "<<", 2) == 0)
		return (true);
	return (false);
}

t_redirects	*set_redirect_member(t_list **words)
{
	t_redirects	*redir;

	redir = xmalloc(sizeof(t_redirects), "parser");
	redir->fd = ft_atoi((*words)->content);
	*words = (*words)->next;
	if (ft_strncmp((*words)->content, ">>", 2) == 0)
		redir->redirect = APPEND;
	else if (ft_strncmp((*words)->content, "<<", 2) == 0)
		redir->redirect = HEREDOC;
	else if (ft_strncmp((*words)->content, ">", 1) == 0)
		redir->redirect = WRITE;
	else if (ft_strncmp((*words)->content, "<", 1) == 0)
		redir->redirect = INPUT;
	*words = (*words)->next;
	redir->target = (*words)->content;
	*words = (*words)->next;
	return (redir);
}

void	set_args(t_list **words, t_cmd_block *cmd)
{
	t_list		*tmp_words;
	int			i;
	int			j;
	int			count;
	char		**save;

	i = 0;
	j = 0;
	count = 0;
	tmp_words = *words;
	while (tmp_words != NULL && tmp_words->next != NULL && !is_redirect(tmp_words->next)
		&& ft_strncmp(tmp_words->next->content, "|", 1) != 0)
	{
		count++;
		tmp_words = tmp_words->next;
	}
	if (tmp_words->next == NULL || (tmp_words->next != NULL
		&& ft_strncmp(tmp_words->next->content, "|", 1) == 0))
		count++;
	if (cmd->args == NULL)
	{
		cmd->args = xmalloc(sizeof(char *) * (count + 1), "parser");
		while (i < count)
		{
			cmd->args[j + i] = (*words)->content;
			*words = (*words)->next;
			i++;
		}
		cmd->args[j + i] = NULL;
	}
	else
	{
		save = cmd->args;
		while (save[j] != NULL)
			j++;
		cmd->args = xmalloc(sizeof(char *) * (j + count + 1), "parser");
		while (save[i] != NULL)
		{
			cmd->args[i] = save[i];
			i++;
		}
		i = 0;
		while (i < count)
		{
			cmd->args[j + i] = (*words)->content;
			*words = (*words)->next;
			i++;
		}
		cmd->args[j + i] = NULL;
	}
}

void	set_redirects(t_list **words, t_cmd_block *cmd, t_list **redirects)
{
	t_redirects	*redir;

	if (cmd->command == NULL && *words != NULL && (*words)->next != NULL
		&& !is_redirect((*words)->next))
		cmd->command = (*words)->content;
	set_args(words, cmd);
	while (*words != NULL && (*words)->next != NULL && is_redirect((*words)->next))
	{
		redir = set_redirect_member(words);
		if (*redirects == NULL)
			*redirects = xlstnew(redir, "parser");
		else
			ft_lstadd_back(redirects, xlstnew(redir, "parser"));
		if (cmd->command == NULL && *words != NULL && ft_strncmp((*words)->content, "|", 1) != 0
			&& (((*words)->next != NULL && !is_redirect((*words)->next)) || (*words)->next == NULL))
			cmd->command = (*words)->content;
		if (*words != NULL && ft_strncmp((*words)->content, "|", 1) != 0
			&& ((((*words)->next != NULL && !is_redirect((*words)->next))) || (*words)->next == NULL))
			set_args(words, cmd);
	}
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
		cmd->redirects = NULL;
		cmd->command = NULL;
		cmd->args = NULL;
		set_redirects(&words, cmd, &cmd->redirects);
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
