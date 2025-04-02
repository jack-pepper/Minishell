/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmalie <mmalie@student.42nice.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 13:59:23 by mmalie            #+#    #+#             */
/*   Updated: 2025/04/02 11:19:57 by mmalie           ###   ########.fr       */
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
	if (init_cmds(sh) != 0)
		return (-1);	
	return (0);
}

int	init_env(t_shell *sh, char **env)
{
	t_list	*node;
	size_t	nb_vars;
	size_t	i;

	nb_vars = ft_strslen(env);

	// malloc t_list
	sh->this_env = malloc(sizeof(t_list) * (nb_vars + 1));
	if (!sh->this_env)
		return (-1);

	// copy the data
	i = 0;
	node = NULL;
	while (i < nb_vars)
	{
		printf("env[%ld]: %s\n", i, env[i]);
		node = ft_lstnew((char *)env[i]);
		if (!node)
			return (-1);
		ft_lstadd_back(&sh->this_env, node);
		i++;
	}

	// display all (TEST DEBUG)
	i = 0;
	while (sh->this_env[i].next != NULL)
	{
		printf("%s\n", (char *)sh->this_env[i].content);
		i++;
	}

	//ft_memmove(sh->this_env, env, sizeof(char *) * ft_strslen(env));
	

	// set last node to null
	//sh->this_env[ft_strslen(env)] = NULL;
	return (0);
}
