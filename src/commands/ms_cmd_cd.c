/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_cmd_cd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmalie <mmalie@student.42nice.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 12:59:33 by mmalie            #+#    #+#             */
/*   Updated: 2025/04/21 18:43:19 by mmalie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	cmd_cd(t_shell *sh)
{
	char	*path;
	//char	*cur_path;
	//char	**split_path;
	t_list	*home_var;
	
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
	if (path[0] == '/')
		change_directory(sh, path);
	//else if (path[0] == '.')
	//	{
	//		cur_path = set_cur_path();
	//		change_directory(sh, cur_path);
	//	}

	// - begins with slash => set curpath to the operand, then step 7
	// - first component is dot or dot-dot: step 6
	// else

	return (0);
}

int	change_directory(t_shell *sh, char *path)
{
	if (chdir(path) != 0)
	{
		printf("Error on chdir!\n");
		return (-1);
	}
	update_pwds(sh, path);
	return (0);
}

void	update_pwds(t_shell *sh, char *new_pwd) // or pass just the new path and join it inside this function?
{
	t_list	*pwd_var;
//	t_list	*oldpwd_var;
	char	*joined_pwd_var;
	char	**split_pwd_var;

	pwd_var = ft_getenv("PWD", &sh->this_env);
//	oldpwd_var = ft_getenv("OLDPWD", &sh->this_env);
	joined_pwd_var = ft_strjoin("PWD=", new_pwd);
	if (!joined_pwd_var)
		return ;
	split_pwd_var = ft_split(joined_pwd_var, '=');
	free(joined_pwd_var);
	if (!split_pwd_var)
		return ;
	if (pwd_var != NULL)
		ft_update_env_value(pwd_var, split_pwd_var);
	else
		add_new_env_var(sh, split_pwd_var);
//	free_args(split_pwd_var);
//	joined_pwd_var = ft_strjoin("OLDPWD=", )
//	if (oldpwd_var != NULL)
//	{
//		ft_update_env_value(oldpwd_var, split_pwd_var); // debug, to be updated
//	}
//	else
//		add_new_env_var(sh, split_pwd_var); // same
}
