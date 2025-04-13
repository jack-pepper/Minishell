/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_env_interpreter.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmalie <mmalie@student.42nice.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 21:05:23 by mmalie            #+#    #+#             */
/*   Updated: 2025/04/14 01:08:21 by mmalie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

// Interpret the env variables when needed.
int     ft_interpret_env(t_shell *sh)
{
	char	**split_args;
	char	*rejoined_arg;
	int	i;

	i = 0;
	while (sh->input_args[i])
	{
		split_args = NULL;
		rejoined_arg = "";
		split_args = ft_split_args(split_args, sh->input_args[i]);
		if (!split_args)
			return (-1);
		rejoined_arg = ft_name_to_value(rejoined_arg, split_args, &sh->this_env);
		if (!rejoined_arg)
			return (-1);	

		ft_copy_free(&sh->input_args[i], rejoined_arg);
		if (!sh->input_args[i])
			return (-1);
		// solve leaks on exit, but creates issues with other cmds 
		int j = 0;
 		while (split_args[j] != NULL)
		{
			free(split_args[j]);
			j++;
		}
		free(split_args);
		//
		free(rejoined_arg);
		i++;
	}
	return (0);
}

// Delimit the tokens to retrieve the spaces on rejoin, then split
char	**ft_split_args(char **split_args, char *input_arg)
{
        int     i;

        i = 0;
        while (input_arg[i] != '\0') // Might need to check it for edge cases and improve it
        {
		if (i > 0 && input_arg[i] != ' ' && input_arg[i + 1] == ' ')
		{
			input_arg[i + 1] = CTRL_CHAR_SUBARG_DELIM;
			i++;
		}
                if (input_arg[i] == ' ' && input_arg[i + 1] != ' ')
                {
                        input_arg[i] = CTRL_CHAR_SUBARG_DELIM;
                        i++;
                }
                i++;
        }
	split_args = ft_split(input_arg, CTRL_CHAR_SUBARG_DELIM);
	if (!split_args)
		return (NULL);
	return (split_args);
}

char	*ft_name_to_value(char *rejoined_arg, char **split_args, t_list **this_env)
{
        t_list  *set_var;
	int	i;

        i = 0;
        while (split_args[i])
        {
                if (split_args[i][0] == CTRL_CHAR_VAR_TO_INTERPRET)
                {
                        set_var = ft_getenv(&split_args[i][1], this_env);
                        if (set_var != NULL)
                        {
                        //      free(split_args[i]);
                                split_args[i] = ft_strdup(((char **)set_var->content)[1]);
                                if (!split_args)
                                        return (NULL);
                        }
                        else
				split_args[i][0] = '$';
		}
		rejoined_arg = ft_rejoin_subarg(split_args, rejoined_arg, i);
		i++;
        }
	return (rejoined_arg);
}

char	*ft_rejoin_subarg(char **split_args, char *rejoined_arg, int i)
{
	rejoined_arg = ft_strjoin(rejoined_arg, split_args[i]);
	if (!rejoined_arg)
		return (NULL);
	if (split_args[i + 1])
	{
		rejoined_arg = ft_strjoin(rejoined_arg, " ");
		if (!rejoined_arg)
			return (NULL);
	}
	return (rejoined_arg);
}

char	**ft_copy_free(char **input_arg, char *rejoined_arg)
{
        (*input_arg) = ft_strdup(rejoined_arg);
	if (!(*input_arg))
		return (NULL);
	return (input_arg);
}
