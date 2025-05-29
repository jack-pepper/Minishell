/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_here_doc.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-bouk <yel-bouk@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/16 18:55:38 by yel-bouk          #+#    #+#             */
/*   Updated: 2025/05/29 10:56:22 by yel-bouk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	handle_heredoc(const char *limiter)
{
	char	*line;
	int		fd;

	line = NULL;
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
		write(fd, line, strlen(line));
		write(fd, "\n", 1);
		free(line);
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
		if (args[i])
		{
			strcat(result, args[i]);
			if (args[i + 1])
				strcat(result, " ");
		}
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
		total += strlen(args[i++]) + 1;
	return (total);
}