/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xin <xin@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/07 16:45:58 by xin               #+#    #+#             */
/*   Updated: 2025/12/19 17:04:58 by xin              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	g_signal = 0;

/**
 * @brief disable the print of ctrl c  ctrl d and ctrl \ 
 * @note
 * ECHOCTL is a local flag of termios
 * TCSANOW means set the change now
 */
void	ft_disable_echo_ctl(void)
{
	struct termios	term;

	if (tcgetattr(STDIN_FILENO, &term) == -1)
		return ;
	term.c_lflag &= ~ECHOCTL;
	if (tcsetattr(STDIN_FILENO, TCSANOW, &term) == -1)
		return ;
}

/**
 * @brief when shell meets the signal ctrl c, clean the line and set exit code
 * @note
 * rl_one_new_line() set where the input cursor should be
 * the second parameter 0 of rl_replace_line means do not save to history
 */
void	ft_handle_sigint(int signum)
{
	(void)signum;
	g_signal = 130;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

/**
 * @brief set the different reactions when user input a signal
 * @param sa_int	ctrl c means interrupt
 * @param sa_quit	ctrl \ means quit 
 */
void	ft_init_signals(void)
{
	struct sigaction	sa_int;
	struct sigaction	sa_quit;

	sigemptyset(&sa_int.sa_mask);
	sa_int.sa_handler = ft_handle_sigint;
	sa_int.sa_flags = SA_RESTART;
	sigaction(SIGINT, &sa_int, NULL);
	sigemptyset(&sa_quit.sa_mask);
	sa_quit.sa_handler = SIG_IGN;
	sa_quit.sa_flags = 0;
	sigaction(SIGQUIT, &sa_quit, NULL);
}
