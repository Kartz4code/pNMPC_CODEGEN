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
#include "SQPSolverCodeGen.hpp"

SQPSolverCodeGen::SQPSolverCodeGen()
	: SQPSolverCodeGen {0.5, 10, 0.5, 10, 0.001}
{}

SQPSolverCodeGen::SQPSolverCodeGen(const Real& alpha, const size_t& Niter, const Real& betaMinus, const Real& betaPlus, const Real& tol)
	: _alpha{ alpha }, _Niter{ Niter }, _betaMinus{ betaMinus }, _betaPlus{ betaPlus }, _tol{ tol }
{}

void SQPSolverCodeGen::setAlpha(const Real& alpha)
{
	this->_alpha = alpha;
}
void SQPSolverCodeGen::setNiter(const size_t& Niter)
{
	this->_Niter = Niter;
}
void SQPSolverCodeGen::setBetaMinus(const Real& betaMinus)
{
	this->_betaMinus = betaMinus;
}
void SQPSolverCodeGen::setBetaPlus(const Real& betaPlus)
{
	this->_betaPlus = betaPlus;
}
void SQPSolverCodeGen::setTolerance(const Real& tol)
{
	this->_tol = tol;
}
const Real& SQPSolverCodeGen::getAlpha() const
{
	return this->_alpha;
}
const size_t& SQPSolverCodeGen::getNiter() const
{
	return this->_Niter;
}
const Real& SQPSolverCodeGen::getBetaMinus() const
{
	return this->_betaMinus;
}
const Real& SQPSolverCodeGen::getBetaPlus() const
{
	return this->_betaPlus;
}
const Real& SQPSolverCodeGen::getTolerance() const
{
	return this->_tol;
}