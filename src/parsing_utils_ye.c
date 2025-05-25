/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils_ye.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-bouk <yel-bouk@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 13:57:41 by yel-bouk          #+#    #+#             */
/*   Updated: 2025/05/25 13:58:06 by yel-bouk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	free_tokens(char **tokens)
{
	int	i;

	i = 0;
	while(tokens[i])
	{
		free(tokens[i]);
		i++;
	}
	free(tokens);
}

void	parse_and_build_pipeline(t_pipeline *pipeline, char **tokens)
{
	int			num_cmds;
	int			i;
	int			cmd_index;
	int			start;
	char		**cmd_tokens;

	i = 0;
	num_cmds = count_pipes(tokens) + 1;
	pipeline->cmd_count = num_cmds;
	pipeline->cmds = ft_calloc(num_cmds, sizeof(t_commands));
	if (!pipeline->cmds)
		exit(1);
	start = 0;
	cmd_index = 0;
	while (tokens[i])
	{
		if (ft_strcmp(tokens[i], (char[]){CC_PIPE, '\0'}) == 0)
		{
			if (cmd_index >= num_cmds)
				exit(1);
			cmd_tokens = extract_tokens(tokens, start, i);
			pipeline->cmds[cmd_index++] = parse_command(cmd_tokens);
			free_tokens(cmd_tokens);
			start = i + 1;
		}
		i++;
	}
	if (cmd_index >= num_cmds)
		exit(1);
	cmd_tokens = extract_tokens(tokens, start, i);
	pipeline->cmds[cmd_index] = parse_command(cmd_tokens);
	free_tokens(cmd_tokens);
}