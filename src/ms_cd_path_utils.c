/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_cd_path_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmalie <mmalie@student.42nice.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 11:48:09 by mmalie            #+#    #+#             */
/*   Updated: 2025/05/08 23:39:23 by mmalie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

char	*handle_dotted_path(char *cwd, char *path)
{
	char	*joined_path;
	char	**split_path;
	char	*rejoined_path;

	joined_path = NULL;
	split_path = NULL;
	rejoined_path = NULL;
	joined_path = get_abs_path(joined_path, cwd, path);
	if (!joined_path)
		return (NULL);
	split_path = split_abs_path(split_path, joined_path);
	if (!split_path)
		return (NULL);
	flag_dotted_path(split_path, CTRL_CHAR_TO_BE_DELETED);
	rejoined_path = rejoin_abs_path(rejoined_path, split_path);
	free_args(split_path);
	if (!rejoined_path)
		return (NULL);
	return (rejoined_path);
}

// Form absolute path from cwd and path and store to joined_path
char	*get_abs_path(char *joined_path, char *cwd, char *path)
{
	char	*temp;

	temp = ft_strjoin("/", path);
	if (!temp)
		return (NULL);
	joined_path = ft_strjoin(cwd, temp);
	free(temp);
	if (!joined_path)
		return (NULL);
	return (joined_path);
}

// Split joined_path (absolute path) to split_path
char	**split_abs_path(char **split_path, char *joined_path)
{
	split_path = ft_split(joined_path, '/');
	free(joined_path);
	if (!split_path)
		return (NULL);
	return (split_path);
}

// Flag dotted strs (./..) and directories to be removed in path
void	flag_dotted_path(char **split_path, char ctrl_char)
{
	int	i;
	int	j;

	i = 0;
	while (split_path[i] != NULL)
	{
		if (ft_strcmp(split_path[i], ".") == 0)
			split_path[i][0] = ctrl_char;
		else if (ft_strcmp(split_path[i], "..") == 0)
		{
			j = i;
			while (split_path[j] && ft_strcmp(split_path[j], "..") == 0)
			{
				i--;
				if (i >= 0 && split_path[i] != NULL)
					split_path[i][0] = ctrl_char;
				split_path[j][0] = ctrl_char;
				j++;
			}
			i = j - 1;
		}
		i++;
	}
}

char	*rejoin_abs_path(char *rejoined_path, char **split_path)
{
	char	*temp;
	char	*prev_rejoined_path;
	int		i;
	int		j;

	rejoined_path = ft_strdup("");
	if (!rejoined_path)
		return (NULL);
	ft_init_two_ints(0, &i, &j);
	while (split_path[i])
	{
		if (split_path[i][0] != CTRL_CHAR_TO_BE_DELETED)
		{
			prev_rejoined_path = rejoined_path;
			temp = ft_strjoin("/", split_path[i]);
			rejoined_path = ft_strjoin(rejoined_path, temp);
			if (!rejoined_path)
				return (NULL);
			free(temp);
			free(prev_rejoined_path);
			j++;
		}
		i++;
	}
	return (rejoined_path);
}
