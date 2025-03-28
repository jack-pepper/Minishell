/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_input_manager.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmalie <mmalie@student.42nice.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 18:30:42 by mmalie            #+#    #+#             */
/*   Updated: 2025/03/28 17:34:51 by mmalie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

// Get the initial input from user
char    *get_input(char *line)
{
        if (line)
        {
                free(line);
                line = (char *)NULL;
        }
        line = readline("ms> ");
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

// Normalize the input and store the arguments for further use
char	**normalize_input(char *line) // unsure whether char *env will be needed aswell, we'll see
{
	char	*clean_input;
	char	**input_args;
        
	clean_input = NULL;
	
	// parsing
	clean_input = ft_normalize(line);
	if (!clean_input)
		return (NULL);     
	//printf("clean_input: %s\n", clean_input); // DEBUG

	// storing args and options
	input_args = ft_split(clean_input, ' ');
	if (!input_args)
		return (NULL);

	/* DISPLAY THE TOKENS STORED FOR DEBUGGING
	int	i = 0;
	while (input_args[i] != NULL)
	{
		printf("([input_args[%d]): %s\n", i, input_args[i]);
		i++;
	}
	 ... I would leave it here for a while to report any unexpected behavior */
	

	return (input_args);
}

// Should call the needed command and handle errors 
void	process_input(char **input_args, char **env)
{
	if (!input_args || input_args[1] == NULL)
		return ;

        if (ft_strncmp(input_args[0], "exit", ft_strlen("exit")) == 0)
                cmd_exit(0);
        else if (ft_strncmp(input_args[0], "pwd", ft_strlen("pwd")) == 0)
                cmd_pwd();
	// [?] Check for 'cd' -> if just 'cd': back to home/$USER, if path: call cmd_cd [?]
	else if (ft_strncmp(input_args[0], "cd", ft_strlen("cd")) == 0)
	{
		if (cmd_cd(input_args[1]) != 0)
			return ;
	}
	else if (ft_strncmp(input_args[0], "env", ft_strlen("env")) == 0)
		cmd_env(env);
	else if (ft_strncmp(input_args[0], "echo", ft_strlen("echo")) == 0)
		cmd_echo(input_args, env);
}
