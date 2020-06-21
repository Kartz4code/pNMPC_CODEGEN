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

#ifdef __cplusplus
extern "C" {
#endif
#include "PNMPC_ROOT.h"
#define MAX(a,b) max(a,b)
#define MIN(a,b) min(a,b)
#define AVG(a,b) (0.5*(a+b))
#define MID(a,b) (0.5*(b-a))
#define DISC(a,b,c) ((b*b) - (4*a*c))
#define SQUARE(x) ((x)*(x))
#define INT_MAX (9999999)
#define INT_MIN (-INT_MAX)
// Main function
void BBO_SQPFunction(Real_C P_in[NMAX], ParaStructC* params, Real_C(*funcJ)(Real_C[NMAX], const void*), Real_C(*funcG)(Real_C[NMAX], const void*), optimset_SQP* opt);
#ifdef __cplusplus
}
#endif