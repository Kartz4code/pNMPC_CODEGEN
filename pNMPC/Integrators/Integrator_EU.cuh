#pragma once
#include "PNMPC_ROOT.cuh"
// X_result after one step integration
__device__ 
Real_C* Integrator_EU(Real_C* X, Real_C* U, Real_C* P, Real_C*(*func)(Real_C*, Real_C*, Real_C*), Real_C step_size)
{
	// X_result after one step integration
	Real_C X_EU_[nx];
	Real_C* func_collect = func(X, U, P);
	// Final result
	for (uint16_t i = 0; i < nx; i++)
		X_EU_[i] = X[i] + step_size*func_collect[i];
	return X_EU_;
}