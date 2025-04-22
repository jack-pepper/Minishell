/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_cmd_cd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmalie <mmalie@student.42nice.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 12:59:33 by mmalie            #+#    #+#             */
/*   Updated: 2025/04/22 12:05:24 by mmalie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	cmd_cd(t_shell *sh)
{
	char	*cwd;
	char	*path;
	//char	*cur_path;
	//char	**split_path;
	t_list	*home_var;
	
	cwd = NULL;
	home_var = ft_getenv("HOME", &sh->this_env);
	if (!sh->input_args[1] && (!home_var || !((char **)home_var->content)[1]))
	{		
		printf("bash: cd: HOME non set\n");
		return (-1);
	}
	else if (!sh->input_args[1] && home_var && ((char **)home_var->content)[1])
		path = ((char **)home_var->content)[1];
	else
		path = sh->input_args[1];	
	cwd = store_cwd(cwd);
	if (path[0] == '/')
		change_directory(sh, cwd, path);
	//else if (path[0] == '.')
	//	{
	//		cur_path = set_cur_path();
	//		change_directory(sh, cur_path);
	//	}

	// - begins with slash => set curpath to the operand, then step 7
	// - first component is dot or dot-dot: step 6
	// else
	free(cwd);
	return (0);
}

int	change_directory(t_shell *sh, char *cwd, char *path)
{
	if (chdir(path) != 0)
	{
		printf("Error on chdir!\n");
		return (-1);
	}
	update_pwds_vars(sh, cwd, path);
	return (0);
}

void	update_pwds_vars(t_shell *sh, char *prev_cwd, char *new_pwd) // or pass just the new path and join it inside this function?
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
	joined_old_pwd = ft_strjoin("OLDPWD=", prev_cwd); // MODIFY TO CUR PWD!
	if (!joined_old_pwd)
		return ;
	split_old_pwd = ft_split(joined_old_pwd, '=');
	free(joined_old_pwd);
	if (!split_old_pwd)
		return ;
	update_old_pwd_var(sh, split_old_pwd);
}

void	update_pwd_var(t_shell *sh, char **split_pwd) // or pass just the new path and join it inside this function?
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
