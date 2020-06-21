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
				break;
			}
			else if (current_clock >= (CP[N - 1] * final_clock))
			{
				u_clock = U_CP[N - 1];
				break;
			}
		}
	}
	return u_clock;
}