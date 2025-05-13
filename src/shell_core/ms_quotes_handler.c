/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_quotes_handler.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmalie <mmalie@student.42nice.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 12:57:41 by mmalie            #+#    #+#             */
/*   Updated: 2025/05/13 22:50:05 by mmalie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

// Interpret $VAR prematurely to solve echo "$HO"ME showing "$HOME" instead of "ME"
/*	premature_env()
{
	char	*start;
	char	*end;
	// if there is a $ in the current quote
	start = ft_strchr(line, );
	ft_str)
	// split line to keep the $VAR (end: closing double quotes ") 

	// interpret the env

	// rejoin
}
*/
void	handle_quote(char *line, char quote_type, int *i, int *to_collapse)
{
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
//		if (line[(*i) + 1] && line[(*i) + 1] != ' ')
//			premature_env(line, i);
	}
	(*to_collapse)++;
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
