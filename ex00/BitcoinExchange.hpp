/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BitcoinExchange.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gyildiz <gyildiz@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/01 13:26:47 by gyildiz           #+#    #+#             */
/*   Updated: 2026/09/07 19:23:49 by gyildiz          ###   ########.fr       */
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
# include <ctime>
# include <limits>

class BitcoinExchange
{
	public:
		BitcoinExchange();
		BitcoinExchange(const BitcoinExchange &copy);
		
		BitcoinExchange	&operator=(const BitcoinExchange &assign);
	
		~BitcoinExchange();
		
		void	load_database();
		//void	load_input();

		void	parse_validate_load\
		(std::ifstream &file, bool(BitcoinExchange::*f)\
		(std::string &, std::string &, std::string &), bool input_file);
		bool	line_check_db\
		(std::string &line, std::string &date, std::string &value);
		//void	is_correct_format_input();

		bool date_check(std::string &date);
		bool value_check(std::string &value, bool is_input_file);
	private:
		std::map<std::string, float>		_db;
		std::multimap<std::string, float>	_input;
};

#endif