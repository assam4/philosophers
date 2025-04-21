/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saslanya <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 12:29:51 by saslanya          #+#    #+#             */
/*   Updated: 2025/04/21 18:48:33 by saslanya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "simulation.h"

void	print_state(t_philosopher *philo, const char *format)
{
	pthread_mutex_lock(&philo->table->print_m);
	pthread_mutex_lock(&philo->table->dead_m);
	if (!philo->table->someone_died)
		printf(format, time_ms() - philo->table->start_time, philo->pid);
	pthread_mutex_unlock(&philo->table->dead_m);
	pthread_mutex_unlock(&philo->table->print_m);
}

static int	take_forks(t_philosopher *philo)
{
	if (!(philo->pid % 2))
	{
		pthread_mutex_lock(philo->left_fork);
		print_state(philo, GET_FORK);
		pthread_mutex_lock(philo->right_fork);
		print_state(philo, GET_FORK);
	}
	else
	{
		pthread_mutex_lock(philo->right_fork);
		print_state(philo, GET_FORK);
		if (philo->table->philos_count == 1)
			return (pthread_mutex_unlock(philo->right_fork), EXIT_FAILURE);
		pthread_mutex_lock(philo->left_fork);
		print_state(philo, GET_FORK);
	}
	return (EXIT_SUCCESS);
}

static void	giveup_forks(t_philosopher *philo);

static void	actions(t_philosopher *philo)
{
	long long	time_start;

	pthread_mutex_lock(&philo->table->time_m);
	philo->last_eat_time = time_ms();
	pthread_mutex_unlock(&philo->table->time_m);
	print_state(philo, EATING);
	time_start = time_ms();
	while (time_ms() - time_start < philo->table->time_to_eat)
		usleep(INTERVAL);
	++philo->meals_eaten;
	if (philo->table->must_eat_count
		&& philo->meals_eaten == philo->table->must_eat_count)
	{
		pthread_mutex_lock(&philo->table->fullnes_m);
		++(philo->table->fullnes);
		pthread_mutex_unlock(&philo->table->fullnes_m);
	}
	giveup_forks(philo);
	print_state(philo, SLEEPING);
	time_start = time_ms();
	while (time_ms() - time_start < philo->table->time_to_sleep)
		usleep(INTERVAL);
	print_state(philo, THINKING);
}

static void	giveup_forks(t_philosopher *philo)
{
	if (!(philo->pid % 2))
	{
		pthread_mutex_unlock(philo->left_fork);
		pthread_mutex_unlock(philo->right_fork);
	}
	else
	{
		pthread_mutex_unlock(philo->right_fork);
		pthread_mutex_unlock(philo->left_fork);
	}
}

void	*lifecycle(void *param)
{
	t_philosopher	*philo;

	philo = (t_philosopher *)param;
	while (1)
	{
		pthread_mutex_lock(&philo->table->dead_m);
		if (philo->table->someone_died)
			return (pthread_mutex_unlock(&philo->table->dead_m), NULL);
		pthread_mutex_unlock(&philo->table->dead_m);
		if (take_forks(philo))
			usleep((philo->table->time_to_die + 1) * ML_TO_MK);
		else
			actions(philo);
	}
	return (NULL);
}
