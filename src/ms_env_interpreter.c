/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_env_interpreter.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmalie <mmalie@student.42nice.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 21:05:23 by mmalie            #+#    #+#             */
/*   Updated: 2025/04/09 09:36:25 by mmalie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

// Interpret the env variables when needed.
int     ft_interpret_env(t_shell *sh)
{
	char	**split_args;
	char	*rejoined_arg;	

	split_args = NULL;
	rejoined_arg = "";
	split_args = ft_split_args(split_args, sh);
	if (!split_args)
		return (-1);
	rejoined_arg = ft_name_to_value(split_args, rejoined_arg, sh);
	if (!rejoined_arg)
		return (-1);
	free(split_args);

	sh->input_args = ft_copy_free(rejoined_arg, sh);
	if (!sh->input_args)
		return (-1);

	free(rejoined_arg);
	return (0);
}

// Delimit the tokens to retrieve the spaces on rejoin, then split
char	**ft_split_args(char **split_args, t_shell *sh) 
{
        int     i;

        i = 0;
        while ((*sh->input_args)[i])
        {
                if ((*sh->input_args)[i] == ' ' && (*sh->input_args)[i + 1] != ' ')
                {
                        (*sh->input_args)[i] = CTRL_CHAR_SUBARG_DELIM;
                        i++;
                }
                i++;
        }
	split_args = ft_split((*sh->input_args), CTRL_CHAR_SUBARG_DELIM);
	if (!split_args)
		return (NULL);
	return (split_args);
}

char	*ft_name_to_value(char **split_args, char *rejoined_arg, t_shell *sh)
{
        t_list  *set_var;
	int	i;

        i = 0;
        while (split_args[i])
        {
                if (split_args[i][0] == CTRL_CHAR_VAR_TO_INTERPRET)
                {
                        set_var = ft_getenv(&split_args[i][1], &sh->this_env);
                        if (set_var != NULL)
                        {
                        //      free(split_arg[i]);
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

char	**ft_copy_free(char *rejoined_arg, t_shell *sh)
{	
        (*sh->input_args) = ft_strdup(rejoined_arg);
	if (!(*sh->input_args))
		return (NULL);
	return (&(*sh->input_args));
}
