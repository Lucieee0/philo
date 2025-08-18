/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lusimon <lusimon@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 19:45:44 by lusimon           #+#    #+#             */
/*   Updated: 2025/08/18 15:10:09 by lusimon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

//If you want, I can show you the next step: how to start one thread per philosopher,
//integrate odd/even fork order, and use reaction_time for desynchronization in the 
//main simulation loop. This will complete the setup for 200 philosophers safely.

pthread_mutex_t	fork_one_lock;
pthread_mutex_t	fork_two_lock;
pthread_mutex_t	fork_three_lock;
int				eaten = 0;

void	*take_two_forks(void *data)
{
	//it takes a certain time for this function to execute
	//Example sleep(2); simulate the time taken to prepare a coffee
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
	//way to initialize the stuct, the lock
	pthread_mutex_init(&fork_two_lock, NULL);
	pthread_mutex_init(&fork_three_lock, NULL);
	pthread_create(&philo_1, NULL, take_two_forks, NULL);
	//pthread_create: adress of the variable (philo_1), attributes (don't need it at the beginning, so NULL),
	//start_routine -> pointer to a function, returns a void pointer -> the actual job a specific thread as to perform,
	//arguments NULL at the beginning
	pthread_create(&philo_2, NULL, take_two_forks, NULL);
	pthread_create(&philo_2, NULL, take_two_forks, NULL);
	//if successful, the pthread_create() function will return zero. Otherwise an error
	//number will be returned to indicate the error
	printf("Identifier -> %lu\n", philo_1);
	printf("Identifier -> %lu\n", philo_2);
	printf("Identifier -> %lu\n", philo_3);
	//this unique identifier allows us to indentify the thread running
	pthread_join(philo_1, NULL);
	pthread_join(philo_2, NULL);
	pthread_join(philo_3, NULL);
	//we need to wait for the threads to perform their tasks
	//the function take_two_forks, will take a certain time
	//so instead of having a sleep function, we have pthread_join
	//we wait for the return value NULL from the function take_two_forks
	printf("%d\n", eaten);
}

//race conditions
//starbucks analogy
//Two threads (the waiters) racing towards a share data (the ledger)

//Ledger -> read value (analogy) -> load operation (computer)
//A load operation in computing is when the CPU reads data from memory (RAM)
//and brings it into its registers so it can use it for processing.
//load = fetch data from memory to CPU -> to work on it.

//Ledger -> write data (analogy) -> store (computer)
//store = CPU → Memory (save data from the CPU back into memory)

//so the solution is to use a lock "for the ledger"


//difference between a thread and a process
//A process is the actual starbucks shop
//threads are the actual workers inside the starbuck accomplishing the mission

//thread share the same memories
//processes are different entities (copy of themselves)

//Race conditions are an intra_process thing

//IT IS NOT an inter-process thing
//child process the ledger is unique in the child
//fater waits for his baby to finish
//the ledger is unique in the father

//so for example a race codition can happen inside the childprocess 




