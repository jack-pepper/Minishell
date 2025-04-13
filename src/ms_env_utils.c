/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_env_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmalie <mmalie@student.42nice.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 11:12:12 by mmalie            #+#    #+#             */
/*   Updated: 2025/04/14 01:01:50 by mmalie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

// Return the number of strings pointed by strs
size_t	ft_strslen(char **strs)
{
	size_t	i;

	i = 0;
	while (strs[i] != NULL)
		i++;
	return (i);
}

// Copy the content of strs (here: env) to a t_list.
int	ft_copy_strs_to_list(t_list **list, char **strs, size_t nb_of_strings, char delim)
{	
	char	**split_str;
	t_list	*node;
	size_t	i;
	
	i = 0;
	node = NULL;
	split_str = NULL;
	while (i < nb_of_strings)
	{
		//printf("env[%ld]: %s\n", i, env[i]);
		if (!delim)
			node = ft_lstnew((char *)strs[i]);
		else
		{
			split_str = ft_split(strs[i], delim);
			if (!split_str)
				return (-1);
			node = ft_lstnew((char **)split_str);
		
			// solves leaks on exit... but creates issues with other cmds	
			//int j = 0;
			//while (split_str[j] != NULL)
			//{
			//	free(split_str[j]);
			//	j++;
			//}
	//		free(split_str[0]);
	//		free(split_str[1]);
	//		free(split_str);
		}
		if (!node)
			return (-1);
		ft_lstadd_back(list, node);
		i++;
	}
	//free(split_str);
	return (0);
}

// Custom implementation of getenv to return a t_list
t_list  *ft_getenv(char *var_name, t_list **this_env)
{
	t_list	*cur_node;
	int	res;

	cur_node = *this_env;
	while (cur_node != NULL)
	{
		res = ft_strncmp(var_name, ((char **)cur_node->content)[0], ft_strlen(var_name));
		//printf("var_name: %s - cur_node->content: %s - Res: %d\n",  var_name, ((char **)cur_node->content)[0], res);
		if (res == 0)
			return (cur_node);
		cur_node = cur_node->next;
	}
	return (NULL);
}

// Update set_var with the name and value pointed by split_str
int	ft_update_env_value(t_list *set_var, char **split_str)
{
        free(set_var->content);
        set_var->content = malloc(sizeof(split_str));
        if (!set_var->content)
                return (-1);
        set_var->content = split_str;
        return (0);
}
