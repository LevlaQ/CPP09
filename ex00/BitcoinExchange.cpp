/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BitcoinExchange.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gyildiz <gyildiz@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/01 13:26:52 by gyildiz           #+#    #+#             */
/*   Updated: 2026/09/30 15:13:55 by gyildiz          ###   ########.fr       */
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

/*
	First we check the syntax for date is true, after that we will
	validate the date.
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
	
	if (year < 1000 || year > 9999) return false;
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

/*
	This value check also returns negative numbers,
	they are not eliminated here yet,
	we are just checking if the value syntax is correct
*/
bool BitcoinExchange::value_check(std::string &value)
{
	size_t length = value.length();
	if (length == 0)
		return false;

	size_t start = (value[0] == '-') ? 1 : 0;
	if (start == length)
		return false;

	int count = std::count(value.begin() + start, value.end(), '.');
	if (count > 1)
		return false;
	if (value[start] == '.' || value[length - 1] == '.')
		return (false);

	for (size_t i = start; i < length; i++)
	{
		if(!std::isdigit(static_cast<unsigned char>(value[i])))
			if(value[i] != '.')
				return false;
	}
	return true;
}

/*
	Checks if database line format is appropriate.
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
	if (!value_check(value))
		return false;
	return true;
}

/*
	date | value
	2005-02-02 | 0.1
	2011-09-14 | 390.57
	2010-11-03 | 517.239
	         21d12
*/
bool	BitcoinExchange::line_check_input\
(std::string &line, std::string &date, std::string &value)
{
	int count = std::count(line.begin(), line.end(), '|');
	std::string::iterator begin = line.begin();
	std::string::iterator pipe = std::find(line.begin(), line.end(), '|');
	
	if (count != 1)
		return false;
	if (pipe == line.begin() || pipe + 1 == line.end())
		return false;
	if (*(pipe - 1) != ' ' || *(pipe + 1) != ' ')
		return false;

	size_t diff = pipe - begin;
	date = line.substr(0, diff - 1);
	if (!date.size())
		return false;
	value = line.substr(diff + 2);
	if (!value.size())
		return false;
	return true;
}

/*
	Must start to load database and as soon as it sees something fishy,
	it should stop.
	Error's are thrown here
*/
void	BitcoinExchange::parse_validate_load(std::ifstream &file)
{
	std::string line;
	bool		valid = false;
	bool		header = true;
	
	std::string	date;
	std::string value;
	double		dvalue;
	
	while (std::getline(file,line))
	{
		if (header)
		{
			if (line != "date,exchange_rate")
			throw std::invalid_argument\
				("File data format is invalid, file must have \"date,exchange_rate\" as header");
			header = false;
			continue;
		}
		valid = line_check_db(line, date, value);
		if (!valid)
		{
				throw std::invalid_argument\
				("File data format is invalid, use : YYYY-MM-DD,<value>");
		}
		dvalue = strtod(value.c_str(),NULL);
		if (dvalue > __FLT_MAX__ || dvalue < 0)
			throw std::range_error\
			("Value in dataset exceeds float range / it is negative");
		_db.insert\
		(std::pair<std::string, float>(date, static_cast<float>(dvalue)));
	}
	if (!valid)
		throw std::invalid_argument\
		("File data format is invalid, file must have date and exchange rates");
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
	parse_validate_load(database);
}

/*
	This function finds the closest rate from the map and writes it into the
	&rate.
*/
bool	BitcoinExchange::find_closest_rate(const std::string &date, float &rate)
{
	std::map<std::string, float>::iterator it = _db.lower_bound(date);

	if (it != _db.end() && it->first == date)
	{
		rate = it->second;
		return true;
	}
	if (it == _db.begin())
	{
		return false;
	}
	--it;
	rate = it->second;
	return true;
}

void	BitcoinExchange::process_input_line(std::string &line)
{
	std::string	date;
	std::string	value;
	double		dvalue;

	if (!line_check_input(line, date, value))
	{
		std::cout
			<< "Error: bad line format, should be YYYY-MM-DD | <value>. Yours is: "
			<< line << std::endl;
		return;
	}

	if (!date_check(date))
	{
		std::cout << "Error: bad input => " << date;

		if (!value_check(value))
			std::cout << " | " << value;

		std::cout << std::endl;
		return;
	}

	if (!value_check(value))
	{
		std::cout << "Error: bad input => " << value << std::endl;
		return;
	}

	dvalue = std::strtod(value.c_str(), NULL);

	if (dvalue < 0)
	{
		std::cout << "Error: not a positive number." << std::endl;
		return;
	}

	if (dvalue > 1000)
	{
		std::cout << "Error: too large a number." << std::endl;
		return;
	}

	float rate;

	if (!find_closest_rate(date, rate))
	{
		std::cout << "Error: no matching rate found for "
			<< date << std::endl;
		return;
	}

	std::cout
		<< date
		<< " => "
		<< value
		<< " = "
		<< format_number(dvalue * rate)
		<< std::endl;
}

/*
	Main file errors are checked here: empty file, header only etc.
*/
void	BitcoinExchange::process_input(const std::string &filepath)
{
	std::ifstream file(filepath.c_str());

	if (access(filepath.c_str(), F_OK) != 0
		|| file.is_open() == false
		|| access(filepath.c_str(), R_OK) != 0)
	{
		throw std::runtime_error("Error: could not open file.");
	}

	std::string	line;
	bool		first = true;
	bool		has_lines = false;
	bool		has_data = false; 
	
	while (std::getline(file, line))
	{
		has_lines = true;
		
		if (first)
		{
			first = false;

			if (line == "date | value")
				continue;
		}
		has_data = true;
		process_input_line(line);
	}
	if (!has_lines)
		std::cout << "Error: input file is empty." << std::endl;
	else if (!has_data)
		std::cout << "Error: input file contains only the header line."
			<< std::endl;
}

std::string	BitcoinExchange::format_number(double value)
{
	std::ostringstream oss;

	oss << std::fixed << std::setprecision(4) << value;

	std::string result = oss.str();

	while (!result.empty() && result[result.size() - 1] == '0')
		result.erase(result.size() - 1);

	if (!result.empty() && result[result.size() - 1] == '.')
		result.erase(result.size() - 1);

	return result;
}