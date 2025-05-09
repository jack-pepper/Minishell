/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_free.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmalie <mmalie@student.42nice.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 11:53:00 by mmalie            #+#    #+#             */
/*   Updated: 2025/05/09 11:23:30 by mmalie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	free_memory(t_shell *sh)
{
	//if (sh->normalized_line != NULL)
	//	free(sh->normalized_line);
	if (sh->input_args != NULL)
		free_args(sh->input_args);
	if (sh->this_env != NULL)
		free_list(&sh->this_env);
	if (sh->env_stash != NULL)
		free_list(&sh->env_stash);
	if (sh->cmds != NULL)
		free_commands(sh->cmds);
	rl_clear_history();
}

void	free_args(char **input_args)
{
	int	i;

	i = 0;
	while (input_args[i])
	{
		free(input_args[i]);
		i++;
	}
	free(input_args);
}

void	free_list(t_list **list)
{
	t_list	*cur_node;
	t_list	*prev_node;
	int	i;

	if (list != NULL && *list != NULL)
	{
		cur_node = *list;
		while (cur_node != NULL)
		{	
			if (cur_node->content != NULL)
			{
				i = 0;
				while (((char **)cur_node->content)[i])
					free(((char **)cur_node->content)[i++]);
				free(((char **)cur_node->content));
				cur_node->content = NULL;
			}
			prev_node = cur_node;
			cur_node = cur_node->next;
			free(prev_node);
		}
		*list = NULL;
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
