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
	}	t_redirects;

	typedef struct s_cmd_block
	{
		char	*command;
		char	**args;
		t_list	*redirects;
	}	t_cmd_block;

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

	// utils/print_error.c
	void	print_error(char *target, char *message);

	// exec_command_line/exec_command_line.c
	int		exec_command_line(t_list *cmd_list, char **envp);

	// runner/run_builtin_command.c
	bool	is_fork_required(t_list *cmd_list);
	int		run_builtin_command(t_cmd_block *cmd_block);

	// [後々削除]debug/debug_funcs.c
	void	print_cmd_lst(t_list *cmd_lst);

	// [後々削除]tmp_parser/tmp_parse_data.c 
	void	tmp_parse_data(t_list **cmd_lst, char *str);
}
