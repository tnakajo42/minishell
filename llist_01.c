#include "minishell.h"

int	llist_len(t_llist *head)
{
	int		i;
	t_llist	*tmp;

	i = 0;
	if (!head)
		return (i);
	tmp = head;
	while (1)
	{
		i++;
		tmp = tmp->next;
		if (head == tmp)
			return (i);
	}
}

void	llist_print(t_llist *head, void(*print_data)(void *))
{
	void	*data;
	t_llist	*tmp;

	if (!head)
	{
		printf("-\n");
		printf("head is empty\n");
		printf("-\n");
		return ;
	}
	tmp = head;
	while (1)
	{
		data = tmp->data;
		tmp = tmp->next;
		printf("-\n");
		print_data(data);
		if (head == tmp)
		{
			printf("-\n");
			return ;
		}
	}
}
