/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_cmd_export.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmalie <mmalie@student.42nice.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 13:03:40 by mmalie            #+#    #+#             */
/*   Updated: 2025/05/05 12:19:04 by mmalie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	cmd_export(t_shell *sh)
{
	t_list	*stashed_var;
	size_t	i;
	int	res;

	if (!(sh->input_args[1]))
		cmd_env((sh));
	else
	{
		i = 1;
		while (sh->input_args[i] != NULL)
		{
			if (sh->input_args[i][0] == '=' || ft_isdigit(sh->input_args[i][0]))
				return (ft_ret(1, EXPORT_INVALID_ID, STDERR));
			if (ft_strchr(sh->input_args[i], '=') == NULL) // cmd: export var_name
			{
				if (ft_strpbrk(sh->input_args[i], "-") != NULL)
					return (ft_ret(1, EXPORT_INVALID_ID, STDERR));
				stashed_var = ft_getenv(sh->input_args[i], &sh->env_stash);
				if (stashed_var != NULL) // if var_name found in stash
					export_from_stash(sh, stashed_var); 
				i++;
			}
			else // cmd: export var_name=[value]
			{
				res = export_from_term(sh, &i);
				if (res != 0)
					return (res);
				i++;
			}
		}
	}
	return (0);
}

int	export_from_term(t_shell *sh, size_t *i)
{
	t_list	*set_var;
	char	**split_str;

	split_str = NULL;
	split_str = ft_split(sh->input_args[(*i)], '='); // split the "var_name=[value]" str
	if (!split_str)
		return (1);
	if (!is_valid_env_name(split_str[0]))
	{
		free_args(split_str);
		return (ft_ret(1, EXPORT_INVALID_ID, STDERR));
	}
	set_var = ft_getenv(split_str[0], &sh->this_env);
	if (set_var != NULL) // if var_name found in this_env
	{
		if (ft_update_env_value(set_var, split_str) != 0)
		{
			free_args(split_str);
			return (1) ;
		}
	}
	else // if var_name not found in this_env
		add_new_env_var(sh, split_str);
	return (0);
}

void	export_from_stash(t_shell *sh, t_list *stashed_var)
{
	t_list	*set_var;
	t_list	*prev_node;
	char	**new_content;

	set_var = ft_getenv(((char **)stashed_var->content)[0], &sh->this_env);
	new_content = ft_strsdup((char **)stashed_var->content);
	if (!new_content)
		return ;
	if (set_var != NULL) // if stashed_var_name found in this_env
		ft_update_env_value(set_var, new_content);
	else // if not
		add_new_env_var(sh, new_content);
	prev_node = sh->env_stash;

	// if the stashed var is the first of the list, move the head to next node 
	if (stashed_var == sh->env_stash)
		sh->env_stash = stashed_var->next;
	else // if not, find the node before stashed var and move next to stashed var next
	{
		while (prev_node && prev_node->next != stashed_var)
			prev_node = prev_node->next;
		if (prev_node)
			prev_node->next = stashed_var->next;
	}
	if (stashed_var->content)
		free_args((char **)stashed_var->content);
	free(stashed_var);
}

int	is_valid_env_name(char *var_name)
{
	size_t	i;

	i = 0;
	if (!var_name || var_name[0] == '\0'
		|| var_name[0] == '='
		|| ft_isdigit(var_name[0]))
		return (0);
	while (var_name[i])
	{
		if (!ft_isalnum(var_name[i])
			&& var_name[i] != '_')
			return (0);
		i++;
	}
	return (1);
}
