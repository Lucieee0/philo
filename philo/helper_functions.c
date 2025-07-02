/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper_functions.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lusimon <lusimon@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 19:40:33 by lusimon           #+#    #+#             */
/*   Updated: 2025/07/02 19:40:44 by lusimon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_atoi(const char *str)
{
	int	integer;
	int	sign;

	integer = 0;
	sign = 1;
	while (*str == 32 || (*str >= 9 && *str <= 13))
	{
		str++;
	}
	if (*str == '+' || *str == '-')
	{
		if (*str == '-')
		{
			sign = -sign;
		}
		str++;
	}
	while (*str >= '0' && *str <= '9')
	{
		integer = integer * 10 + (*str - 48);
		str++;
	}
	return (integer * sign);
}

long	ft_atol(const char *str)
{
	long	integer;
	long	sign;

	integer = 0;
	sign = 1;
	while (*str == 32 || (*str >= 9 && *str <= 13))
	{
		str++;
	}
	if (*str == '+' || *str == '-')
	{
		if (*str == '-')
		{
			sign = -sign;
		}
		str++;
	}
	while (*str >= '0' && *str <= '9')
	{
		integer = integer * 10 + (*str - 48);
		str++;
	}
	return (integer * sign);
}
