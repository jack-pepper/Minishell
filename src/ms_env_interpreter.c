/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_env_interpreter.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmalie <mmalie@student.42nice.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 21:05:23 by mmalie            #+#    #+#             */
/*   Updated: 2025/05/04 11:29:28 by mmalie           ###   ########.fr       */
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
		
//		ft_show_strs(sh->input_args, "[DEBUG] sh->input_args");
		
		if (ft_strchr(sh->input_args[i], CTRL_CHAR_VAR_TO_INTERPRET) != NULL)
		{
			split_args = ft_split(sh->input_args[i], CTRL_CHAR_VAR_TO_INTERPRET);
			if (!split_args)
				return (-1);

		//	ft_show_strs(split_args, "[DEBUG] split_args");

			if (sh->input_args[i][0] == CTRL_CHAR_VAR_TO_INTERPRET)
			{
				rejoined_arg = ft_strdup("");
				rejoined_arg = ft_nametoval(sh, rejoined_arg, split_args);
			}
			else
			{
				rejoined_arg = ft_strdup(split_args[0]);
				rejoined_arg = ft_nametoval(sh, rejoined_arg, &split_args[1]);
			}
			free_args(split_args);
			if (!rejoined_arg)
				return (-1);
			ft_copy_free(&sh->input_args[i], rejoined_arg);
			free(rejoined_arg);
			if (!sh->input_args[i])
				return (-1);
		}
		i++;
	}
	return (0);
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

char    *ft_strjoin_delim(char const *s1, char const *s2, char const *delim)
{
        char    *joined_str;
        size_t  len;
	size_t	s1_len;
	size_t	s2_len;

	s1_len = 0;
	s2_len = 0;
	if (s1 != NULL)
		s1_len = ft_strlen(s1);
	if (s2 != NULL)
		s2_len = ft_strlen(s2);
	if (s1 && s2)
        	len = s1_len + ft_strlen(delim) + s2_len;
	else
		len = s1_len + s2_len;
        joined_str = (char *)malloc(sizeof(char) * (len + 1));
        if (joined_str == NULL)
                return (NULL);
	if (s1 != NULL)
        {
		ft_strlcpy(joined_str, s1, len + 1);
		ft_strlcat(joined_str, delim, len + 1);
	}
	if (s2 != NULL)
        	ft_strlcat(joined_str, s2, len + 1);
        return (joined_str);
}

char	*ft_nametoval(t_shell *sh, char *rejoined_arg, char **split_args)
{
	t_list	*set_var;
	char	**subargs;
	int		i;

	i = 0;
	while (split_args[i])
	{
		char *first_space = ft_strpbrk(split_args[i], "*"); // '*' ust be changed to CTRL_CHAR_SPACE_IN_QUOTE
		if (first_space != NULL)
		{
			first_space[0] = '#'; // MODIFY TO CTRL_CHAR
		//	printf("After first space: %s\n", split_args[i]);
			subargs = ft_split(split_args[i], '#'); // MODIFY TO CTRL_CHAR
		//	ft_show_strs(subargs, "[DEBUG] After split"),	
			set_var = ft_getenv(subargs[0], &sh->this_env);
			free(subargs[0]);
			if (set_var != NULL)
				subargs[0] = ft_strdup(((char **)set_var->content)[1]);
			else
				subargs[0] = ft_strdup("");
			free(split_args[i]);
			split_args[i] = ft_strjoin_delim(subargs[0], subargs[1], " ");
		//	printf("[DEBUG] after join-delim: ~%s~\n", split_args[i]);
			free_args(subargs);
		}
		else
		{	
			set_var = ft_getenv(split_args[i], &sh->this_env);
			free(split_args[i]);
			if (set_var != NULL)
				split_args[i] = ft_strdup(((char **)set_var->content)[1]);
			else
				split_args[i] = ft_strdup("");
		}
		//ft_show_strs(split_args, "[DEBUG] split_args after nametoval"),
		rejoined_arg = ft_rejoin_subarg(split_args, rejoined_arg, i);
		//rejoined_arg = ft_strjoin_delim(rejoined_arg, split_args[i], " ");
		i++;
	}
	return (rejoined_arg);
}

char	*ft_rejoin_subarg(char **split_args, char *rejoined_arg, int i)
{
	char	*temp;
//	char	*temp_with_space;

	temp = ft_strjoin(rejoined_arg, split_args[i]);
	free(rejoined_arg);
	if (!temp)
		return (NULL);
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
