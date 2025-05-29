/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   yahya_parsing.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-bouk <yel-bouk@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 12:04:47 by yel-bouk          #+#    #+#             */
/*   Updated: 2025/05/29 15:29:45 by yel-bouk         ###   ########.fr       */
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
		if (is_token_control_char(tokens[i], CC_PIPE))
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
		if ((is_token_control_char(tokens[i], CC_REDIR_IN)
				|| is_token_control_char(tokens[i], CC_REDIR_OUT)
				|| is_token_control_char(tokens[i], CC_APPEND))
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
	while (i < len)
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
	if (!cmd.argv)
	{
		cmd.infile = NULL;
		cmd.outfile = NULL;
		return (cmd);
	}
	while (tokens[i])
	{
		if (is_token_control_char(tokens[i], CC_REDIR_IN) && tokens[i + 1])
		{
			test_fd = open(tokens[i + 1], O_RDONLY);
			if (test_fd < 0)
			{
				perror(tokens[i + 1]);
				free(cmd.argv);
				cmd.argv = NULL;
				return (cmd);
			}
			close(test_fd);
			free(cmd.infile);
			cmd.infile = ft_strdup(tokens[++i]);
		}
		else if (is_token_control_char(tokens[i], CC_REDIR_OUT)
			&& tokens[i + 1])
		{
			test_fd = open(tokens[i + 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
			if (test_fd < 0)
			{
				perror(tokens[i + 1]);
				free(cmd.argv);
				cmd.argv = NULL;
				return (cmd);
			}
			close(test_fd);
			free(last_outfile);
			last_outfile = ft_strdup(tokens[++i]);
			last_append = false;
		}
		else if (is_token_control_char(tokens[i], CC_APPEND)
			&& tokens[i + 1])
		{
			test_fd = open(tokens[i + 1], O_WRONLY | O_CREAT | O_APPEND, 0644);
			if (test_fd < 0)
			{
				perror(tokens[i + 1]);
				free(cmd.argv);
				cmd.argv = NULL;
				return (cmd);
			}
			close(test_fd);
			free(last_outfile);
			last_outfile = ft_strdup(tokens[++i]);
			last_append = true;
		}
		else if ((unsigned char)tokens[i][0] < 32 && tokens[i][1] == '\0')
			continue ;
		else
			cmd.argv[argc++] = ft_strdup(tokens[i]);
		i++;
	}
	if (last_outfile)
	{
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

	if (cmd->argv)
	{
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
