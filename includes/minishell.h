#ifndef MINISHELL_H
# define MINISHELL_H
# include "libft.h"
# include <readline/readline.h>
# include <readline/history.h>

typedef struct s_cmd_block
{
	char	*command;
	char	*args;
}	t_cmd_bolck;

// sources/parse/parse_data.c
t_cmd_bolck	*parse_data(char *str);

// sources/runner/run_commnad.c
int			run_command(t_cmd_bolck	*cmd_block);
#endif
