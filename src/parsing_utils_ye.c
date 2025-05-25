/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils_ye.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-bouk <yel-bouk@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 13:57:41 by yel-bouk          #+#    #+#             */
/*   Updated: 2025/05/25 15:10:11 by yel-bouk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

#include "../inc/minishell.h"

void	free_tokens(char **tokens)
{
	int	i;

	i = 0;
	while (tokens[i])
	{
		free(tokens[i]);
		i++;
	}
	free(tokens);
}

static void	process_command_segment(t_pipeline *pipeline, char **tokens,
		int *cmd_index, int start, int i)
{
	char	**cmd_tokens;

	cmd_tokens = extract_tokens(tokens, start, i);
	pipeline->cmds[*cmd_index] = parse_command(cmd_tokens);
	(*cmd_index)++;
	free_tokens(cmd_tokens);
}

void	parse_and_build_pipeline(t_pipeline *pipeline, char **tokens)
{
	int		num_cmds;
	int		i;
	int		cmd_index;
	int		start;
	char	pipe_token[2];

	pipe_token[0] = CC_PIPE;
	pipe_token[1] = '\0';
	i = 0;
	start = 0;
	cmd_index = 0;
	num_cmds = count_pipes(tokens) + 1;
	pipeline->cmd_count = num_cmds;
	pipeline->cmds = ft_calloc(num_cmds, sizeof(t_commands));
	if (!pipeline->cmds)
		exit(1);
	while (tokens[i])
	{
		if (ft_strcmp(tokens[i], pipe_token) == 0)
		{
			if (cmd_index >= num_cmds)
				exit(1);
			process_command_segment(pipeline, tokens, &cmd_index, start, i);
			start = i + 1;
		}
		i++;
	}
	if (cmd_index >= num_cmds)
		exit(1);
	process_command_segment(pipeline, tokens, &cmd_index, start, i);
}
