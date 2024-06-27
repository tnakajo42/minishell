#include "minishell.h"

char	**string_2d_init(int n)
{
	int		i;
	char	**str;

	str = (char **)malloc(sizeof(char *) * (n + 1));
	if (!str)
		return (NULL);
	i = 0;
	while (i < n + 1)
	{
		str[i] = NULL;
		i++;
	}
	return (str);
}

void	string_2d_print(char **str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		printf("%s\n", str[i]);
		i++;
	}
}

void	string_2d_free(char **str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		free(str[i]);	// free string
		i++;
	}
	free(str[i]); 		// free last string = NULL
	free(str);			// free str
}
