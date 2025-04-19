/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validation.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saslanya <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 12:28:41 by saslanya          #+#    #+#             */
/*   Updated: 2025/04/19 12:28:43 by saslanya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VALIDATION_H
# define VALIDATION_H

# include <stdio.h>
# include <stdlib.h>
# include <errno.h>

# define VARG_COUNT1 5
# define VARG_COUNT2 6
# define START_VAL 0
# define LOOP_START 0
# define DECIMAL 10
# define MAX_PHILOS 1000
# define ARGC_ERR "\033[1;31mWrong arguments count!.\n\033[0m\n"
# define ARGV_ERR "\033[1;31mWrong arguments passed!.\n\033[0m\n"

int	ft_atoi(const char *str);
int	parse_args(int argc, char **argv);

#endif
