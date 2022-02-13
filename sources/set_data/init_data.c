#include "minishell.h"

// 暫定実装
// 
t_cmd_block *build_cmd_block(char *command_line)
{
	t_cmd_block *cmd_block;
	int		i;
	int		arg_cnt;
	char	**splitted;

	cmd_block = malloc(sizeof(t_cmd_block *));
	if (cmd_block == NULL)
		return (NULL);
	splitted = ft_split(command_line, ' ');
	cmd_block->command = splitted[0];
	arg_cnt = 0;
	while (splitted[arg_cnt])
		arg_cnt++;
	cmd_block->args = malloc(sizeof(char *) * (arg_cnt + 1));
	if (cmd_block->args == NULL)
	{
		free(cmd_block);
		return (NULL);
	}
	i = 0;
	while (i < arg_cnt)
	{
		cmd_block->args[i] = ft_strdup(splitted[i]);
		i++;
	}
	cmd_block->args[i] = NULL;
	return (cmd_block);
}

int cnt_arr(char **str_arr)
{
	int	i;

	i = 0;
	while (str_arr[i])
		i++;
	return (i);
}

int	set_cmd_lst(t_list **cmd_list, char *command_line)
{
	char		**cmd_block_str_array;
	int			blocks_cnt;
	int			i;
	t_cmd_block *cmd_block;

	cmd_block_str_array = ft_split(command_line, '|');
	blocks_cnt = cnt_arr(cmd_block_str_array);
	i = 0;
	while (i < blocks_cnt)
	{
		cmd_block = build_cmd_block(cmd_block_str_array[i]);
		if (cmd_block == NULL)
			return (1); // TODO: free
		if (*cmd_list == NULL)
			*cmd_list = ft_lstnew(cmd_block);
		else
			ft_lstadd_back(cmd_list, ft_lstnew(cmd_block));
		i++;
	}
	return (0);
}
