#pragma once
#include "pNMPC_headers.hpp"
template<typename T>
class ControlParamZ
{
protected:
	const Real _delta{ 0.005 };
	Inputs& _u_param;
	std::vector<Real> _CP_default, _U_default;
	Real _u_min, _u_max; size_t _N;

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
	ControlParamZ(const size_t& N, Inputs& u, const std::vector<Real>& CP_data, const Real& u_min = INT_MIN, const Real& u_max = INT_MAX)
		: _N{N}, _u_param{ u }, _CP_default{ CP_data }, _u_min{ u_min }, _u_max{ u_max }
	{
		// CP - Control points in time, the input is in percentage w.r.t to the time horizon
		for (size_t i{ 0 }; i < this->_N; i++)
			this->_U_default.push_back(this->_u_param.getValue());
		static_cast<T*>(this)->setParameters();
	}

	// Control points setter - Array case
	ControlParamZ(const size_t& N, Inputs& u, Real* CP_data, const Real& u_min = INT_MIN, const Real& u_max = INT_MAX)
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

