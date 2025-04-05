/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_quotes_handler.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmalie <mmalie@student.42nice.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/30 14:16:00 by mmalie            #+#    #+#             */
/*   Updated: 2025/04/05 16:58:08 by mmalie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	handle_quote(char *line, char quote_type, int *i, int *to_collapse)
{
	(*i)++;
	(*to_collapse)++;
	if (quote_type == '\'')
	{
		printf("[DEBUG] quote_type: single ($ IS NOT interpreted)\n");
		while (line[(*i)] != '\'')
		{
			ft_replace_space(&line[(*i)], CTRL_CHAR_SPACE_IN_QUOTE);
			(*i)++;
		}
	}
	else if (quote_type == '\"')
	{
		printf("[DEBUG] quote_type: double ($ IS interpreted)\n");
		while (line[(*i)] != '\"')
		{
			if (line[(*i)] == '$') // Should expand to the env
			{
				printf("$ found!\n");
				(*i)++;
				//ft_interpret_env(&line[(*i)], i);
			}
			ft_replace_space(&line[(*i)], CTRL_CHAR_SPACE_IN_QUOTE);
			(*i)++;
		}
	}
	(*to_collapse)++;
}

void	ft_replace_space(char *cur_c, char new_c)
{
	if (ft_isspace(*cur_c))
		*cur_c = new_c; // DEBUG: replace eventually by ctrl char (non-printable)
}
