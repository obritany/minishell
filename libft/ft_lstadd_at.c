#include "libft.h"

void	ft_lstadd_at(t_list **lst, t_list *new, unsigned int n)
{
	t_list	*prev;
	t_list	*temp;

	if (!lst || !new)
		return ;
	prev = 0;
	temp = *lst;
	while(temp && n--)
	{
		prev = temp;
		temp = temp->next;
	}
	if (prev)
		prev->next = new;
	else
		*lst = new; 
	new->next = temp;
}