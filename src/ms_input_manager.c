/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_input_manager.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmalie <mmalie@student.42nice.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 18:30:42 by mmalie            #+#    #+#             */
/*   Updated: 2025/04/08 19:29:58 by mmalie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

// Get the initial raw input from user
char	*get_input(char *line)
{
	if (line)
	{
		free(line);
		line = (char *)NULL;
	}
	line = readline(PROMPT_STYLE);
	if (line && *line)
	{
		add_history(line); // Should be stored on a file before quitting
		if (rl_on_new_line() != -1) // How to actually handle this error?
			return (line);
	}
	else if (!line) // Handles EOF (sent by CTRL-D)
		return ((char *)NULL);
	return (line);
}

// Normalize the input and store the arguments for further use
int	normalize_input(char *line, t_shell *sh)
{
	sh->normalized_line = ft_normalize(line);
	if (!sh->normalized_line)
		return (-1);
	//printf("[DEBUG] normalized input: ~%s~\n", clean_input);
	sh->input_args = ft_split(sh->normalized_line, ' ');
	if (!sh->input_args)
		return (-1);
	ft_replace_all_chars(sh->input_args, CTRL_CHAR_SPACE_IN_QUOTE, ' '); // change CTRL_CHAR in quotes back to spaces

	// DEBUG: DISPLAY THE CONTENT OF INPUT_ARGS AFTER NORMALIZATION
	//int	i = 0;
	//while (sh->input_args[i] != NULL)
	//{
	//	printf("[DEBUG] input_args[%d]): %s\n", i, sh->input_args[i]);
	//	i++;
	//}
	// DEBUG
	return (0);
}

// Interpret the env variables when needed.
char	**ft_interpret_env(char **input_arg, t_list **this_env)
{
	char	**split_arg;
	char	*rejoined_arg;
	t_list	*set_var;
	int	i;
	
	// if no need to split (solo arg containing only $VAR_NAME)
	if ((*input_arg)[0] == CTRL_CHAR_VAR_TO_INTERPRET && ft_strpbrk((*input_arg), " ") == NULL) // is checking for any space enough?
	{
		set_var = ft_getenv(&(*input_arg)[1], this_env);
		if (set_var != NULL)
		{
			//free(input_arg);
			(*input_arg) = ft_strdup(((char **)set_var->content)[1]);
			if (!(*input_arg))
				return (NULL) ;
		}
		else // make sure to make the behavior match bash, will probably be different
		{
			(*input_arg)[0] = '$';
		}
		return (input_arg);
	}
	// if need to split	
	i = 0;
	while ((*input_arg)[i]) // to retrieve the spaces
	{
		if ((*input_arg)[i] == ' ' && (*input_arg)[i + 1] != ' ')
		{
			(*input_arg)[i] = CTRL_CHAR_SUBARG_DELIM;
			i++;
		}
		i++;
	}
	split_arg = ft_split((*input_arg), CTRL_CHAR_SUBARG_DELIM);
	if (!split_arg)
		return (input_arg);
	i = 0;
	rejoined_arg = "";
	while (split_arg[i])
	{
		if (split_arg[i][0] == CTRL_CHAR_VAR_TO_INTERPRET)
		{
			set_var = ft_getenv(&split_arg[i][1], this_env);
			if (set_var != NULL)
			{	
			//	free(split_arg[i]);
				split_arg[i] = ft_strdup(((char **)set_var->content)[1]);
				if (!split_arg)
					return (input_arg) ;
			}
			else // make sure to make the behavior match bash, will probably be different
			{
				split_arg[i][0] = '$';
			}
		}
		rejoined_arg = ft_strjoin(rejoined_arg, split_arg[i]);
		if (split_arg[i + 1])
			rejoined_arg = ft_strjoin(rejoined_arg, " ");
		// printf("(%d) %s\n", i, rejoined_arg);
		i++;
	}
	(*input_arg) = ft_strdup(rejoined_arg);
	free(rejoined_arg);
	return (input_arg);
}

// Should call the needed command and handle errors 
void	process_input(t_shell *sh)
{
	t_list	*set_var;

	if (!sh->input_args || sh->input_args[0] == NULL)
		return ;

	// check for '|' first
	// output = process_pipe();

	// DEBUG - show the content of input_args
	//int i = 0;
	//while (input_args)
	//{
	//	printf("[%d] '%s' ", i, input_args[i]);
	//	i++;
	//}
	// "<" or "infile.txt >" (check) && NO PIPES

	
	// handle_no_cmd(). To be compared with bash behavior.
	// Need to loop through all args
	if (sh->input_args[0][0] == CTRL_CHAR_VAR_TO_INTERPRET)
	{
		// to be compared with bash behavior.
		set_var = ft_getenv(&(sh->input_args[0])[1], &sh->this_env);
		ft_interpret_env(&(sh->input_args[0]), &set_var); // ...
		printf("%s\n", sh->input_args[0]);
		return ;
	}

	// for other cases, convert the env
	int i = 0;
	while (sh->input_args[i])
	{
		ft_interpret_env(&(sh->input_args[i]), &sh->this_env);
		i++;
	}

        if (ft_strncmp(sh->input_args[0], "exit", ft_strlen("exit")) == 0)
                cmd_exit(sh, 0);
        else if (ft_strncmp(sh->input_args[0], "pwd", ft_strlen("pwd")) == 0)
                cmd_pwd();
	else if (ft_strncmp(sh->input_args[0], "cd", ft_strlen("cd")) == 0)
	{
		if (cmd_cd(sh) != 0)
			return ;
	}
	else if (ft_strncmp(sh->input_args[0], "env", ft_strlen("env")) == 0)
		cmd_env(sh);
	else if (ft_strncmp(sh->input_args[0], "echo", ft_strlen("echo")) == 0)
		cmd_echo(sh);
	else if (ft_strncmp(sh->input_args[0], "export", ft_strlen("export")) == 0)
		cmd_export(sh);
	else if (ft_strncmp(sh->input_args[0], "unset", ft_strlen("unset")) == 0)
		cmd_unset(sh);
	else
		printf("minishell: %s: command not found\n", sh->input_args[0]);
	return ;
}
