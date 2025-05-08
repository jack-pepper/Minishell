/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_cmd_cd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmalie <mmalie@student.42nice.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 12:59:33 by mmalie            #+#    #+#             */
/*   Updated: 2025/05/05 12:54:16 by mmalie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	cmd_cd(t_shell *sh)
{
	char	*cwd;
	char	*path;
	char	*cur_path;
	t_list	*home_var;

	cwd = NULL;
	home_var = ft_getenv("HOME", &sh->this_env);
	if (sh->input_args[2])
		return (ft_ret(1, CD_TOO_MANY_ARGS, STDERR));
	if (!sh->input_args[1] && (!home_var || !((char **)home_var->content)[1]))
		return (ft_ret(-1, CD_HOME_NON_SET, STDERR));
	else if (!sh->input_args[1] && home_var && ((char **)home_var->content)[1])
		path = ((char **)home_var->content)[1];
	else
		path = sh->input_args[1];
	cwd = store_cwd(cwd);
	// cd_process_path
	if (path[0] == '.')
	{
		cur_path = handle_dotted_path(cwd, path);
	//	printf("[DEBUG] [cmd_cd / cur_path] %s\n", cur_path);
		if (change_directory(sh, cwd, cur_path) != 0)
		{
			free(cur_path);
			free(cwd);
			return (-1);
		}
		free(cur_path);
	}
	else
	{
		if (ft_strcmp(path, "-") == 0)
			path = ((char **)(ft_getenv("OLDPWD", &sh->this_env))->content)[1];
		if (change_directory(sh, cwd, path) != 0)
		{
			free(cwd);
			return (1);
		}
	}
	free(cwd);
	return (0);
}

int	change_directory(t_shell *sh, char *cwd, char *path)
{
	if (chdir(path) != 0)
		return (ft_ret(1, CD_NO_FILE_OR_DIR, STDERR));
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
	update_pwd_var(sh, split_pwd);
	joined_old_pwd = ft_strjoin("OLDPWD=", prev_cwd);
	if (!joined_old_pwd)
		return ;
	split_old_pwd = ft_split(joined_old_pwd, '=');
	free(joined_old_pwd);
	if (!split_old_pwd)
		return ;
	update_old_pwd_var(sh, split_old_pwd);
}

void	update_pwd_var(t_shell *sh, char **split_pwd)
{
	t_list	*pwd_var;

	pwd_var = ft_getenv("PWD", &sh->this_env);
	if (pwd_var != NULL)
		ft_update_env_value(pwd_var, split_pwd);
	else
		add_new_env_var(sh, split_pwd);
}

void	update_old_pwd_var(t_shell *sh, char **split_old_pwd)
{
	t_list	*old_pwd_var;

	old_pwd_var = ft_getenv("OLDPWD", &sh->this_env);
	if (old_pwd_var != NULL)
		ft_update_env_value(old_pwd_var, split_old_pwd);
	else
		add_new_env_var(sh, split_old_pwd);
}
