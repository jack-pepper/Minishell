/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   yahya_parsing.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-bouk <yel-bouk@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 12:04:47 by yel-bouk          #+#    #+#             */
/*   Updated: 2025/05/25 13:43:39 by yel-bouk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	count_pipes(char **tokens)
{
	int	count;
	int	i;

	count = 0;
	i = 0;
	while (tokens[i])
	{
		if (ft_strcmp(tokens[i], (char[]){CC_PIPE, '\0'}) == 0)
			count++;
		i++;
	}
	return (count);
}

int	count_args(char **tokens)
{
	int	count;
	int	i;

	count = 0;
	i = 0;
	while (tokens[i])
	{
		if ((ft_strcmp(tokens[i], (char[]){CC_REDIR_IN, '\0'}) == 0
			|| ft_strcmp(tokens[i], (char[]){CC_REDIR_OUT, '\0'}) == 0
			|| ft_strcmp(tokens[i], (char[]){CC_APPEND, '\0'}) == 0)
			&& tokens[i + 1])
			i += 2;
		else if ((unsigned char)tokens[i][0] < 32 && tokens[i][1] == '\0')
			i++;
		else
		{
			count++;
			i++;
		}
	}
	return (count);
}

char	**extract_tokens(char **tokens, int start, int end)
{
	int			len;
	char		**result;
	int			i;

	i = 0;
	len = end - start;
	result = malloc(sizeof(char *) * (len + 1));
	while(i < len)
	{
		result[i] = ft_strdup(tokens[start + i]);
		i++;
	}
	result[len] = NULL;
	return (result);
}

t_commands	parse_command(char **tokens)
{
	t_commands	cmd;
	int			argc;
	char		*last_outfile;
	bool		last_append;
	int			test_fd;
	int			i;

	last_append = false;
	last_outfile = NULL;
	argc = 0;
	i = 0;
	memset(&cmd, 0, sizeof(t_commands));
	cmd.argv = malloc(sizeof(char *) * (count_args(tokens) + 1));
	if (!cmd.argv) {
		cmd.infile = NULL;
		cmd.outfile = NULL;
		return cmd;
	}
	while(tokens[i])
	{
		if (ft_strcmp(tokens[i],
			(char[]){CC_REDIR_IN, '\0'}) == 0 && tokens[i + 1])
		{
			test_fd = open(tokens[i + 1], O_RDONLY);
			if (test_fd < 0)
			{
				perror(tokens[i + 1]);
				free(cmd.argv);
				cmd.argv = NULL;
				return cmd;
			}
			close(test_fd);
			free(cmd.infile);
			cmd.infile = ft_strdup(tokens[++i]);
		}
		else if (ft_strcmp(tokens[i], (char[]){CC_REDIR_OUT, '\0'}) == 0 && tokens[i + 1])
		{
			test_fd = open(tokens[i + 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
			if (test_fd < 0) {
				perror(tokens[i + 1]);
				free(cmd.argv);
				cmd.argv = NULL;
				return cmd;
			}
			close(test_fd);
			free(last_outfile);
			last_outfile = ft_strdup(tokens[++i]);
			last_append = false;
		}
		else if (ft_strcmp(tokens[i], (char[]){CC_APPEND, '\0'}) == 0 && tokens[i + 1])
		{
			test_fd = open(tokens[i + 1], O_WRONLY | O_CREAT | O_APPEND, 0644);
			if (test_fd < 0)
			{
				perror(tokens[i + 1]);
				free(cmd.argv);
				cmd.argv = NULL;
				return cmd;
			}
			close(test_fd);
			free(last_outfile);
			last_outfile = ft_strdup(tokens[++i]);
			last_append = true;
		}
		else if ((unsigned char)tokens[i][0] < 32 && tokens[i][1] == '\0')
			continue;
		else
			cmd.argv[argc++] = ft_strdup(tokens[i]);
		i++;
	}
	if (last_outfile) {
		free(cmd.outfile);
		cmd.outfile = last_outfile;
		cmd.append = last_append;
	}
	cmd.argv[argc] = NULL;
	return (cmd);
}

void	free_command(t_commands *cmd)
{
	int	i;
	
	if (cmd->argv) {
		i = 0;
		while (cmd->argv[i])
		{
			free(cmd->argv[i]);
			i++;
		}
		free(cmd->argv);
	}
	free(cmd->infile);
	free(cmd->outfile);
}

void	free_tokens(char **tokens)
{
	int i;

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
	int num_cmds;
	int i;
	int cmd_index;
	int start;
	char **cmd_tokens;
	
	i = 0;
	num_cmds = count_pipes(tokens) + 1;
	pipeline->cmd_count = num_cmds;
	pipeline->cmds = ft_calloc(num_cmds, sizeof(t_commands));
	if (!pipeline->cmds)
		exit(1);
	start = 0;
	cmd_index = 0;
	while (tokens[i]) {
		if (ft_strcmp(tokens[i], (char[]){CC_PIPE, '\0'}) == 0) {
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
