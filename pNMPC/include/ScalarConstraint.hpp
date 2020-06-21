#pragma once
#include "pNMPC_headers.hpp"
using namespace pNMPC;
class ScalarConstraint : public EquationProcessor {
private:
	static size_t _countconst;
	static std::vector<std::unique_ptr<ScalarConstraint>> _XConst;
	CONSTTYPE _ConstraintType{CONSTTYPE::REGULAR};
	static size_t _keepConstSize_reg;
	static size_t _keepConstSize_term;
public:
	ScalarConstraint();
	ScalarConstraint(const HyperStates& S);
	ScalarConstraint(HyperStates&& S);
	ScalarConstraint(const MATHyperStates& HSC);
	ScalarConstraint(MATHyperStates&& HSC);
	ScalarConstraint(const Real&& val);
	ScalarConstraint(const ScalarConstraint& SCS);
	ScalarConstraint(const EquationProcessor& EQ);
	void operator=(HyperStates&& S);
	void operator=(MATHyperStates&& HSC);
	void operator=(Real&& val);
	const CONSTTYPE& getConstraintType() const;
	~ScalarConstraint() = default;
};
