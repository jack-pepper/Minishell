/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   yahya_ms_to_pipex.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-bouk <yel-bouk@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 18:07:30 by mmalie            #+#    #+#             */
/*   Updated: 2025/05/30 11:55:44 by yel-bouk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

bool	has_heredoc(t_pipeline *p)
{
	int	i;

	i = 0;
	while (i < p->cmd_count)
	{
		if (p->cmds[i].limiter)
			return (true);
		i++;
	}
	return (false);
}

bool	is_builtin(const char *cmd)
{
	if (!cmd)
		return (false);
	if (ft_strcmp(cmd, "cd") == 0
		|| ft_strcmp(cmd, "echo") == 0
		|| ft_strcmp(cmd, "env") == 0
		|| ft_strcmp(cmd, "pwd") == 0
		|| ft_strcmp(cmd, "export") == 0
		|| ft_strcmp(cmd, "unset") == 0
		|| ft_strcmp(cmd, "exit") == 0
		|| ft_strchr(cmd, '=') != NULL
		|| (cmd[0] == CC_VAR_TO_INTERPRET)
		|| ((cmd[0] == CC_VAR_TO_INTERPRET) && (cmd[1] == '?'))
		|| (cmd[0] == '$')
		|| ((cmd[0] == '$') && (cmd[1] == '?'))
	)
		return (true);
	return (false);
}

static t_pipeline	*init_pipeline(char **tokens)
{
	t_pipeline	*p;

	p = ft_calloc(1, sizeof(t_pipeline));
	if (!p)
		return (NULL);
	p->cmd_count = count_cmds(tokens);
	p->cmds = ft_calloc(p->cmd_count, sizeof(t_commands));
	if (!p->cmds)
	{
		free(p);
		return (NULL);
	}
	return (p);
}

t_pipeline	*build_pipeline_from_tokens(char **tokens)
{
	int			i;
	t_pipeline	*p;
	int			current_cmd;
	int			count;
	int			arg_count;

	i = 0;
	p = init_pipeline(tokens);
	if (!p)
		return (NULL);
	current_cmd = 0;
	while (tokens[i])
	{
		if (handle_redirection_tokens(tokens, &i, p, current_cmd))
			continue ;
		if (ft_strcmp(tokens[i], (char[]){CC_PIPE, '\0'}) == 0)
		{
			current_cmd++;
			i++;
			continue ;
		}
		if (!p->cmds[current_cmd].argv)
		{
			count = count_command_tokens(tokens + i, 0);
			if (count > 0)
			{
				p->cmds[current_cmd].argv = malloc(sizeof(char *) * (count
							+ 1));
				if (!p->cmds[current_cmd].argv)
					return (NULL);
				p->cmds[current_cmd].argv[0] = NULL;
			}
		}
		if (p->cmds[current_cmd].argv)
		{
			arg_count = 0;
			while (p->cmds[current_cmd].argv[arg_count])
				arg_count++;
			p->cmds[current_cmd].argv[arg_count] = ft_strdup(tokens[i]);
			p->cmds[current_cmd].argv[arg_count + 1] = NULL;
		}
		i++;
	}
	return (p);
}

void	free_pipeline(t_pipeline *p)
{
	int	i;
	int	j;

	i = 0;
	if (!p)
		return ;
	while (i < p->cmd_count)
	{
		if (p->cmds[i].argv)
		{
			j = 0;
			while (p->cmds[i].argv[j])
			{
				free(p->cmds[i].argv[j]);
				j++;
			}
			free(p->cmds[i].argv);
		}
		i++;
	}
}
