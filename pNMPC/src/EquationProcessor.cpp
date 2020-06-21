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
#include "EquationProcessor.hpp"

EquationProcessor::EquationProcessor(std::vector<std::string>& expressions,
								     std::vector<std::vector<std::string>>& all_tokens,
									 std::vector<std::pair<HyperStates*, std::pair<Operation, std::pair<std::string, std::string>>>>& OpSet,
								     const size_t& count):
									_expressions{ expressions }, _all_tokens{ all_tokens }, _OpSet{ OpSet }, _count{count}
{}


// Depreciated code
EquationProcessor::operator MATHyperStates()
{
	if (this->_h_MAT.getNumRows() == 1 && this->_h_MAT.getNumColumns() == 1)
	{
		MATHyperStates temp(1, 1);
		temp[0][0] = this->_h_store;
		return temp;
	}
	else
	return this->_h_MAT;
}

// Access the MATHyperStates element
HyperStates* EquationProcessor::operator[](const size_t& n) 
{ 
	return &this->_h_MAT[n][0]; 
}

// With respect to other Equation Processor objects (Elemenary Operations) => HyperStates operation

MATHyperStates EquationProcessor::operator+(const HyperStates& s)
{
	return this->_h_MAT + s;
}
MATHyperStates EquationProcessor::operator*(const HyperStates& s)
{
	return this->_h_MAT*s;
}
MATHyperStates EquationProcessor::operator-(const HyperStates& s)
{
	return this->_h_MAT - s;
}
MATHyperStates EquationProcessor::operator/(const HyperStates& s)
{
	return this->_h_MAT / s;
}

MATHyperStates EquationProcessor::operator^(const HyperStates& s)
{
	return ((this->_h_MAT) ^ s);
}

MATHyperStates EquationProcessor::operator<=(const HyperStates& s)
{
	return this->_h_MAT <= s;
}
MATHyperStates EquationProcessor::operator>=(const HyperStates& s)
{
	return this->_h_MAT >= s;
}

MATHyperStates EquationProcessor::operator+(const MATHyperStates& S)
{
	return this->_h_MAT + S;
}

MATHyperStates EquationProcessor::operator*(const MATHyperStates& S)
{
	return this->_h_MAT*S;
}
MATHyperStates EquationProcessor::operator-(const MATHyperStates& S)
{
	return this->_h_MAT - S;
}

MATHyperStates EquationProcessor::operator<=(const MATHyperStates& S)
{
	return this->_h_MAT <= S;
}
MATHyperStates EquationProcessor::operator>=(const MATHyperStates& S)
{
	return this->_h_MAT >= S;
}

// With respect to other Equation Processor objects (Elemenary Operations) => scalar operation
MATHyperStates EquationProcessor::operator+(const Real& val)
{
	return this->_h_MAT + val;
}

MATHyperStates EquationProcessor::operator*(const Real& val)
{
	return this->_h_MAT + val;
}

MATHyperStates EquationProcessor::operator-(const Real& val)
{
	return this->_h_MAT - val;
}
MATHyperStates EquationProcessor::operator/(const Real& val)
{
	return this->_h_MAT / val;
}

MATHyperStates EquationProcessor::operator^(const Real& val)
{
	return ((this->_h_MAT) ^ val);
}

MATHyperStates EquationProcessor::operator<=(const Real& val)
{
	return this->_h_MAT <= val;
}
MATHyperStates EquationProcessor::operator>=(const Real& val)
{
	return this->_h_MAT >= val;
}

// Negation operator
MATHyperStates EquationProcessor::operator- ()
{
	return -this->_h_MAT;
}

void EquationProcessor::replace(const HyperStates* s_old, const HyperStates* s_new)
{
	std::string::size_type pos = 0u; std::string str_temp;
	while ((pos = this->_h_store.getExpression().find(s_old->getName(), pos)) != std::string::npos) {
		this->_changes_vector.push_back(this->_h_store); str_temp = this->_h_store.getExpression();
		this->_h_store.setAliasExpression(str_temp.replace(pos, s_old->getName().length(), s_new->getName()));
		this->_h_store.setExpression(str_temp.replace(pos, s_old->getName().length(), s_new->getName()));
		pos += s_new->getName().length(); this->_changes++;
	}
}

void EquationProcessor::revert()
{
	if(this->_changes != 0)
		this->_h_store = this->_changes_vector[--this->_changes];
}

const HyperStates& EquationProcessor::getHyperStates() const
{
	return this->_h_store;
}

const MATHyperStates& EquationProcessor::getMATHyperStates() const
{
	return this->_h_MAT;
}