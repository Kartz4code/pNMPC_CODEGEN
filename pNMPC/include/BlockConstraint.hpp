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

#ifndef BLOCK_CONSTRAINT_H
#define BLOCK_CONSTRAINT_H
#include "pNMPC_headers.hpp"
class BlockConstraint
{
private:
	HyperStates _BC;
	std::unique_ptr<ScalarConstraint> _BCptr;
	bool _activation_flag{ false };
public:
	BlockConstraint(HyperStates&& S);
	BlockConstraint(HyperStates&& S, bool activation_flag);
	BlockConstraint(const HyperStates& S);
	BlockConstraint(const HyperStates& S, bool activation_flag);
	void Intersect(const BlockConstraint& bc2int);
	void Union(const BlockConstraint& bc2int);
	~BlockConstraint();
};
#endif
