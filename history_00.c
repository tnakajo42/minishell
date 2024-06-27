#include "minishell.h"

void init_history(t_history *history)
{
    history->last_exit_status = 0;
    history->count = 0;
    history->current_history_index = 0;
}

void add_to_history(t_history *history, const char *command)
{
    int i;
    
    i = 1;
    if (history->count < MAX_HISTORY)
    {
        history->commands[history->count] = strdup(command); // strdup allocates memory and copies the string
        history->count++;
    }
    else
    {
        // If history is full, free the oldest entry and shift others
        free(history->commands[0]);
        while (i < MAX_HISTORY)
        {
            history->commands[i - 1] = history->commands[i];
            i++;
        }
        history->commands[MAX_HISTORY - 1] = strdup(command);
    }
}

int show_history(t_history *history)
{
    int i;

    i = 0;
    while (history->count > i) {
        printf("    %i  %s\n", i + 1, history->commands[i]);
        i++;
    }
    return (0);
}

void free_history(t_history *history)
{
    int i;

    i = 0;
    while (history->count > i) {
        free(history->commands[i]);
        i++;
    }
}
