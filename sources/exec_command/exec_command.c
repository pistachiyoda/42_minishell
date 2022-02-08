#include "minishell.h"

bool	is_relative_or_absolute(char *command_path)
{
	if (ft_strncmp(command_path, "/", 1) == 0
		|| ft_strncmp(command_path, "./", 2) == 0
		|| ft_strncmp(command_path, "../", 3) == 0)
		return (true);
	return (false);
}

int	exec_relative_or_absolute(char *command_path, char **args, char **envp)
{
	if (is_executable(command_path))
		execve(command_path, args, envp);
	print_error(command_path, EMESS_NO_FILE_DIR);
	return (1);
}

int	exec_command(char *command_path, char **args, char **envp)
{
	char	*path_val;
	char	*real_command_path;

	if (is_relative_or_absolute(command_path))
		return (exec_relative_or_absolute(command_path, args, envp));
	path_val = get_env_val("PATH", envp);
	real_command_path = resolve_path(command_path, path_val);
	free(path_val);
	execve(real_command_path, args, envp);
	return (1);
}
