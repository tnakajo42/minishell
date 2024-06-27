NAME = minishell

CC = cc
CCFLAGS = -Wall -Wextra -Werror

RLFLAGS = -lreadline

RM = rm -rf

DEPS = minishell.h

SRCS = main.c \
	signal_00.c \
	llist_00.c \
	llist_01.c \
	token_00.c \
	string_00.c \
	parser_00.c \
	parser_01.c \
	parser_02.c \
	command_00.c \
	command_01.c \
	history_00.c \
	history_01.c \
	ft_func.c

OBJS = $(SRCS:.c=.o)

%.o: %.c $(DEPS)
	$(CC) -c $< -o $@ $(CCFLAGS)

$(NAME): $(OBJS)
	$(CC) -o $@ $^ $(RLFLAGS)

all: $(NAME)

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean all

bonus:

.PHONY: all clean fclean re bonus
