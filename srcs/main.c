/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mapham <mapham@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 06:52:23 by mapham            #+#    #+#             */
/*   Updated: 2025/07/04 19:53:01 by mapham           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

int main(int argc, char **argv)
{
	t_rules *data = NULL;

	
	if (valid_argument(argc, argv))
		init_data(argv, data);
	return (0);
    //init structure
    //create threads
    //lancer monitor pour death thread
 

}