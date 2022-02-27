#include "minishell.h"

bool	is_directory(char *path)
{
	struct stat	st;

	stat(path, &st);
	return (S_ISDIR(st.st_mode));
}

bool	is_exists(char *path)
{
	if (access(path, F_OK) == 0)
		return (true);
	return (false);
}

bool	is_executable(char *command_path)
{
	if (access(command_path, X_OK) == 0)
		return (true);
	return (false);
}
