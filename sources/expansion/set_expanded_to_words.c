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
	t_quote	quote;
	char	*head;
	bool	splitted;

	i = 0;
	quote.prev_q = 0;
	quote.status = NONE;
	quote.managed_i = 0;
	head = NULL;
	splitted = false;
	while (str[i] != '\0')
	{
		if (is_quote_type_switched(str, i, &quote.status))
			quote_removal(str, &head, i, &quote);
		if (quote.status != QUOTE && str[i] == '$')
		{
			set_head_before_dollar(str, &head, i, quote.prev_q);
			quote.managed_i = param_expansion(env, str, &head, &i);
			word_splitting(words, quote.status, &head, &splitted);
		}
		i++;
	}
	add_to_words(words, head, str);
	return (splitted);
}
