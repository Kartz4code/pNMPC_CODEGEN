#pragma once
#include "pNMPC_headers.hpp"
using namespace pNMPC;
class Element
{
public:
	Element() = default;
	// Abstract classes
	virtual void setValue(const Real& val) = 0;
	virtual const Real getValue() const = 0;
	virtual const std::string& getName() const = 0;
	virtual const std::string& getExpression() const = 0;
	virtual void setExpression(const std::string& str) = 0;
	virtual const std::string& getAliasExpression() const = 0;
	virtual void setAliasExpression(const std::string& str) = 0;
	~Element() = default;
};

