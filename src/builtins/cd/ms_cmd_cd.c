/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_cmd_cd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-bouk <yel-bouk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/24 00:03:54 by mmalie            #+#    #+#             */
/*   Updated: 2025/06/15 19:44:20 by yel-bouk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../inc/minishell.h"

char	*join_parts(char **parts)
{
	char	*res;
	char	*tmp;
	char	*tmp2;
	int		i;

	res = ft_strdup("/");
	if (!res)
		return (NULL);
	i = 0;
	while (parts[i])
	{
		tmp = ft_strjoin(res, parts[i]);
		free(res);
		if (parts[i + 1])
			tmp2 = ft_strjoin(tmp, "/");
		else
			tmp2 = ft_strdup(tmp);
		free(tmp);
		res = tmp2;
		i++;
	}
	return (res);
}

char	*apply_logical_cd(char *pwd, char *cd_path)
{
	char	**pwd_split;
	char	**path_split;
	char	**result;
	int		i;
	int		j;

	pwd_split = ft_split(pwd, '/');
	path_split = ft_split(cd_path, '/');
	result = malloc(sizeof(char *) * 1024);
	if (!pwd_split || !path_split || !result)
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
	free_args(pwd_split);
	free_args(path_split);
	return (join_parts(result));
}

int	cmd_cd(t_shell *sh)
{
	char	*cwd;
	char	*path;
	t_list	*home_var;
	int		res;
	char	*user_path;
	t_list	*pwd_var;

	printf("hey [1]\n");
	home_var = ft_getenv("HOME", &sh->this_env);
	path = NULL;
	res = cd_set_path(sh, home_var, &path);
	if (res < 2)
		return (res);
	user_path = ft_strdup(path);
	cwd = store_cwd(NULL);
	if (!cwd)
	{
		printf("I am here [2]\n");
		pwd_var = ft_getenv("PWD", &sh->this_env);
		if (pwd_var != NULL && ((char **)pwd_var->content)[1])
		{
			printf("I amhere [3]\n");
			cwd = ft_strdup(((char **)pwd_var->content)[1]);
		}
		if (!cwd)
			return (ms_err("cd", "", PWD_NON_SET, 1));
	}
	if (cd_process_path(sh, cwd, path, user_path) != 0)
	{
		printf("I am here 4\n");
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
	char	*cur_path;

	if (path[0] == '.')
	{
		char *logical_path;

		// Try normal way
		cur_path = handle_dotted_path(cwd, path);
		if (chdir(cur_path) == 0)
		{
			update_pwds_vars(sh, cwd, cur_path);
			free(cur_path);
			return (0);
		}

		// Fallback: simulate logical cd .. behavior
		logical_path = resolve_logical_path(cwd, user_path);
		if (!logical_path)
		{
			free(cur_path);
			return (1);
		}
		update_pwds_vars(sh, cwd, logical_path);
		free(logical_path);
		free(cur_path);
		return (0);
	}
	else
	{
		if (ft_strcmp(path, "-") == 0 )
		{
			t_list *oldpwd_var = ft_getenv("OLDPWD", &sh->this_env);
			if (!oldpwd_var)
				return ms_err("cd", "", OLDPWD_NON_SET, 1);
			else
			{
				path = ((char **)(ft_getenv("OLDPWD", &sh->this_env))
						->content)[1];
			}
		}
		if (change_directory(sh, cwd, path, user_path) != 0)
		{
			printf("heey\n");
			free(cwd);
			return (1);
		}
	}
	return (0);
}

int	change_directory(t_shell *sh, char *cwd, char *path, char *user_path)
{
	char	*trimmed;
	char	*logical_pwd;

	printf("Hello \n");
	if (!path)
	{
		sh->last_exit_status = ms_err("chdir", NO_CUR_DIR, NO_ACC_PAR, 0);
		return (0);
	}

	printf("[change_dir] Attempting to chdir to: %s\n", path);
	if (chdir(path) != 0)
	{
		printf("[change_dir] chdir failed. Attempting fallback using logical_cd.\n");

		if (!cwd || !user_path)
		{
			if(!getcwd(NULL, 0))
			{
				ft_strlcat(path, cwd, ft_strlen(".."));
			}
			return (ms_err("cd: ", sh->input_args[1], NO_FILE_OR_DIR, 127));
		}

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
		printf("[change_dir] getcwd failed after chdir. Fallback to logical.\n");
		logical_pwd = apply_logical_cd(cwd, user_path);
		if (!logical_pwd)
			return (1);
		update_pwds_vars(sh, cwd, logical_pwd);
		printf("[logical_cd] Logical fallback path: %s\n", logical_pwd);
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
