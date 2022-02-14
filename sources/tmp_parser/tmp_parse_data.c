#include "minishell.h"

void	tmp_parse_data(t_list **cmd_lst, char *str)
{
	char		**datas;
	t_cmd_block	*cmd_block;

	cmd_block = (t_cmd_block *)malloc(sizeof(t_cmd_block));
	datas = ft_split(str, ' ');
	cmd_block->command = datas[0];
	cmd_block->args = datas;
	*cmd_lst = ft_lstnew(cmd_block);
}
