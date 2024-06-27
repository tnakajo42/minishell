#include "minishell.h"

void	signal_handler(int signal)
{
	if (signal == SIGINT)
	{
		printf("\n");
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

void	handle_sigint(int signal) {
    (void)signal;
    printf("\nminishell$ ");
    fflush(stdout);
}

void	handle_sigquit(int signal) {
    (void)signal;
    // Do nothing
}
