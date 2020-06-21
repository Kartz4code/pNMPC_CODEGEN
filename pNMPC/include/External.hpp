#pragma once
#include "pNMPC_headers.hpp"
using namespace pNMPC;
class External : public HyperStates
{
private:
	std::string _func_str;
public:
	External() = default; 
	External(const std::string& str);
	void operator=(const std::string& str);
	~External() = default;
};

