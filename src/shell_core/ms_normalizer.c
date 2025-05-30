/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_normalizer.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-bouk <yel-bouk@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 23:57:21 by mmalie            #+#    #+#             */
/*   Updated: 2025/05/25 18:15:33 by mmalie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

// Returns a normalized (trimmed, flagged and spaced) line
char	*ft_normalize(char *line)
{
	char	*trimmed;
	char	*flagged;
	char	*spaced;

	trimmed = ft_strtrim(line, " \f\n\r\t\v");
	if (trimmed == NULL)
		return (NULL);
	flagged = ft_strflag(trimmed);
	free(trimmed);
	if (flagged == NULL)
		return (NULL);
	spaced = ft_add_spaces_around(flagged, CC_PIPE);
	free(flagged);
	if (!spaced)
		return (NULL);
	spaced = ft_add_spaces_around_redir(spaced);
	return (spaced);
}

// If there is more than one consecutive space, they are suppressed.
char	*ft_strflag(char *line)
{
	char	*flagged_line;
	size_t	line_len;
	int		i;

	flagged_line = NULL;
	i = 0;
	while (line[i])
	{
		if (line[i] == '\0')
			break ;
		if (line[i] == '$')
			flag_dollar(line, &i);
		else if (ft_is_in_set(line[i], "|<>"))
			flag_pipe_and_redir(line, &i);
		else if (ft_is_in_set(line[i], "\"\'"))
			flag_quote(line, &i);
		i++;
	}
	ft_replace_chars_in_str(line, CC_DOLLAR_UNCLOSED, '$');
	line_len = ft_strlen(line);
	flagged_line = malloc(sizeof(char) * ((line_len + 1)));
	if (!flagged_line)
		return (NULL);
	flagged_line = copy_collapse(flagged_line, line, line_len);
	return (flagged_line);
}

// Both the src and dst are expected to be non-NULL and their size is correct
char	*copy_collapse(char *dst, char *src, size_t src_len)
{
	size_t	i;
	size_t	j;

	if (!src)
		return (NULL);
	ft_init_two_size_t(0, &i, &j);
	while (i < src_len)
	{
		if ((src[i + 1]
				&& (ft_is_in_set(src[i], "\"\'")
					|| src[i] == CC_VAR_BOUND
					|| src[i] == CC_VAR_BOUND_SQUOTE)))
			pass_quotes(dst, src, &i, &j);
		else if ((ft_isspace(src[i]) && ft_isspace(src[i + 1]))
			|| (src[i] == CC_LONE_DOLLAR))
		{
			i++;
		}
		else
			dst[j++] = src[i++];
	}
	dst[j] = '\0';
	return (dst);
}
