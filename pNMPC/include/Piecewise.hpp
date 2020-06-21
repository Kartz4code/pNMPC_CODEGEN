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
class Piecewise : public ControlParamZ<Piecewise>
{
private:
	Real u_clock;
public:
	Piecewise() = default;
	void setParameters()
	{
		_u_param.setControlParameterization(this->_N);
		_u_param.setCPType(CONTROLPARAM::PIECEWISE);
		_u_param.setCPTime(this->_CP_default);
		_u_param.setCPValue(this->_U_default);
		_u_param.setUpperBound(this->_u_max + this->_delta);
		_u_param.setLowerBound(this->_u_min - this->_delta);
	}

	Real ControlClockPieceWise(Real current_clock, Real final_clock)
	{
		// current_clock - Current time (in seconds) of the Prediction period
		// final_clock - Prediction time of the MPC problem (in seconds)
		u_clock = 0;
		if (this->_N == 1)
			u_clock = this->_U_default[0];
		else
		{
			for (uint16_t i = 0; i < this->_N; i++)
			{
				if (current_clock >= (this->_CP_default[i] * final_clock) && current_clock <= (this->_CP_default[i + 1] * final_clock))
				{
					u_clock = this->_U_default[i];
					break;
				}
				else if (current_clock >= (this->_CP_default[this->_N - 1] * final_clock))
				{
					u_clock = this->_U_default[this->_N - 1];
					break;
				}
			}
		}
		return u_clock;
	}
	~Piecewise() = default;
};
