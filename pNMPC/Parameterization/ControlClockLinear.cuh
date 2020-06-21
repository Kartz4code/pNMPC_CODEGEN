#pragma once
#include <stdio.h>
#include "PNMPC_ROOT.cuh"
__device__ 
Real_C ControlClockLinear(Real_C current_clock, Real_C final_clock, Real_C* U_CP, Real_C* CP, uint16_t N)
{
	// current_clock - Current time (in seconds) of the Prediction period
	// final_clock - Prediction time of the MPC problem (in seconds)
	Real_C u_clock = 0.0f;
	if (N == 1)
		u_clock = U_CP[0];
	else
	{
		for (uint16_t i = 0; i < N; i++)
		{
			if (current_clock >= (CP[i] * final_clock) && current_clock <= (CP[i + 1] * final_clock))
			{
				u_clock = U_CP[i] + ((current_clock - CP[i] * final_clock) / (((CP[i + 1] - CP[i])*final_clock))*(U_CP[i + 1] - U_CP[i]));
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