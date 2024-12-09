/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oabdelka <oabdelka@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/09 13:16:24 by oabdelka          #+#    #+#             */
/*   Updated: 2024/12/09 13:16:24 by oabdelka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

void	*single_philosopher_routine(void *philo_void)
{
	t_philo	*philo;

	philo = (t_philo *)philo_void;
	log_philosopher_action(
		philo->id,
		philo->simulation,
		"has taken a fork."
		);
	usleep(philo->simulation->time_to_die * 1000);
	log_philosopher_action(
		philo->id,
		philo->simulation,
		"died."
		);
	return (NULL);
}

static int	acquire_fork_mutex(t_fork *fork, t_philo *philo)
{
	pthread_mutex_lock(&fork->mutex);
	if (!fork->philo_take)
	{
		log_philosopher_action(
			philo->id,
			philo->simulation,
			"has taken a fork."
			);
		fork->philo_take = philo->id;
	}
	if (fork->philo_take == philo->id)
	{
		pthread_mutex_unlock(&fork->mutex);
		return (1);
	}
	pthread_mutex_unlock(&fork->mutex);
	return (0);
}

static void	release_fork_mutex(t_fork *fork)
{
	pthread_mutex_lock(&fork->mutex);
	fork->philo_take = 0;
	pthread_mutex_unlock(&fork->mutex);
}

static void	philosopher_eat(t_philo *philo)
{
	while (1)
	{
		if (acquire_fork_mutex(&philo->simulation->fork[philo->left_fork],
				philo))
			break ;
		usleep(100);
	}
	while (1)
	{
		if (acquire_fork_mutex(&philo->simulation->fork[philo->right_fork],
				philo))
			break ;
		usleep(100);
	}
	pthread_mutex_lock(&philo->simulation->meal);
	log_philosopher_action(philo->id, philo->simulation, "is eating.");
	philo->lastmeal = current_timestamp();
	pthread_mutex_unlock(&philo->simulation->meal);
	accurate_sleep(philo->simulation->time_to_eat, philo->simulation);
	pthread_mutex_lock(&philo->simulation->eat);
	philo->eat_count++;
	pthread_mutex_unlock(&philo->simulation->eat);
	release_fork_mutex(&philo->simulation->fork[philo->left_fork]);
	release_fork_mutex(&philo->simulation->fork[philo->right_fork]);
}

void	*philosopher_thread(void *philo_void)
{
	t_philo	*philo;

	philo = (t_philo *)philo_void;
	if (philo->id % 2 == 0)
		usleep(1500);
	while (!has_any_philosopher_died(philo->simulation))
	{
		philosopher_eat(philo);
		if (has_simulation_end(philo->simulation))
			break ;
		log_philosopher_action(philo->id, philo->simulation, "is sleeping.");
		accurate_sleep(philo->simulation->time_to_sleep, philo->simulation);
		log_philosopher_action(philo->id, philo->simulation, "is thinking.");
	}
	return (NULL);
}
