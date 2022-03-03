#include "minishell.h"

bool	is_relative_or_absolute(char *command_path)
{
	if (ft_strncmp(command_path, "/", 1) == 0
		|| ft_strncmp(command_path, "./", 2) == 0
		|| ft_strncmp(command_path, "../", 3) == 0)
		return (true);
	return (false);
}

void	exec_relative_or_absolute(char *command_path, char **args, char **envp)
{
	if (is_directory(command_path))
	{
		print_error(command_path, EMESS_IS_DIR);
		exit(1);
	}
	if (!is_exists(command_path))
	{
		print_error(command_path, EMESS_NO_FILE_DIR);
		exit(1);
	}
	if (!is_executable(command_path))
	{
		print_error(command_path, EMESS_NO_PERM);
		exit(1);
	}
	execve(command_path, args, envp);
	exit(1);
}

void	exec_command(char *command_path, char **args, char **envp)
{
	char	*path_val;
	char	*real_command_path;

	if (is_relative_or_absolute(command_path))
		exec_relative_or_absolute(command_path, args, envp);
	path_val = get_env_val("PATH", envp);
	real_command_path = resolve_path(command_path, path_val);
	if (!real_command_path)
		exit(1);
	free(path_val);
	execve(real_command_path, args, envp);
	exit(1);
}
