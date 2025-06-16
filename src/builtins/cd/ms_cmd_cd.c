/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_cmd_cd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-bouk <yel-bouk@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/24 00:03:54 by mmalie            #+#    #+#             */
/*   Updated: 2025/06/16 11:20:50 by yel-bouk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../inc/minishell.h"

char	*join_parts(char **parts)
{
	char	*res;
	char	*tmp;
	char	*tmp2;
	int		i;

	i = 0;
	res = ft_strdup("/");
	if (!res)
		return (NULL);
	while (parts[i])
	{
		tmp = ft_strjoin(res, parts[i]);
		free(res);
		if (!tmp)
			return (NULL);
		if (parts[i + 1])
			tmp2 = ft_strjoin(tmp, "/");
		else
			tmp2 = ft_strdup(tmp);
		free(tmp);
		if (!tmp2)
			return (NULL);
		res = tmp2;
		i++;
	}
	return (res);
}

char	*find_last_existing_dir(char *path)
{
	char	**path_parts;
	char	*current_path;
	char	*last_existing = NULL;
	int		i;
	char	*temp;

	path_parts = ft_split(path, '/');
	if (!path_parts)
		return (NULL);
	current_path = ft_strdup("/");
	if (!current_path)
	{
		free_args(path_parts);
		return (NULL);
	}
	i = 0;
	while (path_parts[i])
	{
		temp = ft_strjoin(current_path, path_parts[i]);
		free(current_path);
		if (!temp)
		{
			free_args(path_parts);
			return (last_existing);
		}
		current_path = temp;
		if (access(current_path, F_OK) == 0)
		{
			if (last_existing)
				free(last_existing);
			last_existing = ft_strdup(current_path);
		}
		temp = ft_strjoin(current_path, "/");
		free(current_path);
		if (!temp)
		{
			free_args(path_parts);
			return (last_existing);
		}
		current_path = temp;
		i++;
	}
	free(current_path);
	free_args(path_parts);
	return (last_existing);
}

char	*apply_logical_cd(char *pwd, char *cd_path)
{
	char	**pwd_split;
	pwd_split = ft_split(pwd, '/');
	char	**path_split = ft_split(cd_path, '/');
	char	**result;
	char	*final_result;
	int		i = 0, j = 0;
	static int	cd_up_count = 0;
	static int	deleted_dirs = 0;

	if (!pwd_split || !path_split)
		return (NULL);
	if (ft_strcmp(cd_path, "..") == 0)
	{
		if (cd_up_count == 0)
		{
			char *current_path = ft_strdup("/");
			if (!current_path)
			{
				free_args(pwd_split);
				free_args(path_split);
				return (NULL);
			}
			i = 0;
			while (pwd_split[i])
			{
				char *temp = ft_strjoin(current_path, pwd_split[i]);
				free(current_path);
				if (!temp)
				{
					free_args(pwd_split);
					free_args(path_split);
					return (NULL);
				}
				current_path = temp;

				if (access(current_path, F_OK) != 0)
					deleted_dirs++;

				temp = ft_strjoin(current_path, "/");
				free(current_path);
				if (!temp)
				{
					free_args(pwd_split);
					free_args(path_split);
					return (NULL);
				}
				current_path = temp;
				i++;
			}
			free(current_path);
		}
		cd_up_count++;
		if (cd_up_count >= deleted_dirs)
		{
			cd_up_count = 0;
			deleted_dirs = 0;
			char *last_existing = find_last_existing_dir(pwd);
			free_args(pwd_split);
			free_args(path_split);
			if (last_existing)
				return (last_existing);
			return (ft_strdup("/"));
		}
		free_args(pwd_split);
		free_args(path_split);
		ft_printf("cd: error retrieving current directory: getcwd: cannot access parent directories: No such file or directory\n");
		return (ft_strjoin(pwd, "/.."));
	}
	result = malloc(sizeof(char *) * 1024);
	if (!result)
	{
		free_args(pwd_split);
		free_args(path_split);
		return (NULL);
	}
	while (pwd_split[i])
	{
		result[i] = ft_strdup(pwd_split[i]);
		i++;
	}
	while (path_split[j])
	{
		if (!ft_strcmp(path_split[j], "..") && i > 0)
			i--;
		else if (ft_strcmp(path_split[j], ".") && path_split[j][0])
			result[i++] = ft_strdup(path_split[j]);
		j++;
	}
	result[i] = NULL;
	final_result = join_parts(result);
	free_args(result);
	free_args(pwd_split);
	free_args(path_split);
	return (final_result);
}

int	cmd_cd(t_shell *sh)
{
	char	*cwd;
	char	*path;
	t_list	*home_var;
	int		res;
	char	*user_path;
	t_list	*pwd_var;

	home_var = ft_getenv("HOME", &sh->this_env);
	path = NULL;
	res = cd_set_path(sh, home_var, &path);
	if (res < 2)
		return (res);
	user_path = ft_strdup(path);
	cwd = store_cwd(NULL);
	if (!cwd)
	{
		pwd_var = ft_getenv("PWD", &sh->this_env);
		if (pwd_var != NULL && ((char **)pwd_var->content)[1])
			cwd = ft_strdup(((char **)pwd_var->content)[1]);
		if (!cwd)
			return (ms_err("cd", "", PWD_NON_SET, 1));
	}
	if (cd_process_path(sh, cwd, path, user_path) != 0)
	{
		free(user_path);
		return (1);
	}
	free(user_path);
	free(cwd);
	return (0);
}

char *resolve_logical_path(char *cwd, char *user_path)
{
	if (!cwd || !user_path)
		return (NULL);
	return apply_logical_cd(cwd, user_path);
}

int	cd_process_path(t_shell *sh, char *cwd, char *path, char *user_path)
{
	char			*cur_path;
	char			*logical_path;

	cur_path = NULL;
	logical_path = NULL;
	if (path[0] == '.')
	{
		cur_path = handle_dotted_path(cwd, path);
		if (cur_path && chdir(cur_path) == 0)
		{
			char *real_path = store_cwd(NULL);
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
		logical_path = resolve_logical_path(cwd, user_path);
		if (!logical_path)
			return (1);
		if (chdir(logical_path) != 0)
		{
			char *simulated = ft_strjoin(cwd, "/..");
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
		char *new_real = store_cwd(NULL);
		if (!new_real)
		{
			char *simulated = ft_strjoin(cwd, "/..");
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
	else
	{
		if (ft_strcmp(path, "-") == 0)
		{
			t_list *oldpwd_var = ft_getenv("OLDPWD", &sh->this_env);
			if (!oldpwd_var)
				return ms_err("cd", "", OLDPWD_NON_SET, 1);
			path = ((char **)oldpwd_var->content)[1];
		}
		return (change_directory(sh, cwd, path, user_path));
	}
}

int	change_directory(t_shell *sh, char *cwd, char *path, char *user_path)
{
	char	*trimmed;
	char	*logical_pwd;
	t_list	*home_var;

	if (!path)
	{
		home_var = ft_getenv("HOME", &sh->this_env);
		if (!home_var)
			return (ms_err("cd", "", HOME_NON_SET, 1));
		path = ((char **)home_var->content)[1];
	}
	if (access(path, F_OK) != 0)
		return (ms_err("cd", path, NO_FILE_OR_DIR, 1));
	if (chdir(path) != 0)
	{
		if (!cwd || !user_path)
			return (ms_err("cd: ", sh->input_args[1], NO_FILE_OR_DIR, 127));
		logical_pwd = apply_logical_cd(cwd, user_path);
		if (!logical_pwd)
			return (1);
		update_pwds_vars(sh, cwd, logical_pwd);
		free(logical_pwd);
		return (0);
	}
	trimmed = store_cwd(NULL);
	if (!trimmed)
	{
		logical_pwd = apply_logical_cd(cwd, user_path);
		if (!logical_pwd)
			return (1);
		update_pwds_vars(sh, cwd, logical_pwd);
		free(logical_pwd);
		return (0);
	}
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
