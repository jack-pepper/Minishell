/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_env_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmalie <mmalie@student.42nice.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 11:12:12 by mmalie            #+#    #+#             */
/*   Updated: 2025/04/04 13:40:27 by mmalie           ###   ########.fr       */
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

// Copy the content of strs (in Minishell: env) to a list.
int	ft_copy_strs_to_list(t_list **list, char **strs, size_t nb_of_strings, char delim)
{
	t_list	*node;
        char	**split_str;
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
			node = ft_lstnew((char **)split_str);
		}
		if (!node)
			return (-1);
		ft_lstadd_back(list, node);
		i++;
	}
	return (0);
}

t_list  *ft_getenv(char *var_name, t_list **this_env)
{
	t_list	*cur_node;
	int	res;

	cur_node = *this_env;
	while (cur_node != NULL)
	{
		res = ft_strncmp(var_name, ((char **)cur_node->content)[0], ft_strlen(((char **)cur_node->content)[0]));
		if (res == 0)
			return (cur_node);
		cur_node = cur_node->next;
	}
	return (NULL);
}

/*int	ft_copy_strs_to_list(t_list **list, char **strs, size_t nb_of_strings)
{
	t_list	*node;
	size_t	i;
        
	i = 0;
	node = NULL;
	while (i < nb_of_strings)
	{
		//printf("env[%ld]: %s\n", i, env[i]);
		node = ft_lstnew((char *)strs[i]);
		if (!node)
			return (-1);
		ft_lstadd_back(list, node);
		i++;
	}
	return (0);
}*/
