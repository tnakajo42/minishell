#include "minishell.h"

t_llist	*llist_create(void *data)
{
	t_llist	*node;

	node = (t_llist *)malloc(sizeof(t_llist));
	if (!node)
		return (NULL);
	node->data = data;
	node->prev = node;
	node->next = node;
	return (node);
}

void	llist_add(t_llist **head, t_llist *node)
{
	if (!*head)
		*head = node;
	else
	{
		node->prev = (*head)->prev;
		node->next = *head;
		((*head)->prev)->next = node;
		(*head)->prev = node;
	}
}

t_llist	*llist_del(t_llist **head)
{
	t_llist	*node;

	node = (*head)->prev;
	if (*head == (*head)->next)
		*head = NULL;
	else
	{
		(((*head)->prev)->prev)->next = *head;
		(*head)->prev = ((*head)->prev)->prev;
	}
	node->next = node;
	node->prev = node;
	return (node);
}

void	llist_free(t_llist **head, void(*free_data)(void *))
{
	t_llist	*tmp;

	if (!*head)
		return ;
	((*head)->prev)->next = NULL;
	while (*head)
	{
		tmp = *head;
		*head = (*head)->next;
		free_data(tmp->data);
		free(tmp);
	}
}
