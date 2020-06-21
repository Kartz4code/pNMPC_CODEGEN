#include "pNMPC_headers.hpp"
#include "External.hpp"

External::External(const std::string& str) : _func_str{ str },
											 HyperStates{ TYPE::EXTERNAL, str}
{

}

void External::operator=(const std::string& str)
{
	this->_func_str = str;
}