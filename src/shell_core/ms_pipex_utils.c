/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_pipex_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-bouk <yel-bouk@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 12:57:41 by mmalie            #+#    #+#             */
/*   Updated: 2025/05/28 12:33:32 by yel-bouk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

char	**extract_command_args(char **tokens, int *i, int count)
{
	char	**argv;
	int		j;

	j = 0;
	argv = malloc(sizeof(char *) * (count + 1));
	if (!argv)
		return (NULL);
	while (tokens[*i]
		&& ft_strcmp(tokens[*i], (char[]){CC_PIPE, '\0'}) != 0
		&& ft_strcmp(tokens[*i], (char[]){CC_REDIR_IN, '\0'}) != 0
		&& ft_strcmp(tokens[*i], (char[]){CC_REDIR_OUT, '\0'}) != 0
		&& ft_strcmp(tokens[*i], (char[]){CC_APPEND, '\0'}) != 0)
	{
		if ((ft_strcmp(tokens[*i], (char[]){CC_REDIR_IN, '\0'}) == 0
		|| ft_strcmp(tokens[*i], (char[]){CC_REDIR_OUT, '\0'}) == 0
		|| ft_strcmp(tokens[*i], (char[]){CC_APPEND, '\0'}) == 0)
		&& tokens[*i + 1])
		{
			*i +=2;
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

	while (tokens[*i] && ft_strcmp(tokens[*i], (char[]){CC_PIPE, '\0'}) == 0)
		(*i)++;
	count = count_command_tokens(tokens, *i);
	if (count == 0)
		return ;
	p->cmds[*cmd_i].argv = extract_command_args(tokens, i, count);
	if (!p->cmds[*cmd_i].argv)
		return ;
	(*cmd_i)++;
	if (tokens[*i] && ft_strcmp(tokens[*i], (char[]){CC_PIPE, '\0'}) == 0)
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
		if (ft_strcmp(tokens[i], (char[]){CC_PIPE, '\0'}) == 0)
			in_cmd = 0;
		else if (ft_strcmp(tokens[i], (char[]){CC_REDIR_IN, '\0'}) == 0
				|| ft_strcmp(tokens[i], (char[]){CC_REDIR_OUT, '\0'}) == 0
				|| ft_strcmp(tokens[i], (char[]){CC_APPEND, '\0'}) == 0)
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
		&& ft_strcmp(tokens[start], (char[]){CC_PIPE, '\0'}) != 0)
	{
		if ((ft_strcmp(tokens[start], (char[]){CC_REDIR_IN, '\0'}) == 0
		|| ft_strcmp(tokens[start], (char[]){CC_REDIR_OUT, '\0'}) == 0
		|| ft_strcmp(tokens[start], (char[]){CC_APPEND, '\0'}) == 0) &&
		tokens[start + 1])
		{
			start += 2;
			continue ;
		}
		count++;
		start++;
	}
	return (count);
}
