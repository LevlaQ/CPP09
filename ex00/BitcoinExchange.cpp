/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BitcoinExchange.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gyildiz <gyildiz@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/01 13:26:52 by gyildiz           #+#    #+#             */
/*   Updated: 2026/09/08 15:19:16 by gyildiz          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "BitcoinExchange.hpp"

BitcoinExchange::BitcoinExchange()
{}

BitcoinExchange::BitcoinExchange(const BitcoinExchange &copy) : _db(copy._db)
{}

BitcoinExchange	&BitcoinExchange::operator=(const BitcoinExchange &assign)
{
	if(this != &assign)
		_db = assign._db;
	return (*this);
}

BitcoinExchange::~BitcoinExchange()
{}

const char *BitcoinExchange::max_input_value::what() const throw()
{
	return ("The value must be between [0-1000]");
}

/*
	First we check the syntax for date is true, after that we will
	use ctime library to validate date.
	Input's date format is a bit diferent, you must write input function
	according to it.
	YYYY-MM-DD
	0123456789

	basic_string substr( size_type pos = 0, size_type count = npos ) const;
	LEAP YEAR RULE
	In every 400 years, we will have three days extra
	100 | 400 | Leap
	 -     +      -
	 +     +      +
	COMPLETE FUNCTION
*/
bool	BitcoinExchange::date_check(std::string &date)
{
	int length = date.length();
	if (length == 0)
		return false;
	int count = std::count(date.begin(), date.end(), '-');
	if (length != 10 || count != 2 || date[4] != '-' || date[7] != '-')
		return (false);
	for (int i = 0; i < length; i++)
	{
		if(!std::isdigit(date[i]))
		{
			if(date[i] != '-')
				return(false);
		}
	}
	double year, month, day;

	size_t delimiter1 = date.find('-');
	year = std::strtod(date.substr(0, delimiter1).c_str(), NULL);
	
	size_t delimiter2 = date.find('-', delimiter1 + 1);
	month = std::strtod\
	(date.substr(delimiter1 + 1, delimiter2 - delimiter1 - 1).c_str(), NULL);
	
	day = std::strtod(date.substr(delimiter2 + 1).c_str(), NULL);
	
	if (year < 2008 || year > 9999) return false;
	if (month < 1 || month > 12) return false;
	if (day < 1 || day > 31) return false;

	if (month == 4 || month == 6 || month == 9 || month == 11)
		if (day > 30) return false;
	int year_i = static_cast<int>(year);
	if (month == 2)
	{
		bool leap = (year_i % 4 == 0 && (year_i % 100 != 0 || year_i % 400 == 0));
		if (day > (leap ? 29 : 28))
			return false;
	}
	return (true);
}

bool BitcoinExchange::value_check(std::string &value, bool is_input_file)
{
	size_t length = value.length();
	if (length == 0)
		return false;
	int count = std::count(value.begin(), value.end(), '.');

	if(count > 1)
		return false;
	if (value[0] == '.' || value[length - 1] == '.')
		return (false);
	for (size_t i = 0; i < length; i++)
	{
		if(!std::isdigit(static_cast<unsigned char>(value[i])))
			if(value[i] != '.')
				return false;
	}
	return true;
}

/*
	Buna benzer input file için fonksiyon yazacapız
*/
bool	BitcoinExchange::line_check_db\
(std::string &line, std::string &date, std::string &value)
{
	int count = std::count(line.begin(), line.end(), ',');
	std::string::iterator begin = line.begin();
	std::string::iterator comma = std::find(line.begin(), line.end(), ',');
	size_t diff = comma - begin;
	if (count != 1)
		return false;
	date = line.substr(0, diff);
	if (!date.size())
		return false;
	if (!date_check(date))
		return false;
	
	value = line.substr(diff + 1);
	if (!value.size())
		return false;
	if (!value_check(value, false))
		return false;
	return true;
}

/*
	Must start to load database and input_file, as soon as it sees something fishy,
	it should stop.
	
	Invoking member class function class calls are different, they need to be 
	invoked with (this->*f).
*/
void	BitcoinExchange::parse_validate_load\
(std::ifstream &file, bool(BitcoinExchange::*f)\
(std::string &, std::string &, std::string &), bool input_file)
{
	std::string line;
	bool		valid = true;
	
	std::string	date;
	std::string value;
	double		dvalue;
	
	while (std::getline(file,line))
	{
		valid = (this->*f)(line, date, value);
		if (!valid)
		{
			if (!input_file)
				throw std::invalid_argument\
				("File data format is invalid, use : YYYY-MM-DD,<value>");
			else
				throw std::invalid_argument\
				("File data format is invalid, use : YYYY-MM-DD | <value>");
		}
		dvalue = strtod(value.c_str(),NULL);
		if (!input_file)
		{
			if (dvalue > __FLT_MAX__ || dvalue < 0)
				throw std::range_error\
				("Value in dataset exceeds float range / it is negative");
			_db.insert\
			(std::pair<std::string, float>(date, static_cast<float>(dvalue)));
		}
		else
		{
			if (dvalue > 1000 || dvalue < 0)
				throw max_input_value();
			_input.insert\
			(std::pair<std::string, float>(date, static_cast<float>(dvalue)));
		}
	}
}

/*
	An error is thrown here, must be caught
*/
void	BitcoinExchange::load_database()
{
	std::string filepath = "./data.csv";
	std::ifstream database(filepath.c_str());
	
	if (access(filepath.c_str(), F_OK) != 0 || database.is_open() == false || \
		access(filepath.c_str(), R_OK) != 0)
		throw std::runtime_error\
		("Corrupted file format: Could not read expected data.");
	parse_validate_load(database, &BitcoinExchange::line_check_db , false);
}

// void	BitcoinExchange::load_input()
// {
	
// }
