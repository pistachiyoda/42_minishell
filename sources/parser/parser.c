#include "minishell.h"

t_redirects	*set_redirect_member(t_list **words)
{
	t_redirects	*redir;

	redir = malloc(sizeof(t_redirects));
	malloc_check(redir, "parser");
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
	return (redir);
}

void	set_redirects(t_list **words, t_list **redirects)
{
	t_redirects	*redir;
	t_list		*new_ele;

	while (*words != NULL && (ft_strncmp((*words)->content, ">", 1) == 0
			|| ft_strncmp((*words)->content, ">>", 2) == 0
			|| ft_strncmp((*words)->content, "<", 1) == 0
			|| ft_strncmp((*words)->content, "<<", 2) == 0))
	{
		redir = set_redirect_member(words);
		if (*redirects == NULL)
		{
			*redirects = ft_lstnew(redir);
			malloc_check(*redirects, "parser");
		}
		else
		{
			new_ele = ft_lstnew(redir);
			malloc_check(new_ele, "parser");
			ft_lstadd_back(redirects, new_ele);
		}
		*words = (*words)->next;
	}
}

void	set_args(t_list **words, t_cmd_block *cmd)
{
	t_list		*tmp_words;
	int			i;
	int			count;

	i = 0;
	count = 0;
	tmp_words = *words;
	while (tmp_words != NULL && ft_strncmp(tmp_words->content, "|", 1) != 0
		&& ft_strncmp(tmp_words->content, ">", 1) != 0
		&& ft_strncmp(tmp_words->content, ">>", 2) != 0
		&& ft_strncmp(tmp_words->content, "<", 1) != 0
		&& ft_strncmp(tmp_words->content, "<<", 2) != 0)
	{
		count++;
		tmp_words = tmp_words->next;
	}
	cmd->args = malloc(sizeof(char *) * (count + 1));
	malloc_check(cmd->args, "parser");
	while (i < count)
	{
		cmd->args[i] = (*words)->content;
		*words = (*words)->next;
		i++;
	}
	cmd->args[i] = NULL;
}

void	set_tokens(t_list **tokens, t_cmd_block *cmd)
{
	t_list		*new_ele;

	if (tokens == NULL)
	{
		*tokens = ft_lstnew(cmd);
		malloc_check(*tokens, "parser");
	}
	else
	{
		new_ele = ft_lstnew(cmd);
		malloc_check(new_ele, "parser");
		ft_lstadd_back(tokens, new_ele);
	}
}

t_list	*parser(t_list *words)
{
	t_list		*tokens;
	t_cmd_block	*cmd;

	words = words->next;
	tokens = NULL;
	if (!is_valid_words(words))
		return (NULL);
	while (words != NULL)
	{
		cmd = malloc(sizeof(t_cmd_block));
		malloc_check(cmd, "parser");
		cmd->redirects = NULL;
		set_redirects(&words, &cmd->redirects);
		cmd->command = words->content;
		set_args(&words, cmd);
		set_redirects(&words, &cmd->redirects);
		set_tokens(&tokens, cmd);
		if (words != NULL && ft_strncmp(words->content, "|", 1) == 0)
			words = words->next;
		else
			break ;
	}
	return (tokens);
}
