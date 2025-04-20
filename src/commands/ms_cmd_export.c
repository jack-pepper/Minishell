/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_cmd_export.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmalie <mmalie@student.42nice.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 13:03:40 by mmalie            #+#    #+#             */
/*   Updated: 2025/04/21 00:15:03 by mmalie           ###   ########.fr       */
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
			if (ft_strchr(sh->input_args[i], '=') == NULL)
			{
				stashed_var = ft_getenv(sh->input_args[i], &sh->env_stash);
				if (stashed_var != NULL)
					export_from_stash(sh, stashed_var);
				i++;
			}
			else
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
	split_str = ft_split(sh->input_args[(*i)], '=');
	if (!split_str)
		return ;
	set_var = ft_getenv(split_str[0], &sh->this_env);
	if (set_var != NULL)
	{
		if (ft_update_env_value(set_var, split_str) != 0)
			return ;
	}
	else
		add_new_env_var(sh, split_str);
}

void	export_from_stash(t_shell *sh, t_list *stashed_var)
{
	t_list	*set_var;
	t_list	*prev_node;

	set_var = ft_getenv(((char **)stashed_var->content)[0], &sh->this_env);
	if (set_var != NULL)
		ft_update_env_value(set_var, (char **)stashed_var->content);
	else
		add_new_env_var(sh, (char **)stashed_var->content);
	prev_node = sh->env_stash;
	if (stashed_var == sh->env_stash)
	{
		sh->env_stash = &(*stashed_var->next);
		free(stashed_var);
	}
	else
	{
		if (prev_node != NULL && prev_node->next != NULL)
		{
			while (ft_strcmp(((char **)prev_node->next->content)[0],
				(((char **)stashed_var->content)[0])) != 0)
				prev_node = prev_node->next;
		}
		prev_node->next = stashed_var->next;
	}
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

int	ft_update_env_value(t_list *set_var, char **split_str)
{
	size_t	len;
	int		i;

	free_args(set_var->content);
	set_var->content = malloc(sizeof(char *) * (ft_strslen(split_str) + 1));
	if (!set_var->content)
	{
		free_args(split_str);
		return (-1);
	}
	i = 0;
	len = 0;
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
	free_args(split_str);
	return (0);
}
