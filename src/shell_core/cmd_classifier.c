/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_classifier.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-bouk <yel-bouk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 18:07:30 by mmalie            #+#    #+#             */
/*   Updated: 2025/05/27 16:40:56 by yel-bouk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static int is_first_token_redirection(char **tokens)
{
	if (!tokens[0])
		return (0);
	return (
		ft_strcmp(tokens[0], (char[]){CC_REDIR_IN, '\0'}) == 0 ||
		ft_strcmp(tokens[0], (char[]){CC_REDIR_OUT, '\0'}) == 0 ||
		ft_strcmp(tokens[0], (char[]){CC_APPEND, '\0'}) == 0
	);
}
static int token_redirection(char **tokens, int i)
{
	if (!tokens[i])
		return (0);
	return (
		ft_strcmp(tokens[i], (char[]){CC_REDIR_IN, '\0'}) == 0 ||
		ft_strcmp(tokens[i], (char[]){CC_REDIR_OUT, '\0'}) == 0 ||
		ft_strcmp(tokens[i], (char[]){CC_APPEND, '\0'}) == 0
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

	i = 0;
	has_pipe = 0;
	has_redir = 0;
	here_doc = 0;
	has_cmd = 0;
	first_is_redir = 0;
		first_is_redir = is_first_token_redirection(tokens);
	while (tokens[i])
	{
		if (ft_strcmp(tokens[i], (char[]){CC_PIPE, '\0'}) == 0)
			has_pipe = 1;
		else if (ft_strcmp(tokens[i], (char[]){CC_HEREDOC, '\0'}) == 0)
			here_doc = 1;
		else if (token_redirection(tokens, i))
		{
			has_redir = 1;
			if (!tokens[i + 1])
				return (MIXED_INVALID);
			i++;
		}
		else if ((unsigned char)tokens[i][0] >= 32)
			has_cmd = 1;
		i++;
	}
	if(here_doc)
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
	if(has_cmd)
		return (BASIC);
	return (BASIC);
}