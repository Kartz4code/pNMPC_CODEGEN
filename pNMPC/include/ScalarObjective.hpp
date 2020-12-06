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

#ifndef SCALAROBJECTIVE_H
#define SCALAROBJECTIVE_H
#include "pNMPC_headers.hpp"
using namespace pNMPC;
class ScalarObjective : public EquationProcessor {
private:
	static size_t _countobj;
	static std::vector<std::unique_ptr<ScalarObjective>> _XObj;
	OBJTYPE _ObjectiveType{OBJTYPE::LAGRANGIAN};
	static size_t _keepObjSize_may;
	static size_t _keepObjSize_lag;
public:
	ScalarObjective();
	ScalarObjective(const HyperStates& S);
	ScalarObjective(HyperStates&& S);
	ScalarObjective(const MATHyperStates& HSO);
	ScalarObjective(MATHyperStates&& HSO);
	ScalarObjective(const Real&& val);
	ScalarObjective(const ScalarObjective& SOS);
	ScalarObjective(const EquationProcessor& EQ);
	void operator=(HyperStates&& S);
	void operator=(MATHyperStates&& HSO);
	void operator=(Real&& val);
	const OBJTYPE& getObjectiveType() const;
	~ScalarObjective() = default;
};
#endif