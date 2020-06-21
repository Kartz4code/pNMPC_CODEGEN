#pragma once
#include "pNMPC_headers.hpp"
using namespace pNMPC;
class DiffEquation : public EquationProcessor {
private:
	static size_t _countdiff;
	static std::vector<std::unique_ptr<DiffEquation>> _XDiff;
public:
	DiffEquation();
	DiffEquation(const HyperStates& S);
	DiffEquation(HyperStates&& S);
	DiffEquation(const MATHyperStates& HS);
	DiffEquation(MATHyperStates&& HS);
	DiffEquation(const Real&& val);
	DiffEquation(const DiffEquation& DES);
	DiffEquation(const EquationProcessor& EQ);
	// Equal to operator
	void operator=(HyperStates&& S);
	void operator=(MATHyperStates&& HS);
	void operator=(Real&& val);
	~DiffEquation() = default;
};