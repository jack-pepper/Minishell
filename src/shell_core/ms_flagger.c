/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_flagger.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmalie <mmalie@student.42nice.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 15:03:35 by mmalie            #+#    #+#             */
/*   Updated: 2025/05/19 10:05:13 by mmalie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

// Flag $ (outside quotes only)
/*void	flag_dollar(char *line, int *i)
{
	if (line[(*i) + 1])
	{
		if (ft_is_in_set(line[(*i) + 1], "\"\'")
			|| line[(*i)] == ' ')
			ft_replace_char(&line[(*i)], CC_LONE_DOLLAR);
		else if (line[(*i) + 1] == '$')
		{
			;
		}
		else if (line[(*i) + 1] != '_' && ft_ispunct(line[(*i) + 1]))
		{
			if (line[(*i) + 1] == '?')
				ft_replace_char(&line[(*i)], CC_VAR_TO_INTERPRET);
		}
		else if (line[(*i) + 1] != '\0' && line[(*i) + 1] != ' ')
			ft_replace_char(&line[(*i)], CC_VAR_TO_INTERPRET);
	}
	return ;
}*/

/*
void	flag_dollar(char *line, int *i)
{
	if (line[(*i) + 1])
	{
		if (ft_isspace(line[(*i) + 1]))
		{
			if (line[(*i) - 1] && ft_isspace(line[(*i) - 1]))
				ft_replace_char(&line[(*i)], '$');
			else
				ft_replace_char(&line[(*i)], CC_TRAILING_DOLLAR); // must KEEP but NOT be considered in var (echo $USER -> mmalie$)
		}
		else if (ft_ispunct(line[(*i) + 1]))
		{
			if (ft_is_in_set(line[(*i) + 1], "_?"))
				ft_replace_char(&line[(*i)], CC_VAR_TO_INTERPRET);
			else if (ft_is_in_set(line[(*i)] + 1, "\"\'"))
				ft_replace_char(&line[(*i)], CC_LONE_DOLLAR); // must DELETE (echo $USER$"hey" -> mmaliehey)
			else
				ft_replace_char(&line[(*i)], '$');
		}
		else
			ft_replace_char(&line[(*i)], CC_VAR_TO_INTERPRET);
	}
	else if (line[(*i) + 1] == '\0' && ft_isspace(line[(*i) - 1]))
		ft_replace_char(&line[(*i)], '$');
	else
		ft_replace_char(&line[(*i)], CC_TRAILING_DOLLAR);
	return ;
}
*/

void	flag_dollar(char *line, int *i)
{
	if (line[(*i) + 1])
	{
		if (ft_is_in_set(line[(*i) + 1], "\"\'"))
			ft_replace_char(&line[(*i)], CC_LONE_DOLLAR); // must DELETE (echo $USER$"hey" -> mmaliehey)
		else if (ft_is_in_set(line[(*i) + 1], "_?") || ft_isalpha(line[(*i) + 1]))
			ft_replace_char(&line[(*i)], CC_VAR_TO_INTERPRET);
		else if (ft_is_in_set(line[(*i) - 1], "_?") || ft_isalnum(line[(*i) - 1]))	
			ft_replace_char(&line[(*i)], CC_TRAILING_DOLLAR);
	}
	else
		if (line[(*i) - 1] && (ft_is_in_set(line[(*i) - 1], "_?") || ft_isalnum(line[(*i) - 1])))
			ft_replace_char(&line[(*i)], CC_TRAILING_DOLLAR);
		else if (line[(*i) - 1] && ((ft_is_in_set(line[(*i) - 1], "\"\'") || line[(*i) - 1] == CC_VAR_BOUND)))
			ft_replace_char(&line[(*i)], '$');
	return ;
}

void	flag_pipe_and_redir(char *line, int *i)
{
	if (line[(*i)] == '|')
		ft_replace_char(&line[(*i)], CC_PIPE);
	else if (line[(*i)] == '<' && line[(*i) + 1] == '<')
	{
		ft_replace_char(&line[(*i)], CC_HEREDOC);
		ft_replace_char(&line[(*i) + 1], ' ');
		(*i)++;
	}
	else if (line[(*i)] == '>' && line[(*i) + 1] == '>')
	{
		ft_replace_char(&line[(*i)], CC_APPEND);
		ft_replace_char(&line[(*i) + 1], ' ');
		(*i)++;
	}
	else if (line[(*i)] == '<')
		ft_replace_char(&line[(*i)], CC_REDIR_IN);
	else if (line[(*i)] == '>')
		ft_replace_char(&line[(*i)], CC_REDIR_OUT);
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
