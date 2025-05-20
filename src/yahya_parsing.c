/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   yahya_parsing.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-bouk <yel-bouk@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 12:04:47 by yel-bouk          #+#    #+#             */
/*   Updated: 2025/05/20 16:03:16 by yel-bouk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int count_pipes(char **tokens) {
    int count = 0;
    int i = 0;
    while (tokens[i]) {
        if (ft_strcmp(tokens[i], (char[]){CC_PIPE, '\0'}) == 0)
            count++;
        i++;
    }
    return count;
}
int count_args(char **tokens)
{
	int count = 0;
	int i = 0;
	while (tokens[i])
	{
		if ((ft_strcmp(tokens[i], (char[]){CC_REDIR_IN, '\0'}) == 0 ||
			 ft_strcmp(tokens[i], (char[]){CC_REDIR_OUT, '\0'}) == 0 ||
			 ft_strcmp(tokens[i], (char[]){CC_APPEND, '\0'}) == 0) &&
			tokens[i + 1])
		{
			i += 2; // Skip redirection token + filename
		}
		else if ((unsigned char)tokens[i][0] < 32 && tokens[i][1] == '\0') {
			i++; // Skip other control characters (e.g. pipe)
		}
		else {
			count++;
			i++;
		}
	}
	return count;
}


char **extract_tokens(char **tokens, int start, int end) {
    int len = end - start;
    char **result = malloc(sizeof(char *) * (len + 1));
	int i = 0;
	while(i < len)
	{
		result[i] = ft_strdup(tokens[start + i]);
		i++;
	}
    result[len] = NULL;
    return result;
	
}
t_commands parse_command(char **tokens) {
    t_commands cmd;
    memset(&cmd, 0, sizeof(t_commands));
    cmd.argv = malloc(sizeof(char *) * (count_args(tokens) + 1));
    if (!cmd.argv) {
        cmd.infile = NULL;
        cmd.outfile = NULL;
        return cmd;
    }
    int argc = 0;

    for (int i = 0; tokens[i]; i++) {
        if (ft_strcmp(tokens[i], (char[]){CC_REDIR_IN, '\0'}) == 0 && tokens[i + 1]) {
            free(cmd.infile);
            cmd.infile = ft_strdup(tokens[++i]);
        }
        else if (ft_strcmp(tokens[i], (char[]){CC_REDIR_OUT, '\0'}) == 0 && tokens[i + 1]) {
            free(cmd.outfile);
            cmd.outfile = ft_strdup(tokens[++i]);
            cmd.append = false;
        }
        else if (ft_strcmp(tokens[i], (char[]){CC_APPEND, '\0'}) == 0 && tokens[i + 1]) {
            free(cmd.outfile);
            cmd.outfile = ft_strdup(tokens[++i]);
            cmd.append = true;
        }
        else if ((unsigned char)tokens[i][0] < 32 && tokens[i][1] == '\0') {
            continue;
        }
        else {
            cmd.argv[argc++] = ft_strdup(tokens[i]);
        }
    }
    cmd.argv[argc] = NULL;
    return cmd;
}

void free_command(t_commands *cmd) {
    if (cmd->argv) {
        for (int i = 0; cmd->argv[i]; i++)
            free(cmd->argv[i]);
        free(cmd->argv);
    }
    free(cmd->infile);
    free(cmd->outfile);
}

void free_tokens(char **tokens) {
	int i = 0;
	while(tokens[i])
	{
		free(tokens[i]);
		i++;
	}
    free(tokens);
}

void parse_and_build_pipeline(t_pipeline *pipeline, char **tokens) {
    int num_cmds = count_pipes(tokens) + 1;

    pipeline->cmd_count = num_cmds;
    pipeline->cmds = ft_calloc(num_cmds, sizeof(t_commands));
    if (!pipeline->cmds)
        exit(1); // handle malloc failure

    int start = 0;
    int i = 0, cmd_index = 0;

    while (tokens[i]) {
        if (ft_strcmp(tokens[i], (char[]){CC_PIPE, '\0'}) == 0) {
            if (cmd_index >= num_cmds) {
                fprintf(stderr, "Overflow during parsing: cmd_index=%d >= num_cmds=%d\n", cmd_index, num_cmds);
                exit(1);
            }
            char **cmd_tokens = extract_tokens(tokens, start, i);
            pipeline->cmds[cmd_index++] = parse_command(cmd_tokens);
            free_tokens(cmd_tokens);
            start = i + 1;
        }
        i++;
    }

    // Final command
    if (cmd_index >= num_cmds) {
        fprintf(stderr, "Overflow at final command: cmd_index=%d >= num_cmds=%d\n", cmd_index, num_cmds);
        exit(1);
    }
    char **cmd_tokens = extract_tokens(tokens, start, i);
    pipeline->cmds[cmd_index] = parse_command(cmd_tokens);
    free_tokens(cmd_tokens);
}



