#include "minishell.h"

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

bool	is_blank(char c)
{
	if (c == ' ' || c == '\t' || c == '\n')
		return (true);
	return (false);
}

bool	is_special_char(char c)
{
	if (c != '\0' && c != '$' && c != '\'' && c != '"'
		&& ft_isalnum(c) != 1 && c != '_')
		return (true);
	return (false);
}
