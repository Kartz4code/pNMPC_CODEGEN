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
class ScalarConstraint : public EquationProcessor {
private:
	static size_t _countconst;
	static std::vector<std::unique_ptr<ScalarConstraint>> _XConst;
	CONSTTYPE _ConstraintType{CONSTTYPE::REGULAR};
	static size_t _keepConstSize_reg;
	static size_t _keepConstSize_term;
public:
	ScalarConstraint();
	ScalarConstraint(const HyperStates& S);
	ScalarConstraint(HyperStates&& S);
	ScalarConstraint(const MATHyperStates& HSC);
	ScalarConstraint(MATHyperStates&& HSC);
	ScalarConstraint(const Real&& val);
	ScalarConstraint(const ScalarConstraint& SCS);
	ScalarConstraint(const EquationProcessor& EQ);
	void operator=(HyperStates&& S);
	void operator=(MATHyperStates&& HSC);
	void operator=(Real&& val);
	const CONSTTYPE& getConstraintType() const;
	~ScalarConstraint() = default;
};
