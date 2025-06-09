/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_cmd_cd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmalie <mmalie@student.42nice.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/24 00:03:54 by mmalie            #+#    #+#             */
/*   Updated: 2025/06/09 23:07:19 by mmalie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../inc/minishell.h"

int	cmd_cd(t_shell *sh)
{
	char	*cwd;
	char	*path;
	t_list	*home_var;
	int		res;

	home_var = ft_getenv("HOME", &sh->this_env);
	path = NULL;
	res = cd_set_path(sh, home_var, &path);
	if (res < 2)
		return (res);
	cwd = store_cwd(NULL);
	if (!cwd)
	{
		home_var = ft_getenv("PWD", &sh->this_env);
		if (home_var != NULL && ((char **)home_var->content)[1])
			cwd = ft_strdup(((char **)home_var->content)[1]);
		else
			return (ms_err("cd", "", PWD_NON_SET, 1));
	}
	if (cd_process_path(sh, cwd, path) != 0)
		return (1);
	free(cwd);
	return (0);
}

int	cd_process_path(t_shell *sh, char *cwd, char *path)
{
	char	*cur_path;

	if (path[0] == '.')
	{
		cur_path = handle_dotted_path(cwd, path);
		if (change_directory(sh, cwd, cur_path) != 0)
		{
			free(cur_path);
			free(cwd);
			return (1);
		}
		free(cur_path);
	}
	else
	{
		if (ft_strcmp(path, "-") == 0)
			path = ((char **)(ft_getenv("OLDPWD", &sh->this_env))
					->content)[1];
		if (change_directory(sh, cwd, path) != 0)
		{
			free(cwd);
			return (1);
		}
	}
	return (0);
}

int	change_directory(t_shell *sh, char *cwd, char *path)
{
	char	*trimmed;

	trimmed = NULL;
	if (!path)
	{
		sh->last_exit_status = ms_err("chdir", NO_CUR_DIR, NO_ACC_PAR, 0);
		return (0);
	}
	if (chdir(path) != 0)
		return (ms_err("cd: ", sh->input_args[1], NO_FILE_OR_DIR, 127));
	if (sh->input_args[1] && ft_strcmp(sh->input_args[1], "-") == 0)
		printf("%s\n", path);
	trimmed = store_cwd(trimmed);
	update_pwds_vars(sh, cwd, trimmed);
	free(trimmed);
	return (0);
}

void	update_pwds_vars(t_shell *sh, char *prev_cwd, char *new_pwd)
{
	char	*joined_pwd;
	char	*joined_old_pwd;
	char	**split_pwd;
	char	**split_old_pwd;

	joined_pwd = ft_strjoin("PWD=", new_pwd);
	if (!joined_pwd)
		return ;
	split_pwd = ft_split(joined_pwd, '=');
	free(joined_pwd);
	if (!split_pwd)
		return ;
	update_pwd_var(sh, split_pwd, "PWD");
	joined_old_pwd = ft_strjoin("OLDPWD=", prev_cwd);
	if (!joined_old_pwd)
		return ;
	split_old_pwd = ft_split(joined_old_pwd, '=');
	free(joined_old_pwd);
	if (!split_old_pwd)
		return ;
	update_pwd_var(sh, split_old_pwd, "OLDPWD");
}

void	update_pwd_var(t_shell *sh, char **split_pwd, char *key)
{
	t_list	*pwd_var;

	pwd_var = ft_getenv(key, &sh->this_env);
	if (pwd_var != NULL)
		ft_update_env_value(pwd_var, split_pwd);
	else
		add_new_env_var(sh, split_pwd);
}
