/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_classifier.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-bouk <yel-bouk@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 18:07:30 by mmalie            #+#    #+#             */
/*   Updated: 2025/06/01 14:12:29 by yel-bouk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static int	is_first_token_redirection(char **tokens)
{
	if (!tokens[0])
		return (0);
	return (
		is_token_control_char(tokens[0], CC_REDIR_IN)
		|| is_token_control_char(tokens[0], CC_REDIR_OUT)
		|| is_token_control_char(tokens[0], CC_APPEND)
	);
}

static int	token_redirection(char **tokens, int i)
{
	if (!tokens[i])
		return (0);
	return (
		is_token_control_char(tokens[i], CC_REDIR_IN)
		|| is_token_control_char(tokens[i], CC_REDIR_OUT)
		|| is_token_control_char(tokens[i], CC_APPEND)
	);
}

t_cmd_type	classify_command(char **tokens)
{
	int	i;
	int	has_pipe;
	int	has_redir;
	int	here_doc;
	int	has_cmd;
	int	first_is_redir;
	int	consecutive_redirs;

	i = 0;
	has_pipe = 0;
	has_redir = 0;
	here_doc = 0;
	has_cmd = 0;
	first_is_redir = 0;
	consecutive_redirs = 0;
	first_is_redir = is_first_token_redirection(tokens);
	while (tokens[i])
	{
		if (is_token_control_char(tokens[i], CC_PIPE))
		{
			has_pipe = 1;
			if (!tokens[i + 1])
				return (MIXED_INVALID);
		}
		else if (is_token_control_char(tokens[i], CC_HEREDOC))
		{
			here_doc = 1;
			if (!tokens[i + 1])
				return (MIXED_INVALID);
			i++;
		}
		else if (token_redirection(tokens, i))
		{
			has_redir = 1;
			if (!tokens[i + 1])
				return (MIXED_INVALID);
			if (token_redirection(tokens, i + 1))
				consecutive_redirs = 1;
			i++;
		}
		else if ((unsigned char)tokens[i][0] >= 32)
		{
			has_cmd = 1;
			consecutive_redirs = 0;
		}
		i++;
	}
	if (consecutive_redirs)
		return (MIXED_INVALID);
	if (here_doc)
		return (HERE_DOC);
	if (has_pipe && has_redir)
		return (PIPELINE_WITH_RED);
	if (has_pipe)
		return (PIPELINE);
	if (has_redir)
	{
		if (first_is_redir)
			return (BASIC);
		return (REDIR_ONLY);
	}
	if (has_cmd)
		return (BASIC);
	return (BASIC);
}

char	*shell_find_cmd_path(char *cmd, char **paths)
{
	int		i;
	char	*temp;
	char	*full;

	i = 0;
	while (paths[i])
	{
		temp = ft_strjoin(paths[i], "/");
		if (!temp)
			return (NULL);
		full = ft_strjoin(temp, cmd);
		free(temp);
		if (!full)
			return (NULL);
		if (access(full, X_OK) == 0)
			return (full);
		free(full);
		i++;
	}
	return (NULL);
}
