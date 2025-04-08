/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_env_interpreter.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmalie <mmalie@student.42nice.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 21:05:23 by mmalie            #+#    #+#             */
/*   Updated: 2025/04/08 21:06:40 by mmalie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Interpret the env variables when needed.
int     ft_interpret_env(t_shell *sh)
{
        char    **split_arg;
        char    *rejoined_arg;
        t_list  *set_var;
        int     i;

        i = 0;
        while ((*sh->input_args)[i]) // to retrieve the spaces
        {
                if ((*sh->input_args)[i] == ' ' && (*sh->input_args)[i + 1] != ' ')
                {
                        (*sh->input_args)[i] = CTRL_CHAR_SUBARG_DELIM;
                        i++;
                }
                i++;
        }
        split_arg = ft_split((*sh->input_args), CTRL_CHAR_SUBARG_DELIM);
        if (!split_arg)
                return (-1);
        i = 0;
        rejoined_arg = "";
        while (split_arg[i])
        {
                if (split_arg[i][0] == CTRL_CHAR_VAR_TO_INTERPRET)
                {
                        set_var = ft_getenv(&split_arg[i][1], &sh->this_env);
                        if (set_var != NULL)
                        {
                        //      free(split_arg[i]);
                                split_arg[i] = ft_strdup(((char **)set_var->content)[1]);
                                if (!split_arg)
                                        return (-1) ;
                        }
                        else // make sure to make the behavior match bash, will probably be different
                        {
                                split_arg[i][0] = '$';
                        }
                }
                rejoined_arg = ft_strjoin(rejoined_arg, split_arg[i]);
                if (split_arg[i + 1])
                        rejoined_arg = ft_strjoin(rejoined_arg, " ");
                // printf("(%d) %s\n", i, rejoined_arg);
                i++;
        }
        (*sh->input_args) = ft_strdup(rejoined_arg);
        free(rejoined_arg);
        return (0);
}
