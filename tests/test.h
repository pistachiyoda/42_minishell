extern "C" {
	/////////////////////////////////////////////////////////////////////////////////////
	// libft
	void				*ft_memset(void *b, int c, size_t n);
	void				ft_bzero(void *s, size_t n);
	void				*ft_memcpy(void *dst, const void *src, size_t n);
	void				*ft_memccpy(void *dst, const void *src, int c, size_t n);
	void				*ft_memmove(void *dst, const void *src, size_t n);
	void				*ft_memchr(const void *s, int c, size_t n);
	int					ft_memcmp(const void *s1, const void *s2, size_t n);
	size_t				ft_strlen(const char *str);
	size_t				ft_strlcpy(char *dst, const char *src, size_t dstsize);
	size_t				ft_strlcat(char *dst, const char *src, size_t dstsize);
	char				*ft_strchr(const char *s, int c);
	char				*ft_strrchr(const char *s, int c);
	char				*ft_strnstr(
							const char *haystack, const char *needle, size_t len);
	int					ft_strncmp(const char *s1, const char *s2, size_t n);
	int					ft_atoi(const char *str);
	int					ft_isalpha(int c);
	int					ft_isdigit(int c);
	int					ft_isalnum(int c);
	int					ft_isascii(int c);
	int					ft_isprint(int c);
	int					ft_toupper(int c);
	int					ft_tolower(int c);
	void				*ft_calloc(size_t count, size_t size);
	char				*ft_strdup(const char *src);
	char				*ft_substr(char const *s, unsigned int start, size_t len);
	char				*ft_strjoin(char const *s1, char const *s2);
	char				*ft_strtrim(char const *s1, char const *set);
	char				**ft_split(char const *s, char c);
	char				*ft_itoa(int val);
	char				*ft_strmapi(char const *s, char (*f)(unsigned int, char));
	void				ft_putchar_fd(char c, int fd);
	void				ft_putstr_fd(char *s, int fd);
	void				ft_putendl_fd(char *s, int fd);
	void				ft_putnbr_fd(int n, int fd);
	typedef struct s_list
	{
		void			*content;
		struct s_list	*next;
	}					t_list;
	t_list				*ft_lstnew(void *content);
	void				ft_lstadd_front(t_list **lst, t_list *new_node);
	int					ft_lstsize(t_list *lst);
	t_list				*ft_lstlast(t_list *lst);
	void				ft_lstadd_back(t_list **lst, t_list *new_node);
	void				ft_lstdelone(t_list *lst, void (*del)(void*));
	void				ft_lstclear(t_list **lst, void (*del)(void*));
	void				ft_lstiter(t_list *lst, void (*f)(void *));
	t_list				*ft_lstmap(
							t_list *lst, void *(*f)(void *), void (*del)(void *));
	/////////////////////////////////////////////////////////////////////////////////////
	// cmdline_data.h
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
		int					fd;
	}	t_redirects;

	typedef struct s_cmd_block
	{
		char	*command;
		char	**args;
		t_list	*redirects;
	}	t_cmd_block;

	typedef struct s_environ
	{
		char				*key;
		char				*value;
		struct s_environ	*prev;
		struct s_environ	*next;
	}	t_environ;

	/////////////////////////////////////////////////////////////////////////////////////
	// minishell.h
	// exec_command/exec_command.c
	int		exec_command(char *command_path, char **args, char **envp);

	// exec_command/resolve_env.c
	char	*get_env_val(char *key, char **envp);
	char	*resolve_path(char	*command, char *path_val);

	// exec_command/utils.c
	bool	is_executable(char *command_path);

	// utils/free.c
	void	free_2d_array(char **two_d_array);
	void	free_tokens(t_list *tokens);

	// utils/print_error.c
	void	print_error(char *target, char *message);

	// utils/malloc_check.c
	t_list		*xlstnew(void *content, char *target);
	char		*xstrdup(const char *src, char *target);
	char		*xsubstr(char const *s, unsigned int start,
					size_t len, char *target);
	void		*xmalloc(size_t len, char *target);
	void		malloc_check(void *words, char *target);

	// exec_command_line/exec_command_line.c
	int		exec_command_line(t_list *cmd_list, char **envp, int cmd_cnt);

	// exec_command_line/handle_file.c
	bool		is_readable(char *file);
	int			open_file(char *file);
	bool		is_writable(char *file);
	int			open_or_create_file(char *file, int open_flag);

	// runner/run_builtin_command.c
	bool	is_fork_required(t_list *cmd_list);
	int		run_builtin_command(t_cmd_block *cmd_block);

	// env/create_environ.c
	t_environ	*init_environ(char *msg);
	char		**split_by_delimiter(char *env_str, char *msg);
	t_environ	*add_environ(t_environ *env, t_environ *first_ele,
					char **split_ele, char *msg);
	t_environ	*create_environ(char **envp);

	// env/t_environ_to_vector.c
	int			count_environ_ele(t_environ *env);
	char		**t_environ_to_vector(t_environ *env);

	// builtin/env.c
	void		ft_env(t_environ *env);

	// builtin/export.c
	char		*is_registered(t_environ *env, char **split_ele, bool key_only);
	int			update_environ(t_cmd_block *cmd_block, t_environ *env, int i);
	void		ft_export(t_cmd_block *cmd_block, t_environ *env);

	// builtin/export_display_env.c
	t_environ	*get_tmp_min(t_environ *env, int *min_i, char *flags, int f_len);
	void		print_env(t_environ *min);
	void		display_min_env(t_environ *min, int min_i, char *flags, int f_len);
	void		display_sorted_env(t_environ *env, int min_i,
					char *flags, int f_len);

	// builtin/unset.c
	void		ft_unset(t_cmd_block *cmd_block, t_environ *env);

	// [後々削除]debug/debug_funcs.c
	void	print_cmd_lst(t_list *cmd_lst);

	// [後々削除]tmp_parser/tmp_parse_data.c
	void	tmp_parse_data(t_list **cmd_lst, char *str);

	/////////////////////////////////////////////////////////////////////////////////////
	// lexer.h
	enum e_STATUS
	{
		QUOTE,
		DQUOTE,
		NONE
	};

	// lexer/split_by_redirect_pipe.c
	bool		get_valid_fd_num(char *str, t_list **words, int i, int start);
	bool		is_valid_redirect_pipe(char *str, t_list *words, int *i, int start);
	int			split_by_redirect_pipe(char *str, t_list *words, int *i, int start);

	// lexer/lexer.c
	bool		is_space_tab_newline(char c);
	int			is_in_quote_dquote(char *str, int i, int status);
	int			split_by_space(char *str, t_list *words, int *i, int start);
	bool		add_last_str(char *str, t_list *words, int i, int start);
	t_list		*lexer(char *str);

	// parser/set_cmd_block.c
	t_redirects	*set_redirect_member(t_list **words);
	void		set_first_args(t_list **words, t_cmd_block *cmd, int arg_num);
	void		add_args(t_list **words, t_cmd_block *cmd, int arg_num);
	void		set_args(t_list **words, t_cmd_block *cmd);
	void		set_cmd_block(t_list **words, t_cmd_block *cmd);

	// parser/parser.c
	bool		is_valid_words(t_list *words);
	bool		is_redirect(t_list *words);
	void		set_tokens(t_list **tokens, t_cmd_block *cmd);
	t_list		*parser(t_list *words);

	// expansion/expansion.c
	t_list		*expansion(t_list *tokens, t_environ *env);
}
