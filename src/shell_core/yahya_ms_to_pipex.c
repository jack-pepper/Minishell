#include "../../inc/minishell.h"

bool is_builtin(const char *cmd)
{
	if (!cmd)
		return false;

	if (ft_strcmp(cmd, "cd") == 0 ||
		ft_strcmp(cmd, "echo") == 0 ||
		ft_strcmp(cmd, "env") == 0 ||
		ft_strcmp(cmd, "pwd") == 0 ||
		ft_strcmp(cmd, "export") == 0 ||
		ft_strcmp(cmd, "unset") == 0 ||
		ft_strcmp(cmd, "exit") == 0 ||
		ft_strchr(cmd, '=') != NULL || // to handle env stash ("VAR_NAME=[VAR_VALUE]")
		(cmd[0] == CTRL_CHAR_VAR_TO_INTERPRET) ||
		((cmd[0] == CTRL_CHAR_VAR_TO_INTERPRET) && (cmd[1] == '?')) ||
		(cmd[0] == '$') ||
		((cmd[0] == '$') && (cmd[1] == '?'))
		)
		return true;
	return false;
}



int count_command_tokens(char **tokens, int start) {
	int count = 0;
	while (tokens[start] &&
	       ft_strcmp(tokens[start], (char[]){CTRL_CHAR_PIPE, '\0'}) != 0)
	{
		if ((ft_strcmp(tokens[start], (char[]){CTRL_CHAR_REDIR_IN, '\0'}) == 0 ||
		ft_strcmp(tokens[start], (char[]){CTRL_CHAR_REDIR_OUT, '\0'}) == 0 ||
		ft_strcmp(tokens[start], (char[]){CTRL_CHAR_APPEND, '\0'}) == 0) &&
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
		ft_strcmp(tokens[*i], (char[]){CTRL_CHAR_PIPE, '\0'}) != 0 &&
		ft_strcmp(tokens[*i], (char[]){CTRL_CHAR_REDIR_IN, '\0'}) != 0 &&
		ft_strcmp(tokens[*i], (char[]){CTRL_CHAR_REDIR_OUT, '\0'}) != 0 &&
		ft_strcmp(tokens[*i], (char[]){CTRL_CHAR_APPEND, '\0'}) != 0)
	{
					// If it's a redirection token, skip its target
		if ((ft_strcmp(tokens[*i], (char[]){CTRL_CHAR_REDIR_IN, '\0'}) == 0 ||
			ft_strcmp(tokens[*i], (char[]){CTRL_CHAR_REDIR_OUT, '\0'}) == 0 ||
			ft_strcmp(tokens[*i], (char[]){CTRL_CHAR_APPEND, '\0'}) == 0) &&
			tokens[*i + 1])
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
	// Skip leading pipes (important for double pipes or pipe at start)
	while (tokens[*i] && ft_strcmp(tokens[*i], (char[]){CTRL_CHAR_PIPE, '\0'}) == 0)
		(*i)++;

	int count = count_command_tokens(tokens, *i);
	if (count == 0)
		return;

	p->cmds[*cmd_i].argv = extract_command_args(tokens, i, count);
	if (!p->cmds[*cmd_i].argv)
		return;

	(*cmd_i)++;

	// Skip trailing pipe, if any (in case it's still there)
	if (tokens[*i] && ft_strcmp(tokens[*i], (char[]){CTRL_CHAR_PIPE, '\0'}) == 0
)
		(*i)++;
}

int count_cmds(char **tokens) {
    int i = 0;
    int count = 0;
    int in_cmd = 0;

    while (tokens[i]) {
        if (ft_strcmp(tokens[i], (char[]){CTRL_CHAR_PIPE, '\0'}) == 0) {
            in_cmd = 0;  // Reset after encountering a pipe
        } else if (ft_strcmp(tokens[i], (char[]){CTRL_CHAR_REDIR_IN, '\0'}) == 0 ||
                   ft_strcmp(tokens[i], (char[]){CTRL_CHAR_REDIR_OUT, '\0'}) == 0 ||
                   ft_strcmp(tokens[i], (char[]){CTRL_CHAR_APPEND, '\0'}) == 0) {
            if (tokens[i + 1]) {
                i++;  // Skip the file argument
            }
            // Do not count redirections as new commands
            in_cmd = 0;
        } else {
            if (!in_cmd) {
                count++;  // Count new command only when not inside a command
                in_cmd = 1;
            }
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
	if (strcmp(tokens[*i], (char[]){CTRL_CHAR_HEREDOC, '\0'}) == 0) {
		if (tokens[*i + 1]) {
			p->infile = ft_strdup("here_doc"); // This is the key translation
			p->limiter = ft_strdup(tokens[++(*i)]); // Store the LIMITER
			p->append = false;
			(*i)++;
			return true;
		}
		return false;
	}
	
	if (strcmp(tokens[*i], (char[]){CTRL_CHAR_REDIR_IN, '\0'}) == 0) {
		if (tokens[*i + 1]) {
			p->infile = ft_strdup(tokens[++(*i)]);
			(*i)++;
			return true;
		}
		return false;
	}
	if (strcmp(tokens[*i], (char[]){CTRL_CHAR_REDIR_OUT, '\0'}) == 0) {
		if (tokens[*i + 1]) {
			p->outfile = ft_strdup(tokens[++(*i)]);
			p->append = false;
			(*i)++;
			return true;
		}
		return false;
	}
	if (strcmp(tokens[*i], (char[]){CTRL_CHAR_APPEND, '\0'}) == 0) {
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
    if (!p) return NULL;

    while (tokens[i]) {
        // printf("[token loop] i = %d -> '%s'\n", i, tokens[i]);

        // **Ensure redirection tokens are properly handled**
        if (handle_redirection_tokens(tokens, &i, p)) {
            if (tokens[i] != NULL) i++;  // **Prevent infinite loops**
            continue;
        }

        // **Process next command correctly**
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
		if (ft_strcmp(tokens[i], (char[]){CTRL_CHAR_REDIR_IN, '\0'}) == 0)
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
		else if (ft_strcmp(tokens[i], (char[]){CTRL_CHAR_APPEND, '\0'}) == 0)
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
		else if (ft_strcmp(tokens[i], (char[]){CTRL_CHAR_REDIR_OUT, '\0'}) == 0)
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
	// printf("parse redirection only\n");
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
	// Case 1: command has a '/' (like ./echo or /bin/ls)
	if (ft_strchr(cmd, '/'))
	{
		if (access(cmd, X_OK) == 0)
			return ft_strdup(cmd); // valid path
		else
		{
			// printf("YOOOOX \n");
			return NULL; // let execve fail
		}
	}

	// Case 2: search in PATH
	char *path_str = get_path_from_env(envp);
	if (!path_str)
	{
		// printf("yoooo\n");
		return NULL;
	}

	char **paths = ft_split(path_str, ':');
	if (!paths)
		return NULL;

	char *full_path = shell_find_cmd_path(cmd, paths);
	ft_free_array(paths, -1);
	return full_path;
}



static int open_redirection_fds(t_pipeline *cmd, int *in_fd, int *out_fd, t_shell *sh) {
	*in_fd = -1;
	*out_fd = -1;
	// printf("I am here\n");
	if (cmd->infile) {
		*in_fd = open(cmd->infile, O_RDONLY);
		if (*in_fd < 0) {
			// printf("Yo\n");
			sh->last_exit_status = 1;
			perror(cmd->infile);
			return -1;
		}
	}

	if (cmd->outfile) {
		int flags = O_WRONLY | O_CREAT | (cmd->append ? O_APPEND : O_TRUNC);
		*out_fd = open(cmd->outfile, flags, 0644);
		if (*out_fd < 0) {
			sh->last_exit_status = 1;
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

void exec_with_redirection(t_pipeline *cmd, char **env, t_shell *sh) {
	int in_fd;
	int out_fd;
	if (open_redirection_fds(cmd, &in_fd, &out_fd, sh) < 0)
	{
		printf("Invalid file\n");	
		return;
	}
	pid_t pid = fork();
	if (pid == 0)
	{
		setup_redirections(in_fd, out_fd);
		if (cmd->cmd_count < 1)
			exit(0);
		char **argv = cmd->cmds[0].argv;
		if (validate_and_exec_command(argv, env, sh))
		{
			exit(sh->last_exit_status);
		}
		if (strcmp(argv[0], "echo") == 0)
		{
			sh->input_args = argv;  // temporarily point to the correct argv
			exit(cmd_echo(sh));
		}
		else
			execve(get_cmd_path(argv[0], env), argv, env);
		perror("execve failed");
		exit(EXIT_FAILURE);
	}

	int status;
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
t_cmd_type classify_command(char **tokens)
{
	int i = 0;
	int has_pipe = 0;
	int has_redir = 0;
	int seen_pipe = 0;

	while (tokens[i])
	{
		if (ft_strcmp(tokens[i], (char[]){CTRL_CHAR_PIPE, '\0'}) == 0)
		{
			has_pipe = 1;
			seen_pipe = 1;
			i++;
			continue;
		}
		else if (ft_strcmp(tokens[i], (char[]){CTRL_CHAR_REDIR_IN, '\0'}) == 0 || ft_strcmp(tokens[i], (char[]){CTRL_CHAR_REDIR_OUT, '\0'}) == 0 || ft_strcmp(tokens[i], (char[]){CTRL_CHAR_APPEND, '\0'}) == 0)
		{
			has_redir = 1;
			if (!tokens[i + 1])
				return MIXED_INVALID;

			// If redirection appears in the middle of the pipeline
			if (seen_pipe && tokens[i + 2] && ft_strcmp(tokens[i + 2], (char[]){CTRL_CHAR_PIPE, '\0'}) != 0)
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

char **clean_env(char **env) {
	int count = 0;
	for (int i = 0; env[i]; i++) {
		if (strncmp(env[i], "SHLVL=", 6) == 0 || env[i][0] == '_')
			continue;
		count++;
	}

	char **filtered = malloc(sizeof(char *) * (count + 1));
	if (!filtered)
		return NULL;

	int j = 0;
	for (int i = 0; env[i]; i++) {
		if (strncmp(env[i], "SHLVL=", 6) == 0 || env[i][0] == '_')
			continue;
		filtered[j++] = strdup(env[i]);
	}
	filtered[j] = NULL;
	return filtered;
}

void print_env(t_list *env)
{
    while (env)
    {
        char *entry = (char *)env->content;
        if (entry && ft_strchr(entry, '=') && ft_strncmp(entry, "_=", 2) != 0)
            printf("%s\n", entry);
        env = env->next;
    }
}
int exec_builtin_in_child(char **argv, t_shell *sh)
{
    if (ft_strcmp(argv[0], "echo") == 0)
        return cmd_echo(sh);
    else if (ft_strcmp(argv[0], "pwd") == 0)
        return cmd_pwd();
    else if (ft_strcmp(argv[0], "env") == 0)
        return cmd_env(sh);
    else if (argv[0][0] == CTRL_CHAR_VAR_TO_INTERPRET && argv[0][1] == '?' && !argv[0][2])
    {
		perror(" ");
        sh->last_exit_status = 1;
        return 0;
    }
    return 1; // Not a safe builtin for child process
}


void run_pipeline_with_redir(t_pipeline *p, char **env, t_shell *sh) {
	int i = 0;
	int prev_fd = -1;
	int pipe_fd[2];

	while (i < p->cmd_count) {
		if (i < p->cmd_count - 1)
		{
			if (pipe(pipe_fd) < 0)
			{
				perror(" ");
				exit(EXIT_FAILURE);
			}
		}
		// printf("I am here0\n");
		pid_t pid = fork();
		if (pid == 0) {  // Child process
			int in_fd = -1, out_fd = -1;
			// Only open redirection for first or last command
			if (open_redirection_fds(p, &in_fd, &out_fd, sh) < 0)
				exit(1);

			// Apply input redirection or pipe
			if (in_fd != -1)
				dup2(in_fd, STDIN_FILENO), close(in_fd);
			else if (prev_fd != -1)
				dup2(prev_fd, STDIN_FILENO);

			// Apply output redirection only for last command
			if (i == p->cmd_count - 1) {
				if (out_fd != -1)
					dup2(out_fd, STDOUT_FILENO), close(out_fd);
			} else {
				// Not last command: write to next pipe
				close(pipe_fd[0]);
				dup2(pipe_fd[1], STDOUT_FILENO);
				close(pipe_fd[1]);
			}

			if (prev_fd != -1)
				close(prev_fd);
			// Get command path
			char *cmd_path = get_cmd_path(p->cmds[i].argv[0], env);
			if (!cmd_path)
				exit(127);
			// Filter environment
			char **cleaned_env = clean_env(env);
			if (!cleaned_env)
				exit(1);
			if (is_builtin(p->cmds[i].argv[0]))
				exit(exec_builtin_in_child(p->cmds[i].argv, sh)); // you’ll need to implement this
			execve(cmd_path, p->cmds[i].argv, cleaned_env);
			perror("execve failed");
			exit(EXIT_FAILURE);
		}

		// Parent process cleanup
		if (prev_fd != -1)
			close(prev_fd);
		if (i < p->cmd_count - 1)
		{
			close(pipe_fd[1]);
			prev_fd = pipe_fd[0];
		}
		i++;
	}

	// Wait for all children
	i = 0;
	int status;
	while (i < p->cmd_count)
	{
		wait(&status);
		if (WIFEXITED(status))
			sh->last_exit_status = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			sh->last_exit_status = 128 + WTERMSIG(status);
		i++;
	}
}

