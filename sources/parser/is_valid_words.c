#include "minishell.h"

bool	is_valid_words(t_list *words)
{
	if (ft_strncmp(words->content, "|", 1) == 0)
	{
		syntax_error("|");
		return (false);
	}
	return (true);
}
