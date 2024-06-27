#include "minishell.h"

int	ft_strlen(char *str)
{
	int	i;

	i = 0;
	if (!str)
		return (i);
	while (str[i])
		i++;
	return (i);
}

/* how many char before c
returns:
	i (pos.): number of character before c
	i (neg.): number of character before \0
*/
int	ft_strlen_to_char(char *str, char c)
{
	int	i;

	i = 0;
	if (!str)
		return (i);
	while (str[i] != c)
	{
		i++;
		if (!str[i])
			return (-i);
	}
	return (i);
}

/* how many char before space
returns:
	i (pos.): number of character before space
	i (neg.): number of character before \0
*/
int	ft_strlen_to_space(char *str)
{
	int	i;

	i = 0;
	if (!str)
		return (i);
	while (!ft_isspace(str[i]))
	{
		i++;
		if (!str[i])
			return (-i);
	}
	return (i);
}

/* how many char before space or redir or quote or logic
returns:
	i (pos.): number of character before space or redir or quote or logic
	i (neg.): number of character before \0
*/
int	ft_strlen_to_next(char *str)
{
	int	i;

	i = 0;
	if (!str)
		return (i);
	while (!ft_isspace(str[i]) && !ft_isredir(str[i]) && !ft_isquote(str[i]) && !ft_islogic(str[i]))
	{
		i++;
		if (!str[i])
			return (-i);
	}
	return (i);
}

t_llist	*ft_strsplit(char *str, char delimiter)
{
	int		i;
	int		j;
	char	*str_part;
	t_llist	*split;
	t_llist	*node;

	i = 0;
	j = 0;
	split = NULL;
	while (str[i])
	{
		j = ft_strlen_to_char(&str[i], delimiter);
		if (j < 0)
			j *= -1;
		str_part = ft_strget(str, i, i+j);
		node = llist_create(str_part);
		llist_add(&split, node);
		i += j;
		if (str[i])
			i++;
	}
	return split;
}

/* get part of string
args:
	str:	string
	start:	first character (included)
	end:	last character (not included --> \0)
*/
char	*ft_strget(char *str, int start, int end)
{
	int		i;
	char	*str_part;

	str_part = (char *)malloc(sizeof(char) * (end - start + 1));
	if (!str_part)
		return (NULL);
	i = 0;
	while (start+i < end)
	{
		str_part[i] = str[start + i]; 
		i++;
	}
	str_part[i] = '\0';
	return (str_part);
}

/* concatenate two strings
returns:
	str: concatenated string (new malloced)
*/
char	*ft_strcat(char *str1, char *str2)
{
	int		i;
	int		n1;
	int		n2;
	char	*str;

	n1 = ft_strlen(str1);
	n2 = ft_strlen(str2);
	str = (char *)malloc(sizeof(char) * (n1 + n2 + 1));
	if (!str)
		return (NULL);
	i = 0;
	while (i < n1)
	{
		str[i] = str1[i];
		i++;
	}
	i = 0;
	while (i < n2)
	{
		str[n1 + i] = str2[i];
		i++;
	}
	str[n1 + i] = '\0';
	return (str);
}

void	ft_strprint(void *data)
{
	char	*str;

	str = (char *)data;
	printf("%s\n", str);
}

void	ft_strfree(void *data)
{
	char	*str;

	str = (char *)data;
	free(str);
}

int	ft_isspace(char c)
{
	if (c == '\t' || c == '\n' || c == '\v'
		|| c == '\f' || c == '\r' || c == ' ')
		return (1);
	return (0);
}

int	ft_isredir(char c)
{
	if (c == '>' || c == '<')
		return (1);
	return (0);
}

int	ft_isquote(char c)
{
	if (c == '\"' || c == '\'')
		return (1);
	return (0);
}

int	ft_islogic(char c)
{
	if (c == '&' || c == '|')
		return (1);
	return (0);
}
