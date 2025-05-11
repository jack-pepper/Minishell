/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_cmd_cd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmalie <mmalie@student.42nice.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 12:59:33 by mmalie            #+#    #+#             */
/*   Updated: 2025/05/11 21:28:07 by mmalie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../inc/minishell.h"

int	cmd_cd(t_shell *sh)
{
	char	*cwd;
	char	*path;
	t_list	*home_var;

	cwd = NULL;
	home_var = ft_getenv("HOME", &sh->this_env);
	if (!sh->input_args[1]
		&& (!home_var || !((char **)home_var->content)[1]))
		return (ms_err("cd", "", HOME_NON_SET, 1));
	else if (!sh->input_args[1] && home_var
		&& ((char **)home_var->content)[1])
		path = ((char **)home_var->content)[1];
	else if (sh->input_args[2])
		return (ms_err("cd", "", TOO_MANY_ARGS, 1));
	else
		path = sh->input_args[1];
	cwd = store_cwd(cwd);
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
	if (chdir(path) != 0)
		return (ms_err("cd: ", path, NO_FILE_OR_DIR, 1));
	update_pwds_vars(sh, cwd, path);
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
