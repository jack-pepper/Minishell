/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_cmd_cd_dotdot.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmalie <mmalie@student.42nice.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 23:18:31 by mmalie            #+#    #+#             */
/*   Updated: 2025/06/16 23:32:41 by mmalie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
		if (test_pwd_split(pwd_split, current_path, i) != 0)
			return (NULL);
		free(current_path);
	}
	cd_up_count++;
	if (cd_up_count >= deleted_dirs)
	{
		reset_count_and_free(&cd_up_count, &deleted_dirs,
			pwd_split, path_split);
		return (find_last_existing_dir(pwd, 0));
	}
	free_args(pwd_split);
	free_args(path_split);
	ft_printf("cd: error retrieving current directory: getcwd:"
		"cannot access parent directories: No such file or directory\n");
	return (ft_strjoin(pwd, "/.."));
}

int	test_pwd_split(char **pwd_split, char *current_path, int i)
{
	while (pwd_split[i])
	{
		if (!append_and_replace(&current_path, pwd_split[i]))
			return (1);
		if (access(current_path, F_OK) != 0)
			deleted_dirs++;
		if (!append_and_replace(&current_path, "/"))
			return (1);
		i++;
	}
	return (0);
}

void	reset_count_and_free(static int *cd_up_count, static int *deleted_dirs,
		char **pwd_split, char **path_split)
{
	cd_up_count = 0;
	deleted_dirs = 0;
	free_args(pwd_split);
	free_args(path_split);
}
