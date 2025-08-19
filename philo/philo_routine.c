/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lusimon <lusimon@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 16:47:22 by lusimon           #+#    #+#             */
/*   Updated: 2025/08/19 12:19:16 by lusimon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void *philo_routine(void *arg)
{
    t_philo *philo = (t_philo *)arg;

    // small desync trick: odd philosophers wait a bit
	// otherwise possiblity to have a philo reaction time
	//what is the best?
    if (philo->id % 2 == 1)
        usleep(100);

    while (!simulation_stopped(philo->table))
    {
        think(philo);

        take_forks(philo);

        eat(philo);

        put_forks(philo);

        sleep_philo(philo);
    }
    return NULL;
}
