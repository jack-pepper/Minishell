/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_env_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmalie <mmalie@student.42nice.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 11:12:12 by mmalie            #+#    #+#             */
/*   Updated: 2025/04/20 16:45:38 by mmalie           ###   ########.fr       */
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
int	ft_strstolist(t_list **list, char **strs, size_t nb_strs, char delim)
{
	char	**split_str;
	t_list	*node;
	size_t	i;

	i = 0;
	node = NULL;
	split_str = NULL;
	while (i < nb_strs)
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
		}
		if (!node)
			return (-1);
		ft_lstadd_back(list, node);
		i++;
	}
	return (0);
}

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
		((char **)set_var->content)[i] = malloc(sizeof(char) * (len + 1));
		if (!((char **)set_var->content)[i])
		// should I clean up partially allocated here?
			return (-1);
		ft_strlcpy(((char **)set_var->content)[i], split_str[i], len + 1);
		i++;
	}
	((char **)set_var->content)[i] = NULL;
	free_args(split_str);
	return (0);
}

// Return the first str in strs that either contains no c or at forbidden_pos
char	**ft_strschr(char **strs, char c, int forbidden_pos)
{
	int	i;
//	int	j;

	i = 0;
	if (forbidden_pos >= 0)
	{
		while (strs[i] != NULL)
		{
			if (((int)ft_strlen(strs[i]) >= forbidden_pos)
				&& (strs[i][forbidden_pos] == c))
			{
				printf("Found %c at forbidden pos strs[%d][%d]\n", c, i, forbidden_pos);
				return (&(strs[i]));
			}
			i++;
		}
	}
	i = 0;
	while (strs[i] != NULL)
	{
		if (ft_strchr(strs[i], c) == NULL)
		{
			printf("Char not found at strs[%d]\n", i);
			return (&(strs[i]));
		}
		i++;
	}
	return (NULL);
}
