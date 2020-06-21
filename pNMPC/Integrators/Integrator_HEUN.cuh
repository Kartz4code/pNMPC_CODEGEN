#pragma once
#include "PNMPC_ROOT.cuh"
// X_result after one step integration
__device__ 
Real_C* Integrator_HEUN(Real_C* X, Real_C* U, Real_C* Un, Real_C* P, Real_C*(*func)(Real_C*, Real_C*, Real_C*), Real_C step_size)
{
	// Stored variable
	Real_C X_HEUN_[nx], H[nx];
	Real_C* func_collect = func(X, U, P);
	for (uint16_t i = 0; i < nx; i++)
		H[i] = X[i] + step_size*func_collect[i];

	Real_C* func_collect_H = func(H, Un, P);

	// Final result
	for (uint16_t i = 0; i < nx; i++)
		X_HEUN_[i] = X[i] + 0.5f*step_size*(func_collect[i] + func_collect_H[i]);
	return X_HEUN_;
}