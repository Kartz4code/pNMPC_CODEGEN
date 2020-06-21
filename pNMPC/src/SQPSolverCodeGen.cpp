#include "pNMPC_headers.hpp"
#include "SQPSolverCodeGen.hpp"

SQPSolverCodeGen::SQPSolverCodeGen()
	: SQPSolverCodeGen {0.5, 10, 0.5, 10, 0.001}
{}

SQPSolverCodeGen::SQPSolverCodeGen(const Real& alpha, const size_t& Niter, const Real& betaMinus, const Real& betaPlus, const Real& tol)
	: _alpha{ alpha }, _Niter{ Niter }, _betaMinus{ betaMinus }, _betaPlus{ betaPlus }, _tol{ tol }
{}

void SQPSolverCodeGen::setAlpha(const Real& alpha)
{
	this->_alpha = alpha;
}
void SQPSolverCodeGen::setNiter(const size_t& Niter)
{
	this->_Niter = Niter;
}
void SQPSolverCodeGen::setBetaMinus(const Real& betaMinus)
{
	this->_betaMinus = betaMinus;
}
void SQPSolverCodeGen::setBetaPlus(const Real& betaPlus)
{
	this->_betaPlus = betaPlus;
}
void SQPSolverCodeGen::setTolerance(const Real& tol)
{
	this->_tol = tol;
}
const Real& SQPSolverCodeGen::getAlpha() const
{
	return this->_alpha;
}
const size_t& SQPSolverCodeGen::getNiter() const
{
	return this->_Niter;
}
const Real& SQPSolverCodeGen::getBetaMinus() const
{
	return this->_betaMinus;
}
const Real& SQPSolverCodeGen::getBetaPlus() const
{
	return this->_betaPlus;
}
const Real& SQPSolverCodeGen::getTolerance() const
{
	return this->_tol;
}