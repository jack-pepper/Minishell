/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_signals.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmalie <mmalie@student.42nice.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 14:49:10 by mmalie            #+#    #+#             */
/*   Updated: 2025/04/14 20:13:35 by mmalie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

/* Define which signals are caught and how they are handled.
 * 
 * - SIGINT (ctrl + c): display a new prompt on new line.
 * - SICQUIT (ctrl + \): does nothing.
 *
 */

void	init_signals(void)
{
	init_sigset();
	init_sigaction();
}

void	init_sigset(void)
{
	signal(SIGQUIT, SIG_IGN);
}

void	init_sigaction(void)
{
	struct sigaction	act;

	ft_memset(&act, 0, sizeof(act));
	act.sa_handler = &signal_handler;
	act.sa_flags = 0;
	sigaction(SIGINT, &act, NULL);
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
