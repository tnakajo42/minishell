#include "minishell.h"

t_token	*token_create(char *string, char *type)
{
	t_token	*token;

	token = (t_token *)malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->string = string;
	token->type = type;
	return (token);
}

void	token_free(void *data)
{
	t_token	*token;

	token = (t_token *)data;
	free(token->string);
	free(token);
	// free(token->type);
}

void	token_print(void *data)
{
	t_token	*token;

	token = (t_token *)data;
	printf("string: %s\n", token->string);
	printf("type: %s\n", token->type);
}
