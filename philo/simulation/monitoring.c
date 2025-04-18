#include "simulation.h"

static int	check_someone_die(t_table *table, int i)
{
	pthread_mutex_lock(&table->time_m);
	if (time_ms() - table->philos[i].last_eat_time > table->time_to_die)
	{
		pthread_mutex_unlock(&table->time_m);
		pthread_mutex_lock(&table->dead_m);
		table->someone_died = 1;
		pthread_mutex_unlock(&table->dead_m);
		pthread_mutex_lock(&table->print_m);
		printf(DIED, time_ms() - table->start_time, i + 1);
		pthread_mutex_unlock(&table->print_m);
		return (1);
	}
	pthread_mutex_unlock(&table->time_m);
	return (0);
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
	int	i;

	if (pthread_create(die_t, NULL, die_monitoring, (void *)table))
		return (EAGAIN);
	if (table->must_eat_count
		&& pthread_create(eat_t, NULL, satiated_monitoring, (void *)table))
		return (EAGAIN);
	i = LOOP_START;
	while (i < table->philos_count)
	{
		if (pthread_create(&table->philos[i].thread, NULL,
				&lifecycle, (void *)&(table->philos[i])) != 0)
			return (EAGAIN);
		++i;
	}
	return (EXIT_SUCCESS);
}

int	start_simulation(t_table *table)
{
	pthread_t	die_t;
	pthread_t	eat_t;
	int			i;

	table->start_time = time_ms();
	if (create_threads(table, &die_t, &eat_t))
		exit(EAGAIN);
	i = LOOP_START;
	while (i < table->philos_count)
	{
		pthread_join(table->philos[i].thread, NULL);
		++i;
	}
	pthread_join(die_t, NULL);
	if (table->must_eat_count)
		pthread_join(eat_t, NULL);
	return (EXIT_SUCCESS);
}
