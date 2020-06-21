#pragma once
#include "PNMPC_ROOT.cuh"
// X_result after one step integration
__device__
Real_C* Integrator_RK45(Real_C* X, Real_C* U, Real_C* P, Real_C*(*func)(Real_C*, Real_C*, Real_C*), Real_C step_size)
{
	// X_result after one step integration
	Real_C X_RK45_[nx];
	Real_C k1[nx], k2[nx], k3[nx], k4[nx];
	Real_C Te1[nx];
	Real_C* func_collect;

	func_collect = func(X, U, P);
	// Stage 1 
	for (uint16_t i = 0; i < nx; i++)
	{
		k1[i] = step_size*func_collect[i];
		Te1[i] = X[i] + (k1[i] * 0.5f);
	}
	// Stage 2 
	func_collect = func(Te1, U, P);
	for (uint16_t i = 0; i < nx; i++)
	{
		k2[i] = step_size*func_collect[i];
		Te1[i] = X[i] + (k2[i] * 0.5f);
	}
	// Stage 3 
	func_collect = func(Te1, U, P);
	for (uint16_t i = 0; i < nx; i++)
	{
		k3[i] = step_size*func_collect[i];
		Te1[i] = X[i] + k3[i];
	}
	// Stage 4
	func_collect = func(Te1, U, P);
	for (uint16_t i = 0; i < nx; i++)
		k4[i] = step_size*func_collect[i];

	// Final result
	for (uint16_t i = 0; i < nx; i++)
		X_RK45_[i] = X[i] + (0.167f)*(k1[i] + 2.0f * k2[i] + 2.0f * k3[i] + k4[i]);

	return X_RK45_;
}