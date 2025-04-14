/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_free.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmalie <mmalie@student.42nice.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 11:53:00 by mmalie            #+#    #+#             */
/*   Updated: 2025/04/14 13:29:49 by mmalie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	free_memory(t_shell *sh)
{
	if (sh->normalized_line != NULL)
		free(sh->normalized_line);
	if (sh->input_args != NULL)
		free_args(sh->input_args);
	if (sh->this_env != NULL)
		free_env(&sh->this_env);
	if (sh->cmds != NULL)
		free_commands(sh->cmds);

	// history
}

void	free_args(char **input_args)
{
	int	i;

	i = 0;
	while (input_args[i] != NULL)
	{
		free(input_args[i]);
		i++;
	}
	free(input_args);
}

void	free_env(t_list **this_env)
{	
	t_list	*cur_node;
	t_list	*prev_node;

	if (this_env != NULL)
	{
		cur_node = *this_env;
		while (cur_node != NULL)
		{		
			if (cur_node->content != NULL)
			{
				free(((char **)cur_node->content)[0]);
				free(((char **)cur_node->content)[1]);
				free(((char **)cur_node->content));
				cur_node->content = NULL;
			}
			prev_node = cur_node;
			cur_node = cur_node->next;
			free(prev_node);
		}
		free(cur_node);
		this_env = NULL;
	}
}

void	free_commands(t_command **cmds)
{
	int		i;

	i = 0;
	if (cmds != NULL)
	{
		while (i < 4) // MODULATE according to the nb of commands (not implemented yet)
		{
			if (cmds[i]->name != NULL)
				free(cmds[i]->name);
			if (cmds[i]->doc != NULL)
				free(cmds[i]->doc);
			free(cmds[i]);
			i++;
		}
		free(cmds);
	}
}
