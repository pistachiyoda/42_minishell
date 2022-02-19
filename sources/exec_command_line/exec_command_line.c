#include "minishell.h"

// <の処理
void	handle_input(char *filename, bool is_last)
{
	int fd;

	if (!(is_readable(filename)))
		exit(1);
	fd = open_file(filename);
	if (!is_last)
		return ;
	if (dup2(fd, 0) == -1)
		printf("dup2()"); // exit処理
}

// >, >>の処理
void	handle_output(char *filename, enum e_REDIRECTS	redirect, bool is_last)
{
	int fd;

	if (!(is_writable(filename)))
		exit(0);
	if (redirect == WRITE)
		fd = open_or_create_file(filename, O_WRONLY | O_CREAT | O_TRUNC);
	if (redirect == APPEND)
		fd = open_or_create_file(filename, O_APPEND | O_WRONLY | O_CREAT);
	if (fd == -1)
		exit(1);
	if (!is_last)
		return ;
	if (dup2(fd, 1) == -1)
		exit(1);
}

bool	is_last_input_redirect(t_list *redirects, t_list *node)
{
	t_list		*current_node;
	t_list		*last_input_node;
	t_redirects	*current_redirect;
	
	current_node = redirects;
	last_input_node = NULL;
	while (1)
	{
		current_redirect = current_node->content;
		if (current_redirect->redirect == INPUT || current_redirect->redirect == HEREDOC)
			last_input_node = current_node;
		if (!current_node->next)
			break;
		current_node = current_node->next;
	}
	return (last_input_node == node);
}

bool	is_last_output_redirect(t_list *redirects, t_list *node)
{
	t_list		*current_node;
	t_list		*last_output_node;
	t_redirects	*current_redirect;
	
	current_node = redirects;
	last_output_node = NULL;
	while (1)
	{
		current_redirect = current_node->content;
		if (current_redirect->redirect == APPEND || current_redirect->redirect == WRITE)
			last_output_node = current_node;
		if (!current_node->next)
			break;
		current_node = current_node->next;
	}
	return (last_output_node == node);
}

char	*ft_strjoin2(char const *s1, char const *s2)
{
	size_t	len;
	char	*str;
	char	*ret;

	len = ft_strlen((char *)s1) + ft_strlen((char *)s2);
	str = (char *)malloc(sizeof(char) * (len + 2));
	if (str == NULL)
		return (NULL);
	ret = str;
	while (*s1 != '\0')
		*str++ = *s1++;
	while (*s2 != '\0')
		*str++ = *s2++;
	*str++ = '\n';
	*str = '\0';
	return (ret);
}

void	handle_heredoc(char *limiter, bool is_last, int	pipe_fds[2])
{
	char	*buf;
	char	*str;
	char	*tmp;
	int		pid;

	pid = fork();
	str = ft_strdup("");
	if (pid == 0)
	{
		while (1)
		{
			buf = readline("> ");
			if ((ft_strlen(buf) == ft_strlen(limiter)) &&
				ft_strncmp(buf, limiter, ft_strlen(limiter)) == 0)
					break ;
			tmp = str;
			str = ft_strjoin2(str, buf);
			free(buf);
			free(tmp);
		}
		if (!is_last)
		{
			free(str);
			close(pipe_fds[0]);
			close(pipe_fds[1]);
			exit(0);
		}
		tmp = str;
		close(pipe_fds[0]);
		write(pipe_fds[1], str, ft_strlen(str));
		close(pipe_fds[1]);
		free(tmp);
		exit(0);
	}
	waitpid(pid, NULL, 0);
}

int		handle_redirect(t_cmd_block *cmd_block, int	pipe_fds[2])
{
	t_list		*redirect_node;
	t_redirects	*redirect;

	redirect_node = cmd_block->redirects;
	if (!redirect_node)
		return (0);
	while (1)
	{
		redirect = redirect_node->content;
		if (redirect->redirect == HEREDOC)
			handle_heredoc(redirect->target, is_last_input_redirect(cmd_block->redirects, redirect_node), pipe_fds);
		if (redirect_node->next == NULL)
			break ;
		redirect_node = redirect_node->next;
	}
	redirect_node = cmd_block->redirects;
	while (1)
	{
		redirect = redirect_node->content;
		if (redirect->redirect == INPUT)
			handle_input(redirect->target, is_last_input_redirect(cmd_block->redirects, redirect_node));
		if (redirect->redirect == WRITE || redirect->redirect == APPEND)
			handle_output(redirect->target, redirect->redirect, is_last_output_redirect(cmd_block->redirects, redirect_node));
		if (redirect_node->next == NULL)
			break;
		redirect_node = redirect_node->next;
	}
	return (0);
}

bool	is_last_heredoc(t_list *redirects)
{
	t_list		*current_node;
	t_list		*last_input_node;
	t_redirects	*current_redirect;

	if (!redirects)
		return (false);
	current_node = redirects;
	last_input_node = NULL;
	while (1)
	{
		current_redirect = current_node->content;
		if (current_redirect->redirect == INPUT || current_redirect->redirect == HEREDOC)
			last_input_node = current_node;
		if (!current_node->next)
			break;
		current_node = current_node->next;
	}
	if (!last_input_node)
		return false;
	return (((t_redirects *)last_input_node->content)->redirect == HEREDOC);
}

int	exec_command_line(t_list *cmd_list, char **envp)
{
	int	pid;
	int	status;
	int	pipe_fds[2];

	if (pipe(pipe_fds) == -1)
	{
		perror("pipe()");
		exit(1);
	}

	handle_redirect((t_cmd_block *)cmd_list->content, pipe_fds);
	pid = fork();
	if (pid == 0)
	{
		close(pipe_fds[1]);
		if (is_last_heredoc(((t_cmd_block *)cmd_list->content)->redirects))
			dup2(pipe_fds[0], 0);
		close(pipe_fds[0]);
		exec_command(((t_cmd_block *)cmd_list->content)->command,
			((t_cmd_block *)cmd_list->content)->args, envp);
		exit(1);
	}
	close(pipe_fds[0]);
	close(pipe_fds[1]);
	waitpid(pid, &status, 0);
	return (1);
}
