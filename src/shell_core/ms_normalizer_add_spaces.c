/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_normalizer_add_spaces.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-bouk <yel-bouk@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 23:57:21 by mmalie            #+#    #+#             */
/*   Updated: 2025/06/01 07:43:38 by yel-bouk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

char	*ft_add_spaces_around_redir(char *spaced)
{
	char	*tmp;
	char	*cc_append;
	char	*cc_heredoc;
	char	*cc_redir_in;
	char	*cc_redir_out;

	cc_append = ft_chrtostr(CC_APPEND);
	cc_heredoc = ft_chrtostr(CC_HEREDOC);
	cc_redir_in = ft_chrtostr(CC_REDIR_IN);
	cc_redir_out = ft_chrtostr(CC_REDIR_OUT);
	tmp = spaced;
	spaced = ft_add_spaces_around_str(tmp, cc_append);
	ft_free_two_str(tmp, cc_append);
	tmp = spaced;
	spaced = ft_add_spaces_around_str(tmp, cc_heredoc);
	ft_free_two_str(tmp, cc_heredoc);
	tmp = spaced;
	spaced = ft_add_spaces_around_str(tmp, cc_redir_in);
	ft_free_two_str(tmp, cc_redir_in);
	tmp = spaced;
	spaced = ft_add_spaces_around_str(tmp, cc_redir_out);
	ft_free_two_str(tmp, cc_redir_out);
	return (spaced);
}

char	*ft_add_spaces_around(char *str, char special)
{
	int		i;
	int		extra;
	int		len;
	char	*new_str;
	int		j;

	len = ft_strlen(str);
	extra = 0;
	i = 0;
	while (str[i])
	{
		if (str[i] == special)
			extra += 2;
		i++;
	}
	new_str = malloc(len + extra + 1);
	if (!new_str)
		return (NULL);
	j = 0;
	i = 0;
	while (str[i])
	{
		if (str[i] == special)
		{
			new_str[j++] = ' ';
			new_str[j++] = special;
			new_str[j++] = ' ';
		}
		else
			new_str[j++] = str[i];
		i++;
	}
	new_str[j] = '\0';
	return (new_str);
}

char	*ft_add_spaces_around_str(const char *line, const char *str)
{
	int		len;
	int		op_len;
	int		i;
	int		j;
	int		k;
	int		count;
	char	*result;

	count = 0;
	i = 0;
	op_len = strlen(str);
	len = strlen(line);
	while (line[i])
	{
		if (strncmp(&line[i], str, op_len) == 0)
			count++;
		i++;
	}
	if (count == 0)
		return (strdup(line));
	result = malloc(len + count * 2 + 1);
	if (!result)
		return (NULL);
	i = 0;
	j = 0;
	while (line[i])
	{
		if (strncmp(&line[i], str, op_len) == 0)
		{
			result[j++] = ' ';
			k = 0;
			while (k < op_len)
			{
				result[j++] = str[k];
				k++;	
			}
			result[j++] = ' ';
			i += op_len;
		}
		else
			result[j++] = line[i++];
	}
	result[j] = '\0';
	return (result);
}
