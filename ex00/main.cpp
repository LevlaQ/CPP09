/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gyildiz <gyildiz@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/01 13:29:06 by gyildiz           #+#    #+#             */
/*   Updated: 2026/09/30 09:45:28 by gyildiz          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "BitcoinExchange.hpp"

int	main(int ac, char **ag)
{
	if (ac == 1)
	{
		std::cout << "Error: could not open file. As you provided no file" \
		<< std::endl;
		return 1;
	}
	if (ac > 2)
	{
		std::cout << \
		"Error: more then two arguments. Correct usage: ./<program> <input file>"\
		 << std::endl;
		return 1;
	}
	BitcoinExchange bitcoin;
	try
	{
		bitcoin.load_database();
		bitcoin.process_input(ag[1]);
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
		return 1;
	}
}