#ifndef MINISHELL_H
# define MINISHELL_H
# include <unistd.h>
# include <stdbool.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "libft.h"
# include "cmdline_data.h"
# include "lexer.h"
# include "parser.h"

# define EMESS_NO_FILE_DIR "No such file or directory\n"
# define EMESS_IS_DIR "is a directory\n"
# define EMESS_NO_CMD "command not found\n"
# define EMESS_NO_PERM "Permission denied\n"
# define EMESS_REQUIRE_ARGS "Require argument\n"
# define EMESS_INVALID_ARGS "Invalid argument\n"
# define EMESS_MALLOC_FAIL "Memory allocation failure\n"
# define EMESS_UNCLOSED_QUOAT "Unclosed quoat detected\n"
# define EMESS_INVALID_ID "not a valid identifier\n"
# define EMESS_SYNTAX "syntax error near unexpected token"
# define EMESS_LARGE_FD "Bad file descriptor\n"

// exec_command/exec_command.c
int			exec_command(char *command_path, char **args, char **envp);

// exec_command/resolve_env.c
char		*get_env_val(char *key, char **envp);
char		*resolve_path(char	*command, char *path_val);

// exec_command/utils.c
bool		is_directory(char *path);
bool		is_exists(char *path);
bool		is_executable(char *command_path);

// utils/free.c
void		free_2d_array(char **two_d_array);
void		free_tokens(t_list *tokens);

// utils/print_error.c
void		syntax_error(char *str);
void		print_error(char *target, char *message);

// utils/malloc_check.c
t_list		*xlstnew(void *content, char *target);
char		*xstrdup(const char *src, char *target);
char		*xsubstr(char const *s, unsigned int start,
				size_t len, char *target);
void		*xmalloc(size_t len, char *target);
void		malloc_check(void *words, char *target);

// exec_command_line/exec_command_line.c
int			exec_command_line(t_list *cmd_list, char **envp);

// exec_command_line/handle_file.c
bool		is_readable(char *file);
int			open_file(char *file);
bool		is_writable(char *file);
int			open_or_create_file(char *file, int open_flag);

// exec_command_line/handle_heredoc.c
void		handle_heredoc(char *limiter, bool is_last, int	pipe_fds[2]);
void		handle_heredoc_loop(t_cmd_block *cmd_block, int	pipe_fds[2]);

// exec_command_line/handle_redirect.c
int			handle_redirect(t_cmd_block *cmd_block, int	pipe_fds[2]);

// exec_command_line/handle_input_redirect.c
bool		is_last_input_redirect(t_redirects *redirect, t_list *redirects);
int		handle_input(t_redirects *redirect, bool is_last);

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
bool		is_registered(t_environ *env, char **split_ele, bool key_only);
int			update_environ(t_cmd_block *cmd_block, t_environ *env, int i);
void		ft_export(t_cmd_block *cmd_block, t_environ *env);

// builtin/export_display_env.c
t_environ	*get_tmp_min(t_environ *env, int *min_i, char *flags, int f_len);
void		print_env(t_environ *min);
void		display_min_env(t_environ *min, int min_i, char *flags, int f_len);
void		display_sorted_env(t_environ *env, int min_i,
				char *flags, int f_len);

// builtin/unset.c
void		ft_unset(t_cmd_block *cmd_block, t_environ *env);

// [後々削除]debug/debug_funcs.c
void		print_cmd_lst(t_list *cmd_lst);
t_list		*set_dataset(char *str);
// [後々削除]tmp_parser/tmp_parse_data.c
void		tmp_parse_data(t_list **cmd_lst, char *str);
#endif
