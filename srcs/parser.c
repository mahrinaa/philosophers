/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mapham <mapham@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 06:52:36 by mapham            #+#    #+#             */
/*   Updated: 2025/07/07 09:10:18 by mapham           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

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

char	valid_argument(int argc, char **argv)
{
	int 	i;
	long	value;

	if (argc < 5 || argc > 6)
	{
		ft_putstr_fd("Error : expected 4 or 5 arguments\n", 2);
		return (0);
	}	
	i = 1;
	while (i < argc)
	{
		value = ft_atol(argv[i]);
		if (value <= 0)
		{
			ft_putstr_fd("Error: invalid argument value\n", 2);
			return (0);
		}
		i++;
	}
	return (1);
}

