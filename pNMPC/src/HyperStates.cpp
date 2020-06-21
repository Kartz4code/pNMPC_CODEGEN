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

#include "pNMPC_headers.hpp"
#include "HyperStates.hpp"


size_t HyperStates::_count_h{ 0 };

HyperStates::HyperStates() : _var_name{ Paren[ParenOps::open] + "0*0" + Paren[ParenOps::close] },
							 _var_alias_name{ Paren[ParenOps::open] + "0*0" + Paren[ParenOps::close] },
							 _expression{ Paren[ParenOps::open] + "0*0" + Paren[ParenOps::close] }
{
 // A fresh hyperstate not loaded into the Database
}

HyperStates::HyperStates(void** ptr) : _var_name{Var[TYPE::STATE] + std::to_string(_count_h)},
									   _var_alias_name{Var[TYPE::STATE] + std::to_string(_count_h)}
{ 
	db->accessAllHyperMaps()[this->getName()] = &this->getCurrentHyperState(); 
	_count_h++;
}


HyperStates::HyperStates(const int& val) : HyperStates{static_cast<Real>(val)}
{
	// not loaded into the Database
}

HyperStates::HyperStates(const Real& val) : _h_value{ val },
											_var_name{ Paren[ParenOps::open] + std::to_string(val) + "*1" + Paren[ParenOps::close] },
											_var_alias_name{ Paren[ParenOps::open] + std::to_string(val) + "*1" + Paren[ParenOps::close] },
											_expression{ Paren[ParenOps::open] + std::to_string(val) + "*1" + Paren[ParenOps::close] }
{	
	//  not loaded into the Database
}

HyperStates::HyperStates(const std::string& str) : _expression{ str }
				
{
	this->_var_alias_name = this->_expression;
}


HyperStates::HyperStates(const TYPE& ht, const std::string& str) : _func_name{ str },
																   _var_name{ Var[TYPE::STATE] + std::to_string(_count_h) },
																   _var_alias_name{ Var[TYPE::STATE] + std::to_string(_count_h) }
{
	this->_extern_bit = (ht == TYPE::EXTERNAL) ? true : false;
	db->accessAllHyperMaps()[this->getName()] = &this->getCurrentHyperState();
	_count_h++;
}


void HyperStates::setValue(const Real& val)
{
	this->_h_value = val;
}

const Real HyperStates::getValue() const
{
	return this->_h_value;
}

const std::string& HyperStates::getName() const
{
	return this->_var_name;
}

const std::string& HyperStates::getExpression() const
{
	if (!this->_expression.empty())
		return this->_expression;
	else
		return this->_var_name;
}

const std::string& HyperStates::getAliasExpression() const
{
	return this->_var_alias_name;
}

void HyperStates::setExpression(const std::string& str)
{
	this->_expression = str;
}

void HyperStates::setAliasExpression(const std::string& str)
{
	this->_var_alias_name = str;
}

HyperStates HyperStates::operator+ (const HyperStates& s)
{
	this->setExpression(this->getAliasExpression());
	return{ HyperStates(Paren[ParenOps::open]
		+ this->getExpression() + sOps[Operation::add] + s.getAliasExpression() + Paren[ParenOps::close]) };
}

HyperStates HyperStates::operator*(const HyperStates& s)
{
	this->setExpression(this->getAliasExpression());
	return{ HyperStates(Paren[ParenOps::open]
		+ this->getExpression() + sOps[Operation::multiply] + s.getAliasExpression() + Paren[ParenOps::close])};
}

HyperStates HyperStates::operator-(const HyperStates& s)
{
	this->setExpression(this->getAliasExpression());
	return{ HyperStates(Paren[ParenOps::open]
		+ this->getExpression() + sOps[Operation::subract] + s.getAliasExpression() + Paren[ParenOps::close]) };
}

HyperStates HyperStates::operator/(const HyperStates& s)
{
	this->setExpression(this->getAliasExpression());
	return{ HyperStates(Paren[ParenOps::open] 
		+ this->getExpression() + sOps[Operation::divide] + s.getAliasExpression() + Paren[ParenOps::close]) };
}

HyperStates HyperStates::operator^(const HyperStates& s)
{
	this->setExpression(this->getAliasExpression());
	return{ HyperStates(Paren[ParenOps::open]
		+ this->getExpression() + sOps[Operation::power] + s.getAliasExpression() + Paren[ParenOps::close]) };
}

HyperStates HyperStates::operator<=(const HyperStates& s)
{
	this->setExpression(this->getAliasExpression());
	return{ HyperStates(Paren[ParenOps::open]
		+ this->getExpression() + sOps[Operation::leq] + s.getAliasExpression() + Paren[ParenOps::close]) };
}

HyperStates HyperStates::operator>=(const HyperStates& s)
{
	this->setExpression(this->getAliasExpression());
	return{ HyperStates(Paren[ParenOps::open]
		+ this->getExpression() + sOps[Operation::geq] + s.getAliasExpression() + Paren[ParenOps::close]) };
}

// With respect to scalar values (Operations) => L-values
HyperStates HyperStates::operator+(const Real& val)
{
	this->setExpression(this->getAliasExpression());
	if (val > 0)
		return{ HyperStates(Paren[ParenOps::open]
			+ this->getExpression() + sOps[Operation::add] + std::to_string(val) + Paren[ParenOps::close]) };
	else
		return{ HyperStates(Paren[ParenOps::open]
			+ this->getExpression() + Paren[ParenOps::space] + sOps[Operation::subract] + Paren[ParenOps::space] + std::to_string(-val) + Paren[ParenOps::close]) };
}

HyperStates HyperStates::operator-(const Real& val)
{
	this->setExpression(this->getAliasExpression());
	if (val > 0)
		return{ HyperStates(Paren[ParenOps::open]
			+ this->getExpression() + Paren[ParenOps::space] + sOps[Operation::subract] + Paren[ParenOps::space] + std::to_string(val) + Paren[ParenOps::close]) };
	else {
		return{ HyperStates(Paren[ParenOps::open]
			+ this->getExpression() + sOps[Operation::add] + std::to_string(-val) + Paren[ParenOps::close]) };
	}
}

HyperStates HyperStates::operator*(const Real& val)
{
	this->setExpression(this->getAliasExpression());
	return{ HyperStates(Paren[ParenOps::open]
		+ this->getExpression() + sOps[Operation::multiply] + std::to_string(val) + Paren[ParenOps::close]) };
}

HyperStates HyperStates::operator/(const Real& val)
{
	this->setExpression(this->getAliasExpression());
	return{ HyperStates(Paren[ParenOps::open]
		+ this->getExpression() + sOps[Operation::divide] + std::to_string(val) + Paren[ParenOps::close]) };
}

HyperStates HyperStates::operator^(const Real& val)
{
	this->setExpression(this->getAliasExpression());
	return{ HyperStates(Paren[ParenOps::open]
		+ this->getExpression() + sOps[Operation::power] + std::to_string(val) +  Paren[ParenOps::close]) };
}

HyperStates HyperStates::operator<=(const Real& val)
{
	this->setExpression(this->getAliasExpression());
	return{ HyperStates(Paren[ParenOps::open]
		+ this->getExpression() + sOps[Operation::leq] + std::to_string(val) + Paren[ParenOps::close]) };
}

HyperStates HyperStates::operator>=(const Real& val)
{
	this->setExpression(this->getAliasExpression());
	return{ HyperStates(Paren[ParenOps::open]
		+ this->getExpression() + sOps[Operation::geq] + std::to_string(val) + Paren[ParenOps::close]) };
}

// Negation operator
HyperStates HyperStates::operator-()
{
	this->setExpression(this->getAliasExpression());
	return{ HyperStates(std::string(Paren[ParenOps::open])
		+ "-1" + sOps[Operation::multiply] + this->getExpression() + Paren[ParenOps::close]) };
}

// Output stream operator
std::ostream& operator<<(std::ostream& os, const HyperStates& S)
{
	return os << S.getExpression();
}

// Return current HyperStates
HyperStates& HyperStates::getCurrentHyperState()
{
	return *this;
}

HyperStates HyperStates::replace(const HyperStates* s_old, const HyperStates* s_new)
{
	std::string::size_type pos = 0u; std::string str_temp;
	while ((pos = this->getExpression().find(s_old->getName(), pos)) != std::string::npos) {
		str_temp = this->getExpression();
		this->setAliasExpression(str_temp.replace(pos, s_old->getName().length(), s_new->getExpression()));
		this->setExpression(str_temp.replace(pos, s_old->getName().length(), s_new->getExpression()));
		pos += s_new->getExpression().length();
	}
	return HyperStates{ *this };
}

bool HyperStates::replace_check(const HyperStates* s_old) const 
{
	std::string::size_type pos = 0u;
	return ((pos = this->getExpression().find(s_old->getName(), pos)) != std::string::npos);
}

// Find string
size_t HyperStates::find_string(const std::string& str) const
{
	std::string::size_type pos = 0u;
	if ((pos = this->getExpression().find(str, pos)) != std::string::npos)
		return pos;
	else
		return 0;
}

// Flush content
void HyperStates::flush()
{
	this->_var_name = Paren[ParenOps::open] + "0*0" + Paren[ParenOps::close];
	this->_var_alias_name = Paren[ParenOps::open] + "0*0" + Paren[ParenOps::close];
	this->_expression = Paren[ParenOps::open] + "0*0" + Paren[ParenOps::close];
}

void HyperStates::operator=(const Real& val)
{
	this->_var_name = std::to_string(val);
	this->_var_alias_name = Paren[ParenOps::open] + std::to_string(val) + "*1" + Paren[ParenOps::close];
	this->_expression = Paren[ParenOps::open] + std::to_string(val) + "*1" + Paren[ParenOps::close];
}


HyperStates& HyperStates::operator=(const HyperStates& H)
{
	if (dynamic_cast<const States*>(&H))
	{
		this->_h_value = dynamic_cast<const States*>(&H)->getValue();
		this->_var_alias_name = Paren[ParenOps::open] + dynamic_cast<const States*>(&H)->getAliasExpression() + "*1" + Paren[ParenOps::close];
		this->_expression = Paren[ParenOps::open] + dynamic_cast<const States*>(&H)->getExpression() + "*1" + Paren[ParenOps::close];
		return *this;
	}
	else if (dynamic_cast<const Inputs*>(&H))
	{
		this->_h_value = dynamic_cast<const Inputs*>(&H)->getValue();
		this->_var_alias_name = Paren[ParenOps::open] + dynamic_cast<const Inputs*>(&H)->getAliasExpression() + "*1" + Paren[ParenOps::close];
		this->_expression = Paren[ParenOps::open] + dynamic_cast<const Inputs*>(&H)->getExpression() + "*1" + Paren[ParenOps::close];
		return *this;
	}
	else if (dynamic_cast<const Params*>(&H))
	{
		this->_h_value = dynamic_cast<const Params*>(&H)->getValue();
		this->_var_alias_name = Paren[ParenOps::open] + dynamic_cast<const Params*>(&H)->getAliasExpression() + "*1" + Paren[ParenOps::close];
		this->_expression = Paren[ParenOps::open] + dynamic_cast<const Params*>(&H)->getExpression() + "*1" + Paren[ParenOps::close];
		return *this;
	}
	else if (dynamic_cast<const External*>(&H))
	{
		this->_h_value = dynamic_cast<const External*>(&H)->getValue();
		this->_var_alias_name = Paren[ParenOps::open] + dynamic_cast<const External*>(&H)->getAliasExpression() + "*1" + Paren[ParenOps::close];
		this->_expression = Paren[ParenOps::open] + dynamic_cast<const External*>(&H)->getExpression() + "*1" + Paren[ParenOps::close];
		return *this;
	}
	else
	{
		this->_h_value = H.getValue();
		this->_var_alias_name = H.getAliasExpression();
		this->_expression = H.getExpression();
		return *this;
	}
}

// Is external
const bool HyperStates::isExtern() const
{
	return this->_extern_bit;
}
// Get function name
const std::string& HyperStates::getFunctionName() const
{
	return this->_func_name;
}

// Get Count 
const size_t HyperStates::getCountHyperStates() const
{
	return this->_count_h;
}