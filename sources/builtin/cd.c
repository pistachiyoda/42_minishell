/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmai <fmai@student.42tokyo.jp>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/21 23:03:40 by fmai              #+#    #+#             */
/*   Updated: 2022/04/01 15:28:36 by fmai             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_cmd_block	*update_block_data(char *key, char *path)
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

void	update_pwd_envs(char *key, char *wd, t_environ *env)
{
	t_cmd_block	*update_cmd_data;

	update_cmd_data = update_block_data(
			ft_xstrdup(key, "ft_cd"), wd);
	ft_export(update_cmd_data, env);
	free(update_cmd_data->command);
	free_cmd_block_after_exec(update_cmd_data);
}

int	free_str_and_return(char *str)
{
	free(str);
	return (1);
}

int	update_pwd(char *path, t_environ *env)
{
	char		*old_wd;
	char		*current_wd;
	char		**envp;

	envp = t_environ_to_vector(env);
	old_wd = get_env_val("PWD", envp);
	free_2d_array(envp);
	if (old_wd == NULL)
	{
		old_wd = getcwd(NULL, 1024);
		if (old_wd == NULL)
			return (1);
	}
	update_pwd_envs("OLDPWD", old_wd, env);
	current_wd = getcwd(NULL, 1024);
	if (current_wd == NULL)
		return (free_str_and_return(old_wd));
	if (is_double_slash(path)
		|| (is_double_slash(old_wd) && !is_absolute(path)))
		current_wd = ft_xstrjoin_with_free(
				ft_xstrdup("/", "ft_cd"), current_wd, "ft_cd");
	update_pwd_envs("PWD", current_wd, env);
	free(old_wd);
	free(current_wd);
	return (0);
}

int	ft_cd(t_cmd_block *cmd_block, t_environ *env)
{
	char		*path;
	char		**envp;

	if (cmd_block->args[1])
		path = cmd_block->args[1];
	else
	{
		envp = t_environ_to_vector(env);
		path = get_env_val("HOME", envp);
	}
	if (chdir_wrapper(path) != 0)
		return (1);
	if (update_pwd(path, env) != 0)
		return (1);
	if (!cmd_block->args[1])
	{
		free(path);
		free_2d_array(envp);
	}
	return (0);
}
