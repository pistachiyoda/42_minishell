#ifndef CMDLINE_DATA_H
# define CMDLINE_DATA_H

//    WRITE : >
//   APPEND : >>
//    INPUT : <
//  HEREDOC : <<
enum e_REDIRECTS
{
	WRITE,
	APPEND,
	INPUT,
	HEREDOC
};

typedef struct s_redirects
{
	enum e_REDIRECTS	redirect;
	char				*target;
}	t_redirects;

typedef struct s_cmd_block
{
	char	*command;
	char	**args;
	t_list	*redirects;
}	t_cmd_block;
#endif
