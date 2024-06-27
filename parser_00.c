#include "minishell.h"

void	parser(t_llist **input_llist, char *input_str)
{
	int		i;

	if (!input_str)
		return ;
	i = 0;
	while (input_str[i])
	{
		if (ft_isspace(input_str[i]))
			i = parser_space(input_llist, input_str, i);
		else if (input_str[i] == '\'')
			i = parser_quote_s(input_llist, input_str, i);
		else if (input_str[i] == '\"')
			i = parser_quote_d(input_llist, input_str, i);
		else if (input_str[i] == '>')
			i = parser_redir_right(input_llist, input_str, i);
		else if (input_str[i] == '<')
			i = parser_redir_left(input_llist, input_str, i);
		else if (input_str[i] == '|')
			i = parser_bar(input_llist, input_str, i);
		else if (input_str[i] == '&')
			i = parser_and(input_llist, input_str, i);
		else
			i = parser_string(input_llist, input_str, i);
	}
}
