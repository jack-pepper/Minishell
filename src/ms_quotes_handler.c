/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_quotes_handler.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmalie <mmalie@student.42nice.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/30 14:16:00 by mmalie            #+#    #+#             */
/*   Updated: 2025/04/11 15:03:25 by mmalie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	handle_quote(char *line, char quote_type, int *i, int *to_collapse)
{
	(*i)++;
	(*to_collapse)++;

	if (quote_type == '\'')
	{
		while (line[(*i)] != '\'')
		{
			ft_replace_if_space(&line[(*i)], CTRL_CHAR_SPACE_IN_QUOTE);
			(*i)++;
		}
	}
	else if (quote_type == '\"')
	{
		while (line[(*i)] != '\"')
		{
			if (line[(*i)] == '$')
			{
				ft_replace_char(&line[(*i)], CTRL_CHAR_VAR_TO_INTERPRET);
				(*i)++;	
			}
			ft_replace_if_space(&line[(*i)], CTRL_CHAR_SPACE_IN_QUOTE);
			(*i)++;
		}
	}
	(*to_collapse)++;
}
