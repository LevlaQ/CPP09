/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BitcoinExchange.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gyildiz <gyildiz@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/01 13:26:47 by gyildiz           #+#    #+#             */
/*   Updated: 2026/09/30 14:51:30 by gyildiz          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BITCOINEXCHANGE_HPP
#define BITCOINEXCHANGE_HPP

# include <iomanip>
# include <string>
# include <map>
# include <cstdlib>
# include <fstream>
# include <unistd.h>
# include <stdexcept>
# include <iostream>
# include <algorithm>
# include <limits>
# include <sstream>

class BitcoinExchange
{
	public:
		BitcoinExchange();
		BitcoinExchange(const BitcoinExchange &copy);
		
		BitcoinExchange	&operator=(const BitcoinExchange &assign);
	
		~BitcoinExchange();
		
		void	load_database();

		void	parse_validate_load(std::ifstream &file);
		bool	line_check_db\
		(std::string &line, std::string &date, std::string &value);
		bool	line_check_input\
		(std::string &line, std::string &date, std::string &value);
		
		void	process_input(const std::string &filepath);
		void	process_input_line(std::string &line);
		bool	find_closest_rate(const std::string &date, float &rate);
		
		bool date_check(std::string &date);
		bool value_check(std::string &value);
		
		std::string	format_number(double value);
	private:
		std::map<std::string, float>		_db;
};

#endif