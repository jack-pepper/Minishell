/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_input_manager.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-bouk <yel-bouk@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 19:23:41 by mmalie            #+#    #+#             */
/*   Updated: 2025/04/22 13:18:40 by yel-bouk         ###   ########.fr       */
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
char	**normalize_input(char *line, t_shell *sh)
{
	sh->normalized_line = ft_normalize(line);
	if (!sh->normalized_line)
		return (NULL);
	sh->input_args = ft_split(sh->normalized_line, ' ');
	free(sh->normalized_line);
	sh->normalized_line = NULL;
	if (!sh->input_args)
		return (NULL);
	ft_replace_all_chars(sh->input_args, CTRL_CHAR_SPACE_IN_QUOTE, ' '); // change CTRL_CHAR in quotes back to spaces
	ft_replace_all_chars(sh->input_args, CTRL_CHAR_PIPE, '|');
	// DEBUG: DISPLAY THE CONTENT OF INPUT_ARGS AFTER NORMALIZATION
	//int	i = 0;
	//while (sh->input_args[i] != NULL)
	//{
	//	printf("[DEBUG] input_args[%d]): %s\n", i, sh->input_args[i]);
	//	i++;
	//}
	// DEBUG
	return (sh->input_args);
}

// Should call the needed command and handle errors 
void	process_input(t_shell *sh)
{
	if (!sh->input_args || sh->input_args[0] == NULL)
		return ;
	
	// Added by [m] for $? implementation	
	//printf("~%s~\n", sh->input_args[0]);
	//printf("Res: %d\n", ft_strncmp(sh->input_args[0], "?", ft_strlen("?")) == 0);
	if (sh->input_args[0][0] == CTRL_CHAR_VAR_TO_INTERPRET && sh->input_args[0][1] == '?') // need to add more safety for next char
	{
		printf("%d\n", sh->last_exit_status);
		return ;
	}
	// End

	// check for '|' first
	// output = process_pipe();
	// "<" or "infile.txt >" (check) && NO PIPES

	// DEBUG
	//int i = 0;
	//while (sh->input_args[i])
	//{
	//	printf("BEFORE INTERPRET [%d] ~%s~ \n ", i, sh->input_args[i]);
	//	i++;
	//}
	ft_interpret_env(sh);
	// DEBUG
	//i = 0;
	//while (sh->input_args[i])
	//{
	//	printf("AFTER INTERPRET [%d] ~%s~ \n ", i, sh->input_args[i]);
	//	i++;
	//}



        if (ft_strcmp(sh->input_args[0], "exit") == 0)
	{
		if (sh->input_args[1])
                	cmd_exit(sh, (unsigned int)(ft_atoi(sh->input_args[1])));
		else
			cmd_exit(sh, 0);
        }
	else if (ft_strcmp(sh->input_args[0], "pwd") == 0)
                cmd_pwd();
	else if (ft_strcmp(sh->input_args[0], "cd") == 0)
	{
		if (cmd_cd(sh) != 0)
			return ;
	}
	else if (ft_strcmp(sh->input_args[0], "env") == 0)
		cmd_env(sh);
	else if (ft_strcmp(sh->input_args[0], "echo") == 0)
		cmd_echo(sh);
	else if (ft_strcmp(sh->input_args[0], "export") == 0)
		cmd_export(sh);
	else if (ft_strcmp(sh->input_args[0], "unset") == 0)
		cmd_unset(sh);
	else
		stash_var_or_invalidate(sh);
	return ;
}

void	stash_var_or_invalidate(t_shell *sh)
{	
	char	**invalid_cmd;

	invalid_cmd = ft_strschr(sh->input_args, '=', 0);
	if (invalid_cmd == NULL)
	{
//		printf("All args are valid: all contain = not at index [0]\n");
		stash_var(sh);
		
		/* DEBUG
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
		printf("minishell: %s: command not found\n", *invalid_cmd);
}
