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
