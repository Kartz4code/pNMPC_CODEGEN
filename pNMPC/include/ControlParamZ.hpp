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

#ifndef CONTROLPARAMZ_H
#define CONTROLPARAMZ_H
#include "pNMPC_headers.hpp"
template<typename T>
class ControlParamZ
{
protected:
	const Real _delta{ 0.005 };
	Inputs& _u_param;
	std::vector<Real> _CP_default, _U_default;
	Real _u_min{ INT_MIN }, _u_max{INT_MAX}; size_t _N;

public:
	ControlParamZ() = default;

	// Constructor 1
	ControlParamZ(const size_t& N, Inputs& u, const Real& u_min = INT_MIN, const Real& u_max = INT_MAX)
		: _N{N}, _u_param{ u }, _u_min{ u_min }, _u_max{ u_max }
	{
		// CP - Control points in time, the input is in percentage w.r.t to the time horizon
		for (size_t i{ 0 }; i < this->_N; i++)
		{
			this->_CP_default.push_back(static_cast<Real>(i) / static_cast<Real>(this->_N));
			this->_U_default.push_back(this->_u_param.getValue());
		}
		static_cast<T*>(this)->setParameters();
	}

	// Control points setter - Vector case
	ControlParamZ(const size_t& N, Inputs& u, const std::vector<Real>& CP_data, const Real& u_min, const Real& u_max)
		: _N{N}, _u_param{ u }, _CP_default{ CP_data }, _u_min{ u_min }, _u_max{ u_max }
	{
		// CP - Control points in time, the input is in percentage w.r.t to the time horizon
		for (size_t i{ 0 }; i < this->_N; i++)
			this->_U_default.push_back(this->_u_param.getValue());
		static_cast<T*>(this)->setParameters();
	}

	// Control points setter - Array case
	ControlParamZ(const size_t& N, Inputs& u, Real* CP_data, const Real& u_min, const Real& u_max)
		: _N{N}, _u_param{ u }, _u_min{ u_min }, _u_max{ u_max }
	{
		// CP - Control points in time, the input is in percentage w.r.t to the time horizon
		for (size_t i{ 0 }; i < this->_N; i++)
		{
			this->_CP_default.push_back(this->_CP_data[i]);
			this->_U_default.push_back(this->_u_param.getValue());
		}
		static_cast<T*>(this)->setParameters();
	}

	// Get control points
	const size_t getControlPoints() const
	{
		return this->_N;
	}
};
#endif

