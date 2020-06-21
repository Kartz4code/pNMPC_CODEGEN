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
#include "States.hpp"

size_t States::_count_x{ 0 };

States::States() : HyperStates{ nullptr }, 
				   _var_name{ Var[TYPE::STATE] + std::to_string(_count_x) },
				   _var_alias_name{ Var[TYPE::STATE] + std::to_string(_count_x) }
{
	db->accessAllStates().push_back(this); _count_x++;
	db->accessAllHyperMaps()[this->getName()] = &this->getCurrentHyperState();
}

States::States(const Real& val) : HyperStates{ nullptr }, 
								  _x_value{ val }, 
								  _var_name{ Var[TYPE::STATE] + std::to_string(_count_x)},
							      _var_alias_name{ Var[TYPE::STATE] + std::to_string(_count_x) }
{
	db->accessAllStates().push_back(this); _count_x++;
	db->accessAllHyperMaps()[this->getName()] = &this->getCurrentHyperState();
}


States::States(const std::string& str) : _expression{ str }
{
	this->_var_alias_name = this->_expression;
}

void States::setValue(const Real& val)
{
	this->_x_value = val;
}

const Real States::getValue() const
{
	return this->_x_value;
}

const std::string& States::getName() const
{
	return this->_var_name;
}

const std::string& States::getExpression() const
{
	if (!this->_expression.empty())
		return this->_expression;
	else
		return this->_var_name;
}

const std::string& States::getAliasExpression() const
{
	return this->_var_alias_name;
}

void States::setExpression(const std::string& str)
{
	this->_expression = str;
}

void States::operator=(const Real& val)
{
	this->setValue(val);
}

void States::operator=(const HyperStates& S)
{
	if (!S.replace_check(this))
	{
		// Delete from the States vector database
		for (auto item = db->accessAllStates().begin(); item != db->accessAllStates().end(); item++)
		{
			if ((*item)->getName() == this->getName())
			{
				db->accessAllStates().erase(item);
				break;
			}
		}
		// Delete from HyperStates map
		db->accessAllHyperMaps().erase(this->getName());
	}
	this->_var_alias_name = S.getExpression();
}

States& States::operator=(const States& In)
{
	if (this->getName() != In.getName())
		this->operator=(static_cast<HyperStates>(In));
	return *this;
}

States::operator States()
{
	return *this;
}