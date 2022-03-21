#include "minishell.h"

char	*resolve_path(char	*command, char *path_val)
{
	char	**paths;
	char	*path;
	char	*full_path;
	int		i;

	if (path_val == NULL)
	{
		print_error(command, EMESS_NO_FILE_DIR);
		exit(127);
	}
	paths = ft_split_wrapper(path_val, ':');
	i = 0;
	while (paths[i] != NULL)
	{
		path = ft_strjoin_wrapper(paths[i], "/");
		full_path = ft_strjoin_wrapper(path, command);
		free(path);
		if (is_executable(full_path))
		{
			free_2d_array(paths);
			return (full_path);
		}
		free(full_path);
		i++;
	}
	free_2d_array(paths);
	print_error(command, EMESS_NO_CMD);
	exit(127);
}
