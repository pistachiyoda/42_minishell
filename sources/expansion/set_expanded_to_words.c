#include "minishell.h"

char	*left_to_next_head(char *left, char *head, int status)
{
	int		j;
	char	*tmp;

	j = 0;
	if (left)
	{
		while (left[j] != '\0')
		{
			if (left[j] == '$' && is_in_quote_dquote(left, j, status) != QUOTE)
				break ;
			j++;
		}
		tmp = xstrjoin(head, xsubstr(left, 0, j, "expansion"), "expansion");
		free(head);
		head = tmp;
	}
	return (head);
}

void	add_to_words(t_list **words, char *head, char *str)
{
	if (head == NULL)
		head = xstrdup(str, "expansion");
	if (*words == NULL)
		*words = xlstnew(head, "expansion");
	else
		ft_lstadd_back(words, xlstnew(head, "expansion"));
}

int	set_expanded_to_words(t_environ *env, char *str, t_list **words)
{
	int		status;
	int		i;
	bool	splitted;
	char	*head;
	char	*left;

	i = 0;
	status = NONE;
	head = NULL;
	splitted = false;
	while (str[i] != '\0')
	{
		status = is_in_quote_dquote(str, i, status);
		if (status != QUOTE && str[i] == '$')
		{
			if (head == NULL && i != 0)
				head = xsubstr(str, 0, i, "expansion");
			left = param_expansion(env, str, &head, &i);
			head = word_split(words, status, head, &splitted);
			head = left_to_next_head(left, head, status);
		}
		i++;
	}
	add_to_words(words, head, str);
	return (splitted);
}
