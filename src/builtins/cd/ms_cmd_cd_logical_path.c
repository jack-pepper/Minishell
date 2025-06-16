/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_cmd_cd_logical_path.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-bouk <yel-bouk@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/24 00:03:54 by mmalie            #+#    #+#             */
/*   Updated: 2025/06/16 22:51:24 by mmalie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../inc/minishell.h"

char	*handle_cd_dotdot_case(char *pwd,
			char **pwd_split, char **path_split)
{
	static int	cd_up_count;
	static int	deleted_dirs;
	char		*current_path;
	int			i;

	i = 0;
	if (cd_up_count == 0)
	{
		current_path = ft_strdup("/");
		while (pwd_split[i])
		{
			if (!append_and_replace(&current_path, pwd_split[i]))
				return (NULL);
			if (access(current_path, F_OK) != 0)
				deleted_dirs++;
			if (!append_and_replace(&current_path, "/"))
				return (NULL);
			i++;
		}
		free(current_path);
	}
	cd_up_count++;
	if (cd_up_count >= deleted_dirs)
	{
		cd_up_count = 0;
		deleted_dirs = 0;
		free_args(pwd_split);
		free_args(path_split);
		return (find_last_existing_dir(pwd, 0));
	}
	free_args(pwd_split);
	free_args(path_split);
	ft_printf("cd: error retrieving current directory: getcwd:"
		"cannot access parent directories: No such file or directory\n");
	return (ft_strjoin(pwd, "/.."));
}

char	*apply_logical_cd(char *pwd, char *cd_path)
{
	char	**pwd_split;
	char	**path_split;
	char	*result;

	pwd_split = ft_split(pwd, '/');
	path_split = ft_split(cd_path, '/');
	if (!pwd_split || !path_split)
		return (NULL);
	if (ft_strcmp(cd_path, "..") == 0)
	{
		result = handle_cd_dotdot_case(pwd, pwd_split, path_split);
		return (result);
	}
	result = merge_pwd_and_path(pwd_split, path_split, 0, 0);
	free_args(pwd_split);
	free_args(path_split);
	return (result);
}

char	*resolve_logical_path(char *cwd, char *user_path)
{
	if (!cwd || !user_path)
		return (NULL);
	return (apply_logical_cd(cwd, user_path));
}

int	try_logical_cd(t_shell *sh, char *cwd, char *user_path)
{
	char	*logical_path;
	char	*new_real;
	char	*simulated;

	logical_path = resolve_logical_path(cwd, user_path);
	if (!logical_path)
		return (1);
	if (chdir(logical_path) != 0)
	{
		simulated = ft_strjoin(cwd, "/..");
		if (!simulated)
		{
			free(logical_path);
			return (1);
		}
		update_pwds_vars(sh, cwd, simulated);
		free(simulated);
		free(logical_path);
		return (0);
	}
	new_real = store_cwd(NULL);
	if (!new_real)
	{
		simulated = ft_strjoin(cwd, "/..");
		update_pwds_vars(sh, cwd, simulated);
		free(simulated);
	}
	else
	{
		update_pwds_vars(sh, cwd, new_real);
		free(new_real);
	}
	free(logical_path);
	return (0);
}

int	handle_relative_cd(t_shell *sh, char *cwd,
		char *path, char *user_path)
{
	char	*cur_path;
	char	*real_path;

	cur_path = handle_dotted_path(cwd, path);
	if (cur_path && chdir(cur_path) == 0)
	{
		real_path = store_cwd(NULL);
		if (real_path)
		{
			update_pwds_vars(sh, cwd, real_path);
			free(real_path);
		}
		else
			update_pwds_vars(sh, cwd, cur_path);
		free(cur_path);
		return (0);
	}
	free(cur_path);
	return (try_logical_cd(sh, cwd, user_path));
}
