/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mapham <mapham@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 06:52:36 by mapham            #+#    #+#             */
/*   Updated: 2025/07/04 19:55:13 by mapham           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../includes/philo.h"

//si c un nombre positif

long	ft_atol(const char *str)
{
	long	nb;
	int		i;

	nb = 0;
	i = 0;
	if (!str || !*str)
		return (-1);
	while ((str[i] >= 9 && str[i] <= 13) || (str[i] == ' '))
		i++;
	if (str[i] == '+')
		i++;
	if (!str[i])
		return (-1);
	while (str[i])
	{
		if (!(str[i] <= '9' && str[i] >= '0'))
			return (-1);
		nb = nb * 10 + (str[i] - '0');
		if (nb > INT_MAX)
			return (-1);
		i++;
	}
	return (nb);
}

//parser et valider les arguments

char	valid_argument(int argc, char **argv)
{
	int i;

	i = 0;
	if (argc < 5 || argc > 6)
	{
		ft_putstr_fd("Error : Invalid argument", 2);
		return (0);
	}		
	while (i < argc)
	{
		if (!ft_atol(argv[i]))
		{
			ft_putstr_fd("Error : Invalid argument", 2);
			return (0);
		}
		i++;
	}
}

void	init_data(char **argv, t_rules *data)
{
	data->nb_philos = argv[1];
	data->time_to_die = argv[2];
	data->time_to_eat = argv[3];
	data->time_to_sleep = argv[4];
	if (ft_atol(argv[5]))
		data->must_eat = argv[5];
}
