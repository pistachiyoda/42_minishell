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

t_expand	init_struct(void)
{
	t_expand	data;

	data.prev_q = 0;
	data.status = NONE;
	data.i = 0;
	data.managed_i = 0;
	data.end = false;
	return (data);
}

bool	set_expanded_to_words(t_environ *env, char *str, t_list **words)
{
	t_expand	data;
	char		*head;
	bool		redir_error;

	data = init_struct();
	head = NULL;
	redir_error = false;
	while (str[data.i] != '\0')
	{
		if (is_quote_type_switched(str, data.i, &data.status))
			quote_removal(str, &head, &data);
		if (data.status != QUOTE && str[data.i] == '$')
		{
			set_head_before_dollar(str, &head, data);
			param_expansion(env, &data, str, &head);
			word_splitting(words, data, &head, &redir_error);
		}
		data.i++;
	}
	if (!data.end)
		add_to_words(words, head, str);
	return (redir_error);
}
