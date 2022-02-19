#include "minishell.h"

void	add_last_str(char *str, t_list *words, int i, int start)
{
	t_list	*new_ele;
	t_list	*last;

	last = ft_lstlast(words);
	if (start < i)
	{
		new_ele = ft_lstnew(ft_substr(str, start, i - start));
		ft_lstadd_back(&words, new_ele);
	}
	else if (start == i && (ft_strncmp(last->content, ">", 1) == 0
			|| ft_strncmp(last->content, "<", 1) == 0
			|| ft_strncmp(last->content, ">>", 2) == 0
			|| ft_strncmp(last->content, "<<", 2) == 0
			|| ft_strncmp(last->content, "|", 1) == 0))
		print_error("lexer", EMESS_INVALID_ARGS);
}
