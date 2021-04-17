#include "libft.h"

t_list	*ft_lst_at(t_list *lst, unsigned int n)
{
	while (lst && n--)
		lst = lst->next;
	return (lst);
}