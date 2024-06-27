#include "minishell.h"

void main_loop (t_llist **input_llist, t_history *history, char **envp)
{
	char		*input_str;
	// char		*readline_str;
    t_command	*cmd;
    int 		pipe_fds[2];
    int 		last_pipe_read_fd = -1;

    signal(SIGINT, handle_sigint);
    signal(SIGQUIT, handle_sigquit);

	while(1)
	{
        // input_str = readline("minishell$ ");
		
		printf("minishell$ ");
        fflush(stdout);
		input_str = check_each_history(history);
		// check_each_history(history);
		
		// Ctrl + D (EOF)
		if (!input_str)
            break;
		// pareser
		// llist_print(*input_llist, token_print);
		parser(input_llist, input_str);
		while (*input_llist)
		{
			// handle pipes
			if (pipe(pipe_fds) == -1)
			{
				perror("pipe");
				exit(EXIT_FAILURE);
			}

			// get command
			cmd = cmd_get(input_llist);
			/* TODO:
				check here or in the echo_cmd, ... functions
				check whether > in cmd list
				check whether < in cmd list
				check whether >> in cmd list
				check whether << in cmd list
			*/
			if (!*input_llist)
				cmd->out_fd = STDOUT_FILENO; // last command output to stdout which is 1
			else
				cmd->out_fd = pipe_fds[1];
			if (last_pipe_read_fd != -1)
				cmd->in_fd = last_pipe_read_fd;
			// printf("get command:\n");
			// string_2d_print(cmd);
			// execute command
			// printf("cmd->out_fd: %d\n", cmd->out_fd);
			if (cmd->out_fd == STDOUT_FILENO) // 1
				cmd_execute(cmd, envp, history);
			// printf("execute command:\n");
			if (cmd->out_fd != STDIN_FILENO && !cmd->output_file) // not equal to 1
			{
				close(pipe_fds[1]);
				last_pipe_read_fd = pipe_fds[0];
			}
			else
			{
				if (last_pipe_read_fd != -1) {
					close(last_pipe_read_fd);
				}
				last_pipe_read_fd = -1; // reset last_out_fd for the next set of commands
			}
			string_2d_free(cmd->args);
			free(cmd);
		}
		// while (wait(NULL) > 0); // wait for all child processes to finish
		if (last_pipe_read_fd != -1) // close the remaining file descriptor if any
		{
			close(last_pipe_read_fd);
			last_pipe_read_fd = -1;
		}
		history->current_history_index = history->count;
		llist_free(input_llist, free);
		free(input_str);
		// free(readline_str);
	}
    free_history(history);
}

int	main(int argc, char **argv, char **envp)
{
	t_llist			*input_llist;
	t_history		history;

	if (argc != 1)
	{
		printf("no argument needed\n");
		return (0);
	}
	(void) argv;
	// signal(SIGINT, signal_handler);
	input_llist = NULL;
    init_history(&history);
	main_loop(&input_llist, &history, envp);

	return (0);
}
