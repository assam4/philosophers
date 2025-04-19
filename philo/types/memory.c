/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saslanya <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 12:29:12 by saslanya          #+#    #+#             */
/*   Updated: 2025/04/19 12:31:15 by saslanya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
	(*table)->forks = malloc(sizeof(pthread_mutex_t) * count);
	if (!(*table)->forks)
		return (free(*table), printf(ALLOC_ERR), ENOMEM);
	(*table)->philos = malloc(sizeof(t_philosopher) * count);
	if (!(*table)->philos)
		return (free((*table)->forks), free(*table),
			printf(ALLOC_ERR), ENOMEM);
	return (EXIT_SUCCESS);
}

void	deallocation_mem(t_table **table)
{
	free((*table)->forks);
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
	table->someone_died = START_VAL;
	table->fullnes = START_VAL;
}

void	philos_init(t_table *table)
{
	int	i;

	i = LOOP_START;
	table->start_time = time_ms();
	while (i < table->philos_count)
	{
		table->philos[i].pid = i + 1;
		table->philos[i].meals_eaten = START_VAL;
		table->philos[i].last_eat_time = table->start_time;
		table->philos[i].left_fork = &table->forks[i];
		table->philos[i].right_fork = &table->forks[(i + 1)
			% table->philos_count];
		table->philos[i].table = table;
		++i;
	}
}
