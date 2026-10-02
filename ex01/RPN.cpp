#include "RPN.hpp"

RPN::RPN() {};

RPN::RPN(const RPN &copy) : polish(copy.polish) {};

RPN &RPN::operator=(const RPN &other)
{
	if (this != &other)
		this->polish = other.polish;
	return (*this);
}

RPN::~RPN() {};

void	RPN::parser_processor(std::string input)
{
	
}