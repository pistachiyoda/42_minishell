#include "minishell.h"
#include <stdio.h>
void	print_string_array(char **string_array, char *key)
{
	int	i;

	i = 0;
	while (string_array[i])
	{
		printf("%s[%d]: %s\n", key, i, string_array[i]);
		i++;
	}
	return ;
}

char	*print_redirect(enum e_REDIRECTS redirect)
{
	if (redirect == WRITE)
		return (">");
	if (redirect == APPEND)
		return (">>");
	if (redirect == INPUT)
		return ("<");
	if (redirect == HEREDOC)
		return ("<<");
	return ("not_redirection");
}

void	print_list(t_list *lst)
{
	int	i;

	i = 0;
	while (1)
	{
		printf("++++ redirect %d++++\n", i);
		printf("redirect: %s\n",
			print_redirect(((t_redirects *)lst->content)->redirect));
		printf("target: %s\n", ((t_redirects *)lst->content)->target);
		if (lst->next == NULL)
			return ;
		lst = lst->next;
		i++;
	}
}

void	print_cmd_lst(t_list *cmd_lst)
{
	int	i;

	i = 0;
	while (1)
	{
		printf("\n====== block %d ======\n", i);
		printf("cmd: %s\n", ((t_cmd_block *)cmd_lst->content)->command);
		print_string_array(((t_cmd_block *)cmd_lst->content)->args, "args");
		if (((t_cmd_block *)cmd_lst->content)->redirects)
			print_list(((t_cmd_block *)cmd_lst->content)->redirects);
		printf("=====================\n");
		if (cmd_lst->next == NULL)
			break ;
		cmd_lst = cmd_lst->next;
		i++;
	}
}