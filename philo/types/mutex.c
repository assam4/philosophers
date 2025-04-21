/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mutex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saslanya <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 12:29:18 by saslanya          #+#    #+#             */
/*   Updated: 2025/04/19 12:29:20 by saslanya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "types.h"

static int	init_monitoring_m(t_table *table)
{
	if (pthread_mutex_init(&table->print_m, NULL))
		return (EAGAIN);
	if (pthread_mutex_init(&table->time_m, NULL))
		return (pthread_mutex_destroy(&table->print_m), EAGAIN);
	if (pthread_mutex_init(&table->dead_m, NULL))
		return (pthread_mutex_destroy(&table->print_m)
			, pthread_mutex_destroy(&table->time_m), EAGAIN);
	if (table->must_eat_count
		&& pthread_mutex_init(&table->fullnes_m, NULL))
		return (pthread_mutex_destroy(&table->print_m)
			, pthread_mutex_destroy(&table->time_m)
			, pthread_mutex_destroy(&table->dead_m), EAGAIN);
	return (EXIT_SUCCESS);
}

int	init_mutexs(t_table *table)
{
	int	i;

	if (init_monitoring_m(table))
		return (printf(MUTEX_ERR), EAGAIN);
	i = LOOP_START;
	while (i < table->philos_count)
	{
		if (pthread_mutex_init(&table->forks[i], NULL))
			break ;
		++i;
	}
	if (i != table->philos_count)
	{
		while (--i >= LOOP_START)
			pthread_mutex_destroy(&table->forks[i]);
		pthread_mutex_destroy(&table->print_m);
		pthread_mutex_destroy(&table->time_m);
		pthread_mutex_destroy(&table->dead_m);
		if (table->must_eat_count)
			pthread_mutex_destroy(&table->fullnes_m);
		return (printf(MUTEX_ERR), EAGAIN);
	}
	return (EXIT_SUCCESS);
}

void	destroy_mutexs(t_table *table)
{
	int	i;

	pthread_mutex_destroy(&table->print_m);
	pthread_mutex_destroy(&table->time_m);
	pthread_mutex_destroy(&table->dead_m);
	if (table->must_eat_count)
		pthread_mutex_destroy(&table->fullnes_m);
	i = LOOP_START;
	while (i < table->philos_count)
	{
		pthread_mutex_destroy(&table->forks[i]);
		++i;
	}
}
