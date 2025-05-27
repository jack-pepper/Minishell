/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_hand_red.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-bouk <yel-bouk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 18:07:30 by mmalie            #+#    #+#             */
/*   Updated: 2025/05/27 16:10:17 by yel-bouk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	setup_redirections(int in_fd, int out_fd)
{
	if (in_fd != -1)
	{
		dup2(in_fd, STDIN_FILENO);
		close(in_fd);
	}
	if (out_fd != -1)
	{
		dup2(out_fd, STDOUT_FILENO);
		close(out_fd);
	}
}

bool	handle_redirection_tokens(char **tokens, int *i,
	t_pipeline *p, int cmd_index)
{
if (strcmp(tokens[*i], (char[]){CC_HEREDOC, '\0'}) == 0)
{
	if (tokens[*i + 1])
	{
		p->cmds[cmd_index].infile = ft_strdup("here_doc");
		p->cmds[cmd_index].limiter = ft_strdup(tokens[++(*i)]);
		p->cmds[cmd_index].append = false;
		(*i)++;
		return (true);
	}
	return (false);
}
if (strcmp(tokens[*i], (char[]){CC_REDIR_IN, '\0'}) == 0)
{
	if (tokens[*i + 1])
	{
		p->cmds[cmd_index].infile = ft_strdup(tokens[++(*i)]);
		(*i)++;
		return (true);
	}
	return (false);
}
if (strcmp(tokens[*i], (char[]){CC_REDIR_OUT, '\0'}) == 0)
{
	if (tokens[*i + 1])
	{
		p->cmds[cmd_index].outfile = ft_strdup(tokens[++(*i)]);
		p->cmds[cmd_index].append = false;
		(*i)++;
		return (true);
	}
	return (false);
}
if (strcmp(tokens[*i], (char[]){CC_APPEND, '\0'}) == 0)
{
	if (tokens[*i + 1])
	{
		p->cmds[cmd_index].outfile = ft_strdup(tokens[++(*i)]);
		p->cmds[cmd_index].append = true;
		(*i)++;
		return (true);
	}
	return (false);
}
return (false);
}

t_pipeline	*parse_redirection_only(char **tokens)
{
	t_pipeline *p = ft_calloc(1, sizeof(t_pipeline));
	t_commands *cmd = ft_calloc(1, sizeof(t_commands));
	p->cmds = cmd;
	char **argv = ft_calloc(128, sizeof(char *));

	if (!p || !cmd || !argv)
		return (NULL);

	int i = 0;
	int arg_i = 0;

	while (tokens[i])
	{
		if (ft_strcmp(tokens[i], (char[]){CC_REDIR_IN, '\0'}) == 0)
		{
			if (tokens[i + 1])
				p->cmds->infile = ft_strdup(tokens[++i]);
			else
			{
				perror("Error: missing infile\n");
				free(cmd); free(argv); free(p);
				return (NULL);
			}
		}
		else if (ft_strcmp(tokens[i], (char[]){CC_APPEND, '\0'}) == 0)
		{
			if (tokens[i + 1])
				p->cmds->outfile = ft_strdup(tokens[++i]), p->cmds->append = true;
			else
			{
				perror("Error: missing outfile\n");
				free(cmd); free(argv); free(p);
				return (NULL);
			}
		}
		else if (ft_strcmp(tokens[i], (char[]){CC_REDIR_OUT, '\0'}) == 0)
		{
			if (tokens[i + 1])
				p->cmds->outfile = ft_strdup(tokens[++i]), p->cmds->append = false;
			else
			{
				perror("Error: missing outfile\n");
				free(cmd); free(argv); free(p);
				return (NULL);
			}
		}
		else
		{
			argv[arg_i++] = ft_strdup(tokens[i]);
		}
		i++;
	}
	if (arg_i == 0)
	{
		perror("Error: no command found\n");
		free(cmd); free(argv); free(p);
		return (NULL);
	}
	argv[arg_i] = NULL;
	cmd->argv = argv;
	p->cmds = cmd;
	p->cmd_count = 1;
	return (p);
}

