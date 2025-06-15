/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_here_doc_env.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-bouk <yel-bouk@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/14 10:22:18 by yel-bouk          #+#    #+#             */
/*   Updated: 2025/06/15 15:04:48 by mmalie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	open_heredoc_file(void)
{
	int	fd;

	fd = open(".heredoc_tmp", O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
		perror("open .heredoc_tmp");
	return (fd);
}

static int	get_var_bounds(const char *str, int start, int *end)
{
	int	i;

	i = start;
	if (!str[i] || (!ft_isalpha(str[i]) && str[i] != '_'))
		return (0);
	while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
		i++;
	*end = i;
	return (1);
}

static char	*replace_var(char *str, int start, int end, const char *value)
{
	char	*prefix;
	char	*suffix;
	char	*temp;
	char	*result;

	prefix = ft_substr(str, 0, start);
	suffix = ft_strdup(str + end);
	if (value)
		temp = ft_strjoin(prefix, value);
	else
		temp = ft_strjoin(prefix, "");
	result = ft_strjoin(temp, suffix);
	free(prefix);
	free(suffix);
	free(temp);
	free(str);
	return (result);
}

static char	*handle_env_var(char *str, int *index)
{
	int		start;
	int		end;
	char	*var_name;
	char	*var_value;
	char	*new_str;

	start = *index + 1;
	if (!get_var_bounds(str, start, &end))
		return (str);
	var_name = ft_substr(str, start, end - start);
	if (!var_name)
		return (str);
	var_value = getenv(var_name);
	free(var_name);
	new_str = replace_var(str, *index, end, var_value);
	if (var_value)
		*index += ft_strlen(var_value);
	else
		*index += 0;
	return (new_str);
}

char	*expand_env_vars(const char *line)
{
	char	*result;
	int		i;

	i = 0;
	result = ft_strdup(line);
	if (!result)
		return (NULL);
	while (result[i])
	{
		if (result[i] == '$')
		{
			if (result[i + 1] && (ft_isalnum(result[i + 1])
					|| result[i + 1] == '_'))
			{
				result = handle_env_var(result, &i);
				continue ;
			}
		}
		i++;
	}
	return (result);
}
