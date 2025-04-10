#include "../inc/minishell.h"

void parse_next_command(char **tokens, int *i, t_pipeline *p, int *cmd_i)
{
	int start = *i;
	int count = 0;
	int j = 0;

	// Count number of tokens until next pipe or redirection
	while (tokens[*i] &&
		strcmp(tokens[*i], "|") != 0 &&
		strcmp(tokens[*i], "<") != 0 &&
		strcmp(tokens[*i], ">") != 0 &&
		strcmp(tokens[*i], ">>") != 0)
	{
		count++;
		(*i)++;
	}

	if (count == 0)
		return;

	p->cmds[*cmd_i].argv = malloc(sizeof(char *) * (count + 1));
	if (!p->cmds[*cmd_i].argv)
		return;

	*i = start;
	while (tokens[*i] &&
		strcmp(tokens[*i], "|") != 0 &&
		strcmp(tokens[*i], "<") != 0 &&
		strcmp(tokens[*i], ">") != 0 &&
		strcmp(tokens[*i], ">>") != 0)
	{
		p->cmds[*cmd_i].argv[j++] = strdup(tokens[*i]);
		(*i)++;
	}
	p->cmds[*cmd_i].argv[j] = NULL;

	if (tokens[*i] && strcmp(tokens[*i], "|") == 0)
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
		else if (strcmp(tokens[i], "<") == 0 ||
		         strcmp(tokens[i], ">") == 0 ||
		         strcmp(tokens[i], ">>") == 0)
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
t_pipeline *build_pipeline_from_tokens(char **tokens)
{
	int i = 0, cmd_i = 0;
	t_pipeline *p = calloc(1, sizeof(t_pipeline));
	if (!p)
		return (NULL);

	p->cmd_count = count_cmds(tokens);
	p->cmds = malloc(sizeof(t_command) * p->cmd_count);
	if (!p->cmds)
	{
		free(p);
		return (NULL);
	}

	while (tokens[i])
	{
		if (strcmp(tokens[i], "<") == 0) {
			if (tokens[i + 1])
				p->infile = strdup(tokens[++i]);
			else
				return (NULL);
			i++;
			continue;
		}
		else if (strcmp(tokens[i], ">") == 0) {
			if (tokens[i + 1])
				p->outfile = strdup(tokens[++i]), p->append = false;
			else
				return (NULL);
			i++;
			continue;
		}
		else if (strcmp(tokens[i], ">>") == 0) {
			if (tokens[i + 1])
				p->outfile = strdup(tokens[++i]), p->append = true;
			else
				return (NULL);
			i++;
			continue;
		}
		else
			parse_next_command(tokens, &i, p, &cmd_i);
	}
	return p;
}

t_pipeline *parse_redirection_only(char **tokens)
{
	t_pipeline *p = calloc(1, sizeof(t_pipeline));
	t_commands *cmd = calloc(1, sizeof(t_commands));
	char **argv = calloc(128, sizeof(char *)); // Max 128 args for now

	if (!p || !cmd || !argv)
		return (NULL); // Handle allocation failure

	int i = 0;
	int arg_i = 0;

	while (tokens[i])
	{
		if (strcmp(tokens[i], "<") == 0)
		{
			if (tokens[i + 1])
				p->infile = strdup(tokens[++i]);
			else
				return (NULL); // Error: missing filename
		}
		else if (strcmp(tokens[i], ">>") == 0)
		{
			if (tokens[i + 1])
				p->outfile = strdup(tokens[++i]), p->append = true;
			else
				return (NULL);
		}
		else if (strcmp(tokens[i], ">") == 0)
		{
			if (tokens[i + 1])
				p->outfile = strdup(tokens[++i]), p->append = false;
			else
				return (NULL);
		}
		else
		{
			argv[arg_i++] = strdup(tokens[i]); // Command or argument
		}
		i++;
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


void exec_with_redirection(t_pipeline *cmd, char **env)
{
    int in_fd = -1;
    int out_fd = -1;

    // Open input file
    if (cmd->infile)
    {
        in_fd = open(cmd->infile, O_RDONLY);
        if (in_fd < 0)
        {
            perror(cmd->infile);
            return ;
        }
    }

    // Open output file (if needed)
    if (cmd->outfile)
    {
        int flags = O_WRONLY | O_CREAT | (cmd->append ? O_APPEND : O_TRUNC);
        out_fd = open(cmd->outfile, flags, 0644);
        if (out_fd < 0)
        {
            perror(cmd->outfile);
            if (in_fd != -1)
                close(in_fd);
            return ;
        }
    }

    // Fork and exec
    pid_t pid = fork();
    if (pid == 0)
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

        // Execute command
        if (cmd->cmd_count < 1)
            exit(0); // Nothing to execute
        char **argv = cmd->cmds[0].argv;
        execve(get_cmd_path(argv[0], env), argv, env);
        perror("execve failed");
        exit(EXIT_FAILURE);
    }

    // Parent process
    waitpid(pid, NULL, 0);
    if (in_fd != -1)
        close(in_fd);
    if (out_fd != -1)
        close(out_fd);
}


