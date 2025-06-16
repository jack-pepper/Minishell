/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_cmd_cd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-bouk <yel-bouk@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/24 00:03:54 by mmalie            #+#    #+#             */
/*   Updated: 2025/06/16 13:35:02 by yel-bouk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../inc/minishell.h"

static int	handle_cd_path_failure(char *user_path)
{
	free(user_path);
	return (1);
}

static char	*fallback_to_pwd_if_cwd_fails(t_shell *sh)
{
	t_list	*pwd_var;
	char	*cwd;

	pwd_var = ft_getenv("PWD", &sh->this_env);
	if (pwd_var != NULL && ((char **)pwd_var->content)[1])
		cwd = ft_strdup(((char **)pwd_var->content)[1]);
	else
		cwd = NULL;
	return (cwd);
}

int	cmd_cd(t_shell *sh)
{
	char	*cwd;
	char	*path;
	t_list	*home_var;
	int		res;
	char	*user_path;

	home_var = ft_getenv("HOME", &sh->this_env);
	path = NULL;
	res = cd_set_path(sh, home_var, &path);
	if (res < 2)
		return (res);
	user_path = ft_strdup(path);
	cwd = store_cwd(NULL);
	if (!cwd)
	{
		cwd = fallback_to_pwd_if_cwd_fails(sh);
		if (!cwd)
			return (ms_err("cd", "", PWD_NON_SET, 1));
	}
	if (cd_process_path(sh, cwd, path, user_path) != 0)
		return (handle_cd_path_failure(user_path));
	free(user_path);
	free(cwd);
	return (0);
}

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

static int	append_and_replace(char **dst, char *suffix)
{
	char	*temp;

	temp = ft_strjoin(*dst, suffix);
	if (!temp)
		return (0);
	free(*dst);
	*dst = temp;
	return (1);
}

static void	update_if_exists(char **last_existing, char *current)
{
	if (access(current, F_OK) == 0)
	{
		free(*last_existing);
		*last_existing = ft_strdup(current);
	}
}

char	*find_last_existing_dir(char *path)
{
	char	**path_parts;
	char	*current_path;
	char	*last_existing;
	int		i;

	i = 0;
	last_existing = NULL;
	current_path = ft_strdup("/");
	path_parts = ft_split(path, '/');
	if (!path_parts || !current_path)
	{
		free_args(path_parts);
		free(current_path);
		return (NULL);
	}
	while (path_parts[i])
	{
		if (!append_and_replace(&current_path, path_parts[i]))
			break ;
		update_if_exists(&last_existing, current_path);
		if (!append_and_replace(&current_path, "/"))
			break ;
		i++;
	}
	free(current_path);
	free_args(path_parts);
	return (last_existing);
}

static char	*merge_pwd_and_path(char **pwd_split, char **path_split)
{
	char	**result;
	char	*final_result;
	int		i;
	int		j;

	result = malloc(sizeof(char *) * 1024);
	if (!result)
		return (NULL);
	i = 0;
	while (pwd_split[i])
	{
		result[i] = ft_strdup(pwd_split[i]);
		i++;
	}
	j = 0;
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
	return (final_result);
}

static char	*handle_cd_dotdot_case(char *pwd,
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
		return (find_last_existing_dir(pwd));
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
	result = merge_pwd_and_path(pwd_split, path_split);
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

static int	try_logical_cd(t_shell *sh, char *cwd, char *user_path)
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

static int	handle_relative_cd(t_shell *sh, char *cwd,
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

int	cd_process_path(t_shell *sh, char *cwd, char *path, char *user_path)
{
	t_list	*oldpwd_var;

	if (path[0] == '.')
		return (handle_relative_cd(sh, cwd, path, user_path));
	if (ft_strcmp(path, "-") == 0)
	{
		oldpwd_var = ft_getenv("OLDPWD", &sh->this_env);
		if (!oldpwd_var)
			return (ms_err("cd", "", OLDPWD_NON_SET, 1));
		path = ((char **)oldpwd_var->content)[1];
	}
	return (change_directory(sh, cwd, path, user_path));
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
