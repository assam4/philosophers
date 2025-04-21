/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saslanya <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 18:35:06 by saslanya          #+#    #+#             */
/*   Updated: 2025/04/21 18:35:08 by saslanya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "types.h"

long long	time_ms(void)
{
	struct timeval	time;
	long long		current_ms;

	gettimeofday(&time, NULL);
	current_ms = (((long long)time.tv_sec * ML_TO_MK)
			+ (time.tv_usec / ML_TO_MK));
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
	return (EXIT_SUCCESS);
}

void	deallocation_mem(t_table **table)
{
	if (!table || !*table)
		return ;
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
	i = LOOP_START;
	while (i < table->philos_count)
	{
		table->philos[i].number = i + 1;
		table->philos[i].eat_count = START_VAL;
		table->philos[i].last_eat_time = table->start_time;
		table->philos[i].table = table;
		++i;
	}
}
