#include "minishell.h"

int	check_str_type(char *str)
{
	int	i;
	int	status;
	int	count;

	i = 0;
	status = SPACELESS;
	count = 0;
	while (str[i] != '\0')
	{
		if (is_blank(str[i]))
		{
			status = PARTIAL_SPACE;
			count++;
		}
		i++;
	}
	if (i != 0 && count == i)
		status = ALL_SPACE;
	return (status);
}

bool	is_quote_type_switched(char *str, int i, int *status)
{
	if (str[i] == '\'' && *status != DQUOTE)
	{
		if (*status == NONE)
			*status = QUOTE;
		else if (*status == QUOTE)
			*status = NONE;
		return (true);
	}
	else if (str[i] == '"' && *status != QUOTE)
	{
		if (*status == NONE)
			*status = DQUOTE;
		else if (*status == DQUOTE)
			*status = NONE;
		return (true);
	}
	return (false);
}

bool	is_character_contained(char *str, int *i)
{
	while (str[*i] != '\0' && is_blank(str[*i]))
		(*i)++;
	if (str[*i] == '\0')
		return (false);
	return (true);
}

bool	is_blank(char c)
{
	if (c == ' ' || c == '\t' || c == '\n')
		return (true);
	return (false);
}

bool	is_space_at_end(char *str)
{
	size_t	i;

	i = ft_strlen(str);
	if (i == 0 || is_blank(str[i - 1]))
		return (true);
	return (false);
}
