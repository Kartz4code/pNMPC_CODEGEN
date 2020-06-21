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
