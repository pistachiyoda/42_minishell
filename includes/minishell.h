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
# include <sys/syslimits.h>
# include "expansion.h"

# define EMESS_NO_FILE_DIR "No such file or directory\n"
# define EMESS_IS_DIR "is a directory\n"
# define EMESS_IS_DIR2 "Is a directory\n"
# define EMESS_NO_CMD "command not found\n"
# define EMESS_NO_PERM "Permission denied\n"
# define EMESS_REQUIRE_ARGS "Require argument\n"
# define EMESS_INVALID_ARGS "Invalid argument\n"
# define EMESS_MALLOC_FAIL "Memory allocation failure\n"
# define EMESS_UNCLOSED_QUOAT "Unclosed quoat detected\n"
# define EMESS_INVALID_ID "not a valid identifier\n"
# define EMESS_SYNTAX "syntax error near unexpected token"
# define EMESS_LARGE_FD "Bad file descriptor\n"
# define EMESS_REDIRECT "ambiguous redirect\n"

# define FD_MAX 256

extern unsigned char	g_status;

// exec_command/exec_command.c
void		exec_command(t_cmd_block *cmd_block, char **envp);

// exec_command/resolve_env.c
char		*get_env_val(char *key, char **envp);
char		*resolve_path(char	*command, char *path_val);

// exec_command/utils.c
bool		is_directory(char *path);
bool		is_exists(char *path);
bool		is_executable(char *command_path);

// utils/free.c
void		free_2d_array(char **two_d_array);
void		ft_lstclear2(t_list **lst);
bool		free_words_str(t_list **words, char *str);
void		free_tokens(t_list *tokens);

// utils/is_env_registered.c
char		*is_env_registerd(t_environ *env, char **split_ele, bool key_only);

// utils/is_quote_type_switched.c
bool		is_quote_type_switched(char *str, int i, int *status);

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

// utils/malloc_check2.c
char		*xstrjoin(char const *s1, char const *s2, char *target);

// utils/wrapper.c
void		pipe_wrapper(int pipe_fds[2]);
int			fork_wrapper(void);
void		close_wrapper(int fd);
void		waitpid_wrapper(pid_t pid, int *wstatus, int options);
char		**ft_split_wrapper(char const *s, char c);

// utils/wrapper2.c
char		*ft_strjoin_wrapper(char const *s1, char const *s2);
void		execve_wrapper(
				const char *pathname, char *const argv[], char *const envp[]);
void		dup2_wrapper(int oldfd, int newfd);

// exec_command_line/exec_command_line.c
int			exec_command_line(t_list *cmd_list, char **envp, int cmd_cnt);
void		handle_command_line(t_cmd_block *cmd_block, char **envp);

// exec_command_line/handle_file.c
bool		is_readable(char *file);
int			open_file(char *file);
bool		is_writable(char *file);
int			open_or_create_file(char *file, int open_flag);

// exec_command_line/handle_heredoc.c
int			handle_heredoc_input(t_list *cmd_list);

// exec_command_line/handle_heredoc2.c
char		*ft_strjoin2(char const *s1, char const *s2);

// exec_command_line/close_doc_pipe_fds.c
int			close_doc_pipe_fd(t_cmd_block *cmd_block);

// exec_command_line/handle_redirect.c
int			handle_redirect(t_redirects	*redirect, t_cmd_block *cmd_block);
void		handle_redirects(t_cmd_block *cmd_block);

// exec_command_line/handle_input_redirect.c
bool		is_last_fd_input_redirect(t_redirects *redirect, t_list *redirects);
int			handle_input(t_redirects *redirect, bool is_last);

// /exec_command_line/handle_pipe.c
int			handle_first_block(
				t_cmd_block *cmd_block, char **envp, int pipe_write[2]);
int			handle_middle_block(
				t_cmd_block *cmd_block, char **envp,
				int pipe_read[2], int pipe_write[2]);
int			handle_last_block(
				t_cmd_block *cmd_block, char **envp, int pipe_read[2]);

// runner/run_builtin_command.c
bool		is_builtin_command(t_cmd_block *cmd_block);
bool		is_fork_required(t_list *cmd_list);
int			run_builtin_command(t_cmd_block *cmd_block, t_environ *env);

// exec_builtin
int			run_builtin_only_command(t_list *cmd_list, t_environ *env);

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
char		*is_env_registerd(t_environ *env, char **split_ele, bool key_only);
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

// builtin/pwd.c
int			ft_pwd(void);

// builtin/echo.c
int			ft_echo(t_cmd_block *cmd_block);

// [後々削除]debug/debug_funcs.c
void		print_cmd_lst(t_list *cmd_lst);
t_list		*set_dataset(char *str);
// [後々削除]tmp_parser/tmp_parse_data.c
void		tmp_parse_data(t_list **cmd_lst, char *str);
#endif
