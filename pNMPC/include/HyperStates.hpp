/*
*    This file is part of pNMPC software.
*    Copyright (c) 2020 GIPSA lab [https://github.com/Kartz4code/pNMPC_CODEGEN]
*
*    Main developer - Karthik Murali Madhavan Rathai
*
*    pNMPC - A Code Generation Software Tool For Implementation of Derivative Free
*    Parameterized NMPC Scheme for Embedded Control Systems
*    The software was developed by Karthik Murali Madhavan Rathai under the supervision of
*    Mazen Alamir and Olivier Sename affiliated to Univ. Grenoble Alpes, CNRS, Grenoble INP,
*    GIPSA lab, 38000 Grenoble, France.
*
* 	 pNMPC software is free software: you can redistribute it and/or modify
*    it under the terms of the GNU General Public License as published by
*    the Free Software Foundation, either version 3 of the License, or
*    (at your option) any later version.
*
*	 pNMPC software is distributed in the hope that it will be useful,
*	 but WITHOUT ANY WARRANTY; without even the implied warranty of
*	 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*	 GNU General Public License for more details.
*
*	 You should have received a copy of the GNU General Public License
* 	 along with pNMPC software.  If not, see <http://www.gnu.org/licenses/>.
*
*/

#ifndef HYPERSTATES_H
#define HYPERSTATES_H
#include "pNMPC_headers.hpp"
using namespace pNMPC;
class HyperStates : public Element
{
private:
	// Value of HyperState, variable name and alias name, expression
	Real _h_value{ 0 };
	std::string _var_name{};
	std::string _var_alias_name{};
	std::string _expression{};

	// Static variable (counter variable)
	static size_t _count_h;

	// External function link
	bool _extern_bit{ false };
	std::string _func_name{};

public:
	// HyperStates Constructors
	HyperStates();
	HyperStates(const int& val);
	HyperStates(const Real& val);
	HyperStates(void** ptr);
	HyperStates(const std::string& str);
	HyperStates(const TYPE& ht, const std::string& str);

	// Setters and getters
	void setValue(const Real& val) override;
	const Real getValue() const override;
	const std::string& getName() const override;
	const std::string& getExpression() const override;
	void setExpression(const std::string& str) override;
	const std::string& getAliasExpression() const override;
	void setAliasExpression(const std::string& str) override;

	// With respect to other HyperStates object (Operations) => L-values
	HyperStates operator+(const HyperStates& s);
	HyperStates operator*(const HyperStates& s);
	HyperStates operator-(const HyperStates& s);
	HyperStates operator/(const HyperStates& s);
	HyperStates operator^(const HyperStates& s);
	HyperStates operator<=(const HyperStates& s);
	HyperStates operator>=(const HyperStates& s);

	// With respect to scalar values (Operations) => L-values
	HyperStates operator+(const Real& val);
	HyperStates operator*(const Real& val);
	HyperStates operator-(const Real& val);
	HyperStates operator/(const Real& val);
	HyperStates operator^(const Real& val);
	HyperStates operator<=(const Real& val);
	HyperStates operator>=(const Real& val);
	// Negation operator
	HyperStates operator- ();
	
	// Equal to operator 
	void operator=(const Real& val);
	HyperStates& operator=(const HyperStates& H);

	// Logical " and "
	friend HyperStates operator&&(const HyperStates& s_lhs, const HyperStates& s_rhs);
	// Logical " or "
	friend HyperStates operator||(const HyperStates& s_lhs, const HyperStates& s_rhs);

	// Friend functions (L-value)
	friend HyperStates operator+(const Real& lhs, const HyperStates& rhs);
	friend HyperStates operator*(const Real& lhs, const HyperStates& rhs);
	friend HyperStates operator-(const Real& lhs, const HyperStates& rhs);
	friend HyperStates operator/(const Real& lhs, const HyperStates& rhs);
	friend HyperStates operator^(const Real& lhs, const HyperStates& rhs);
	friend HyperStates operator<=(const Real& lhs, const HyperStates& rhs);
	friend HyperStates operator>=(const Real& lhs, const HyperStates& rhs);

	// Functions (Trigonometry, log, etc)
	// Trignometric functions (L-Value)
	friend HyperStates sin(const HyperStates& s);
	friend HyperStates cos(const HyperStates& s);
	friend HyperStates tan(const HyperStates& s);
	friend HyperStates sinh(const HyperStates& s);
	friend HyperStates cosh(const HyperStates& s);
	friend HyperStates tanh(const HyperStates& s);
	friend HyperStates exp(const HyperStates& s);
	friend HyperStates log(const HyperStates& s);
	friend HyperStates abs(const HyperStates& s);
	friend HyperStates asin(const HyperStates& s);
	friend HyperStates acos(const HyperStates& s);
	friend HyperStates atan(const HyperStates& s);
	friend HyperStates asinh(const HyperStates& s);
	friend HyperStates acosh(const HyperStates& s);
	friend HyperStates atanh(const HyperStates& s);
	friend HyperStates sign(const HyperStates& s);

	// All max functions
	friend HyperStates maximum(const HyperStates& s_lhs, const HyperStates& s_rhs);
	friend HyperStates maximum(const Real& s_lhs, const HyperStates& s_rhs);
	friend HyperStates maximum(const HyperStates& s_lhs, const Real& s_rhs);
	friend HyperStates maximum(const Real& s_lhs, const Real& s_rhs);

	// All min functions
	friend HyperStates minimum(const HyperStates& s_lhs, const HyperStates& s_rhs);
	friend HyperStates minimum(const Real& s_lhs, const HyperStates& s_rhs);
	friend HyperStates minimum(const HyperStates& s_lhs, const Real& s_rhs);
	friend HyperStates minimum(const Real& s_lhs, const Real& s_rhs);

	// Output stream
	friend std::ostream& operator<<(std::ostream& os, const HyperStates& S);
	// Return current hyperstate
	HyperStates& getCurrentHyperState();
	// Check whether an expression exists 
	bool replace_check(const HyperStates* s_old) const;
	// Replace with new HyperState and return a fresh HyperState
	HyperStates replace(const HyperStates* s_old, const HyperStates* s_new);
	// Find string
	size_t find_string(const std::string& str) const;
	// Get Count 
	const size_t getCountHyperStates() const;
	// Flush HyperStates content
	void flush();
	// Is external
	const bool isExtern() const;
	// Get function name
	const std::string& getFunctionName() const;
	// Destructor
	~HyperStates() = default;
};
#endif
