#include "minishell.h"

int handle_heredoc(char *delimiter)
{
    int pipefd[2];
    char *line = NULL;
    size_t len = 0;
    ssize_t read;

    if (pipe(pipefd) == -1)
    {
        perror("pipe");
        return -1;
    }

    while ((read = getline(&line, &len, stdin)) != -1)
    {
        if (strcmp(line, delimiter) == 0)
            break;
        write(pipefd[1], line, read);
    }

    free(line);
    close(pipefd[1]);
    return pipefd[0];
}

void parse_redirections(t_command *cmd, t_llist **head)
{
    t_llist *tmp = *head;
    t_token *token = tmp->data;
    char *redir_type = token->type;

    // Move to the next token which should be the file name
    tmp = tmp->next;
    token = tmp->data;

    char *file_name = token->string;
    if (strcmp(redir_type, "redir_write") == 0) {
        if (cmd->output_file) free(cmd->output_file);
        cmd->output_file = strdup(file_name);
        cmd->append_output = 0;
    } else if (strcmp(redir_type, "redir_read") == 0) {
        if (cmd->input_file) free(cmd->input_file);
        cmd->input_file = strdup(file_name);
    } else if (strcmp(redir_type, "redir_append") == 0) {
        if (cmd->output_file) free(cmd->output_file);
        cmd->output_file = strdup(file_name);
        cmd->append_output = 1;
    } else if (strcmp(redir_type, "redir_delimiter") == 0) {
        if (cmd->heredoc_delimiter) free(cmd->heredoc_delimiter);
        cmd->heredoc_delimiter = strdup(file_name);
    }

    // Remove the redirection and file tokens from the list
    t_llist *redir_node = tmp->prev; // redirection token node
    t_llist *file_node = tmp; // file token node

    // Update the head if we are removing the first node
    if (redir_node == *head) {
        *head = file_node->next;
    }

    // Update links
    if (redir_node->prev) {
        redir_node->prev->next = file_node->next;
    }
    if (file_node->next) {
        file_node->next->prev = redir_node->prev;
    }

    // Free the nodes
    // Avoid freeing pointers to elements within the list's data structure itself, ensure to free the data inside the nodes
    free(redir_node->data); // Free the data within the node
    free(file_node->data); // Free the data within the node
    free(redir_node); // Free the node
    free(file_node); // Free the node
    printf("redirections are okay\n");
}
