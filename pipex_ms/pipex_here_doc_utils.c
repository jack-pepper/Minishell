/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_here_doc_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-bouk <yel-bouk@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/14 22:03:51 by mmalie            #+#    #+#             */
/*   Updated: 2025/06/17 04:58:36 by yel-bouk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	free_line_and_close_fd(char *line, int fd)
{
	free(line);
	close(fd);
	return ;
}

int	heredoc_loop(char *line, const char *limiter, int fd, int exit_status)
{
	char	*expanded;

	line = readline("> ");
	if (!line)
		return (2);
	else if (strcmp(line, limiter) == 0)
	{
		free(line);
		return (2);
	}
	expanded = expand_env_vars(line, exit_status);
	if (!expanded)
	{
		free_line_and_close_fd(line, fd);
		return (1);
	}
	write(fd, expanded, strlen(expanded));
	write(fd, "\n", 1);
	ft_free_two_str(line, expanded);
	return (0);
}
