/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mapham <mapham@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 05:02:09 by mapham            #+#    #+#             */
/*   Updated: 2025/07/07 05:02:12 by mapham           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

//libere memoire et detruit tous les mutex
void	free_rules(t_rules *rules)
{
	int	i;

	if (!rules)
		return ;
	if (rules->forks)
	{
		i = 0;
		while (i < rules->nb_philos)
			pthread_mutex_destroy(&rules->forks[i++]); //detruit tous les mutedx de fourchetters
		free(rules->forks);  //liberer tableau des fourchettes
	}
	if (rules->philos)
	{
		i = 0;
		while (i < rules->nb_philos)
			pthread_mutex_destroy(&rules->philos[i++].timing_mutex); //detruiit chqaue mutex perso
		free(rules->philos); //libere structure des philos
	}
	pthread_mutex_destroy(&rules->print_mutex); //mutex global daffichage
	pthread_mutex_destroy(&rules->death_mutex); //mutex tester la mort
	pthread_mutex_destroy(&rules->meal_check); //mutex verif repas
	free(rules); //libere structure rules
}

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

