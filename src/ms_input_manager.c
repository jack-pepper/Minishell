/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_input_manager.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmalie <mmalie@student.42nice.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 18:30:42 by mmalie            #+#    #+#             */
/*   Updated: 2025/04/03 13:44:38 by mmalie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

// Get the initial input from user
char	*get_input(char *line)
{
	if (line)
	{
		free(line);
		line = (char *)NULL;
	}
	line = readline("ms> ");
	if (line && *line)
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
	clean_input = ft_normalize(line);
	if (!clean_input)
		return (NULL);
	//printf("[DEBUG] normalized input: ~%s~\n", clean_input);
	input_args = ft_split(clean_input, ' ');
	if (!input_args)
		return (NULL);

	// DISPLAY THE TOKENS STORED FOR DEBUGGING
	//int	i = 0;
	//while (input_args[i] != NULL)
	//{
	//	printf("[DEBUG] input_args[%d]): %s\n", i, input_args[i]);
	//	i++;
	//}
	// DEBUG
	
	return (input_args);
}

// Should call the needed command and handle errors 
void	process_input(char **input_args, t_list **this_env)
{
	if (!input_args || input_args[0] == NULL)
		return ;

	// check for '|' first
	// output = process_pipe();

	// "<" or "infile.txt >" (check) && NO PIPES

        if (ft_strncmp(input_args[0], "exit", ft_strlen("exit")) == 0)
                cmd_exit(0);
        else if (ft_strncmp(input_args[0], "pwd", ft_strlen("pwd")) == 0)
                cmd_pwd();
	else if (ft_strncmp(input_args[0], "cd", ft_strlen("cd")) == 0)
	{
		if (cmd_cd(input_args[1]) != 0)
			return ;
	}
	else if (ft_strncmp(input_args[0], "env", ft_strlen("env")) == 0)
		cmd_env((*this_env));
	else if (ft_strncmp(input_args[0], "echo", ft_strlen("echo")) == 0)
		cmd_echo(input_args, this_env);
	else if (ft_strncmp(input_args[0], "export", ft_strlen("export")) == 0)
		cmd_export(input_args, this_env);
	else if (ft_strncmp(input_args[0], "unset", ft_strlen("unset")) == 0)
		cmd_unset(input_args, this_env);
	return ;
}
