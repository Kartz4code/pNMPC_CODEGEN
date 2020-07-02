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
#include "PNMPC_ROOT.cuh"
// X_result after one step integration
__device__
Real_C* Integrator_RK45(Real_C* X, Real_C* U, Real_C* P, Real_C*(*func)(Real_C*, Real_C*, Real_C*), Real_C step_size)
{
	// X_result after one step integration
	Real_C X_RK45_[nx];
	Real_C k1[nx], k2[nx], k3[nx], k4[nx];
	Real_C Te1[nx];
	Real_C* func_collect;

	func_collect = func(X, U, P);
	// Stage 1 
	for (uint16_t i = 0; i < nx; i++)
	{
		k1[i] = step_size*func_collect[i];
		Te1[i] = X[i] + (k1[i] * 0.5f);
	}
	// Stage 2 
	func_collect = func(Te1, U, P);
	for (uint16_t i = 0; i < nx; i++)
	{
		k2[i] = step_size*func_collect[i];
		Te1[i] = X[i] + (k2[i] * 0.5f);
	}
	// Stage 3 
	func_collect = func(Te1, U, P);
	for (uint16_t i = 0; i < nx; i++)
	{
		k3[i] = step_size*func_collect[i];
		Te1[i] = X[i] + k3[i];
	}
	// Stage 4
	func_collect = func(Te1, U, P);
	for (uint16_t i = 0; i < nx; i++)
		k4[i] = step_size*func_collect[i];

	// Final result
	for (uint16_t i = 0; i < nx; i++)
		X_RK45_[i] = X[i] + (1.0f/6.0f)*(k1[i] + 2.0f * k2[i] + 2.0f * k3[i] + k4[i]);

	return X_RK45_;
}
