/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_handle_basic_pipe.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-bouk <yel-bouk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 18:07:30 by mmalie            #+#    #+#             */
/*   Updated: 2025/05/27 16:28:56 by yel-bouk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"


int	cmd_echo_x(char **argv)
{
	int	i;

	i = 1;
	while (argv[i])
	{
		printf("%s", argv[i]);
		if (argv[i + 1])
			printf(" ");
		i++;
	}
	printf("\n");
	return (0);
}

static bool	validate_pipeline_commands(t_pipeline *p, t_shell *sh)
{
	int			i;
	char		**argv;
	struct stat	st;

	i = 0;
	while (i < p->cmd_count)
	{
		argv = p->cmds[i].argv;
		if (!argv || !argv[0])
			continue ;
		if (strcmp(argv[0], "cd") == 0)
		{
			if (!argv[1])
			{
				fprintf(stderr, "cd: missing argument\n");
				sh->last_exit_status = 1;
				return (false);
			}
			if (stat(argv[1], &st) != 0)
			{
				fprintf(stderr, "cd: %s: No such file or directory\n", argv[1]);
				sh->last_exit_status = 1;
				return (false);
			}
			if (!S_ISDIR(st.st_mode))
			{
				fprintf(stderr, "cd: %s: Not a directory\n", argv[1]);
				sh->last_exit_status = 1;
				return (false);
			}
		}
		i++;
	}
	return (true);
}

void	run_pipeline_basic_pipeline(t_pipeline *p, char **env, t_shell *sh)
{
	int		j;
	char	*cmd_path;
	int		i;
	int		prev_fd;
	int		pipe_fd[2];
	pid_t	pid;
	int		dev_null;
	int		status;

	prev_fd = -1;
	if (!validate_pipeline_commands(p, sh))
		return ;
	j = 0;
	while (j < p->cmd_count)
	{
		if (!is_builtin(p->cmds[j].argv[0]))
		{
			cmd_path = get_cmd_path(p->cmds[j].argv[0], env);
			if (!cmd_path)
			{
				ft_printf("command not found\n");
				sh->last_exit_status = 127;
				return ;
			}
			free(cmd_path);
		}
		j++;
	}
	i = 0;
	while (i < p->cmd_count)
	{
		if (i < p->cmd_count - 1)
		{
			if (pipe(pipe_fd) < 0)
			{
				perror("pipe");
				exit(EXIT_FAILURE);
			}
		}
		pid = fork();
		if (pid == 0)
		{
			if (strcmp(p->cmds[i].argv[0], "cat") == 0 && !p->cmds[i].argv[1])
			{
				signal(SIGPIPE, SIG_IGN);
				if (i < p->cmd_count - 1)
				{
					dev_null = open("/dev/null", O_WRONLY);
					if (dev_null != -1)
					{
						dup2(dev_null, STDOUT_FILENO);
						dup2(dev_null, STDERR_FILENO);
						close(dev_null);
					}
				}
			}
			if (prev_fd != -1)
			{
				dup2(prev_fd, STDIN_FILENO);
				close(prev_fd);
			}
			if (i < p->cmd_count - 1)
			{
				close(pipe_fd[0]);
				dup2(pipe_fd[1], STDOUT_FILENO);
				close(pipe_fd[1]);
			}
			if (is_builtin(p->cmds[i].argv[0]))
				exit(exec_builtin_in_child(p->cmds[i].argv, sh));
			cmd_path = get_cmd_path(p->cmds[i].argv[0], env);
			if (!cmd_path)
				exit(127);
			execve(cmd_path, p->cmds[i].argv, env);
			perror("execve failed");
			exit(EXIT_FAILURE);
		}
		if (prev_fd != -1)
			close(prev_fd);
		if (i < p->cmd_count - 1)
		{
			close(pipe_fd[1]);
			prev_fd = pipe_fd[0];
		}
		i++;
	}
	while (wait(&status) > 0)
	{
		if (WIFEXITED(status))
			sh->last_exit_status = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			sh->last_exit_status = 128 + WTERMSIG(status);
	}
}
