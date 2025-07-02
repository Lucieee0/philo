/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lusimon <lusimon@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 19:45:44 by lusimon           #+#    #+#             */
/*   Updated: 2025/07/02 19:46:07 by lusimon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

pthread_mutex_t	fork_one_lock;
pthread_mutex_t	fork_two_lock;
pthread_mutex_t	fork_three_lock;
int				eaten = 0;

void	*take_two_forks(void *data)
{
	//if I am philo 1
	pthread_mutex_lock(&fork_one_lock);
	pthread_mutex_lock(&fork_three_lock);
	//if fork 1 and 3 locked proceed
	if (//fork 1 and 3 locked)
			eaten += 1;
	//else if free everyforks
	pthread_mutex_unlock(&fork_one_lock);
	pthread_mutex_unlock(&fork_three_lock);
	return (NULL);
}

int	main(void)
{
	pthread_t	philo_1;
	pthread_t	philo_2;
	pthread_t	philo_3;

	pthread_mutex_init(&fork_one_lock, NULL);
	pthread_mutex_init(&fork_two_lock, NULL);
	pthread_mutex_init(&fork_three_lock, NULL);
	pthread_create(&philo_1, NULL, take_two_forks, NULL);
	pthread_create(&philo_2, NULL, take_two_forks, NULL);
	pthread_create(&philo_2, NULL, take_two_forks, NULL);
	pthread_join(philo_1, NULL);
	pthread_join(philo_2, NULL);
	pthread_join(philo_3, NULL);
	printf("%d\n", eaten);
}
