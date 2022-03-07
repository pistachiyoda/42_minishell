#include "minishell.h"

void	add_to_words(t_list **words, char *head, char *str)
{
	if (head == NULL)
		head = xstrdup(str, "expansion");
	if (*words == NULL)
		*words = xlstnew(head, "expansion");
	else
		ft_lstadd_back(words, xlstnew(head, "expansion"));
}

bool	set_expanded_to_words(t_environ *env, char *str, t_list **words)
{
	int		i;
	int		prev_q;
	int		status;
	char	*head;
	bool	splitted;

	i = 0;
	prev_q = 0;
	status = NONE;
	head = NULL;
	splitted = false;
	while (str[i] != '\0')
	{
		if (is_quote_type_switched(str, i, &status))
			quote_removal(str, &head, i, &prev_q);
		if (status != QUOTE && str[i] == '$')
		{
			set_head_before_dollar(str, &head, i, prev_q);
			param_expansion(env, str, &head, &i);
			word_splitting(words, status, &head, &splitted);
		}
		i++;
	}
	add_to_words(words, head, str);
	return (splitted);
}
