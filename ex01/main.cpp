#include "RPN.hpp"

int	main(int ac, char **av)
{
	if(ac != 2)
	{
		std::cerr << \
		"The program must have two arguments: ./exe \"input\"" << std::endl;
		return 1;
	}
	
	try
	{
		RPN kowalski;
		kowalski.parser_processor(av[1]);
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
		return 1;
	}
	return 0;
}