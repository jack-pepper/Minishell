/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_quotes_handler.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmalie <mmalie@student.42nice.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 12:57:41 by mmalie            #+#    #+#             */
/*   Updated: 2025/05/16 20:27:44 by mmalie           ###   ########.fr       */
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
		return (NULL);
	ft_replace_char(start, CTRL_CHAR_VAR);
	len = (size_t)(&line[(*i)] - start);
	
	printf("start: %s\n - len: %lu\n", start, len);
	substr = ft_substr(line, (*i) - len, len);
	printf("substr: %s\n", substr);

	flagged_substr = ft_strjoin(substr, (char[]) { CTRL_CHAR_VAR, '\"', '\0'});
	free(substr);
	if (!flagged_substr)
		return (NULL);
	printf("flagged_substr: %s\n", flagged_substr);

	updated_line = malloc(ft_strlen(line) + 2);
	if (!updated_line)
	{
		free(flagged_substr);
		return (NULL);
	}
	int	j;
	j = 0;
	int	k;
	k = 0;
	int	l;
	l = 0;
	while (line[j])
	{
		if (line[j] == CTRL_CHAR_VAR)
		{
			updated_line[k++] = flagged_substr[l++];
			while (flagged_substr[l])
				updated_line[k++] = flagged_substr[l++];
			j = j + ft_strlen(flagged_substr) - 1;
			free(flagged_substr);
		}
		else
			updated_line[k++] = line[j++];
	}
	updated_line[k] = '\0';
	printf("updated_line: %s\n", updated_line);
	return (updated_line);
}

void	handle_quote(char *line, char quote_type, int *i)
{
	(*i)++;
	if (quote_type == '\'')
	{
		while (line[(*i)] != '\'')
			ft_replace_if_space(&line[(*i)++], CTRL_CHAR_SPACE_IN_QUOTE);
	}
	else if (quote_type == '\"')
	{
		while (line[(*i)] != '\"' && line[(*i)] != CTRL_CHAR_VAR)
		{
			if (line[(*i)] == '$'
				&& line[(*i + 1)] != '\"'
				&& line[(*i + 1)] != ' '
				&& line[(*i + 1)] != CTRL_CHAR_VAR)
				ft_replace_char(&line[(*i)++], CTRL_CHAR_VAR_TO_INTERPRET);
			if (line[(*i)] != '\"' && line[(*i)] != CTRL_CHAR_VAR)
				ft_replace_if_space(&line[(*i)++], CTRL_CHAR_SPACE_IN_QUOTE);
		}
	}
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
	else if (src[(*i)] == CTRL_CHAR_VAR)
	{
		src[(*i)] = CTRL_CHAR_STICKY_VAR;
		while (src[(*i)] != '\"' && src[(*i)] != CTRL_CHAR_VAR)
		{
			printf("[%c] ", src[(*i)]);
			dst[(*j)++] = src[(*i)++];
		}
		if (src[(*i)] == CTRL_CHAR_VAR)
			src[(*i)] = CTRL_CHAR_STICKY_VAR;
		else
			(*i)++;
		while (src[(*i)] && src[(*i)] != ' ')
		{
			printf("[%c] ", src[(*i)]);
			dst[(*j)++] = src[(*i)++];
		}
		
	}
	else if (src[(*i)] == '\"' && ft_strrchr(src, CTRL_CHAR_VAR) != NULL)
	{
		(*i)++;
		while (src[(*i)] != CTRL_CHAR_VAR)
		{
			printf("[%c] ", src[(*i)]);
			dst[(*j)++] = src[(*i)++];
		}
		src[(*i)] = CTRL_CHAR_STICKY_VAR;
		while (src[(*i)] && src[(*i)] != ' ')
		{
			printf("[%c] ", src[(*i)]);
			dst[(*j)++] = src[(*i)++];
		}
	}
	else if (src[(*i)] == '\"' && ft_count_char(&src[(*i)], '\"') > 1)
	{
		(*i)++;
		while (src[(*i)] != '\"')
			dst[(*j)++] = src[(*i)++];
		(*i)++;
	}
}
