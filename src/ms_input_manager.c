/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_input_manager.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmalie <mmalie@student.42nice.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 18:30:42 by mmalie            #+#    #+#             */
/*   Updated: 2025/04/06 15:35:52 by mmalie           ###   ########.fr       */
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
char	**normalize_input(char *line, t_list **this_env)
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

	ft_replace_all_chars(input_args, CTRL_CHAR_SPACE_IN_QUOTE, ' '); // change CTRL_CHAR in quotes back to spaces
	
	this_env++ ; this_env--;
	
	// DISPLAY THE TOKENS STORED FOR DEBUGGING
	int	i = 0;
	while (input_args[i] != NULL)
	{
		printf("[DEBUG] input_args[%d]): %s\n", i, input_args[i]);
		i++;
	}
	// DEBUG
	
	return (input_args);
}

void	ft_interpret_env(char *input_arg, t_list **this_env)
{
	char	**split_arg;
	char	*rejoined_arg;
	t_list	*set_var;
	int	i;
	
	printf("[FT_INTERPRET_ENV - 1]\n");
	// if no need to split (solo arg containing only $VAR_NAME)
	if (input_arg[0] == CTRL_CHAR_VAR_TO_INTERPRET && ft_strpbrk(input_arg, " ") == NULL) // is checking for any space enough?
	{
		printf("[FT_INTERPRET_ENV - 2]\n");
		set_var = ft_getenv(&(input_arg)[1], this_env);
		if (set_var != NULL)
		{
			free(input_arg);
			input_arg = ft_strdup(((char **)set_var->content)[1]);
			if (!input_arg)
				return ;
		//	printf("input_arg: %s\n", input_arg);
		}
		else // make sure to make the behavior match bash, will probably be different
		{
			input_arg[0] = '$';
		}
		return ;
	}

	// if need to split	
	printf("[FT_INTERPRET_ENV - 3]\n");
	i = 0;
	// isolate the spaces to keep it
	while (input_arg[i])
	{
		if (input_arg[i] == ' ' && input_arg[i + 1] != ' ')
		{
			input_arg[i] = '#'; // DEBUG
			i++;
			while (input_arg[i] == ' ' && input_arg[i + 1] == ' ')
				i++;
			//input_arg[i] = '#';
		}
		i++;
	}
	split_arg = ft_split(input_arg, '#');
	if (!split_arg)
		return ;
	i = 0;
	while (split_arg[i])
	{
		if (split_arg[i][0] == CTRL_CHAR_VAR_TO_INTERPRET)
		{
			set_var = ft_getenv(&split_arg[0][1], this_env);
			if (set_var != NULL)
			{	
				free(input_arg);
				input_arg = ((char **)set_var->content)[1];
			}
			else // make sure to make the behavior match bash, will probably be different
			{
				input_arg[0] = '$'; // wrong
			}
		}
		i++;
	}
	i = 0;
	rejoined_arg = NULL;
	while (split_arg[i])
	{
		rejoined_arg = ft_strjoin(rejoined_arg, split_arg[i]);
		if (!rejoined_arg)
			return ;
		i++;
	}
	return ;
}

// Should call the needed command and handle errors 
void	process_input(char **input_args, t_list **this_env)
{
	t_list	*set_var;

	if (!input_args || input_args[0] == NULL)
		return ;

	// check for '|' first
	// output = process_pipe();

	// DEBUG
	//int i = 0;
	//while (input_args)
	//{
	//	printf("[%d] '%s' ", i, input_args[i]);
	//	i++;
	//}
	// "<" or "infile.txt >" (check) && NO PIPES

	
	// handle_no_cmd() 

	if (input_args[0][0] == CTRL_CHAR_VAR_TO_INTERPRET) // To be compared with bash behavior
	{
		set_var = ft_getenv(&(input_args[0])[1], this_env);
		ft_interpret_env(input_args[0], &set_var); // ...
		printf("%s\n", input_args[0]);
		return ;
	}

	// for other cases, convert the env
	int i = 0;
	while (input_args[i])
	{
		ft_interpret_env(input_args[i], this_env);
		i++;
	}

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
		cmd_env((this_env));
	else if (ft_strncmp(input_args[0], "echo", ft_strlen("echo")) == 0)
		cmd_echo(input_args, this_env);
	else if (ft_strncmp(input_args[0], "export", ft_strlen("export")) == 0)
		cmd_export(input_args, this_env);
	else if (ft_strncmp(input_args[0], "unset", ft_strlen("unset")) == 0)
		cmd_unset(input_args, this_env);
	else
		printf("minishell: %s: command not found\n", input_args[0]);
	return ;
}
