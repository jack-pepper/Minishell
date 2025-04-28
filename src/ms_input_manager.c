/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_input_manager.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-bouk <yel-bouk@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 19:23:41 by mmalie            #+#    #+#             */
/*   Updated: 2025/04/28 09:49:05 by yel-bouk         ###   ########.fr       */
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
		add_history(line); // Could be stored on a file before quitting
		if (rl_on_new_line() != -1) // How to handle this error?
			return (line);
	}
	else if (!line) // Handles EOF (sent by CTRL-D)
		return ((char *)NULL);
	return (line);
}

// Normalize the input and store the arguments for further use
char	**normalize_input(char *line, t_shell *sh)
{
	sh->normalized_line = ft_normalize(line);
	// printf("Normalized %s\n", sh->normalized_line);
	if (!sh->normalized_line)
		return (NULL);
	sh->input_args = ft_split(sh->normalized_line, ' ');
	// int i = 0;
	// while (sh->input_args[i]) {
	// 	printf(" normalize input argv[%d] = %s\n", i, sh->input_args[i]);
    // i++;
	// }
	free(sh->normalized_line);
	sh->normalized_line = NULL;
	if (!sh->input_args)
		return (NULL);
	ft_replace_all_chars(sh->input_args, CTRL_CHAR_SPACE_IN_QUOTE, ' '); // change CTRL_CHAR in quotes back to spaces
	ft_replace_all_chars(sh->input_args, '|', CTRL_CHAR_PIPE);
	// DEBUG: DISPLAY THE CONTENT OF INPUT_ARGS AFTER NORMALIZATION
	//int	i = 0;
	//while (sh->input_args[i] != NULL)
	//{
	//	printf("[DEBUG] input_args[%d]): %s\n", i, sh->input_args[i]);
	//	i++;
	//}
	// DEBUG
// 	int i = 0;
// 	while (sh->input_args[i]) {
// 		printf(" normalize input argv[%d] = %s\n", i, sh->input_args[i]);
//     i++;
// }
	return (sh->input_args);
}

// Should call the needed command and handle errors 
int	process_input(t_shell *sh)
{
	if (!sh->input_args || sh->input_args[0] == NULL)
		return (-1) ;
	if ((sh->input_args[0][0] == CTRL_CHAR_VAR_TO_INTERPRET && sh->input_args[0][1] == '?')
		|| (ft_strcmp(sh->input_args[0], "echo") == 0 && sh->input_args[1] && sh->input_args[1][0] == CTRL_CHAR_VAR_TO_INTERPRET
		&& sh->input_args[1][1] == '?')) // need to add more safety for next char
	{
		printf("%d\n", sh->last_exit_status);
		return (0);
	}

	// check for '|' first
	// output = process_pipe();
	// "<" or "infile.txt >" (check) && NO PIPES

//	ft_show_strs(sh->input_args, "[DEBUG] input_args BEFORE env interpret"); // DEBUG	
	ft_interpret_env(sh);
//	ft_show_strs(sh->input_args, "[DEBUG] input_args AFTER env interpret"); // DEBUG

        if (ft_strcmp(sh->input_args[0], "exit") == 0)
	{
		if (sh->input_args[1])
			sh->last_exit_status = cmd_exit(sh, (unsigned int)(ft_atoi(sh->input_args[1])));
		else
			sh->last_exit_status = cmd_exit(sh, 0);
        }
	else if (ft_strcmp(sh->input_args[0], "pwd") == 0)
                sh->last_exit_status = cmd_pwd();
	else if (ft_strcmp(sh->input_args[0], "cd") == 0) // need to handle input starting with '/' too...
	{
		sh->last_exit_status = cmd_cd(sh);
		if (sh->last_exit_status != 0)
			return (sh->last_exit_status);
	}
	else if (ft_strcmp(sh->input_args[0], "env") == 0)
		sh->last_exit_status = cmd_env(sh);
	else if (ft_strcmp(sh->input_args[0], "echo") == 0)
		sh->last_exit_status = cmd_echo(sh);
	else if (ft_strcmp(sh->input_args[0], "export") == 0)
		sh->last_exit_status = cmd_export(sh);
	else if (ft_strcmp(sh->input_args[0], "unset") == 0)
		sh->last_exit_status = cmd_unset(sh);
	else
		sh->last_exit_status = stash_var_or_invalidate(sh);
	return (sh->last_exit_status);
}

int	stash_var_or_invalidate(t_shell *sh)
{
	char	**invalid_cmd;

	invalid_cmd = ft_strschr(sh->input_args, '=', 0);
	if (invalid_cmd == NULL)
	{
		stash_var(sh);
		/*DEBUG
		t_list *cur_node = sh->env_stash;
		while (cur_node != NULL)
        	{
                	printf("%s=%s\n",
                        	((char **)cur_node->content)[0],
                        	((char **)cur_node->content)[1]);
                	cur_node = cur_node->next;
        	}
        	return ;
		*/
	}
	else
	{
		printf("minishell: %s: command not found\n", *invalid_cmd);
		return (127);
	}
	return (0);
}
