#ifndef MINISHELL_H
# define MINISHELL_H
# include "libft.h"
# include "cmdline_data.h"
# include <readline/readline.h>
# include <readline/history.h>
# include <unistd.h>
# include <stdbool.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <fcntl.h>

# define EMESS_NO_FILE_DIR "No such file or directory\n"
# define EMESS_REQUIRE_ARGS "Require argument\n"
# define EMESS_INVALID_ARGS "Invalid argument\n"
# define EMESS_MALLOC_FAIL "Memory allocation failure\n"

// exec_command/exec_command.c
int			exec_command(char *command_path, char **args, char **envp);

// exec_command/resolve_env.c
char		*get_env_val(char *key, char **envp);
char		*resolve_path(char	*command, char *path_val);

// exec_command/utils.c
bool		is_executable(char *command_path);

// utils/free.c
void		free_2d_array(char **two_d_array);

// utils/print_error.c
void		print_error(char *target, char *message);

// exec_command_line/exec_command_line.c
int			exec_command_line(t_list *cmd_list, char **envp);

// runner/run_builtin_command.c
bool		is_fork_required(t_list *cmd_list);
int			run_builtin_command(t_cmd_block *cmd_block, t_environ *env);

// env/create_environ.c
t_environ	*init_environ(char *msg);
char		**split_by_delimiter(char *env_str, char *msg);
t_environ	*add_environ(t_environ *env, t_environ *first_ele,
				char **split_ele, char *msg);
t_environ	*create_environ(char **envp);

// env/t_environ_to_vector.c
int			count_environ_ele(t_environ *env);
char		**t_environ_to_vector(t_environ *env);

// builtin/env.c
void		ft_env(t_environ *env);

// builtin/export.c
void		ft_export(t_cmd_block *cmd_block, t_environ *env);

// builtin/unset.c
void		ft_unset(t_cmd_block *cmd_block, t_environ *env);

// [後々削除]debug/debug_funcs.c
void		print_cmd_lst(t_list *cmd_lst);

// [後々削除]tmp_parser/tmp_parse_data.c
void		tmp_parse_data(t_list **cmd_lst, char *str);
#endif
