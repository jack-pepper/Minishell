/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_env_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmalie <mmalie@student.42nice.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 11:12:12 by mmalie            #+#    #+#             */
/*   Updated: 2025/05/05 11:53:19 by mmalie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

t_list	*ft_getenv(char *var_name, t_list **this_env)
{
	t_list	*cur_node;
	int		res;

	cur_node = *this_env;
	while (cur_node != NULL)
	{
		res = ft_strcmp(var_name, (((char **)cur_node->content)[0]));
		if (res == 0)
			return (cur_node);
		cur_node = cur_node->next;
	}
	return (NULL);
}

void	stash_var(t_shell *sh)
{
	t_list	*stashed_var;
	t_list	*node;
	char	**split_str;
	size_t	i;
	size_t	nb_args;

	i = 0;
	node = NULL;
	split_str = NULL;
	nb_args = ft_strslen(sh->input_args);
	while (i < nb_args)
	{
		split_str = ft_split(sh->input_args[i], '=');
		if (!split_str)
			return ;
		if (!is_valid_env_name(split_str[0]))
		{
			free_args(split_str);
			return ;
		}
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
			ft_show_strs((char **)node->content, "[DEBUG] node_content -");
			if (!node)
			{
				free_args(split_str);
				return ;
			}
			ft_lstadd_back(&sh->env_stash, node);
		}
		i++;
	}
	return ;
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
	if (set_var->content)
		free_args(set_var->content);
	set_var->content = split_str;
	return (0);
}
