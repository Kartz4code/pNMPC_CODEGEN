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

inline Real_C ControlClockPieceWise(Real_C current_clock, Real_C final_clock, Real_C* U_CP, Real_C* CP, uint16_t N)
{
    Real_C u_clock;
	// current_clock - Current time (in seconds) of the Prediction period
	// final_clock - Prediction time of the MPC problem (in seconds)
	u_clock = 0;
	if (N == 1)
		u_clock = U_CP[0];
	else
	{
		for (uint16_t i = 0; i < N; i++)
		{
			if (current_clock >= (Real_C)(CP[i] * final_clock) && current_clock <= (Real_C)(CP[i + 1] * final_clock))
			{
				u_clock = U_CP[i];
				return u_clock;
			}
			else if (current_clock >= (CP[N - 1] * final_clock))
			{
				u_clock = U_CP[N - 1];
				return u_clock;
			}
		}
	}
}
