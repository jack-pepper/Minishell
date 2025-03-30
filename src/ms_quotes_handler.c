/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_quotes_handler.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmalie <mmalie@student.42nice.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/30 14:16:00 by mmalie            #+#    #+#             */
/*   Updated: 2025/03/30 14:28:14 by mmalie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	ft_replace_char(char *cur_c, char new_c)
{
	if (ft_isspace(*cur_c))
		*cur_c = new_c; // DEBUG: replace eventually by ctrl char (non-printable)
}
