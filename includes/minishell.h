/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmai <fmai@student.42tokyo.jp>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/23 01:15:20 by fmai              #+#    #+#             */
/*   Updated: 2022/03/31 22:31:26 by fmai             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H
# include <unistd.h>
# include <stdio.h>
# include <stdbool.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <string.h>
# include <errno.h>
# include <signal.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <sys/syslimits.h>
# include <limits.h>
# include "libft.h"
# include "cmdline_data.h"
# include "lexer.h"
# include "parser.h"
# include "expansion.h"

# define EMESS_NO_FILE_DIR "No such file or directory\n"
# define EMESS_IS_DIR "is a directory\n"
# define EMESS_IS_DIR2 "Is a directory\n"
# define EMESS_NO_CMD "command not found\n"
# define EMESS_NO_PERM "Permission denied\n"
# define EMESS_REQUIRE_ARGS "Require argument\n"
# define EMESS_MALLOC_FAIL "Memory allocation failure\n"
# define EMESS_INVALID_ID "not a valid identifier\n"
# define EMESS_SYNTAX "syntax error near unexpected token"
# define EMESS_LARGE_FD "Bad file descriptor\n"
# define EMESS_TM_ARGS "too many arguments\n"
# define EMESS_AMBIGU "ambiguous redirect\n"
# define EMESS_BADF "Bad file descriptor\n"

# define FD_MAX 255

extern volatile int	g_status;

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
void		free_env(t_environ *env);
void		free_tokens(t_list **tokens);

// utils/free2.c
void		free_cmd_block_after_exec(t_cmd_block *cmd_block);
void		free_cmd_list_after_exec(t_list *cmd_list);

// utils/check_str_type.c
int			check_str_type(char *str);
bool		is_character_contained(char *str, int *i);
bool		is_space_at_end(char *str);

// utils/check_char_type.c
bool		is_quote_type_switched(char *str, int i, int *status);
bool		is_blank(char c);
bool		is_special_char(char c);

// utils/print_error.c
void		syntax_error(char *str);
void		invalid_id_error(char *str, char *cmd);
void		print_error(char *target, char *message);

// utils/malloc_check.c
t_list		*ft_xlstnew(void *content, char *target);
char		*ft_xstrdup(const char *src, char *target);
char		*ft_xsubstr(char const *s, unsigned int start,
				size_t len, char *target);
void		*ft_xmalloc(size_t len, char *target);
void		malloc_check(void *words, char *target);

// utils/malloc_check2.c
char		*ft_xstrjoin(char const *s1, char const *s2, char *target);
char		*ft_xstrjoin_with_free(char *s1, char *s2, char *target);
char		*ft_xstrjoin2_with_free(char *s1, char *s2, char *target);
char		*ft_xitoa(int val, char *target);

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
int			dup2_wrapper(int oldfd, int newfd);

// utils/validation.c
bool		is_valid_arg(char *str, char *original, char *cmd);

// utils/ft_atol.c
long long	ft_atol(const char *str, bool *is_invalid);

// utils/get_env_val.c
char		*get_env_val(char *key, char **envp);

// utils/exit_program.c
void		exit_program(int status);

// exec_command_line/exec_command_line.c
int			exec_command_line(
				t_environ *env, t_list *cmd_list, char **envp, int cmd_cnt);
void		handle_command_line(t_cmd_block *cmd_block, char **envp);

// exec_command_line/exec_command_line2.c
int			*crp(int i, int pipe_a[2], int pipe_b[2]);
int			*cwp(int i, int pipe_a[2], int pipe_b[2]);
int			get_child_status(int status, bool sigint);
int			wait_pids(int cmd_cnt, int pids[1000]);

// exec_command_line/handle_file.c
bool		is_readable(char *file);
int			open_file(char *file);
bool		is_writable(char *file);
int			open_or_create_file(char *file, int open_flag);

// exec_command_line/handle_heredoc.c
int			handle_heredoc_input(t_environ *env, t_list *cmd_list);

// exec_command_line/handle_heredoc2.c
char		*expand_env_variables_in_buf(t_environ *env, char *buf);
void		handle_unused_heredoc(char *str);
void		flush_heredoc(char *str, int doc_pipe_fds[2]);

// exec_command_line/handle_heredoc3.c
void		handle_unused_heredoc(char *str);
void		flush_heredoc(char *str, int doc_pipe_fds[2]);

// exec_command_line/close_doc_pipe_fds.c
int			close_doc_pipe_fd(t_cmd_block *cmd_block);

// exec_command_line/handle_redirect.c
int			handle_redirect(t_redirects	*redirect, t_cmd_block *cmd_block);
int			handle_redirects(t_cmd_block *cmd_block);

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
t_environ	*add_environ(t_environ *env, t_environ *first_ele,
				char **split_ele, char *msg);
char		**split_by_delimiter(char *str, bool *key_only, char *target);
t_environ	*create_environ(char **envp);

// env/t_environ_to_vector.c
int			count_environ_ele(t_environ *env);
char		**t_environ_to_vector(t_environ *env);

// env/is_env_registered.c
void		update_value(t_environ *env, char **split_ele, char *target);
bool		is_env_registered(t_environ *env, char **split_ele, char **value);

// builtin/env.c
int			ft_env(t_environ *env);

// builtin/export.c
void		update_environ(char *str, t_environ *env, int *status);
int			ft_export(t_cmd_block *cmd_block, t_environ *env);

// builtin/export_display_env.c
t_environ	*get_tmp_min(t_environ *env, int *min_i, char *flags, int f_len);
void		print_env(t_environ *min);
void		display_min_env(t_environ *min, int min_i, char *flags, int f_len);
void		display_sorted_env(t_environ *env, int min_i,
				char *flags, int f_len);

// builtin/unset.c
void		unset_env(char *str, t_environ *env, size_t len);
int			ft_unset(t_cmd_block *cmd_block, t_environ *env);

// builtin/pwd.c
int			ft_pwd(t_environ *env);

// builtin/echo.c
int			ft_echo(t_cmd_block *cmd_block);

// builtin/exit.c
int			ft_exit(t_cmd_block *cmd_block);

// builtin/cd.c
int			ft_cd(t_cmd_block *cmd_block, t_environ *env);

// builtin/cd2.c
bool		is_double_slash(char *command_path);
bool		is_absolute(char *command_path);

// signal/signal.c
void		sigint_handler(int sig);
void		set_signal(void (*func1)(int), void (*func2)(int));
#endif
