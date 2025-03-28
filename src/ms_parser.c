/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_parser.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmalie <mmalie@student.42nice.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 23:57:21 by mmalie            #+#    #+#             */
/*   Updated: 2025/03/28 19:47:32 by mmalie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

// Whitespace trim and collapse
char	*ft_normalize(char *line)
{
	char	*trimmed_line;
	char	*normalized_line;

	trimmed_line = NULL;
	trimmed_line = ft_strtrim(line, " \f\n\r\t\v");
	if (trimmed_line == NULL)
	{
		return (NULL);
	}
	normalized_line = ft_strcollapse(trimmed_line);
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
	int	to_collapse;
	int	i;

	collapsed_line = NULL;
	line_len = ft_strlen(line);
	to_collapse = 0;
	i = 0;
	while (line[i] != '\0')
	{
		if (ft_isspace(line[i]) && ft_isspace(line[i + 1]))
			to_collapse++;
		i++;
	}
	collapsed_line = malloc(sizeof(char) * ((line_len - to_collapse) + 1));
	if (!collapsed_line)
		return (NULL);
	collapsed_line = copy_collapse(collapsed_line, line, line_len);
	return (collapsed_line);
}

// Both the src and dst are expected to be non-NULL and their size is correct
char	*copy_collapse(char *dst, char *src, size_t src_len)
{
	size_t	i;
	size_t	j;

	if (!src)
		return (NULL);
	i = 0;
	j = 0;
	while (i < src_len)
	{
		if (ft_isspace(src[i]) && ft_isspace(src[i + 1]))
			i++;
		else
		{
			dst[j] = src[i];
			j++;
			i++;
		}
	}
	dst[j] = '\0';
	return (dst);
}
