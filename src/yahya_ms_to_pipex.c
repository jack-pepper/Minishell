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
