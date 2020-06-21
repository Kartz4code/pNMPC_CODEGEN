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
