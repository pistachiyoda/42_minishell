#ifndef CMDLINE_DATA_H
# define CMDLINE_DATA_H

//    WRITE : >
//   APPEND : >>
//    INPUT : <
//  HEREDOC : <<
//  QUOTED_HEREDOC : << 'EOL' or << "EOL"
enum e_REDIRECTS
{
	WRITE,
	APPEND,
	INPUT,
	HEREDOC,
	QUOTED_HEREDOC
};

typedef struct s_environ
{
	char				*key;
	char				*value;
	struct s_environ	*prev;
	struct s_environ	*next;
}	t_environ;

typedef struct s_redirects
{
	enum e_REDIRECTS	redirect;
	char				*target;
	int					fd;
	int					doc_fd;
	bool				error;
}	t_redirects;

typedef struct s_cmd_block
{
	char	*command;
	char	**args;
	t_list	*redirects;
}	t_cmd_block;

typedef struct s_pipes
{
	int	pipe_a[2];
	int	pipe_b[2];
}	t_pipes;
#endif
