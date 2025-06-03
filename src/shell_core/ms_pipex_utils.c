/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_pipex_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-bouk <yel-bouk@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 12:57:41 by mmalie            #+#    #+#             */
/*   Updated: 2025/06/03 21:01:19 by mmalie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	is_token_control_char(char *token, char ctrl_char)
{
	char	*ctrl_char_str;
	char	*res;
	
	ctrl_char_str = ft_chrtostr(ctrl_char);
	res = ft_strcmp(token, ctrl_char_str);
	free(ctrl_char_str);
	return (res == 0);
}

int	control_token(char *token)
{
	return (is_token_control_char(token, CC_PIPE)
		|| is_token_control_char(token, CC_REDIR_IN)
		|| is_token_control_char(token, CC_REDIR_OUT)
		|| is_token_control_char(token, CC_APPEND));
}

char	**extract_command_args(char **tokens, int *i, int count)
{
	char	**argv;
	int		j;

	j = 0;
	argv = malloc(sizeof(char *) * (count + 1));
	if (!argv)
		return (NULL);
	while (tokens[*i] && !control_token(tokens[*i]))
	{
		if (control_token(tokens[*i])
			&& tokens[*i + 1])
		{
			*i += 2;
			continue ;
		}
		argv[j++] = ft_strdup(tokens[*i]);
		(*i)++;
	}
	argv[j] = NULL;
	return (argv);
}

void	parse_next_command(char **tokens, int *i, t_pipeline *p, int *cmd_i)
{
	int	count;

	while (tokens[*i] && is_token_control_char(tokens[*i], CC_PIPE))
		(*i)++;
	count = count_command_tokens(tokens, *i);
	if (count == 0)
		return ;
	p->cmds[*cmd_i].argv = extract_command_args(tokens, i, count);
	if (!p->cmds[*cmd_i].argv)
		return ;
	(*cmd_i)++;
	if (tokens[*i] && is_token_control_char(tokens[*i], CC_PIPE))
		(*i)++;
}

int	count_cmds(char **tokens)
{
	int	i;
	int	count;
	int	in_cmd;

	in_cmd = 0;
	count = 0;
	i = 0;
	while (tokens[i])
	{
		if (is_token_control_char(tokens[i], CC_PIPE))
			in_cmd = 0;
		else if (control_token(tokens[i]))
		{
			if (tokens[i + 1])
				i++;
			in_cmd = 0;
		}
		else
		{
			if (!in_cmd)
			{
				count++;
				in_cmd = 1;
			}
		}
		i++;
	}
	return (count);
}

int	count_command_tokens(char **tokens, int start)
{
	int	count;

	count = 0;
	while (tokens[start]
		&& !is_token_control_char(tokens[start], CC_PIPE))
	{
		if ((is_token_control_char(tokens[start], CC_REDIR_IN)
				|| is_token_control_char(tokens[start], CC_REDIR_OUT)
				|| is_token_control_char(tokens[start], CC_APPEND))
			&& tokens[start + 1])
		{
			start += 2;
			continue ;
		}
		count++;
		start++;
	}
	return (count);
}
