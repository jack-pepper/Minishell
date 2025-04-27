/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_initer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmalie <mmalie@student.42nice.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 11:56:45 by mmalie            #+#    #+#             */
/*   Updated: 2025/04/27 14:16:42 by mmalie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

// Initialize what is needed for the shell (signals, env, pipex, commands)
int	init_shell(t_shell *sh, char **env)
{
	sh->normalized_line = NULL;
	sh->input_args = NULL;
	sh->last_exit_status = 0;
	init_signals();
	if (init_env(sh, env) != 0)
		return (-1);
        // init_pipex (make a pointer to this_env)
	if (init_cmds(sh) != 0)
		return (-1);
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
