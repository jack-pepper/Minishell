/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_here_doc.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-bouk <yel-bouk@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/16 18:55:38 by yel-bouk          #+#    #+#             */
/*   Updated: 2025/06/14 06:08:48 by yel-bouk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static char	*expand_env_vars(const char *line)
{
	char	*result;
	char	*var_name;
	char	*var_value;
	int		i;
	int		j;

	result = ft_strdup(line);
	if (!result)
		return (NULL);
	i = 0;
	while (result[i])
	{
		if (result[i] == '$' && result[i + 1] && result[i + 1] != ' ')
		{
			j = i + 1;
			while (result[j] && (ft_isalnum(result[j]) || result[j] == '_'))
				j++;
			var_name = ft_substr(result, i + 1, j - i - 1);
			if (!var_name)
			{
				free(result);
				return (NULL);
			}
			var_value = getenv(var_name);
			free(var_name);
			if (var_value)
			{
				char	*temp = ft_strjoin(ft_substr(result, 0, i), var_value);
				if (!temp)
				{
					free(result);
					return (NULL);
				}
				char	*new_result = ft_strjoin(temp, result + j);
				free(temp);
				free(result);
				if (!new_result)
					return (NULL);
				result = new_result;
				i = ft_strlen(var_value) - 1;
			}
			else
			{
				char	*temp = ft_strjoin(ft_substr(result, 0, i), result + j);
				free(result);
				if (!temp)
					return (NULL);
				result = temp;
				i--;
			}
		}
		i++;
	}
	return (result);
}

int	handle_heredoc(const char *limiter)
{
	char	*line;
	char	*expanded;
	int		fd;

	line = NULL;
	expanded = NULL;
	fd = open(".heredoc_tmp", O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
	{
		perror("open .heredoc_tmp");
		return (1);
	}
	while (1)
	{
		line = readline("> ");
		if (!line || strcmp(line, limiter) == 0)
			break ;
		expanded = expand_env_vars(line);
		if (!expanded)
		{
			free(line);
			close(fd);
			return (1);
		}
		write(fd, expanded, strlen(expanded));
		write(fd, "\n", 1);
		free(line);
		free(expanded);
	}
	free(line);
	close(fd);
	return (0);
}

char	*join_args(char **args)
{
	int		i;
	size_t	total_len;
	char	*result;

	if (!args || !args[0])
		return (strdup(""));
	i = 0;
	total_len = total_args_length(args);
	if (total_len == 0)
		return (strdup(""));
	if (!allocate_mem(&result, total_len))
		return (NULL);
	result[0] = '\0';
	i = 0;
	while (args[i])
	{
		ft_strlcat(result, args[i], total_len + 1);
		if (args[i + 1])
			ft_strlcat(result, " ", total_len + 1);
		i++;
	}
	return (result);
}

int	allocate_mem(char **str, int length)
{
	*str = malloc(length);
	if (!*str)
		return (0);
	return (1);
}

size_t	total_args_length(char **args)
{
	size_t	total;
	int		i;

	i = 0;
	total = 0;
	while (args[i])
		total += ft_strlen(args[i++]) + 1;
	return (total);
}

void	init_pipex(t_pipex *pipex, char **envp)
{
	memset(pipex, 0, sizeof(t_pipex));
	pipex->envp = envp;
	pipex->here_doc = true;
	pipex->in_fd = -1;
	pipex->out_fd = -1;
}
