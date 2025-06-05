/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_input_manager_converter.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-bouk <yel-bouk@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 16:34:25 by yel-bouk          #+#    #+#             */
/*   Updated: 2025/06/05 16:47:26 by yel-bouk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

char	**env_list_to_array(t_list *env_list)
{
	t_list	*cur;
	char	**env_array;
	int		count;
	int		i;

	count = 0;
	cur = env_list;
	while (cur)
	{
		count++;
		cur = cur->next;
	}
	env_array = malloc(sizeof(char *) * (count + 1));
	if (!env_array)
		return (NULL);
	i = 0;
	cur = env_list;
	while (cur)
	{
		env_array[i] = ft_strjoin_delim(((char **)cur->content)[0],
				((char **)cur->content)[1], "=");
		if (!env_array[i])
		{
			while (--i >= 0)
				free(env_array[i]);
			free(env_array);
			return (NULL);
		}
		i++;
		cur = cur->next;
	}
	env_array[i] = NULL;
	return (env_array);
}

void	free_env_array(char **env_array)
{
	int	i;

	if (!env_array)
		return ;
	i = 0;
	while (env_array[i])
	{
		free(env_array[i]);
		i++;
	}
	free(env_array);
}