#include "minishell.h"

int	parser_space(t_llist **input_llist, char *input_str, int i)
{
	char	*string;
	char	*type;
	t_token	*token;
	t_llist	*node;

	while (ft_isspace(input_str[i]))
		i++;
	string = ft_strget(" ", 0, 1);
	type = "space";
	token = token_create(string, type);
	node = llist_create(token);
	llist_add(input_llist, node);
	return (i);
}

int	parser_quote_s(t_llist **input_llist, char *input_str, int i)
{
	int		j;
	char	*string;
	char	*type;
	t_token	*token;
	t_llist	*node;

	i++;
	j = ft_strlen_to_char(&input_str[i], '\'');
	type = "string";
	if (j < 0) // no closing '
	{
		printf("-->no closing \' <--\n");
		j *= -1;
	}
	string = ft_strget(input_str, i, i+j);
	token = token_create(string, type);
	node = llist_create(token);
	llist_add(input_llist, node);
	i += j + 1;
	return (i);
}

int	parser_quote_d(t_llist **input_llist, char *input_str, int i)
{
	int		j;
	char	*string;
	char	*type;
	t_token	*token;
	t_llist	*node;

	i++;
	j = ft_strlen_to_char(&input_str[i], '\"');
	type = "string";
	if (j < 0) // no closing "
	{
		printf("-->no closing \" <--\n");
		j *= -1;
	}
	string = ft_strget(input_str, i, i+j);
	token = token_create(string, type);
	node = llist_create(token);
	llist_add(input_llist, node);
	i += j + 1;
	return (i);
}

int	parser_redir_right(t_llist **input_llist, char *input_str, int i)
{
	int		j;
	char	*string;
	char	*type;
	t_token	*token;
	t_llist	*node;

	j = 1;
	type = "redir_write";
	if (input_str[i+1] == '>')
	{
		j = 2;
		type = "redir_append";
	}
	string = ft_strget(input_str, i, i+j);
	token = token_create(string, type);
	node = llist_create(token);
	llist_add(input_llist, node);
	i += j;
	return (i);
}

int	parser_redir_left(t_llist **input_llist, char *input_str, int i)
{
	int		j;
	char	*string;
	char	*type;
	t_token	*token;
	t_llist	*node;

	j = 1;
	type = "redir_read";
	if (input_str[i+1] == '<')
	{
		j = 2;
		type = "redir_delimiter";
	}
	string = ft_strget(input_str, i, i+j);
	token = token_create(string, type);
	node = llist_create(token);
	llist_add(input_llist, node);
	i += j;
	return (i);
}

int	parser_bar(t_llist **input_llist, char *input_str, int i)
{
	int		j;
	char	*string;
	char	*type;
	t_token	*token;
	t_llist	*node;

	j = 1;
	type = "pipe";
	if (input_str[i+1] == '|')
	{
		j = 2;
		type = "or";
	}
	string = ft_strget(input_str, i, i+j);
	token = token_create(string, type);
	node = llist_create(token);
	llist_add(input_llist, node);
	i += j;
	return (i);
}

int	parser_and(t_llist **input_llist, char *input_str, int i)
{
	int		j;
	char	*string;
	char	*type;
	t_token	*token;
	t_llist	*node;

	j = 1;
	type = "background";
	if (input_str[i+1] == '&')
	{
		j = 2;
		type = "and";
	}
	string = ft_strget(input_str, i, i+j);
	token = token_create(string, type);
	node = llist_create(token);
	llist_add(input_llist, node);
	i += j;
	return (i);
}

int	parser_string(t_llist **input_llist, char *input_str, int i)
{
	int		j;
	char	*string;
	char	*type;
	t_token	*token;
	t_llist	*node;

	j = ft_strlen_to_next(&input_str[i]);
	type = "string";
	if (j < 0)
	{
		j *= -1;
	}
	string = ft_strget(input_str, i, i+j);
	token = token_create(string, type);
	node = llist_create(token);
	llist_add(input_llist, node);
	i += j;
	return (i);
}
