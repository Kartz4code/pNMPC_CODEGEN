#pragma once
#include "pNMPC_headers.hpp"
class SQPSolverCodeGen
{
private:
	Real _alpha;
	size_t _Niter;
	Real _betaMinus;
	Real _betaPlus;
	Real _tol;

public:
	SQPSolverCodeGen();
	SQPSolverCodeGen(const Real& alpha, const size_t& Niter, const Real& betaMinus, const Real& betaPlus, const Real& tol);

	// Setters and getters for optimization properties
	void setAlpha(const Real& alpha);
	void setNiter(const size_t& Niter);
	void setBetaMinus(const Real& betaMinus);
	void setBetaPlus(const Real& betaPlus);
	void setTolerance(const Real& tol);

	const Real& getAlpha() const;
	const size_t& getNiter() const;
	const Real& getBetaMinus() const;
	const Real& getBetaPlus() const;
	const Real& getTolerance() const;

	~SQPSolverCodeGen() = default;
};

