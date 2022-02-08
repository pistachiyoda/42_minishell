#include "minishell.h"

bool	is_executable(char *command_path)
{
	if (access(command_path, X_OK) == 0)
		return (true);
	return (false);
}
