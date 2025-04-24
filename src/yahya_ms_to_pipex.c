#include "../inc/minishell.h"

bool is_builtin(const char *cmd)
{
	return (
		ft_strcmp(cmd, "cd") == 0 ||
		ft_strcmp(cmd, "echo") == 0 ||
		ft_strcmp(cmd, "env") == 0 ||
		ft_strcmp(cmd, "pwd") == 0 ||
		ft_strcmp(cmd, "export") == 0 ||
		ft_strcmp(cmd, "unset") == 0 ||
		ft_strcmp(cmd, "exit") == 0 ||
		((cmd[0] == CTRL_CHAR_VAR_TO_INTERPRET) &&
		(cmd[1] == '?') && (!cmd[2]))
		//ft_strcmp(cmd, "$?") == 0
	);
}


int count_command_tokens(char **tokens, int start) {
	int count = 0;
	while (tokens[start] &&
	       ft_strcmp(tokens[start], "|") != 0)
	{
		// skip redirection tokens AND their target
		if ((ft_strcmp(tokens[start], "<") == 0 ||
		     ft_strcmp(tokens[start], ">") == 0 ||
		     ft_strcmp(tokens[start], ">>") == 0) &&
		    tokens[start + 1])
		{
			start += 2;
			continue;
		}
		count++;
		start++;
	}
	return count;
}


char **extract_command_args(char **tokens, int *i, int count) {
	char **argv = malloc(sizeof(char *) * (count + 1));
	int j = 0;

	if (!argv)
		return NULL;

	while (tokens[*i] &&
		ft_strcmp(tokens[*i], "|") != 0 &&
		ft_strcmp(tokens[*i], "<") != 0 &&
		ft_strcmp(tokens[*i], ">") != 0 &&
		ft_strcmp(tokens[*i], ">>") != 0)
	{
					// If it's a redirection token, skip its target
		if ((ft_strcmp(tokens[*i], "<") == 0 || ft_strcmp(tokens[*i], ">") == 0 || ft_strcmp(tokens[*i], ">>") == 0) && tokens[*i + 1])
		{
			*i += 2;
			continue;
		}

		argv[j++] = ft_strdup(tokens[*i]);
		(*i)++;
	}
	argv[j] = NULL;
	return argv;
}

void parse_next_command(char **tokens, int *i, t_pipeline *p, int *cmd_i) {
	int count = count_command_tokens(tokens, *i);

	if (count == 0)
		return;

	p->cmds[*cmd_i].argv = extract_command_args(tokens, i, count);
	if (!p->cmds[*cmd_i].argv)
		return;

	// Skip the pipe symbol
	if (tokens[*i] && ft_strcmp(tokens[*i], "|") == 0)
		(*i)++;

	(*cmd_i)++;
}

int count_cmds(char **tokens)
{
	int i = 0;
	int count = 0;
	int in_cmd = 0;

	while (tokens[i])
	{
		if (strcmp(tokens[i], "|") == 0)
			in_cmd = 0;
		else if (ft_strcmp(tokens[i], "<") == 0 ||
		         ft_strcmp(tokens[i], ">") == 0 ||
		         ft_strcmp(tokens[i], ">>") == 0)
		{
			if (tokens[i + 1])
				i++; // skip file
			in_cmd = 0;
		}
		else if (!in_cmd)
		{
			count++;
			in_cmd = 1;
		}
		i++;
	}
	return count;
}

static t_pipeline *init_pipeline(char **tokens) {
	t_pipeline *p = ft_calloc(1, sizeof(t_pipeline));
	if (!p)
		return NULL;

	p->cmd_count = count_cmds(tokens);
	p->cmds = malloc(sizeof(t_command) * p->cmd_count);
	if (!p->cmds) {
		free(p);
		return NULL;
	}
	return p;
}

static bool handle_redirection_tokens(char **tokens, int *i, t_pipeline *p) {
	if (strcmp(tokens[*i], "<<") == 0) {
		if (tokens[*i + 1]) {
			p->infile = ft_strdup("here_doc"); // This is the key translation
			p->limiter = ft_strdup(tokens[++(*i)]); // Store the LIMITER
			p->append = false;
			(*i)++;
			return true;
		}
		return false;
	}
	
	if (strcmp(tokens[*i], "<") == 0) {
		if (tokens[*i + 1]) {
			p->infile = ft_strdup(tokens[++(*i)]);
			(*i)++;
			return true;
		}
		return false;
	}
	if (strcmp(tokens[*i], ">") == 0) {
		if (tokens[*i + 1]) {
			p->outfile = ft_strdup(tokens[++(*i)]);
			p->append = false;
			(*i)++;
			return true;
		}
		return false;
	}
	if (strcmp(tokens[*i], ">>") == 0) {
		if (tokens[*i + 1]) {
			p->outfile = ft_strdup(tokens[++(*i)]);
			p->append = true;
			(*i)++;
			return true;
		}
		return false;
	}
	return false;
}

t_pipeline *build_pipeline_from_tokens(char **tokens) {
	int i = 0, cmd_i = 0;
	t_pipeline *p = init_pipeline(tokens);
	if (!p)
		return NULL;
	printf("after initilizing pipeline\n");
	while (tokens[i]) {
		printf("[token loop] i = %d -> '%s'\n", i, tokens[i]);
	
		if (handle_redirection_tokens(tokens, &i, p)) {
			continue;
		}
		parse_next_command(tokens, &i, p, &cmd_i);
	}
	
	return p;
}



t_pipeline *parse_redirection_only(char **tokens)
{
	t_pipeline *p = ft_calloc(1, sizeof(t_pipeline));
	t_commands *cmd = ft_calloc(1, sizeof(t_commands));
	char **argv = ft_calloc(128, sizeof(char *));

	if (!p || !cmd || !argv)
		return NULL;

	int i = 0;
	int arg_i = 0;

	while (tokens[i])
	{
		if (ft_strcmp(tokens[i], "<") == 0)
		{
			if (tokens[i + 1])
				p->infile = ft_strdup(tokens[++i]);
			else
			{
				fprintf(stderr, "Error: missing infile\n");
				free(cmd); free(argv); free(p);
				return NULL;
			}
		}
		else if (ft_strcmp(tokens[i], ">>") == 0)
		{
			if (tokens[i + 1])
				p->outfile = ft_strdup(tokens[++i]), p->append = true;
			else
			{
				fprintf(stderr, "Error: missing outfile\n");
				free(cmd); free(argv); free(p);
				return NULL;
			}
		}
		else if (ft_strcmp(tokens[i], ">") == 0)
		{
			if (tokens[i + 1])
				p->outfile = ft_strdup(tokens[++i]), p->append = false;
			else
			{
				fprintf(stderr, "Error: missing outfile\n");
				free(cmd); free(argv); free(p);
				return NULL;
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
		fprintf(stderr, "Error: no command found\n");
		free(cmd); free(argv); free(p);
		return NULL;
	}

	argv[arg_i] = NULL;
	cmd->argv = argv;
	p->cmds = cmd;
	p->cmd_count = 1;

	return p;
}

void free_pipeline(t_pipeline *p)
{
	if (!p) return;
    int i = 0;
	while (i < p->cmd_count) 
    {
		if (p->cmds[i].argv)
        {
            int j = 0;
			while (p->cmds[i].argv[j])
            {
				free(p->cmds[i].argv[j]);
                j++;
            }
			free(p->cmds[i].argv);
		}
        i++;
	}
	free(p->cmds);
	free(p->infile);
	free(p->outfile);
	free(p);
}
char *shell_find_cmd_path(char *cmd, char **paths)
{
	int i = 0;
	char *temp;
	char *full;

	while (paths[i])
	{
		temp = ft_strjoin(paths[i], "/");
		if (!temp)
			return NULL;
		full = ft_strjoin(temp, cmd);
		free(temp);
		if (!full)
			return NULL;
		if (access(full, X_OK) == 0)
			return full;
		free(full);
		i++;
	}
	return NULL;
}
char *get_cmd_path(char *cmd, char **envp)
{
	char *path_str = get_path_from_env(envp);
	if (!path_str)
		return NULL;

	char **paths = ft_split(path_str, ':');
	if (!paths)
		return NULL;

	char *full_path = shell_find_cmd_path(cmd, paths);
	ft_free_array(paths, -1);
	return full_path;
}


static int open_redirection_fds(t_pipeline *cmd, int *in_fd, int *out_fd) {
	*in_fd = -1;
	*out_fd = -1;

	if (cmd->infile) {
		*in_fd = open(cmd->infile, O_RDONLY);
		if (*in_fd < 0) {
			perror(cmd->infile);
			return -1;
		}
	}

	if (cmd->outfile) {
		int flags = O_WRONLY | O_CREAT | (cmd->append ? O_APPEND : O_TRUNC);
		*out_fd = open(cmd->outfile, flags, 0644);
		if (*out_fd < 0) {
			perror(cmd->outfile);
			if (*in_fd != -1)
				close(*in_fd);
			return -1;
		}
	}
	return 0;
}

static void setup_redirections(int in_fd, int out_fd) {
	if (in_fd != -1) {
		dup2(in_fd, STDIN_FILENO);
		close(in_fd);
	}
	if (out_fd != -1) {
		dup2(out_fd, STDOUT_FILENO);
		close(out_fd);
	}
}

void exec_with_redirection(t_pipeline *cmd, char **env) {
	int in_fd, out_fd;

	if (open_redirection_fds(cmd, &in_fd, &out_fd) < 0)
		return;

	pid_t pid = fork();
	if (pid == 0) {
		setup_redirections(in_fd, out_fd);
		if (cmd->cmd_count < 1)
			exit(0);

		char **argv = cmd->cmds[0].argv;
		execve(get_cmd_path(argv[0], env), argv, env);
		perror("execve failedx");
		exit(EXIT_FAILURE);
	}

	waitpid(pid, NULL, 0);
	if (in_fd != -1)
		close(in_fd);
	if (out_fd != -1)
		close(out_fd);
}
t_cmd_type classify_command(char **tokens)
{
	int i = 0;
	int has_pipe = 0;
	int has_redir = 0;
	int seen_pipe = 0;

	while (tokens[i])
	{
		if (ft_strcmp(tokens[i], "|") == 0)
		{
			has_pipe = 1;
			seen_pipe = 1;
			i++;
			continue;
		}
		else if (ft_strcmp(tokens[i], "<") == 0 || ft_strcmp(tokens[i], ">") == 0 || ft_strcmp(tokens[i], ">>") == 0)
		{
			has_redir = 1;
			if (!tokens[i + 1])
				return MIXED_INVALID;

			// If redirection appears in the middle of the pipeline
			if (seen_pipe && tokens[i + 2] && ft_strcmp(tokens[i + 2], "|") != 0)
				return MIXED_INVALID;

			i += 2;
			continue;
		}
		else
		{
			i++;
		}
	}
	if (has_pipe)
		return PIPELINE;
	if (has_redir)
		return REDIR_ONLY;
	return BASIC;
}

// Wrapper that dispatches between full pipex and simple pipeline
void run_pipeline_from_minshell(t_pipeline *p, char **env) {
	if ((p->infile && p->outfile) || (p->infile && ft_strcmp(p->infile, "here_doc") == 0 && p->outfile)) {
		printf("run_pipex_from_minishell\n");
		run_pipex_from_minshell(p, env); // assumes pipex-style interface
	} else {
		printf("no pipex\n");
		// When only pipes (no infile/outfile), fall back to simple execution
		int i;
		int prev_fd = -1;
		int pipe_fd[2];
		for (int i = 0; i < p->cmd_count; i++) {
			printf("cmd[%d]:\n", i);
			for (int j = 0; p->cmds[i].argv[j]; j++) {
				printf("  argv[%d] = %s\n", j, p->cmds[i].argv[j]);
			}
		}
		i = 0; 
		while (i < p->cmd_count) {
			if (i < p->cmd_count - 1 && pipe(pipe_fd) < 0) {
				perror("pipe");
				exit(EXIT_FAILURE);
			}

			pid_t pid = fork();
			if (pid == 0) {
				bool is_piped_forward = (i < p->cmd_count - 1);
			
				if (!is_piped_forward && p->outfile) {
					int fd;
					if (p->append)
						fd = open(p->outfile, O_WRONLY | O_CREAT | O_APPEND, 0644);
					else
						fd = open(p->outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
					if (fd < 0) {
						perror(p->outfile);
						exit(EXIT_FAILURE);
					}
					dup2(fd, STDOUT_FILENO);
					close(fd);
				}
			
				if (i == 0 && p->infile) {
					int fd = open(p->infile, O_RDONLY);
					if (fd < 0) {
						perror(p->infile);
						exit(EXIT_FAILURE);
					}
					dup2(fd, STDIN_FILENO);
					close(fd);
				}
			
				if (prev_fd != -1) {
					dup2(prev_fd, STDIN_FILENO);
					close(prev_fd);
				}
				if (i < p->cmd_count - 1) {
					close(pipe_fd[0]);
					dup2(pipe_fd[1], STDOUT_FILENO);
					close(pipe_fd[1]);
				}
			
				char *cmd_path = get_cmd_path(p->cmds[i].argv[0], env);
				if (!cmd_path) {
					fprintf(stderr, "%s: command not found\n", p->cmds[i].argv[0]);
					exit(127);
				}
				execve(cmd_path, p->cmds[i].argv, env);
				perror("execve failed");
				exit(EXIT_FAILURE);
			}
			if (prev_fd != -1)
				close(prev_fd);
			if (i < p->cmd_count - 1) {
				close(pipe_fd[1]);
				prev_fd = pipe_fd[0];
			}
			i++;
		}
		i = 0; 
		while( i < p->cmd_count)
		{
			wait(NULL);
			i++;
		}
	}
}


