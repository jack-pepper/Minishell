/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_quotes_handler.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmalie <mmalie@student.42nice.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 12:57:41 by mmalie            #+#    #+#             */
/*   Updated: 2025/05/25 01:05:11 by mmalie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	handle_quote(char *line, char quote_type, int *i)
{
	(*i)++;
	if (quote_type == '\'' && ft_count_char(&line[(*i - 1)], '\'') > 1)
	{
		while (line[(*i)] != '\'')
		{
			ft_replace_if_space(&line[(*i)++], CC_SPACE_IN_QUOTE);	
			if (DEBUG == 1)
				printf("---> handle_quote ('') / (line[%d] ~%c~)\n", (*i), line[(*i)]);
		}
	}
	else if (quote_type == '\"' && ft_count_char(&line[(*i - 1)], '\"') > 0)
	{
		while (line[(*i)] != '\"' && line[(*i)] != CC_VAR_BOUND)
		{
			if (line[(*i)] == '$' && ft_isalnum_x_chr(&line[(*i) + 1], "_?"))
			{
				ft_replace_char(&line[(*i)], CC_VAR_TO_INTERPRET);
				(*i)++;
				while (ft_isalnum_x_chr(&line[(*i)], "_?"))
					(*i)++;
				if (line[(*i)] == '\'')
				{
					if (DEBUG == 1)
						printf("SINGLE QUOTE!\n");
					line[(*i)] = CC_VAR_BOUND_SQUOTE;
				}
				else if (line[(*i)] == '\"')
				{
					if (DEBUG == 1)
						printf("DOUBLE QUOTE\n");
					line[(*i)] = CC_VAR_BOUND;
				}
				(*i)--;
			}
			else if (line[(*i)] != '\"' && line[(*i)] != CC_VAR_BOUND && line[(*i)] != CC_VAR_BOUND_SQUOTE)
				ft_replace_if_space(&line[(*i)], CC_SPACE_IN_QUOTE);	
			if (DEBUG == 1)
				printf("---> handle_quote ("") / (line[%d] ~%c~)\n", (*i), line[(*i)]);
			(*i)++;
		}
	}
}

void	ante_merge_quote(char *line, int *i)
{
	int	temp_i;

	if ((*i) > 0 && line[(*i) - 1] != ' ' && line[(*i) - 1] != '\"'
		&& line[(*i) - 1] != CC_VAR_BOUND
		&& ft_strchr(line, CC_VAR_TO_INTERPRET) != NULL)
	{
		temp_i = (*i);
		while (temp_i > 0 && line[temp_i] != ' ')
		{
			temp_i--;
			if (line[temp_i] == CC_VAR_TO_INTERPRET)
			{
				ft_replace_char(&line[(*i)], CC_VAR_BOUND);
				if (DEBUG == 1)
					printf("/ante_merge/ found CC_VAR_TO_INTERPRET at line[%d] (%c) ! (line[%d] ~%c~)\n", temp_i, line [(temp_i)], (*i), line[(*i)]);
				break ;
			}
		}
	}
}

void	post_merge_quote(char *line, int *i, int temp_i)
{
	if (line[(*i) + 1] && line[(*i) + 1] != ' '
		&& ft_strchr(line, CC_VAR_TO_INTERPRET) != NULL)
	{
		while (temp_i >= 0 && line[temp_i] != '\"'
			&& line[temp_i] != CC_VAR_BOUND && line[temp_i] != ' ')
		{
			temp_i--;
			if (line[temp_i] == CC_VAR_TO_INTERPRET)
			{
				ft_replace_char(&line[(*i)], CC_VAR_BOUND);
				if (DEBUG == 1)
					printf("/post_merge/ found CC_VAR_TO_INTERPRET at line[%d] (%c) ! (line[%d] ~%c~)\n", temp_i, line [(temp_i)], (*i), line[(*i)]);
			}
		}
		(*i)++;
		while (line[(*i)] != '\0' && line[(*i)] != ' ')
		{
			if (line[(*i)] == '\"' && ft_count_char(&line[(*i)], '\"') > 1)
			{
				handle_quote(line, '\"', i);
			}
			if (line[(*i)] == '$' && line[(*i) + 1] != '\0'
				&& line[(*i) + 1] != '\"' && line[(*i) + 1] != ' '
				&& line[(*i) + 1] != CC_VAR_BOUND && line[(*i) + 1] != '$')
			{
				ft_replace_char(&line[(*i)], CC_VAR_TO_INTERPRET);
				if (DEBUG == 1)
					printf("/post_merge/ found CC_VAR_TO_INTERPRET at line[%d] (%c) !\n", (*i), line[(*i)]);
			}
			(*i)++;
		}
		(*i)--;
	}
}

void	pass_quotes(char *dst, char *src, size_t *i, size_t *j)
{
	if (DEBUG == 1)
		printf("---> /pass_quotes/ (line[%ld] ~%c~)\n", (*i), src[(*i)]);
	if (src[(*i)] == '\'' && ft_count_char(&src[(*i)], '\'') > 1)
	{
		(*i)++;
		while (src[(*i)] != '\'')
		{
			if (DEBUG == 1)
				printf("----> [copy while ']/ (line[%ld] ~%c~)\n", (*i), src[(*i)]);
			dst[(*j)++] = src[(*i)++];
		}
		(*i)++;
	}
	else if (src[(*i)] == CC_VAR_BOUND)
	{
		while (src[(*i)] != '\"' && src[(*i)] != CC_VAR_BOUND)
		{
			if (DEBUG == 1)
				printf("----> [copy while \"]/ (line[%ld] ~%c~)\n", (*i), src[(*i)]);
			dst[(*j)++] = src[(*i)++];
			if (src[(*i)] == CC_VAR_BOUND)
			{
				if (DEBUG == 1)
					printf("----> [found CC_VAR_BOUND! stop]/ (line[%ld] ~%c~)\n", (*i), src[(*i)]);
				dst[(*j)] = src[(*i)];
				break ;
			}
		}
		if (src[(*i)] == CC_VAR_BOUND)
		{
			if (DEBUG == 1)
				printf("----> [found CC_VAR_BOUND! continue]/ (line[%ld] ~%c~)\n", (*i), src[(*i)]);
			dst[(*j)] = src[(*i)];
		}
		(*i)++;
		while (src[(*i)] && src[(*i)] != ' ')
		{
			if (DEBUG == 1)
				printf("----> [copy while not space]/ (line[%ld] ~%c~)\n", (*i), src[(*i)]);
			dst[(*j)++] = src[(*i)++];
		}
	}
	else if (src[(*i)] == '\"' && ft_strrchr(src, CC_VAR_BOUND) != NULL)
	{
		(*i)++;
		while (src[(*i)] != CC_VAR_BOUND && src[(*i)] != '\"')
		{
			if (DEBUG == 1)
				printf("----> [copy while not CC_VAT_BOUND and not \"]/ (line[%ld] ~%c~)\n", (*i), src[(*i)]);
			dst[(*j)++] = src[(*i)++];
		}
		if (src[(*i)] == '\"')
		{
			if (DEBUG == 1)
				printf("----> [is \": pass]/ (line[%ld] ~%c~)\n", (*i), src[(*i)]);
			(*i)++;
		}
		while (src[(*i)] && src[(*i)] != ' ' && src[(*i)] != '\"')
		{
			if (DEBUG == 1)
				printf("----> [copy while not space and not \"]/ (line[%ld] ~%c~)\n", (*i), src[(*i)]);
			dst[(*j)++] = src[(*i)++];
		}
	}
	else if (src[(*i)] == '\"' && ft_count_char(&src[(*i)], '\"') > 1)
	{
		(*i)++;
		while (src[(*i)] != '\"')
		{
			if (DEBUG == 1)
				printf("----> [copy while not \"]/ (line[%ld] ~%c~)\n", (*i), src[(*i)]);
			dst[(*j)++] = src[(*i)++];
		}
		(*i)++;
	}
	if (DEBUG == 1)
		printf("--> [pass quotes: last char/ (line[%ld] ~%c~)\n", (*i) - 1, src[(*i - 1)]);
}
