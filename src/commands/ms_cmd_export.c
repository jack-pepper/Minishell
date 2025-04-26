/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_cmd_export.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmalie <mmalie@student.42nice.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 13:03:40 by mmalie            #+#    #+#             */
/*   Updated: 2025/04/26 19:31:19 by mmalie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	cmd_export(t_shell *sh)
{
	t_list	*stashed_var;
	size_t	i;

	if (!(sh->input_args[1]))
		cmd_env((sh));
	else
	{
		i = 1;
		while (sh->input_args[i] != NULL)
		{
			if (ft_strchr(sh->input_args[i], '=') == NULL) // cmd: export var_name
			{
				stashed_var = ft_getenv(sh->input_args[i], &sh->env_stash);
				if (stashed_var != NULL) // if var_name found in stash
					export_from_stash(sh, stashed_var); 
				i++;
			}
			else // cmd: export var_name=[value]
			{
				export_from_term(sh, &i);
				i++;
			}
		}
	}
	return (0);
}

void	export_from_term(t_shell *sh, size_t *i)
{
	t_list	*set_var;
	char	**split_str;

	split_str = NULL;
	split_str = ft_split(sh->input_args[(*i)], '='); // split the "var_name=[value]" str
	if (!split_str)
		return ;
	set_var = ft_getenv(split_str[0], &sh->this_env);
	if (set_var != NULL) // if var_name found in this_env
	{
		if (ft_update_env_value(set_var, split_str) != 0)
		{
			free_args(split_str);
			return ;
		}
		free_args(split_str);
	}
	else // if var_name not found in this_env
		add_new_env_var(sh, split_str);
}

void	export_from_stash(t_shell *sh, t_list *stashed_var)
{
	t_list	*set_var;
	t_list	*prev_node;

	set_var = ft_getenv(((char **)stashed_var->content)[0], &sh->this_env);
	if (set_var != NULL) // if stashed_var_name found in this_env
		ft_update_env_value(set_var, (char **)stashed_var->content);
	else // if not
		add_new_env_var(sh, (char **)stashed_var->content);

	prev_node = sh->env_stash;

	// if the stashed var is the first of the list, move the head to next node 
	if (stashed_var == sh->env_stash)
		sh->env_stash = stashed_var->next;
	else // if not, find the node before stashed var and move next to stashed var next
	{
		if (prev_node != NULL && prev_node->next != NULL)
		{
			while (prev_node->next && prev_node->next != stashed_var)
				prev_node = prev_node->next;
			if (prev_node->next == stashed_var)
				prev_node->next = stashed_var->next;
		}
	// free stashed_var
	if (stashed_var->content)
		free_args((char **)stashed_var->content);
	free(stashed_var);
	}
}

// add a new node with split_str content to the end of the list
void	add_new_env_var(t_shell *sh, char **split_str)
{
	t_list	*new_node;
	
	new_node = ft_lstnew((char **)split_str);
	if (!new_node)
	{
		free_args(split_str);
		return ;
	}
	ft_lstadd_back(&sh->this_env, new_node);
}

// free previous content, malloc for new content and copy it from split_str
int	ft_update_env_value(t_list *set_var, char **split_str)
{
	size_t	len;
	int		i;

	if (set_var->content)
		free_args(set_var->content);
	set_var->content = malloc(sizeof(char *) * (ft_strslen(split_str) + 1));
	if (!set_var->content)
	{
		free_args(split_str);
		return (-1);
	}
	i = 0;
	len = 0;

	// NB: use ft_strsdup instead?
	while (split_str[i] != NULL)
	{
		len = ft_strlen(split_str[i]);
		((char **)set_var->content)[i] = malloc(len + 1);
		if (!((char **)set_var->content)[i]) // should I clean already mallocated?
			return (-1);
		ft_strlcpy(((char **)set_var->content)[i], split_str[i], len + 1);
		i++;
	}
	((char **)set_var->content)[i] = NULL;
	return (0);
}
