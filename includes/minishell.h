#ifndef MINISHELL_H
# define MINISHELL_H
# include "libft.h"
# include <unistd.h>
# include <stdbool.h>
# include <stdio.h>
# include <signal.h>
# include <readline/readline.h>
# include <readline/history.h>

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

void	set_sigaction(void);
void	wait_signal(void);
#endif
