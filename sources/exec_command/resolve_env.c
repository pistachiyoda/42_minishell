#include "minishell.h"

char	*get_env_val(char *key, char **envp)
{
	int		i;
	char	**value;

	i = 0;
	while (envp[i] != NULL)
	{
		if (ft_strncmp(envp[i], key, ft_strlen(key)) == 0)
		{
			value = ft_split(envp[i], '=');
			return (value[1]);
		}
		i++;
	}
	return (NULL);
}

char	*resolve_path(char	*command, char *path_val)
{
	char	**paths;
	char	*path;
	char	*full_path;
	int		i;

	paths = ft_split(path_val, ':');
	i = 0;
	while (paths[i] != NULL)
	{
		path = ft_strjoin(paths[i], "/");
		full_path = ft_strjoin(path, command);
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
	print_error(command, EMESS_NO_FILE_DIR);
	return (NULL);
}
