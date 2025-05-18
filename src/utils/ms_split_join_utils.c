/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_split_join_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-bouk <yel-bouk@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 21:05:23 by mmalie            #+#    #+#             */
/*   Updated: 2025/05/18 18:21:33 by mmalie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

char	*join_all_subargs(char **args, char delim)
{
	char	*result;
	char	*temp;
	int		i;

	result = ft_strdup("");
	if (!result)
		return (NULL);
	i = 0;
	while (args[i])
	{
		temp = ft_strjoin(result, args[i]);
		free(result);
		result = temp;
		if (delim != 'n' && args[i + 1])
		{
			temp = ft_strjoin(result, (char[]){delim, '\0'});
			free(result);
			result = temp;
		}
		i++;
	}
	return (result);
}

char	*ft_strjoin_delim(char const *s1, char const *s2, char const *delim)
{
	char	*joined_str;
	size_t	len;
	size_t	s1_len;
	size_t	s2_len;

	ft_init_two_size_t(0, &s1_len, &s2_len);
	if (s1 != NULL)
		s1_len = ft_strlen(s1);
	if (s2 != NULL)
		s2_len = ft_strlen(s2);
	if (s1 && s2)
		len = s1_len + ft_strlen(delim) + s2_len;
	else
		len = s1_len + s2_len;
	joined_str = (char *)malloc(sizeof(char) * (len + 1));
	if (joined_str == NULL)
		return (NULL);
	if (s1 != NULL)
	{
		ft_strlcpy(joined_str, s1, len + 1);
		ft_strlcat(joined_str, delim, len + 1);
	}
	if (s2 != NULL)
		ft_strlcat(joined_str, s2, len + 1);
	return (joined_str);
}

char	*ft_rejoin_subarg(char *arg, char *rejoined_arg)
{
	char	*temp;

	temp = ft_strjoin(rejoined_arg, arg);
	free(rejoined_arg);
	if (!temp)
		return (NULL);
	rejoined_arg = ft_strdup(temp);
	free(temp);
	return (rejoined_arg);
}
