#include "minishell.h"

t_redirects	*set_redirect_member(t_list **words)
{
	t_redirects	*redir;

	redir = ft_xmalloc(sizeof(t_redirects), "parser");
	redir->fd = ft_atoi((*words)->content);
	*words = (*words)->next;
	if (ft_strncmp((*words)->content, ">>", 2) == 0)
		redir->redirect = APPEND;
	else if (ft_strncmp((*words)->content, "<<", 2) == 0)
	{
		if (ft_strchr((char *)(*words)->next->content, '\'')
			|| ft_strchr((char *)(*words)->next->content, '"'))
			redir->redirect = QUOTED_HEREDOC;
		else
			redir->redirect = HEREDOC;
	}
	else if (ft_strncmp((*words)->content, ">", 1) == 0)
		redir->redirect = WRITE;
	else if (ft_strncmp((*words)->content, "<", 1) == 0)
		redir->redirect = INPUT;
	*words = (*words)->next;
	redir->target = (*words)->content;
	*words = (*words)->next;
	redir->error = false;
	return (redir);
}

void	set_first_args(t_list **words, t_cmd_block *cmd, int arg_num)
{
	int	i;

	i = 0;
	cmd->args = ft_xmalloc(sizeof(char *) * (arg_num + 1), "parser");
	while (i < arg_num)
	{
		cmd->args[i] = (*words)->content;
		*words = (*words)->next;
		i++;
	}
	cmd->args[i] = NULL;
}

void	add_args(t_list **words, t_cmd_block *cmd, int arg_num)
{
	int		i;
	int		j;
	char	**old;

	i = 0;
	j = 0;
	old = cmd->args;
	while (old[j] != NULL)
		j++;
	cmd->args = ft_xmalloc(sizeof(char *) * (j + arg_num + 1), "parser");
	while (old[i] != NULL)
	{
		cmd->args[i] = old[i];
		i++;
	}
	free(old);
	i = 0;
	while (i < arg_num)
	{
		cmd->args[j + i] = (*words)->content;
		*words = (*words)->next;
		i++;
	}
	cmd->args[j + i] = NULL;
}

void	set_args(t_list **words, t_cmd_block *cmd)
{
	t_list	*tmp;
	int		arg_num;

	arg_num = 0;
	tmp = *words;
	while (tmp != NULL && tmp->next != NULL && !is_redirect(tmp->next)
		&& ft_strncmp(tmp->next->content, "|", 1) != 0)
	{
		arg_num++;
		tmp = tmp->next;
	}
	if (tmp->next == NULL || ft_strncmp(tmp->next->content, "|", 1) == 0)
		arg_num++;
	if (cmd->args == NULL)
		set_first_args(words, cmd, arg_num);
	else
		add_args(words, cmd, arg_num);
}

void	set_cmd_block(t_list **words, t_cmd_block *cmd)
{
	cmd->command = NULL;
	cmd->args = NULL;
	cmd->redirects = NULL;
	if (((*words)->next != NULL && !is_redirect((*words)->next))
		|| (*words)->next == NULL)
		cmd->command = (*words)->content;
	if (cmd->command != NULL)
		set_args(words, cmd);
	while (*words != NULL && (*words)->next != NULL
		&& is_redirect((*words)->next))
	{
		if (cmd->redirects == NULL)
			cmd->redirects = ft_xlstnew(set_redirect_member(words), "parser");
		else
			ft_lstadd_back(&cmd->redirects,
				ft_xlstnew(set_redirect_member(words), "parser"));
		if (*words != NULL && ft_strncmp((*words)->content, "|", 1) != 0
			&& (((*words)->next != NULL && !is_redirect((*words)->next))
				|| (*words)->next == NULL))
		{
			if (cmd->command == NULL)
				cmd->command = (*words)->content;
			set_args(words, cmd);
		}
	}
}
