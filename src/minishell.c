/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmalie <mmalie@student.42nice.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 13:59:23 by mmalie            #+#    #+#             */
/*   Updated: 2025/04/03 19:32:26 by mmalie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	main(int argc, char **argv, char **env)
{
	t_shell		sh;
	static char	*line = (char *)NULL;

	if (argc != 1 || argv[1])
		return (-1);
	if (init_shell(&sh, env) != 0)
		return (-1);
	while (1)
	{
		line = get_input(line);
		if (line == NULL) // CTRL-D sends EOF, which is handled here
			exit(1);
		if (line[0] != '\0') // or if it contains only whitespace, add func
			process_input(normalize_input(line), &sh.this_env);
	}
	free(line);
	rl_clear_history(); // free memory mallocated in readline 
	return (0);
}

int	init_shell(t_shell *sh, char **env)
{
	init_signals();
	if (init_env(sh, env) != 0)
		return (-1);
	// init_pipex (make a pointer to this_env)
	if (init_cmds(sh) != 0)
		return (-1);	
	return (0);
}

int	init_env(t_shell *sh, char **env)
{
	size_t	nb_vars;

	nb_vars = ft_strslen(env);
	sh->this_env = malloc(sizeof(t_list *) * (nb_vars + 1));
	if (!sh->this_env)
		return (-1);
	sh->this_env = NULL;
	if (ft_copy_strs_to_list(&sh->this_env, env, nb_vars, '=') != 0)
		return (-1);
	return (0);
}
