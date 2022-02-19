#include "minishell.h"

void	tmp_parse_data(t_list **cmd_lst, char *str)
{
	char		**datas;
	t_cmd_block	*cmd_block;
	t_redirects	*redirect1;
	t_redirects	*redirect2;

	// cat << EOF1 << EOF2
	cmd_block = (t_cmd_block *)malloc(sizeof(t_cmd_block));
	datas = ft_split(str, ' ');
	cmd_block->command = datas[0];
	cmd_block->args = datas;
	redirect1 = (t_redirects *)malloc(sizeof(t_redirects));
	redirect1->redirect = HEREDOC;
	redirect1->target = ft_strdup("EOF1");
	cmd_block->redirects = ft_lstnew(redirect1);
	redirect2 = (t_redirects *)malloc(sizeof(t_redirects));
	redirect2->redirect = HEREDOC;
	redirect2->target = ft_strdup("EOF2");
	ft_lstadd_back(&cmd_block->redirects, ft_lstnew(redirect2));
	*cmd_lst = ft_lstnew(cmd_block);
}
