#include "minishell.h"

int	cmd_len(t_llist **head)
{
	int		n;
	int		flag;
	t_llist	*tmp;
	t_token	*token;

	n = 0;
	if (!*head)
		return (n);
	flag = 0;
	tmp = *head;
	while (1)
	{
		token = tmp->data;
		if (!strcmp(token->type, "pipe"))
			flag = -1;
		else if (!strcmp(token->type, "space"))
		{
			if (flag)
				flag = 0;
		}
		else
		{
			if (!flag)
			{
				n++;
				flag = 1;
			}	
		}
		tmp = tmp->next;
		if (flag == -1 || tmp == *head)
			break;
	}
	return (n);
}

t_command *cmd_get(t_llist **head)
{
    int i;
    int n;
    int flag;
    char *str;
    char *str_tmp;
    t_command *cmd;
    t_llist *tmp;
    t_llist *node;
    t_token *token;

    n = cmd_len(head);
    cmd = malloc(sizeof(t_command));
    cmd->args = string_2d_init(n);  // Ensure enough space for null-termination
    cmd->in_fd = STDIN_FILENO;  // Default input is stdin
    cmd->out_fd = STDOUT_FILENO; // Default output is stdout
    cmd->input_file = NULL;
    cmd->output_file = NULL;
    cmd->append_output = 0;
    cmd->heredoc_delimiter = NULL;
    if (!*head) {
        free(cmd);
        return NULL;
    }
    i = 0;
    flag = 0;
    str = NULL;
    tmp = *head;
    while (tmp)
    {
        token = tmp->data;
        if (strstr(token->type, "redir") != NULL)
        {
            // printf("redir\n");
            parse_redirections(cmd, &tmp);
            continue; // Skip processing this token further
        }
        else if (!strcmp(token->type, "pipe"))
        {
            flag = -1;
            cmd->out_fd = -1; // set up in main_loop
        }
        else if (!strcmp(token->type, "space"))
        {
            if (str)
            {
                cmd->args[i] = str;
                i++;
                str = NULL;
            }
        }
        else
        {
            str_tmp = str;
            str = ft_strcat(str_tmp, token->string);
            free(str_tmp);
        }
        tmp = tmp->next;
        node = llist_del(&tmp);
        llist_free(&node, token_free);
        if (flag == -1)
            break;
    }
    if (str)
        cmd->args[i] = str;
    // cmd->args[i] = NULL;  // Null-terminate the args array
    *head = tmp;
    return cmd;
}

char	*cmd_access(t_llist *paths, char *cmd)
{
	char	*full_path;
	t_llist	*tmp;

	if (!paths)
		return (NULL);
	tmp = paths;
	while (1)
	{
		full_path = ft_strcat(tmp->data, cmd);
		if (access(full_path, X_OK) == 0)
			return (full_path);
		free(full_path);
		tmp = tmp->next;
		if (paths == tmp)
			return (NULL);
	}
}

void cmd_execute(t_command *cmd, char **envp, t_history *history)
{
    int i;
    char *cmd_;
    char *path = NULL;
    t_llist *paths;
    int is_execve;

    if (!cmd || !cmd->args || !cmd->args[0])
    {
        fprintf(stderr, "Invalid command structure.\n");
        return;
    }

    if (!strcmp(cmd->args[0], "cd"))
        history->last_exit_status = cd_cmd(cmd->args);
    else if (!strcmp(cmd->args[0], "echo"))
        history->last_exit_status = echo_cmd(cmd->args, history);
    else if (!strcmp(cmd->args[0], "env"))
        history->last_exit_status = env_cmd();
    else if (!strcmp(cmd->args[0], "exit"))
        history->last_exit_status = exit_cmd(cmd->args);
    else if (!strcmp(cmd->args[0], "export"))
        history->last_exit_status = export_cmd(cmd->args);
    else if (!strcmp(cmd->args[0], "pwd"))
        history->last_exit_status = pwd_cmd();
    else if (!strcmp(cmd->args[0], "unset"))
        history->last_exit_status = unset_cmd(cmd->args);
    else if (!strcmp(cmd->args[0], "history"))
        history->last_exit_status = show_history(history);
    else
    {
        i = 0;
        while (envp[i] && strncmp(envp[i], "PATH=", 5) != 0)
            i++;
        if (envp[i])
            paths = ft_strsplit(&(envp[i])[5], ':');
        else
            paths = NULL;
        cmd_ = ft_strcat("/", cmd->args[0]);
        path = cmd_access(paths, cmd_);
        free(cmd_);
        llist_free(&paths, ft_strfree);

        int pid = fork();
        if (pid == 0)
        {
            // Handle input redirection
            if (cmd->input_file)
            {
                int fd = open(cmd->input_file, O_RDONLY);
                if (fd == -1)
                {
                    perror("open");
                    exit(1);
                }
                dup2(fd, STDIN_FILENO);
                close(fd);
            }
            else if (cmd->heredoc_delimiter)
            {
                int fd = handle_heredoc(cmd->heredoc_delimiter);
                if (fd == -1)
                    exit(1);
                dup2(fd, STDIN_FILENO);
                close(fd);
            }
            // Handle output redirection
            if (cmd->output_file)
            {
                int flags = O_WRONLY | O_CREAT;
                if (cmd->append_output)
                    flags |= O_APPEND;
                else
                    flags |= O_TRUNC;
                int fd = open(cmd->output_file, flags, 0644);
                if (fd == -1)
                {
                    perror("open");
                    exit(1);
                }
                dup2(fd, STDOUT_FILENO);
                close(fd);
            }
            // Execute the command
            if (strcmp(cmd->args[0], "echo") == 0)
                exit(echo_cmd(cmd->args, history));
            else
            {
                if (cmd->in_fd != STDIN_FILENO) 
                {
                    dup2(cmd->in_fd, STDIN_FILENO);
                    close(cmd->in_fd);
                }
                if (cmd->out_fd != STDOUT_FILENO) 
                {
                    dup2(cmd->out_fd, STDOUT_FILENO);
                    close(cmd->out_fd);
                }
                is_execve = execve(path, cmd->args, envp);
                if (is_execve)
                {
                    printf("%s: command not found\n", cmd->args[0]);
                    exit(127);
                }
            }
        }
        else if (pid > 0)
        {
            int status;
            waitpid(pid, &status, 0);
            if (WIFEXITED(status))
                history->last_exit_status = WEXITSTATUS(status);
            else if (WIFSIGNALED(status))
                history->last_exit_status = 128 + WTERMSIG(status);
        }
        else
        {
            perror("fork");
            history->last_exit_status = 1;
        }
        if (path)
            free(path);
    }
}
