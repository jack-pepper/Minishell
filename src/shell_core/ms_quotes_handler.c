/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_quotes_handler.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmalie <mmalie@student.42nice.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 12:57:41 by mmalie            #+#    #+#             */
/*   Updated: 2025/05/18 10:44:41 by mmalie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	handle_quote(char *line, char quote_type, int *i)
{
	(*i)++;
	if (quote_type == '\'')
	{
		while (line[(*i)] != '\'')
		{
			ft_replace_if_space(&line[(*i)++], CC_SPACE_IN_QUOTE);
		}
	}
	else if (quote_type == '\"')
	{
		while (line[(*i)] != '\"' && line[(*i)] != CC_VAR_BOUND)
		{
			if (line[(*i)] == '$'
				&& line[(*i + 1)] != '\"'
				&& line[(*i + 1)] != ' '
				&& line[(*i + 1)] != CC_VAR_BOUND)
				ft_replace_char(&line[(*i)++], CC_VAR_TO_INTERPRET);
			if (line[(*i)] != '\"' && line[(*i)] != CC_VAR_BOUND)
				ft_replace_if_space(&line[(*i)++], CC_SPACE_IN_QUOTE);
		}
	}
}

void	ante_merge_quote(char *line, int *i)
{
	int	temp_i;

	if ((*i) > 0 && line[(*i) - 1] != ' ' && line[(*i) - 1] != '\"'
		&& ft_strchr(line, CC_VAR_TO_INTERPRET) != NULL)
	{
		temp_i = (*i);
		while (temp_i > 0 && line[temp_i] != ' ')
		{
			temp_i--;
			if (line[temp_i] == CC_VAR_TO_INTERPRET)
			{
				ft_replace_char(&line[(*i)], CC_VAR_BOUND);
				break ;
			}
		}
	}
}

void	post_merge_quote(char *line, int *i)
{
	int	temp_i;

	if (line[(*i) + 1] && line[(*i) + 1] != ' ' && line[(*i) + 1] != '\"'
		&& ft_strchr(line, CC_VAR_TO_INTERPRET) != NULL)
	{
		temp_i = (*i) - 1;
		while (temp_i >= 0 && line[temp_i] != '\"'
			&& line[temp_i] != CC_VAR_BOUND && line[temp_i] != ' ')
		{
			temp_i--;
			if (line[temp_i] == CC_VAR_TO_INTERPRET)
			{
				ft_replace_char(&line[(*i)], CC_VAR_BOUND);
				break ;
			}
		}
		while (line[(*i)] != '\0' && line[(*i)] != ' ')
		{
			if (line[(*i)] == '$' && line[(*i) + 1] != '\"'
				&& line[(*i) + 1] != ' '
				&& line[(*i) + 1] != CC_VAR_BOUND)
				ft_replace_char(&line[(*i)++], CC_VAR_TO_INTERPRET);
			(*i)++;
		}
		(*i)--;
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
	else if (src[(*i)] == CC_VAR_BOUND)
	{
		src[(*i)] = CC_STICKY_VAR;
		while (src[(*i)] != '\"' && src[(*i)] != CC_VAR_BOUND)
		{
			printf("[%c] ", src[(*i)]);
			dst[(*j)++] = src[(*i)++];
		}
		if (src[(*i)] == CC_VAR_BOUND)
			src[(*i)] = CC_STICKY_VAR;
		else
			(*i)++;
		while (src[(*i)] && src[(*i)] != ' ')
		{
			printf("[%c] ", src[(*i)]);
			dst[(*j)++] = src[(*i)++];
		}	
	}
	else if (src[(*i)] == '\"' && ft_strrchr(src, CC_VAR_BOUND) != NULL)
	{
		(*i)++;
		while (src[(*i)] != CC_VAR_BOUND)
		{
			printf("[%c] ", src[(*i)]);
			dst[(*j)++] = src[(*i)++];
		}
		src[(*i)] = CC_STICKY_VAR;
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
