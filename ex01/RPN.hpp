#ifndef RPN_HPP
#define RPN_HPP

# include <iostream>
# include <stack>
# include <string>
# include <exception>

class RPN
{
	public:
		RPN();
		RPN(const RPN &copy);
		RPN &operator=(const RPN &other);
		~RPN();

		void	parser_processor(std::string input);
	private:
		std::stack<int> polish;
};

#endif