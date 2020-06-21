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
#include "PNMPC_ROOT.h"
static Real_C X_HEUN_[nx]; 
// X_result after one step integration
inline Real_C* Integrator_HEUN(Real_C* X, Real_C* U, Real_C* P, Real_C*(*func)(Real_C*, Real_C*, Real_C*), Real_C step_size)
{
    // Stored variable
    Real_C H[nx];
    Real_C* func_collect; 
    Real_C* func_collect_H;

	func_collect = func(X, U, P);
	for (uint16_t i = 0; i < nx; i++)
		H[i] = X[i] + step_size*func_collect[i];

	func_collect_H = func(H, U, P);

	// Final result
	for (uint16_t i = 0; i < nx; i++)
		X_HEUN_[i] = X[i] + 0.5*step_size*(func_collect[i] + func_collect_H[i]);
	return X_HEUN_;
}