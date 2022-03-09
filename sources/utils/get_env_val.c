#include "minishell.h"

char	*get_env_val(char *key, char **envp)
{
	int		i;
	char	**data;

	i = 0;
	while (envp[i] != NULL)
	{
		data = ft_split_wrapper(envp[i], '=');
		if (ft_strlen(data[0]) == ft_strlen(key) && ft_strncmp(envp[i], key, ft_strlen(key)) == 0)
			return (data[1]);
		i++;
	}
	return (NULL);
}
