/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saslanya <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 18:35:41 by saslanya          #+#    #+#             */
/*   Updated: 2025/06/11 00:45:50 by saslanya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "simulation.h"

void	print_state(t_philosopher *philo, const char *message)
{
	sem_wait(philo->table->print);
	printf(message, time_ms() - philo->table->start_time, philo->number);
	sem_post(philo->table->print);
}

static void	must_eat(t_philosopher *philo)
{
	if (++philo->eat_count == philo->table->must_eat_count)
		sem_post(philo->table->full_stop);
}

static void	actions(t_philosopher *philo)
{
	long long	time;

	print_state(philo, EATING);
	time = time_ms();
	sem_wait(philo->die);
	philo->last_eat_time = time_ms();
	sem_post(philo->die);
	while (time_ms() - time < philo->table->time_to_eat)
		usleep(INTERVAL);
	sem_post(philo->table->forks);
	sem_post(philo->table->forks);
	if (philo->table->must_eat_count)
		must_eat(philo);
	time = time_ms();
	print_state(philo, SLEEPING);
	while (time_ms() - time < philo->table->time_to_sleep)
		usleep(INTERVAL);
	print_state(philo, THINKING);
}

static void	*check_is_dead(void *param)
{
	t_philosopher	*philo;

	philo = (t_philosopher *)param;
	while (1)
	{
		usleep(INTERVAL);
		sem_wait(philo->die);
		if (time_ms() - philo->last_eat_time - 2 > philo->table->time_to_die)
		{
			sem_wait(philo->table->print);
			printf(PHILO_DEAD,
				time_ms() - philo->table->start_time, philo->number);
			sem_post(philo->table->dead_stop);
			sem_post(philo->die);
			return (NULL);
		}
		else
			sem_post(philo->die);
	}
	return (NULL);
}

void	*life_cycle(void *param)
{
	t_philosopher	*philo;

	philo = (t_philosopher *)param;
	if (pthread_create(&philo->is_dead, NULL, check_is_dead, param))
		return (sem_post(philo->table->dead_stop), NULL);
	while (1)
	{
		sem_wait(philo->table->secure_lock);
		sem_wait(philo->table->forks);
		print_state(philo, GET_FORK);
		if (philo->table->philos_count == 1)
		{
			sem_post(philo->table->forks);
			sem_post(philo->table->secure_lock);
			usleep(philo->table->time_to_die * ML_TO_MK);
			print_state(philo, DEAD);
			sem_post(philo->table->dead_stop);
			return (NULL);
		}
		sem_wait(philo->table->forks);
		print_state(philo, GET_FORK);
		sem_post(philo->table->secure_lock);
		actions(philo);
	}
	return (NULL);
}
