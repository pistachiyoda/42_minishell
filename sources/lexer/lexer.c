#include "lexer.h"

bool	is_space_tab_newline(char c)
{
	if (c == ' ' || c == '\t' || c == '\n')
		return (true);
	return (false);
}

//exitは後で関数使いまわせるようにする
void	exit_program()
{
	exit(EXIT_SUCCESS);
}

void	lexer(char *str)
{
	int	i;
	int	j;
	bool	quoat;
	bool	dquoat;
	t_list	*token;
	t_list	*new_ele;
	char	*save;
	char	*tmp;

	i = 0;
	j = 0;
	quoat = false;
	dquoat = false;
	save = str;
	token = ft_lstnew(NULL);
	while (str[i] != '\0')
	{
		if (str[i] == QUOAT)
			quoat = !quoat;
		else if (str[i] == DQUOAT)
			dquoat = !dquoat;
		else if (is_space_tab_newline(str[i]))
		{
			tmp = &save[i + 1];
			save[i] = '\0';
			new_ele = ft_lstnew(save);
			ft_lstadd_back(&token, new_ele);
			j = i + 1;
			save = tmp;
		}
		i++;
	}
	if (quoat == false || dquoat == false)
		exit_program();
}
