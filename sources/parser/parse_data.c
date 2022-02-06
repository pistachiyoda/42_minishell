#include "minishell.h"

t_cmd_bolck	*parse_data(char *str)
{
	char		**datas;
	t_cmd_bolck	*cmd_block;

	cmd_block = (t_cmd_bolck *)malloc(sizeof(t_cmd_bolck));
	datas = ft_split(str, ' ');
	cmd_block->command = datas[0];
	cmd_block->args = datas[1];
	return (cmd_block);
}
