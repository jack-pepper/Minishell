/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_normalizer_add_spaces.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-bouk <yel-bouk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 23:57:21 by mmalie            #+#    #+#             */
/*   Updated: 2025/06/06 20:05:31 by yel-bouk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

char	*ft_add_spaces_around_redir(char *spaced)
{
	char	*tmp;
	char	*cc_append;
	char	*cc_heredoc;
	char	*cc_redir_in;
	char	*cc_redir_out;

	cc_append = ft_chrtostr(CC_APPEND);
	cc_heredoc = ft_chrtostr(CC_HEREDOC);
	cc_redir_in = ft_chrtostr(CC_REDIR_IN);
	cc_redir_out = ft_chrtostr(CC_REDIR_OUT);
	tmp = spaced;
	spaced = ft_add_spaces_around_str(tmp, cc_append);
	ft_free_two_str(tmp, cc_append);
	tmp = spaced;
	spaced = ft_add_spaces_around_str(tmp, cc_heredoc);
	ft_free_two_str(tmp, cc_heredoc);
	tmp = spaced;
	spaced = ft_add_spaces_around_str(tmp, cc_redir_in);
	ft_free_two_str(tmp, cc_redir_in);
	tmp = spaced;
	spaced = ft_add_spaces_around_str(tmp, cc_redir_out);
	ft_free_two_str(tmp, cc_redir_out);
	return (spaced);
}

void	increase_str(char *new_str, char special, int *j)
{
	new_str[(*j)++] = ' ';
	new_str[(*j)++] = special;
	new_str[(*j)++] = ' ';
}

int	count_extra_spaces(const char *str, char special)
{
	int	i;
	int	extra;

	i = 0;
	extra = 0;
	while (str[i])
	{
		if (str[i] == special)
			extra += 2;
		i++;
	}
	return (extra);
}
