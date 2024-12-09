/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oabdelka <oabdelka@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/09 12:29:33 by oabdelka          #+#    #+#             */
/*   Updated: 2024/12/09 12:29:33 by oabdelka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

int	ft_atoi(const char *str)
{
	long	result;
	int		sign;

	result = 0;
	sign = 1;
	while (*str == ' ' || (*str >= '\t' && *str <= '\r'))
		str++;
	if (*str == '-' || *str == '+')
	{
		if (*str == '-')
			sign = -1;
		str++;
	}
	while (*str >= '0' && *str <= '9')
	{
		result = result * 10 + (*str - '0');
		if (sign == 1 && result > INT_MAX)
			return (-1);
		if (sign == -1 && (-result) < INT_MIN)
			return (-1);
		str++;
	}
	return ((int)(result * sign));
}

long int	current_timestamp(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

void	accurate_sleep(long int time_in_ms, t_simulation *simulation)
{
	long int	start_time;

	start_time = current_timestamp();
	while (current_timestamp() - start_time < time_in_ms)
	{
		if (has_any_philosopher_died(simulation))
			break ;
		if (has_simulation_end(simulation))
			break ;
		usleep(100);
	}
}

void	log_philosopher_action(int id, t_simulation *simulation, char *action)
{
	pthread_mutex_lock(&(simulation->print));
	if (!has_any_philosopher_died(simulation))
	{
		if (!has_simulation_end(simulation))
		{
			printf("%ld %d %s\n",
				current_timestamp() - simulation->start_time,
				id,
				action);
		}
	}
	pthread_mutex_unlock(&(simulation->print));
}
