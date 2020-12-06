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

#ifndef EQUATIONPROCESSOR_H
#define EQUATIONPROCESSOR_H
#include "pNMPC_headers.hpp"
class EquationProcessor
{
private:
	// EquationProcessor function variables
	HyperStates* _temper{nullptr};
	// Underived
	std::vector<std::string>& _expressions;
	std::vector<std::vector<std::string>>& _all_tokens;
	std::vector<std::pair<HyperStates*, std::pair<Operation, std::pair<std::string, std::string>>>>& _OpSet;
	size_t _count; 
protected:
	// Protected method for derived and underived
	void parse();
	void expression_tree();
	void tree_reduction(const size_t& start);
	// Protected variables
	HyperStates _h_store; MATHyperStates _h_MAT;
	std::vector<HyperStates> _changes_vector;
	size_t _changes{0};
public:

	EquationProcessor() = default;

	EquationProcessor(std::vector<std::string>& expressions,
					  std::vector<std::vector<std::string>>& all_tokens,
					  std::vector<std::pair<HyperStates*, std::pair<Operation, std::pair<std::string, std::string>>>>& OpSet,
		              const size_t& count = 0);

	// Depreciated code
	operator MATHyperStates();

	// Access the MATHyperStates element
	HyperStates* operator[](const size_t& n);

	// With respect to other States object (Scalar Operations)
	MATHyperStates operator+(const HyperStates& s);
	MATHyperStates operator*(const HyperStates& s);
	MATHyperStates operator-(const HyperStates& s);
	MATHyperStates operator/(const HyperStates& s);
	MATHyperStates operator^(const HyperStates& s);
	MATHyperStates operator<=(const HyperStates& s);
	MATHyperStates operator>=(const HyperStates& s);

	// With respect to other States object (Matrix Operations)
	MATHyperStates operator+(const MATHyperStates& S);
	MATHyperStates operator*(const MATHyperStates& S);
	MATHyperStates operator-(const MATHyperStates& S);
	MATHyperStates operator<=(const MATHyperStates& S);
	MATHyperStates operator>=(const MATHyperStates& S);

	// With respect to scalar values (Operations)
	MATHyperStates operator+(const Real& val);
	MATHyperStates operator*(const Real& val);
	MATHyperStates operator-(const Real& val);
	MATHyperStates operator/(const Real& val);
	MATHyperStates operator^(const Real& val);
	MATHyperStates operator<=(const Real& val);
	MATHyperStates operator>=(const Real& val);

	// Negation operator
	MATHyperStates operator- ();

	// Replace and revert back functions
	void replace(const HyperStates* s_old, const HyperStates* s_new);
	void revert();

	// Getters for HyperState and MATHyperStates 
	const HyperStates& getHyperStates() const;
	const MATHyperStates& getMATHyperStates() const;

	// Destructor
	~EquationProcessor() = default;
};
#endif
