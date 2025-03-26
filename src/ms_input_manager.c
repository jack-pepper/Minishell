/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_input_manager.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmalie <mmalie@student.42nice.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 18:30:42 by mmalie            #+#    #+#             */
/*   Updated: 2025/03/26 10:24:10 by mmalie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

char    *get_input(char *line)
{
        if (line)
        {
                free(line);
                line = (char *)NULL;
        }
        line = readline("> ");
        if (line && *line) // save in history only if not empty
        {
                add_history(line); // Should be stored on a file before quitting
                if (rl_on_new_line() != -1) // How to actually handle this error?
                        return (line);
        }
        else if (!line) // CTRL-D sends EOF, which is handled here
                return ((char *)NULL);
        return (line);
}

void    process_input(char *line, char **env)
{
        // parsing
        
	// execution
        // Naive approach, for testing purposes. Should find a way to parse and execute cmd more efficiently
        if (ft_strncmp(line, "exit", ft_strlen("exit")) == 0)
                cmd_exit(0);
        else if (ft_strncmp(line, "pwd", ft_strlen("pwd")) == 0)
                cmd_pwd();

	// [?] Check for 'cd' -> if just 'cd': back to home/$USER, if path: call cmd_cd [?]
	else if (ft_strncmp(line, "cd /home", ft_strlen("cd /home")) == 0)
	{
		if (cmd_cd("/home") != 0)
			return ;
	}
	
	else if (ft_strncmp(line, "env", ft_strlen("env")) == 0)
		cmd_env(env);
}
