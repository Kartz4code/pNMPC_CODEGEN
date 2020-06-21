#pragma once
#include "pNMPC_headers.hpp"
using namespace pNMPC;
class Inputs : public HyperStates {
private:
	// Private members value, variable name and alias name, expression
	Real _u_value{ 0 };
	std::string _var_name{};
	std::string _var_alias_name{};
	std::string _expression{};
	// Count inputs
	static size_t _count_u;
	// Parameterization data 
	// 1. Number of parameterization over time (nPC = 1 by default)
	// 2. Type of Control Parameterization (Piecewise by default)
	// 3. Vector of default Control points over time
	// 4. Vector of default Control values over the Control points
	// 5. Maximum and minimum bounds on the input
	size_t _nPC{1};
	CONTROLPARAM _CP{CONTROLPARAM::PIECEWISE};
	std::vector<Real> _CP_time{0}, _CP_value{_u_value};
	Real _U_up = INT_MAX, _U_low = INT_MIN;

public:
	// Input Constructors
	Inputs();
	Inputs(const Real& val);
	Inputs(const std::string& str);
	Inputs& operator=(const Inputs& In);
	
	// Setters and getters
	virtual void setValue(const Real& val) override;
	virtual const Real getValue() const override;
	virtual const std::string& getName() const override;
	virtual const std::string& getExpression() const override;
	virtual void setExpression(const std::string& str) override;
	virtual const std::string& getAliasExpression() const override;

	// Assignment operator over value and HyperStates
	void operator=(const Real& val);
	void operator=(const HyperStates& S);

	// Parameterization getters and setters
	void setControlParameterization(const size_t& val);
	const size_t getControlParameterization() const;
	void setCPType(const CONTROLPARAM& CP);
	const CONTROLPARAM& getCPType() const;
	void setCPTime(const std::vector<Real>& CP_time);
	const std::vector<Real>& getCPTime() const;
	void setCPValue(const std::vector<Real>& CP_value);
	const std::vector<Real>& getCPValue() const;

	// Setters and getters for upper and lowerbounds
	void setUpperBound(const Real& val);
	const Real getUpperBound() const;
	void setLowerBound(const Real& val);
	const Real getLowerBound() const;

};
