/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitoring.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saslanya <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 12:29:57 by saslanya          #+#    #+#             */
/*   Updated: 2025/07/31 14:00:07 by saslanya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "simulation.h"

static int	check_someone_die(t_table *table, int i)
{
	pthread_mutex_lock(&table->time_m);
	if (time_ms() - table->philos[i].last_eat_time - 1 > table->time_to_die)
	{
		pthread_mutex_unlock(&table->time_m);
		pthread_mutex_lock(&table->dead_m);
		table->someone_died = 1;
		pthread_mutex_unlock(&table->dead_m);
		pthread_mutex_lock(&table->print_m);
		printf(DIED, time_ms() - table->start_time, i + 1);
		pthread_mutex_unlock(&table->print_m);
		return (EXIT_FAILURE);
	}
	pthread_mutex_unlock(&table->time_m);
	return (EXIT_SUCCESS);
}

static void	*die_monitoring(void *param)
{
	t_table		*table;
	int			i;

	table = (t_table *)param;
	while (1)
	{
		i = LOOP_START;
		while (i < table->philos_count)
		{
			if (check_someone_die(table, i))
				return (NULL);
			pthread_mutex_lock(&table->fullnes_m);
			if (table->fullnes == table->philos_count)
				return (pthread_mutex_unlock(&table->fullnes_m), NULL);
			pthread_mutex_unlock(&table->fullnes_m);
			++i;
		}
	}
	return (NULL);
}

static void	*satiated_monitoring(void *param)
{
	t_table	*table;

	table = (t_table *)param;
	while (1)
	{
		pthread_mutex_lock(&table->fullnes_m);
		if (table->must_eat_count && table->fullnes == table->philos_count)
		{
			pthread_mutex_unlock(&table->fullnes_m);
			pthread_mutex_lock(&table->dead_m);
			table->someone_died = 1;
			pthread_mutex_unlock(&table->dead_m);
			pthread_mutex_lock(&table->print_m);
			printf(DINNER_OVER, time_ms() - table->start_time);
			pthread_mutex_unlock(&table->print_m);
			return (NULL);
		}
		pthread_mutex_unlock(&table->fullnes_m);
		pthread_mutex_lock(&table->dead_m);
		if (table->someone_died)
			return (pthread_mutex_unlock(&table->dead_m), NULL);
		pthread_mutex_unlock(&table->dead_m);
	}
	return (NULL);
}

static int	create_threads(t_table *table, pthread_t *die_t, pthread_t *eat_t)
{
	int			i;
	pthread_t	*monitorings[2];

	monitorings[0] = NULL;
	monitorings[1] = NULL;
	i = LOOP_START;
	if (pthread_create(die_t, NULL, die_monitoring, (void *)table))
		return (EAGAIN);
	monitorings[0] = die_t;
	if (table->must_eat_count)
	{
		if (pthread_create(eat_t, NULL, satiated_monitoring, (void *)table))
			return (join_threads(table, i, monitorings, 1), EAGAIN);
		else
			monitorings[1] = eat_t;
	}
	while (i < table->philos_count)
	{
		if (pthread_create(&table->philos[i].thread, NULL,
				&lifecycle, (void *)&(table->philos[i])))
			return (join_threads(table, i, monitorings, 1), EAGAIN);
		++i;
	}
	return (EXIT_SUCCESS);
}

int	start_simulation(t_table *table)
{
	pthread_t	die_t;
	pthread_t	eat_t;

	table->start_time = time_ms();
	if (create_threads(table, &die_t, &eat_t))
		return (EAGAIN);
	if (table->must_eat_count)
		join_threads(table, table->philos_count,
			(pthread_t *[]){&die_t, &eat_t}, 0);
	else
		join_threads(table, table->philos_count,
			(pthread_t *[]){&die_t, NULL}, 0);
	return (EXIT_SUCCESS);
}
