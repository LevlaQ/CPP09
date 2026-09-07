/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gyildiz <gyildiz@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/01 13:29:06 by gyildiz           #+#    #+#             */
/*   Updated: 2026/09/02 13:17:17 by gyildiz          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "BitcoinExchange.hpp"

int	main(int ac, char **ag)
{
	BitcoinExchange bitcoin;
	try
	{
		bitcoin.load_database();
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
	
}