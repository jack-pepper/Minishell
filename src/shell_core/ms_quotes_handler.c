/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_quotes_handler.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmalie <mmalie@student.42nice.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 12:57:41 by mmalie            #+#    #+#             */
/*   Updated: 2025/05/14 14:51:21 by mmalie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

// Interpret $VAR prematurely to solve echo "$HO"ME showing "$HOME" instead of "ME"
char	*flag_edge_var(char *line, int *i)
{
	char	*updated_line;
	char	*substr;
	char	*flagged_substr;
	char	*start;
	size_t	len;

	// if there is a $ in the current quote
	start = ft_strrchr(line, CTRL_CHAR_VAR_TO_INTERPRET);
	if (start == NULL)
		return (line);
	ft_replace_char(start, 'X');	
	len = (size_t)(&line[(*i)] - start);
	
	printf("start: %s\n - len: %lu\n", start, len);
	substr = ft_substr(line, (*i) - len, len);
	printf("substr: %s\n", substr);

	flagged_substr = ft_strjoin(substr, "X");
	free(substr);
	if (!flagged_substr)
		return (line);
	printf("flagged_substr: %s\n", flagged_substr);

	updated_line = malloc(ft_strlen(line) + 3);
	if (!updated_line)
	{
		free(flagged_substr);
		return (line);
	}
	int	j;
	j = 0;
	int	k;
	k = 0;
	int	l;
	l = 0;
	while (line[j])
	{
		if (line[j] == 'X')
		{
			updated_line[k++] = flagged_substr[l++];
			while (flagged_substr[l])
				updated_line[k++] = flagged_substr[l++];
			j = j + ft_strlen(flagged_substr);
			free(flagged_substr);
		}
		else
			updated_line[k++] = line[j++];
	}
	updated_line[k] = '\0';
	printf("updated_line: %s\n", updated_line);
	return (updated_line);
}

char	*handle_quote(char *line, char quote_type, int *i, int *to_collapse)
{
	char	*updated_line;

	updated_line = NULL;
	(*i)++;
	(*to_collapse)++;
	if (quote_type == '\'')
	{
		while (line[(*i)] != '\'')
			ft_replace_if_space(&line[(*i)++], CTRL_CHAR_SPACE_IN_QUOTE);
	}
	else if (quote_type == '\"')
	{
		while (line[(*i)] != '\"')
		{
			if (line[(*i)] == '$'
				&& line[(*i + 1)] != '\"'
				&& line[(*i + 1)] != ' ')
				ft_replace_char(&line[(*i)++], CTRL_CHAR_VAR_TO_INTERPRET);
			if (line[(*i)] != '\"')
				ft_replace_if_space(&line[(*i)++], CTRL_CHAR_SPACE_IN_QUOTE);
		}
		// echo "$HO"ME case
		if (line[(*i) + 1] && line[(*i) + 1] != ' ')
		{
			updated_line = flag_edge_var(line, i);
			//free(line);
			//line = ft_strdup(updated_line); // need to pass it to solve leak
			//free(updated_line);
			(*i) = (*i) + 2;
		}
	}
	(*to_collapse)++;
	if (updated_line == NULL)
		return (line);
	else
		return (updated_line);
}

void	pass_quotes(char *dst, char *src, size_t *i, size_t *j)
{
	if (src[(*i)] == '\'' && ft_count_char(&src[(*i)], '\'') > 1)
	{
		(*i)++;
		while (src[(*i)] != '\'')
			dst[(*j)++] = src[(*i)++];
		(*i)++;
	}
	else if (src[(*i)] == '\"' && ft_count_char(&src[(*i)], '\"') > 1)
	{
		(*i)++;
		while (src[(*i)] != '\"')
			dst[(*j)++] = src[(*i)++];
		(*i)++;
	}
}
