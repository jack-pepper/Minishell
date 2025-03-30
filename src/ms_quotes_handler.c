/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_quotes_handler.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmalie <mmalie@student.42nice.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/30 14:16:00 by mmalie            #+#    #+#             */
/*   Updated: 2025/03/30 17:48:54 by mmalie           ###   ########.fr       */
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
			ft_replace_char(&line[(*i)], '*');
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
			}
			ft_replace_char(&line[(*i)], '*');
			(*i)++;
		}
	}
	(*to_collapse)++;
}

void	ft_replace_char(char *cur_c, char new_c)
{
	if (ft_isspace(*cur_c))
		*cur_c = new_c; // DEBUG: replace eventually by ctrl char (non-printable)
}
