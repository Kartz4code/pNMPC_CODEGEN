#pragma once
#include "PNMPC_ROOT.h"
static Real_C X_HEUN_[nx]; 
// X_result after one step integration
inline Real_C* Integrator_HEUN(Real_C* X, Real_C* U, Real_C* P, Real_C*(*func)(Real_C*, Real_C*, Real_C*), Real_C step_size)
{
    // Stored variable
    Real_C H[nx];
    Real_C* func_collect; 
    Real_C* func_collect_H;

	func_collect = func(X, U, P);
	for (uint16_t i = 0; i < nx; i++)
		H[i] = X[i] + step_size*func_collect[i];

	func_collect_H = func(H, U, P);

	// Final result
	for (uint16_t i = 0; i < nx; i++)
		X_HEUN_[i] = X[i] + 0.5*step_size*(func_collect[i] + func_collect_H[i]);
	return X_HEUN_;
}