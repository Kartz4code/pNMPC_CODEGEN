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

#ifndef SQPSOLVERCODEGEN_H
#define SQPSOLVERCODEGEN_H
#include "pNMPC_headers.hpp"
class SQPSolverCodeGen
{
private:
	Real _alpha;
	size_t _Niter;
	Real _betaMinus;
	Real _betaPlus;
	Real _tol;

public:
	SQPSolverCodeGen();
	SQPSolverCodeGen(const Real& alpha, const size_t& Niter, const Real& betaMinus, const Real& betaPlus, const Real& tol);

	// Setters and getters for optimization properties
	void setAlpha(const Real& alpha);
	void setNiter(const size_t& Niter);
	void setBetaMinus(const Real& betaMinus);
	void setBetaPlus(const Real& betaPlus);
	void setTolerance(const Real& tol);

	const Real& getAlpha() const;
	const size_t& getNiter() const;
	const Real& getBetaMinus() const;
	const Real& getBetaPlus() const;
	const Real& getTolerance() const;

	~SQPSolverCodeGen() = default;
};
#endif

