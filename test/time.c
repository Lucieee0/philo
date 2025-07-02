/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lusimon <lusimon@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 19:46:28 by lusimon           #+#    #+#             */
/*   Updated: 2025/07/02 19:46:30 by lusimon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include <sys/time.h>
# include <stdio.h>
# include <unistd.h>

//printing the elapsed time in milliseconds since the start of the program

int	main(void)
{
	struct timeval	tv;
	long			start;
	long			now;

	start = 0;
	now = 0;
	gettimeofday(&tv, NULL);
	start = tv.tv_sec * 1000 + tv.tv_usec / 1000;
	printf("start time: %ld\n", start);
	while (1)
	{
		usleep(5000000); //pause 5 seconds
		gettimeofday(&tv, NULL);
		now = tv.tv_sec * 1000 + tv.tv_usec / 1000;
		printf("%ld\n", (now - start));
	}
	return (1);
}
