#include <iomanip>
#include <string>
#include <map>
#include <cstdlib>
#include <fstream>
#include <unistd.h>
#include <stdexcept>
#include <iostream>
#include <algorithm>

int main(void)
{
	std::string line = "2400-02-29,0\n";
	std::string date;
	
	int count = std::count(line.begin(), line.end(), ',');
	std::string::iterator begin = line.begin();
	std::string::iterator comma = find(line.begin(), line.end(), ',');
	size_t diff = comma - begin;
	if (count != 1)
		return (false);
	date = line.substr(0, diff);
	std::cout << date << "\n";

	int length = date.length();
	count = std::count(date.begin(), date.end(), '-');
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

	if (year < 2008 || year > 9999)
		return (false);
	if (month < 1 || month > 12)
		return (false);
	if (day < 1 || day > 31) 
		return	(false);

	if (month == 4 || month == 6 || month == 9 || month == 11)
		if (day > 30) return (false);
	int year_i = static_cast<int>(year);
	if (month == 2)
	{
		bool leap = (year_i % 4 == 0 && (year_i % 100 != 0 || year_i % 400 == 0));
		if (day > (leap ? 29 : 28))
			return (false);
	}

	std::cout << year << "\n";
	std::cout << month << "\n";
	std::cout << day << "\n";


}