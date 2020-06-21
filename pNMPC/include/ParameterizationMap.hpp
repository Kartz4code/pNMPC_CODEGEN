#pragma once
#include "pNMPC_headers.hpp"
class ParameterizationMap : public EquationProcessor
{
private:
	static size_t _countpme;
	static std::vector<std::unique_ptr<ParameterizationMap>> _XPME;
public:
	ParameterizationMap();
	ParameterizationMap(const HyperStates& S);
	ParameterizationMap(HyperStates&& S);
	ParameterizationMap(const MATHyperStates& HP);
	ParameterizationMap(MATHyperStates&& HP);
	ParameterizationMap(const Real&& val);
	ParameterizationMap(const ParameterizationMap& PME);
	ParameterizationMap(const EquationProcessor& EQ);
	void operator=(HyperStates&& S);
	void operator=(MATHyperStates&& HP);
	void operator=(Real&& val);
	~ParameterizationMap() = default;
};

