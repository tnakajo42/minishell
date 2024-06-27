#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <unistd.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <stdlib.h>
# include <signal.h>
# include <sys/wait.h>
# include <termios.h>
# include <pwd.h>
# include <sys/stat.h> // for <, <<, >, >>
# include <fcntl.h>// for <, <<, >, >>

# define MAX_INPUT_SIZE	1024
# define MAX_ARG_SIZE	64
# define MAX_PATH_SIZE	256
# define PATH_MAX		4096
# define MAX_HISTORY 	1000

typedef struct s_llist
{
	void			*data;
	struct s_llist	*prev;
	struct s_llist	*next;
}	t_llist;

typedef struct s_token
{
	char	*string;
	char	*type;
}	t_token;

typedef struct s_history
{
    int last_exit_status;
    char *commands[MAX_HISTORY];
    int count;
	int current_history_index;
}	t_history;

typedef struct s_command
{
    char    **args;
    int     in_fd;
    int     out_fd;
    char    *input_file;
    char    *output_file;
    int     append_output;
    char    *heredoc_delimiter;
}   t_command;

// ft_func.c
int				ft_strlen(char *str);
int				ft_strlen_to_char(char *str, char c);
int				ft_strlen_to_space(char *str);
int				ft_strlen_to_next(char *str);

t_llist			*ft_strsplit(char *str, char delimiter);
char			*ft_strget(char *str, int start, int end);
char			*ft_strcat(char *str1, char *str2);
void			ft_strprint(void *data);
void			ft_strfree(void *data);

int				ft_isspace(char c);
int				ft_isredir(char c);
int				ft_isquote(char c);
int				ft_islogic(char c);

// signal_*.c
void			signal_handler(int signal);
void	handle_sigint(int signal);
void	handle_sigquit(int signal);

// parser_*.c
void			parser(t_llist **input_llist, char *input_str);

int				parser_space(t_llist **input_llist, char *input_str, int i);
int				parser_quote_s(t_llist **input_llist, char *input_str, int i);
int				parser_quote_d(t_llist **input_llist, char *input_str, int i);
int				parser_redir_right(t_llist **input_llist, char *input_str, int i);
int				parser_redir_left(t_llist **input_llist, char *input_str, int i);
int				parser_bar(t_llist **input_llist, char *input_str, int i);
int				parser_and(t_llist **input_llist, char *input_str, int i);
int				parser_string(t_llist **input_llist, char *input_str, int i);
int				handle_heredoc(char *delimiter);
void			parse_redirections(t_command *cmd, t_llist **tokens);

// command_*.c
t_command		*cmd_get(t_llist **tokens);
int				cmd_len(t_llist **tokens);
char			*cmd_access(t_llist *paths, char *cmd);
void			cmd_execute(t_command *cmd, char **envp, t_history *history);
int				cd_cmd(char **args);
int				echo_cmd(char **arg, t_history *history);
int				env_cmd();
int				exit_cmd(char **args);
int				export_cmd(char **args);
int				pwd_cmd();
int				unset_cmd(char **args);

// llist_00.c
t_llist			*llist_create(void *data);
void			llist_add(t_llist **head, t_llist *node);
t_llist			*llist_del(t_llist **head);
void			llist_free(t_llist **head, void(*free_data)(void *));

// llist_01.c
int				llist_len(t_llist *head);
void			llist_print(t_llist *head, void(*print_data)(void *));

// token_00.c
t_token			*token_create(char *token, char *type);
void			token_print(void *data);
void			token_free(void *data);

// string_00.c
char			**string_2d_init(int n);
void			string_2d_print(char **str);
void			string_2d_free(char **str);

// history_00.c
void			init_history(t_history *history);
void			add_to_history(t_history *history, const char *command);
int				show_history(t_history *history);
void			free_history(t_history *history);

// history_01.c
void			enable_raw_mode(struct termios *orig_termios);
void			disable_raw_mode(struct termios *orig_termios);
// void 			check_each_history(t_history *history);
char 			*check_each_history(t_history *history);

#endif
