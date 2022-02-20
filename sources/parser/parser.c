#include "minishell.h"


// t_list wordの先頭から順番に見ていく
// パイプがあったらcmd_block->next
//

void	parser(t_list *words)
{
	t_list	*tokens;
	t_cmd_block	*cmd;

	tokens->content = cmd;
	while (words->content != NULL)
	{
		if (ft_strncmp(words->content, "|", 1) == 0)
		{
			tokens->content = tokens->content->next; //みたいな感じ？？
		}

	}
}

void	set_args(char *str)
{
	t_list	*words;
	//redirect, pipe終わりだった場合、次の処理に進まないようにする
	words = lexer(str)
	if (words == NULL)
		return ;
	// ここでparserを呼ぶ感じにするかな
	// 変数展開の関数呼ぶ
}
