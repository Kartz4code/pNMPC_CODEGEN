#pragma once
#include "pNMPC_headers.hpp"
using namespace pNMPC;
class ScalarObjective : public EquationProcessor {
private:
	static size_t _countobj;
	static std::vector<std::unique_ptr<ScalarObjective>> _XObj;
	OBJTYPE _ObjectiveType{OBJTYPE::LAGRANGIAN};
	static size_t _keepObjSize_may;
	static size_t _keepObjSize_lag;
public:
	ScalarObjective();
	ScalarObjective(const HyperStates& S);
	ScalarObjective(HyperStates&& S);
	ScalarObjective(const MATHyperStates& HSO);
	ScalarObjective(MATHyperStates&& HSO);
	ScalarObjective(const Real&& val);
	ScalarObjective(const ScalarObjective& SOS);
	ScalarObjective(const EquationProcessor& EQ);
	void operator=(HyperStates&& S);
	void operator=(MATHyperStates&& HSO);
	void operator=(Real&& val);
	const OBJTYPE& getObjectiveType() const;
	~ScalarObjective() = default;
};
