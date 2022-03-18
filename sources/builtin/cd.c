#include "minishell.h"

bool	is_double_slash(char *command_path)
{
	if (ft_strncmp(command_path, "//", 2) == 0 && ft_strncmp(command_path, "///", 3) != 0)
		return (true);
	return (false);
}

bool	is_absolute(char *command_path)
{
	if (ft_strncmp(command_path, "/", 1) == 0)
		return (true);
	return (false);
}

// // export key=path(絶対パスに変換されたもの)を実行するためのcmd_blockを作る
t_cmd_block *update_block_data(char *key, char *path)
{
	t_cmd_block	*update_block;
	char		*tmp_str;
	char		*tmp_path;

	update_block = (t_cmd_block *)ft_xmalloc(sizeof(t_cmd_block) * 1, "ft_cd");
	update_block->command = ft_xstrdup("export", "ft_cd");
	tmp_str = ft_xstrjoin_with_free(key, ft_xstrdup("=", "ft_cd"), "ft_cd");
	update_block->args = (char **)ft_xmalloc(sizeof(char *) * 3, "ft_cd");
	update_block->args[0] = ft_xstrdup("export", "ft_cd");
	tmp_path = ft_xstrdup(path, "ft_cd");
	update_block->args[1] = ft_xstrjoin_with_free(tmp_str, tmp_path, "ft_cd");
	update_block->args[2] = NULL;
	update_block->redirects = NULL;
	return (update_block);
}

void	update_pwd(char *path, t_environ *env)
{
	t_cmd_block *update_pwd_block;
	t_cmd_block *update_oldpwd_block;
	char		*old_wd;
	char		*current_wd;

	old_wd = get_env_val("PWD", t_environ_to_vector(env));
	if (old_wd == NULL)
		old_wd = getcwd(NULL, 1024);
	update_oldpwd_block = update_block_data(ft_xstrdup("OLDPWD", "ft_cd"), old_wd);
	ft_export(update_oldpwd_block, env);
	current_wd = getcwd(NULL, 1024);
	if (is_double_slash(path) || (is_double_slash(old_wd) && !is_absolute(path)))
		current_wd = ft_xstrjoin_with_free(ft_xstrdup("/", "ft_cd"), current_wd, "ft_cd");
	update_pwd_block = update_block_data(ft_xstrdup("PWD", "ft_cd"), current_wd);
	ft_export(update_pwd_block, env);
	free(current_wd);
}

int	ft_cd(t_cmd_block *cmd_block, t_environ *env)
{
	char		*path;
	char		**envp;
	int			ret;
	char		*e_mes;

	if (cmd_block->args[1])
		path = cmd_block->args[1];
	else
	{
		envp = t_environ_to_vector(env);
		path = get_env_val("HOME", envp);
	}
	ret = chdir(path);
	if (ret == -1)
	{
		e_mes = ft_strjoin_wrapper("minishell: cd: ", path);
		perror(e_mes);
		free(e_mes);
		return (1);
	}
	update_pwd(path, env);
	return (0);
}
