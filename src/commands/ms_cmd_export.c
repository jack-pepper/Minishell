/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_cmd_export.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmalie <mmalie@student.42nice.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 13:03:40 by mmalie            #+#    #+#             */
/*   Updated: 2025/04/20 16:16:50 by mmalie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	cmd_export(t_shell *sh)
{
	t_list	*set_var;
	t_list	*stashed_var;
	char	**split_str;
	size_t	i;

	if (!sh->input_args)
		return (-1) ;
	if (!(sh->input_args[1])) // if no args, man says result is unspecified
		cmd_env((sh)); // ... we may just display the env variables
	else
	{
		i = 1;
		while (sh->input_args[i] != NULL)
		{
			if (ft_strchr(sh->input_args[i], '=') == NULL)
			{
				stashed_var = ft_getenv(sh->input_args[i], &sh->env_stash);
				if (stashed_var != NULL)
					export_from_stash(sh, stashed_var);
				else
					return (-1);
				i++;
			}
			else
			{
				split_str = NULL;
				split_str = ft_split(sh->input_args[i], '='); // leak
				if (!split_str)
					return (-1);
				set_var = ft_getenv(split_str[0], &sh->this_env);
				if (set_var != NULL)
				{
					if (ft_update_env_value(set_var, split_str) != 0) // leak
						return (0) ;
				}
				else
					add_new_env_var(sh, split_str);
				i++;
			}
		}
	}
	return (0);
}

void	stash_var(t_shell *sh)
{
	t_list	*stashed_var;
	t_list	*node;
	char    **split_str;
        size_t  i;
	size_t	nb_args;

        i = 0;
        node = NULL;
        split_str = NULL;
	nb_args = ft_strslen(sh->input_args);
	//printf("[export_stash_var] nb_args: %ld", nb_args); // DEBUG
        while (i < nb_args)
        {
		split_str = ft_split(sh->input_args[i], '=');
                if (!split_str)
			return ;
		stashed_var = ft_getenv(split_str[0], &sh->env_stash);
		if (stashed_var != NULL)
		{
			//printf("%s=%s\n", ((char **)stashed_var->content)[0], ((char **)stashed_var->content)[1]);
			if (ft_update_env_value(stashed_var, split_str) != 0)
			{
				free_args(split_str);
				return ;
			}
		}
		else
		{
			node = ft_lstnew((char **)split_str);
			if (!node)
				return ;
			ft_lstadd_back(&sh->env_stash, node);
		}
		i++;
        }
        return ;
}

void	export_from_stash(t_shell *sh, t_list *stashed_var)
{
	t_list	*set_var;
	t_list	*prev_node;

	set_var = ft_getenv(((char **)stashed_var->content)[0], &sh->this_env);
	if (set_var != NULL)
	{
		if (ft_update_env_value(set_var, (char **)stashed_var->content) != 0)
			free_args((char **)stashed_var->content);
	}
	else
		add_new_env_var(sh, (char **)stashed_var->content);
	if (sh->env_stash == stashed_var)	
		sh->env_stash = stashed_var->next;
	else
	{
		prev_node = sh->env_stash;
		if (prev_node != NULL && prev_node->next != NULL)
		{
			while (ft_strcmp(((char **)prev_node->next->content)[0],
				(((char **)stashed_var->content)[0])) != 0)
				prev_node = prev_node->next;
		}
		prev_node->next = stashed_var->next;
	}
	//free_args(stashed_var->content);
	//free(stashed_var);
}

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
