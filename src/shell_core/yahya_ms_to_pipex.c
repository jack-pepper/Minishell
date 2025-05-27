/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   yahya_ms_to_pipex.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-bouk <yel-bouk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 18:07:30 by mmalie            #+#    #+#             */
/*   Updated: 2025/05/27 16:29:00 by yel-bouk         ###   ########.fr       */
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

char	*get_cmd_path(char *cmd, char **envp)
{
	char	*path_str;
	char	**paths;
	char	*full_path;

	if (ft_strchr(cmd, '/'))
	{
		if (access(cmd, X_OK) == 0)
			return (ft_strdup(cmd));
		else
			return (NULL);
	}
	path_str = get_path_from_env(envp);
	if (!path_str)
		return (NULL);
	paths = ft_split(path_str, ':');
	if (!paths)
		return (NULL);
	full_path = shell_find_cmd_path(cmd, paths);
	ft_free_array(paths, -1);
	return (full_path);
}

static int	open_redirection_fds(t_pipeline *cmd,
				int *in_fd, int *out_fd, t_shell *sh)
{
	int	flags;

	*in_fd = -1;
	*out_fd = -1;
	if (cmd->cmds->infile)
	{
		*in_fd = open(cmd->cmds->infile, O_RDONLY);
		if (*in_fd < 0)
		{
			sh->last_exit_status = 1;
			perror(cmd->cmds->infile);
			return (-1);
		}
	}
	if (cmd->cmds->outfile)
	{
		flags = O_WRONLY | O_CREAT;
		if (cmd->cmds->append)
			flags |= O_APPEND;
		else
			flags |= O_TRUNC;
		*out_fd = open(cmd->cmds->outfile, flags, 0644);
		if (*out_fd < 0)
		{
			sh->last_exit_status = 1;
			perror(cmd->cmds->outfile);
			if (*in_fd != -1)
				close(*in_fd);
			return (-1);
		}
	}
	return (0);
}

void	exec_with_redirection(t_pipeline *cmd, char **env, t_shell *sh)
{
	int			in_fd;
	int			out_fd;
	pid_t		pid;
	int			status;
	char		**argv;

	if (open_redirection_fds(cmd, &in_fd, &out_fd, sh) < 0)
	{
		printf("Invalid file\n");
		return ;
	}
	pid = fork();
	if (pid == 0)
	{
		setup_redirections(in_fd, out_fd);
		if (cmd->cmd_count < 1)
			exit(0);
		argv = cmd->cmds[0].argv;
		if (validate_and_exec_command(argv, env, sh))
		{
			exit(sh->last_exit_status);
		}
		if (strcmp(argv[0], "echo") == 0)
		{
			sh->input_args = argv;
			exit(cmd_echo(sh));
		}
		else
			execve(get_cmd_path(argv[0], env), argv, env);
		perror("execve failed");
		exit(EXIT_FAILURE);
	}
	status = 0;
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		sh->last_exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		sh->last_exit_status = 128 + WTERMSIG(status);
	if (in_fd != -1)
		close(in_fd);
	if (out_fd != -1)
		close(out_fd);
}

int	exec_builtin_in_child(char **argv, t_shell *sh)
{
	if (ft_strcmp(argv[0], "echo") == 0)
		return (cmd_echo_x(argv));
	else if (ft_strcmp(argv[0], "pwd") == 0)
		return (cmd_pwd());
	else if (ft_strcmp(argv[0], "env") == 0)
		return (cmd_env(sh));
	else if (argv[0][0] == CC_VAR_TO_INTERPRET
				&& argv[0][1] == '?' && !argv[0][2])
	{
		perror(" ");
		sh->last_exit_status = 1;
		return (0);
	}
	return (1);
}