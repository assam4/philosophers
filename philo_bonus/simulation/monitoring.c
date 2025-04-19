#include "simulation.h"

static void	*die_monitoring(void *param)
{
	t_philosopher	*philo;

	philo = (t_philosopher *)param;
	while (1)
	{
		usleep(INTERVAL);
		sem_wait(philo->dead_s);
		if (time_ms() - philo->last_eat_time > philo->table->time_to_die)
		{
			sem_wait(philo->table->stop);
			print_state(philo, DIED);
			sem_post(philo->dead_s);
			exit(EXIT_SUCCESS);
		}
		sem_post(philo->dead_s);
	}
	return (NULL);
}

static void	*eat_monitoring(t_table *table)
{
	while (1)
	{
		sem_wait(table->full);
			if (table->fullnes == table->philos_count)
			{
				sem_wait(table->print);
				printf(DINNER_OVER, time_ms() - table->start_time);
				sem_post(table->print);
				sem_post(table->full);
				sem_wait(table->stop);
				exit(EXIT_SUCCESS);
			}
		sem_post(table->full);
		usleep(INTERVAL);
	}
}

static int	create_forks(t_table *table)
{
	int	i;

	i = START_VAL;
	while (i < table->philos_count)
	{
		table->pids[i] = fork();
		if (table->pids[i] == -1)
			return (EAGAIN);
		else if (!table->pids[i])
		{
			pthread_create(&table->philos[i].die_m, NULL
				, die_monitoring, &table->philos[i]);
			life_cycle(&table->philos[i]);
			exit(EXIT_SUCCESS);
		}
		++i;
	}
	return (EXIT_SUCCESS);
}

int	start_simulation(t_table *table)
{
	int	i;

	table->start_time = time_ms();
	if (create_forks(table))
		return (printf(FORK_ERR), EAGAIN);
	if (table->must_eat_count)
		eat_monitoring(table);
	sem_wait(table->stop);
	i = START_VAL;
	while (i < table->philos_count)
	{
		kill(table->pids[i], SIGTERM);
		++i;
	}
	while (wait(NULL))
	{ }
	return (EXIT_SUCCESS);
}
