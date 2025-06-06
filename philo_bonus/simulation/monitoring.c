/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitoring.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saslanya <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 18:35:47 by saslanya          #+#    #+#             */
/*   Updated: 2025/04/21 18:35:49 by saslanya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "simulation.h"

static void	*die_monitoring(void *param)
{
	int		i;
	t_table	*table;

	i = LOOP_START;
	table = (t_table *)param;
	sem_wait(table->dead_stop);
	while (i < table->philos_count)
	{
		kill(table->philos[i].pid, SIGKILL);
		++i;
	}
	destroy_semaphores(table);
	deallocation_mem(&table);
	exit(EXIT_SUCCESS);
}

static void	*eat_monitoring(void *param)
{
	t_table	*table;

	table = (t_table *)param;
	while (1)
	{
		sem_wait(table->full_stop);
		if (++table->fullnes_philos == table->philos_count)
		{
			sem_wait(table->print);
			printf(DINNER_OVER, time_ms() - table->start_time);
			sem_post(table->dead_stop);
			break ;
		}
	}
	return (NULL);
}

static int	create_forks(t_table *table)
{
	int	i;

	i = LOOP_START;
	while (i < table->philos_count)
	{
		table->philos[i].pid = fork();
		if (table->philos[i].pid == -1)
			return (EAGAIN);
		if (!table->philos[i].pid)
			life_cycle(&table->philos[i]);
		++i;
	}
	return (EXIT_SUCCESS);
}

int	start_simulation(t_table *table)
{
	table->start_time = time_ms();
	if (create_forks(table))
		return (printf(FORK_ERR), EAGAIN);
	pthread_create(&(table->dead_m), NULL, die_monitoring, table);
	if (table->must_eat_count)
	{
		table->fullnes_philos = START_VAL;
		pthread_create(&(table->eat_m), NULL, eat_monitoring, table);
		pthread_join(table->eat_m, NULL);
	}
	pthread_join(table->dead_m, NULL);
	return (EXIT_SUCCESS);
}
