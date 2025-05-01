/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_env_interpreter.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmalie <mmalie@student.42nice.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 21:05:23 by mmalie            #+#    #+#             */
/*   Updated: 2025/05/01 23:48:06 by mmalie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	ft_interpret_env(t_shell *sh)
{
	char	**split_args;
	char	*rejoined_arg;
	int		i;

	i = 0;
	while (sh->input_args[i])
	{
		split_args = NULL;
		rejoined_arg = ft_strdup("");
		split_args = ft_split_args(split_args, sh->input_args[i]);
		if (!split_args)
			return (-1);

//		ft_show_strs(split_args, "[DEBUG] split_args");

		rejoined_arg = ft_nametoval(sh, rejoined_arg, split_args);
		free_args(split_args);
		if (!rejoined_arg)
			return (-1);
		ft_copy_free(&sh->input_args[i], rejoined_arg);
		free(rejoined_arg);
		if (!sh->input_args[i])
			return (-1);
		i++;
	}
	return (0);
}

char	**ft_split_args(char **split_args, char *input_arg)
{
	int	i;

	i = 0;
	while (input_arg[i] != '\0')
	{
		if (ft_isspace(input_arg[i]) && ft_isspace(input_arg[i + 1]) && ft_isspace(input_arg[i + 2]))
		{
			input_arg[i] = CTRL_CHAR_SUBARG_DELIM;
			while (ft_isspace(input_arg[i + 1]))
				i++;
			input_arg[i] = CTRL_CHAR_SUBARG_DELIM;
		}	
		i++;
	}
	split_args = ft_split(input_arg, CTRL_CHAR_SUBARG_DELIM);
	if (!split_args)
		return (NULL);
	return (split_args);
}

char	*join_all_subargs(char **args, char delim)
{
	char	*result = ft_strdup("");
	char	*temp;
	int i = 0;

	result = ft_strdup("");
	if (!result)
		return (NULL);
	i = 0;
	while (args[i])
	{
		temp = ft_strjoin(result, args[i]);
        	free(result);
		result = temp;
		if (delim != 'n' && args[i + 1])
		{
			temp = ft_strjoin(result, " ");
			free(result);
			result = temp;
		}
		i++;
	}
	return result;
}

char	*ft_nametoval(t_shell *sh, char *rejoined_arg, char **split_args)
{
	t_list	*set_var;
	char	**subargs;
	char	**subsubargs;
	int		i;
	int		j;
	char	*temp;

	i = 0;
	while (split_args[i])
	{
		if (ft_strchr(split_args[i], CTRL_CHAR_VAR_TO_INTERPRET) != NULL)
		{
			subargs = ft_split(split_args[i], CTRL_CHAR_VAR_TO_INTERPRET);
			if (!subargs)
				return (NULL);

//			 ft_show_strs(subargs, "[DEBUG] subargs");

			j = 0;
			if (split_args[i][0] != CTRL_CHAR_VAR_TO_INTERPRET)
				j = 1;
			while (subargs[j])
			{
				if (ft_strchr(subargs[j], ' ') == NULL)
				{
					set_var = ft_getenv(subargs[j], &sh->this_env);
					if (subargs[j][0] == '?')
					{
						if (subargs[j][1])
						{
							temp = ft_strdup(&(subargs[j][1]));
							free(subargs[j]);
							subargs[j] = ft_strjoin(ft_itoa(sh->last_exit_status), temp);
							free(temp);
						}
						else
						{
							free(subargs[j]);
							subargs[j] = ft_strdup(ft_itoa(sh->last_exit_status));
						}
					}
					else
					{
						free(subargs[j]);
						if (set_var != NULL)
							subargs[j] = ft_strdup(((char **)set_var->content)[1]);
						else
							subargs[j] = ft_strdup("");
					}
				}
				else // if there is a space!
				{
					subsubargs = ft_split(subargs[j], ' ');
					
//					ft_show_strs(subsubargs, "[DEBUG] subsubargs");
				
					set_var = ft_getenv(subsubargs[0], &sh->this_env);

					if (subsubargs[0][0] == '?')
					{
						if (subsubargs[0][1])
						{
							temp = ft_strdup(&(subsubargs[0][1]));
							free(subsubargs[0]);
							subsubargs[0] = ft_strjoin(ft_itoa(sh->last_exit_status), temp);
							free(temp);
						}
						else
						{	
							free(subsubargs[0]);
							subsubargs[0] = ft_strdup(ft_itoa(sh->last_exit_status));
						}
					}
					else
					{ 
						free(subsubargs[0]);
						if(set_var != NULL)	
							subsubargs[0] = ft_strdup(((char **)set_var->content)[1]);
						else
							subsubargs[0] = ft_strdup("");
					}
					free(subargs[j]);
					subargs[j] = ft_strdup("");
					subargs[j] = join_all_subargs(subsubargs, ' ');
					free(subsubargs);
				}
				j++;
			}
			free(split_args[i]);
			split_args[i] = join_all_subargs(subargs, 'n');
			free_args(subargs);
		}
		rejoined_arg = ft_rejoin_subarg(split_args, rejoined_arg, i);
		i++;
	}
	return (rejoined_arg);
}

char	*ft_rejoin_subarg(char **split_args, char *rejoined_arg, int i)
{
	char	*temp;
	char	*temp_with_space;

	temp = ft_strjoin(rejoined_arg, split_args[i]);
	free(rejoined_arg);
	if (!temp)
		return (NULL);
	if (split_args[i + 1])
	{
		temp_with_space = ft_strjoin(temp, " ");
		free(temp);
		if (!temp_with_space)
			return (NULL);
		rejoined_arg = ft_strdup(temp_with_space);
		free(temp_with_space);
		if (!rejoined_arg)
			return (NULL);
		return (rejoined_arg);
	}
	rejoined_arg = ft_strdup(temp);
	free(temp);
	return (rejoined_arg);
}

char	**ft_copy_free(char **input_arg, char *rejoined_arg)
{
	free(*input_arg);
	(*input_arg) = ft_strdup(rejoined_arg);
	if (!(*input_arg))
		return (NULL);
	return (input_arg);
}
