#ifndef MINISHELL_H
# define MINISHELL_H
# include "libft.h"
# include "cmdline_data.h"
# include <readline/readline.h>
# include <readline/history.h>
# include <unistd.h>
# include <stdbool.h>

# define EMESS_NO_FILE_DIR "No such file or directory\n"

// exec_command/exec_command.c
int		exec_command(char *command_path, char **args, char **envp);

// exec_command/resolve_env.c
char	*get_env_val(char *key, char **envp);
char	*resolve_path(char	*command, char *path_val);

// exec_command/utils.c
bool	is_executable(char *command_path);

// utils/free.c
void	free_2d_array(char **two_d_array);

// utils/print_error.c
void	print_error(char *target, char *message);

// exec_command_line/exec_command_line.c
int		exec_command_line(t_list *cmd_list, char **envp);

// runner/run_builtin_command.c
bool	is_fork_required(t_list *cmd_list);
int		run_builtin_command(t_cmd_block *cmd_block);

// [後々削除]debug/debug_funcs.c
void	print_cmd_lst(t_list *cmd_lst);

// [後々削除]tmp_parser/tmp_parse_data.c 
void	tmp_parse_data(t_list **cmd_lst, char *str);
#endif
