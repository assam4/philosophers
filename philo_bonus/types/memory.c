#include "types.h"

long long	time_ms(void)
{
	struct timeval	time;
	long long		current_ms;

	gettimeofday(&time, NULL);
	current_ms = (((long long)time.tv_sec * 1000LL)
			+ (time.tv_usec / 1000));
	return (current_ms);
}

int	allocation_mem(int count, t_table **table)
{
	*table = malloc(sizeof(t_table));
	if (!*table)
		return (printf(ALLOC_ERR), ENOMEM);
	(*table)->philos = malloc(count * sizeof(t_philosopher));
	if (!(*table)->philos)
		return (free(*table), printf(ALLOC_ERR), ENOMEM);
	(*table)->pids = malloc(count * sizeof(pid_t));
	if (!(*table)->pids)
	{
		free((*table)->philos);
		free(*table);
		return (printf(ALLOC_ERR), ENOMEM);
	}
	return (EXIT_SUCCESS);
}

void	deallocation_mem(t_table **table)
{
	if (!table || !*table)
		return ;
	free((*table)->pids);
	free((*table)->philos);
	free(*table);
	*table = NULL;
}

void	set_userdef_params(int count, const char **params, t_table *table)
{
	if (count == VARG_COUNT2)
		table->must_eat_count = ft_atoi(params[--count]);
	else
		table->must_eat_count = START_VAL;
	table->time_to_sleep = ft_atoi(params[--count]);
	table->time_to_eat = ft_atoi(params[--count]);
	table->time_to_die = ft_atoi(params[--count]);
	table->philos_count = ft_atoi(params[--count]);
}

void	philos_init(t_table *table)
{
	int	i;

	table->start_time = time_ms();
	i = START_VAL;
	while ( i < table->philos_count)
	{
		table->philos[i].number = i + 1;
		table->philos[i].eat_count = START_VAL;
		table->philos[i].last_eat_time = table->start_time;
		table->philos[i].table = table;
		++i;
	}
}
