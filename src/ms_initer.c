/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_initer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-bouk <yel-bouk@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 11:56:45 by mmalie            #+#    #+#             */
/*   Updated: 2025/05/08 23:13:19 by mmalie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

// Initialize what is needed for the shell (signals, env, pipex, commands)
int	init_shell(t_shell *sh, char **env)
{
	sh->normalized_line = NULL;
	sh->input_args = NULL;
	sh->tokens = NULL;
	sh->last_exit_status = 0;
	init_signals();
	if (init_env(sh, env) != 0)
		return (-1);
	if (init_cmds(sh) != 0)
		return (-1);
	return (0);
}

// If a value from env contains more than one equal sign, it has been split.
// 
int	normalize_env(t_list *this_env)
{
	t_list	*cur_node;
	char	*rejoined_var;
	int		i;

	cur_node = this_env;
	while (cur_node != NULL)
	{
		i = 2;
		if (ft_strslen((char **)cur_node->content) > 2)
		{
			rejoined_var = ft_strdup(((char **)cur_node->content)[1]);
			free(((char **)cur_node->content)[1]);
			while (((char **)cur_node->content)[i])
			{
				char *temp = ft_strdup(rejoined_var);
				free(rejoined_var);
				rejoined_var = ft_strjoin_delim(temp, ((char **)cur_node->content)[i], "=");
				free(temp);
				i++;
			}
			((char **)cur_node->content)[1] = ft_strdup(rejoined_var);
			free(rejoined_var);
		}
		ft_unflag_delim(((char **)cur_node->content)[1], '=', CTRL_CHAR_EXTRA_DELIM);
		cur_node = cur_node->next;
	}
	return (0);
}

// Initialize a local environment by storing the env variables to a t_list
int	init_env(t_shell *sh, char **env)
{
	size_t	nb_vars;

	nb_vars = ft_strslen(env);
	sh->this_env = NULL;
	sh->env_stash = NULL;
	if (ft_strstolist(&sh->this_env, env, nb_vars, '=') != 0)
		return (-1);
	if (normalize_env(sh->this_env) != 0)
		return (-1);
	return (0);
}

void	init_signals(void)
{
	struct sigaction	act_sigint;
	struct sigaction	act_sigquit;

	ft_memset(&act_sigquit, 0, sizeof(act_sigquit));
	act_sigquit.sa_handler = SIG_IGN;
	act_sigquit.sa_flags = 0;
	sigaction(SIGQUIT, &act_sigquit, NULL);
	ft_memset(&act_sigint, 0, sizeof(act_sigint));
	act_sigint.sa_handler = &signal_handler;
	act_sigint.sa_flags = 0;
	sigaction(SIGINT, &act_sigint, NULL);
	signal(SIGPIPE, SIG_IGN);
}

void	signal_handler(int signum)
{
	if (signum == SIGINT)
	{
		printf("\n");
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}
