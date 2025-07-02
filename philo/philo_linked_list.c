/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_linked_list.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lusimon <lusimon@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 19:42:35 by lusimon           #+#    #+#             */
/*   Updated: 2025/07/02 19:50:14 by lusimon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

//I want to create here a circular linked list of philos
//the program take as argument the "number_of_philosophers"
//one node per philo
//one philo + one fork

t_philo	*ft_lstnew(int id, t_table *table)
{
	t_philo	*philo;

	philo = (t_philo *)malloc(sizeof(t_philo));
	if (!philo)
		return (NULL);
	philo->id = id;
	philo->next = NULL;
	philo->times_eaten = 0;
	philo->last_meal_time = 0;
	philo->state = THINKING;
	philo->table = table;
	if (pthread_mutex_init(&philo->fork, NULL) != 0)
	{
		free(philo);
		return (NULL);
	}
	return (philo);
}

t_philo	*ft_lstlast(t_philo *philo)
{
	if (!philo)
		return (NULL);
	while (philo->next)
	{
		philo = philo->next;
	}
	return (philo);
}

void	ft_lstadd_back(t_philo **lst_philo, int id, t_table *table)
{
	t_philo	*last;
	t_philo	*new_philo;

	if (!lst_philo)
		return ;
	new_philo = ft_lstnew(id, table);
	if (!new_philo)
		return ;
	last = ft_lstlast(*lst_philo);
	if (!last)
	{
		*lst_philo = new_philo;
		return ;
	}
	last->next = new_philo;
}

t_philo	*create_philo_circular_linked_list(t_table *table)
{
	int		i;
	t_philo	*lst_philo;
	t_philo	*last;

	lst_philo = NULL;
	i = 0;
	while (i < table->nbr_philo)
	{
		ft_lstadd_back(&lst_philo, i + 1, table);
		i++;
	}
	//make the last node point to the first one
	//circular linked list
	if (lst_philo)
	{
		last = ft_lstlast(lst_philo);
		last->next = lst_philo;
	}
	return (lst_philo);
}
