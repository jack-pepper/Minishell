/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_cmd_echo.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmalie <mmalie@student.42nice.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 17:16:17 by mmalie            #+#    #+#             */
/*   Updated: 2025/05/04 17:16:20 by mmalie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

char	*echo_join_input(char *joined_input, char **input_args, int *i)
{
	char	*temp;

	temp = ft_strjoin(joined_input, input_args[(*i)]);
	if (!temp)
		return (NULL);
	if (joined_input && joined_input[0] != '\0')
		free(joined_input);
	joined_input = temp;
	(*i)++;
	if (input_args[(*i)] != NULL)
	{
		temp = ft_strjoin(joined_input, " ");
		if (!temp)
			return (NULL);
		free(joined_input);
		joined_input = temp;
	}
	return (joined_input);
}

int	cmd_echo(t_shell *sh)
{
	char	*joined_input;
	char	head[1];
	bool	opt_n;
	int		i;

	if (!sh->input_args || !(sh->input_args[1]))
		return (0);
	i = 1;
	echo_set_n(sh->input_args, &opt_n, &i);
	if (!sh->input_args[i])
		return (0);
	head[0] = 0;
	joined_input = &head[0];
    ft_replace_all_chars(sh->input_args, CTRL_CHAR_PIPE, '|'); // added to display this char correctly with echo
	ft_replace_all_chars(sh->input_args, CTRL_CHAR_SPACE_IN_QUOTE, ' ');
	while (sh->input_args[i] != NULL)
	{
		joined_input = echo_join_input(joined_input, sh->input_args, &i);
		if (!joined_input)
			return (-1);
	}
	printf("%s", joined_input);
	if (opt_n == false)
		printf("\n");
	free(joined_input);
	return (0);
}

void	echo_set_n(char **input_args, bool *opt_n, int *i)
{
	if (ft_strncmp(input_args[1], "-n", ft_strlen(input_args[1])) == 0)
	{
		*opt_n = true;
		(*i)++;
	}
	else
		*opt_n = false;
}
