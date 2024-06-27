#include "minishell.h"

void enable_raw_mode(struct termios *orig_termios) {
    struct termios raw;
    tcgetattr(STDIN_FILENO, orig_termios);
    raw = *orig_termios;
    raw.c_lflag &= ~(ECHO | ICANON);
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

void disable_raw_mode(struct termios *orig_termios) {
    tcsetattr(STDIN_FILENO, TCSAFLUSH, orig_termios);
}

void refresh_line(const char *buffer, int index) {
    printf("\r\033[K"); // Clear the current line
    printf("%s%s", "minishell$ ", buffer); // Print the prompt and the buffer
    printf("\r%s", "minishell$ ");
    if (index > 0) {
        printf("\033[%dC", index); // Move cursor to the correct position
    }
    fflush(stdout);
}

char	*check_each_history(t_history *history)
// void    check_each_history(t_history *history)
{
    struct termios orig_termios;
    enable_raw_mode(&orig_termios);

    int index = 0;
    int command_length = 0;
    char c;
    char command[1024];

    memset(command, 0, sizeof(command));
    fflush(stdout);
    while (1) {
        read(STDIN_FILENO, &c, 1);
        if (c == '\n') {
            command[command_length] = '\0';
            printf("\n");
            break;
        } else if (c == '\x7f') {  // Backspace character
            if (index > 0) {
                for (int i = index - 1; i < command_length - 1; i++) {
                    command[i] = command[i + 1];
                }
                command[--command_length] = '\0';
                index--;
                refresh_line(command, index);
            }
        } else if (c == 27) { // Escape sequence
            char seq[2];
            if (read(STDIN_FILENO, &seq[0], 1) != 1) continue;
            if (read(STDIN_FILENO, &seq[1], 1) != 1) continue;
            if (seq[0] == '[') {
                if (seq[1] == 'A') {  // Up arrow
                    if (history->current_history_index > 0) {
                        history->current_history_index--;
                        strcpy(command, history->commands[history->current_history_index]);
                        index = command_length = strlen(command);
                        refresh_line(command, index);
                    }
                } else if (seq[1] == 'B') {  // Down arrow
                    if (history->current_history_index < history->count - 1) {
                        history->current_history_index++;
                        strcpy(command, history->commands[history->current_history_index]);
                        index = command_length = strlen(command);
                        refresh_line(command, index);
                    } else if (history->current_history_index == history->count - 1) {
                        history->current_history_index++;
                        memset(command, 0, sizeof(command));
                        index = command_length = 0;
                        refresh_line(command, index);
                    }
                } else if (seq[1] == 'C') {  // Right arrow
                    if (index < command_length) {
                        index++;
                        refresh_line(command, index);
                    }
                } else if (seq[1] == 'D') {  // Left arrow
                    if (index > 0) {
                        index--;
                        refresh_line(command, index);
                    }
                }
            }
        }
        else if (c == 4) // Ctrl+D
        {
            if (command_length == 0)
            {
                disable_raw_mode(&orig_termios);
                return NULL;
            }
        } else {
            if (command_length < (int)sizeof(command) - 1) {  // Cast sizeof(command) to int
                for (int i = command_length; i > index; i--) {
                    command[i] = command[i - 1];
                }
                command[index++] = c;
                command[++command_length] = '\0';
                refresh_line(command, index);
            }
        }
    }
    if (strlen(command) > 0) {
        add_to_history(history, command);
    }
    disable_raw_mode(&orig_termios);
    return strdup(command);
}
