/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_normalizer.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmalie <mmalie@student.42nice.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 23:57:21 by mmalie            #+#    #+#             */
/*   Updated: 2025/04/14 20:36:26 by mmalie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

// Whitespace trim and collapse
char	*ft_normalize(char *line)
{
	char	*trimmed_line;
	char	*normalized_line;

	trimmed_line = NULL;
	normalized_line = NULL;
	trimmed_line = ft_strtrim(line, " \f\n\r\t\v");
	if (trimmed_line == NULL)
	{
		return (NULL);
	}
	normalized_line = ft_strcollapse(trimmed_line);
	free(trimmed_line);
	if (normalized_line == NULL)
	{
		return (NULL);
	}
	return (normalized_line);
}

// If there is more than one consecutive space, they are suppressed.
char	*ft_strcollapse(char *line)
{
	char	*collapsed_line;
	size_t	line_len;
	int		to_collapse;
	int		i;

	collapsed_line = NULL;
	line_len = ft_strlen(line);
	ft_init_two_ints(0, &i, &to_collapse);
	while (line[i] != '\0')
	{
		if (line[i] == '$')
			ft_replace_char(&line[i], CTRL_CHAR_VAR_TO_INTERPRET);
		else if (line[i] == '\'' && ft_count_char(&line[i], '\'') > 1)
			handle_quote(line, '\'', &i, &to_collapse);
		else if (line[i] == '\"' && ft_count_char(&line[i], '\"') > 1)
			handle_quote(line, '\"', &i, &to_collapse);
		else if (ft_isspace(line[i]) && ft_isspace(line[i + 1]))
			to_collapse++;
		i++;
	}
	collapsed_line = malloc(sizeof(char) * ((line_len - to_collapse) + 1));
	if (!collapsed_line)
		return (NULL);
	collapsed_line = copy_collapse(collapsed_line, line, line_len);
	return (collapsed_line);
}

int	ft_count_char(char *str, char c)
{
	size_t	str_len;
	size_t	i;
	int		count;

	str_len = ft_strlen(str);
	i = 0;
	count = 0;
	while (i < str_len)
	{
		if (str[i] == c)
			count++;
		i++;
	}
//	printf("[DEBUG] ~%c~ found %u times in ~%s~\n", c, count, str);
	return (count);
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
		if (src[i] == '\'' && ft_count_char(&src[i], '\'') > 1)
		{
			i++;
			while (src[i] != '\'')
				dst[j++] = src[i++];
			i++;
		}
		else if (src[i] == '\"' && ft_count_char(&src[i], '\"') > 1)
		{
			i++;
			while (src[i] != '\"')
				dst[j++] = src[i++];
			i++;
		}
		else if (ft_isspace(src[i]) && ft_isspace(src[i + 1]))
			i++;
		else
			dst[j++] = src[i++];
	}
	dst[j] = '\0';
	return (dst);
}
