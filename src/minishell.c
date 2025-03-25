/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmalie <mmalie@student.42nice.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 13:59:23 by mmalie            #+#    #+#             */
/*   Updated: 2025/03/25 13:49:41 by mmalie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	main(void)
{
	t_shell		sh;
	static char	*line = (char *)NULL;

	if (init_shell(&sh) != 0)
		return (-1);
	while (1)
	{
		line = get_input(line);
		if (line == NULL) // CTRL-D sends EOF, which is handled here
			exit(1);
		process_input(line);
	}
	free(line);
	rl_clear_history(); // free memory mallocated in readline 
	return (0);
}


t_command	*register_cmd(char *name, void *func, char *doc)
{
	t_command	*cmd;
	size_t		name_len;
	size_t		doc_len;

	cmd = malloc(sizeof(t_command));
	if (!cmd)
		return (NULL);


	name_len = ft_strlen(name);
	doc_len = ft_strlen(doc);

	cmd->name = malloc(sizeof(char) * name_len);
	if (!cmd->name)
		return (NULL);

	cmd->doc = malloc(sizeof(char) * doc_len);
	if (!cmd->doc)
		return (NULL);

	ft_strlcpy(cmd->name, name, name_len);
	cmd->func = func;
	ft_strlcpy(cmd->doc, doc, doc_len);

	return (cmd);
}

int	init_cmds(t_shell *sh)
{
	sh->cmds = malloc(sizeof(t_command) * NB_CMDS);
	if (!sh)
		return (-1);

	// We can use open, read and write, so we may want to loop this logic from a file
	// containing the names and doc

	// exit
	sh->cmds[0] = register_cmd("exit", cmd_exit, "Cause the shell to exit");
	if (!sh->cmds[0])
		return (-1);

	sh->cmds[1] = register_cmd("pwd", cmd_pwd, "Print name of current/working directory");
	if (!sh->cmds[1])
		return (-1);

	return (0);
}


int	init_shell(t_shell *sh)
{
	init_signals();
	if (init_cmds(sh) != 0)
		return (-1);
	return (0);
}

char	*get_input(char *line)
{
	if (line)
	{
		free(line);
		line = (char *)NULL;
	}
	line = readline("> ");
	if (line && *line) // save in history only if not empty
	{
		add_history(line); // NB: We should probably store the content in some external file on quit
		// printf("%s\n", line);
		if (rl_on_new_line() != 0) // How to actually handle this error?
			return (line);
	}
	else if (!line) // CTRL-D sends EOF, which is handled here
		return ((char *)NULL);
	return (line);
}



void	process_input(char *line)
{
	// parsing
	// execution


	// Naive approach, for testing purposes. We should probably find a way to parse and execute cmd more efficiently... */
	if (ft_strncmp(line, "exit", ft_strlen("exit")) == 0)
		cmd_exit(0);
	else if (ft_strncmp(line, "pwd", ft_strlen("pwd")) == 0)
		cmd_pwd();

}
