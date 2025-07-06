/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mai <mai@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 06:52:23 by mapham            #+#    #+#             */
/*   Updated: 2025/07/06 06:59:14 by mai              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

void	free_rules(t_rules *rules)
{
	int	i;

	if (!rules)
		return ;
	if (rules->forks)
	{
		i = 0;
		while (i < rules->nb_philos)
			pthread_mutex_destroy(&rules->forks[i++]);
		free(rules->forks);
	}
	if (rules->philos)
	{
		i = 0;
		while (i < rules->nb_philos)
			pthread_mutex_destroy(&rules->philos[i++].timing_mutex);
		free(rules->philos);
	}
	pthread_mutex_destroy(&rules->print_mutex);
	pthread_mutex_destroy(&rules->death_mutex);
	pthread_mutex_destroy(&rules->meal_check);
	free(rules);
}

#include "philo.h"

int	main(int argc, char **argv)
{
	t_rules	*rules;

	if (!valid_argument(argc, argv))
		return (1);
	rules = init_sim_rules(argc, argv);
	if (!rules)
		return (1);
	if (!init_philo(rules))
		return (1);
	start_simulation(rules);
	free_rules(rules);
	return (0);
}

