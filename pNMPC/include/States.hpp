#pragma once
#include "pNMPC_headers.hpp"
using namespace pNMPC;
class States : public HyperStates {
private:
	// Private members value, variable name and alias name, expression
	Real _x_value{ 0 };
	std::string _var_name{};
	std::string _var_alias_name{};
	std::string _expression{};
	static size_t _count_x;

public:
	// States constructor
	States();
	States(const Real& val);
	States(const std::string& str);
	States& operator=(const States& In);
	operator States();
	// Setters and getters
	virtual void setValue(const Real& val) override;
	virtual const Real getValue() const override;
	virtual const std::string& getName() const override;
	virtual const std::string& getExpression() const override;
	virtual void setExpression(const std::string& str) override;
	virtual const std::string& getAliasExpression() const override;
	void operator=(const Real& val);
	void operator=(const HyperStates& S);
};
