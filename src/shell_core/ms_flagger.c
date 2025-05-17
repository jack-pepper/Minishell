/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_flagger.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmalie <mmalie@student.42nice.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 15:03:35 by mmalie            #+#    #+#             */
/*   Updated: 2025/05/17 10:08:36 by mmalie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	flag_dollar(char *line, int *i)
{
	if (line[(*i)] == '$' && (line[(*i) + 1] == '\"' || line[(*i) + 1] == '\''))
		ft_replace_char(&line[(*i)], CTRL_CHAR_TO_BE_DELETED);
	else if (line[(*i)] == '$' && line[(*i) + 1] == '$')
		;
	else if (line[(*i)] == '$' && line[(*i) + 1] != '_' && ft_ispunct(line[(*i) + 1]))
        	;
	else if (line[(*i)] == '$' && line[(*i) + 1] != '\0' && line[(*i) + 1] != ' ')
		ft_replace_char(&line[(*i)], CTRL_CHAR_VAR_TO_INTERPRET);
	//else if (line[(*i)] == '$' && (line[(*i) + 1] == '?'))
	//	ft_replace_char(&line[(*i)], CTRL_CHAR_VAR_TO_INTERPRET);
	return ;
}

void	flag_pipe_and_redir(char *line, int *i)
{
	if (line[(*i)] == '|')
		ft_replace_char(&line[(*i)], CTRL_CHAR_PIPE);
        else if (line[(*i)] == '<' && line[(*i) + 1] == '<')
        {
	        ft_replace_char(&line[(*i)], CTRL_CHAR_HEREDOC);
                ft_replace_char(&line[(*i) + 1], ' ');
                (*i)++; // are you sure?
        }
	else if (line[(*i)] == '>' && line[(*i) + 1] == '>')
        {
		ft_replace_char(&line[(*i)], CTRL_CHAR_APPEND);
		ft_replace_char(&line[(*i) + 1], ' ');
		(*i)++; // are you sure?
        }
        else if (line[(*i)] == '<')
		ft_replace_char(&line[(*i)], CTRL_CHAR_REDIR_IN);
	else if (line[(*i)] == '>')
		ft_replace_char(&line[(*i)], CTRL_CHAR_REDIR_OUT);
}

void	ante_merge_quote(char *line, int *i)
{
	int	temp_i;

	if ((*i) > 0 && line[(*i) - 1] != ' ' && line[(*i) - 1] != '\"' && ft_strchr(line, CTRL_CHAR_VAR_TO_INTERPRET) != NULL)
	{
		temp_i = (*i);
		while (temp_i > 0 && line[temp_i] != ' ')
		{
			temp_i--;
			if (line[temp_i] == CTRL_CHAR_VAR_TO_INTERPRET)
			{
				ft_replace_char(&line[(*i)], CTRL_CHAR_VAR);
				break;
			}
		}
	}
}

void	post_merge_quote(char *line, int *i)
{
	int	temp_i;

	if (line[(*i) + 1] && line[(*i) + 1] != ' ' && line[(*i) + 1] != '\"'
		&& ft_strchr(line, CTRL_CHAR_VAR_TO_INTERPRET) != NULL)
	{
		temp_i = (*i) - 1;
		while (temp_i >= 0 && line[temp_i] != '\"' && line[temp_i] != CTRL_CHAR_VAR && line[temp_i] != ' ')
		{
			temp_i--;
			if (line[temp_i] == CTRL_CHAR_VAR_TO_INTERPRET)
			{
				ft_replace_char(&line[(*i)], CTRL_CHAR_VAR);
				break;
			}
		}
		while (line[(*i)] != '\0' && line[(*i)] != ' ')
		{
			if (line[(*i)] == '$' && line[(*i) + 1] != '\"'
				&& line[(*i) + 1] != ' '
				&& line[(*i) + 1] != CTRL_CHAR_VAR)
				ft_replace_char(&line[(*i)++], CTRL_CHAR_VAR_TO_INTERPRET);
			(*i)++;
		}
		(*i)--;
	}
}

void	flag_quote(char *line, int *i)
{
	if (line[(*i)] == '\'' && ft_count_char(&line[(*i)], '\'') > 1)
		handle_quote(line, '\'', i);
	else if (line[(*i)] == '\"' && ft_count_char(&line[(*i)], '\"') > 1)
        {
		ante_merge_quote(line, i);

		handle_quote(line, '\"', i);
                
		post_merge_quote(line, i);
	}
}
