#include "types.h"

int	allocation_mem(int count, t_table *table)
{
	table = malloc(sizeof(t_table));
	if (!table)
		return (printf(ALLOC_ERR), ENOMEM);
	table->philos = malloc(count * sizeof(t_philosopher));
	if (!table->philos)
		return (free(table), printf(ALLOC_ERR), ENOMEM);
	table->pids = malloc(count * sizeof(pid_t));
	if (!table->pids)
	{
		free(table->philos);
		free(table);
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
