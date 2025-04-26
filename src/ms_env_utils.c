/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_env_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmalie <mmalie@student.42nice.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 11:12:12 by mmalie            #+#    #+#             */
/*   Updated: 2025/04/26 17:45:38 by mmalie           ###   ########.fr       */
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
			//	printf("Found %c at forbidden pos strs[%d][%d]\n", c, i, forbidden_pos);
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
		//	printf("Char not found at strs[%d]\n", i);
			return (&(strs[i]));
		}
		i++;
	}
	return (NULL);
}

char	**ft_strsdup(char **strs)
{
	char	**dup_strs;
	size_t	nb_strs;
	size_t	i;
	size_t	j;

	nb_strs = ft_strslen(strs);
	dup_strs = malloc(sizeof(char *) * (nb_strs + 1));
	if (!dup_strs)
		return (NULL);
	i = 0;
	while (i < nb_strs)
	{
		dup_strs[i] = ft_strdup(strs[i]);
		if (!dup_strs[i])
		{
			j = 0;
			while (j < i)
			{
				free(dup_strs[j]);
				j++;
			}
			free(dup_strs);
			return (NULL);
		}
		i++;
	}
	dup_strs[i] = NULL;
	return (dup_strs);
}

void    stash_var(t_shell *sh)
{
        t_list  *stashed_var;
        t_list  *node;
        char    **split_str;
        size_t  i;
        size_t  nb_args;

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
			//free_args(split_str);
                }
                else
                {
		//	char **node_content = NULL;
		//	node_content = ft_strsdup(split_str);
                        node = ft_lstnew((char **)split_str);
			ft_show_strs((char **)node->content, "[DEBUG] node_content -");
			//printf("[DEBUG] node content: %s\n", (char **)node->content);
			//node = ft_lstnew((char **)split_str); // leak...
                        if (!node)
			{
				free_args(split_str);
                                return ;
			}
                        ft_lstadd_back(&sh->env_stash, node);
                }
		//free_args(split_str);
                i++;
        }
        return ;
}
