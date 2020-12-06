#include "pNMPC_headers.hpp"
#include "EquationProcessor.hpp"

EquationProcessor::EquationProcessor(std::vector<std::string>& expressions,
								     std::vector<std::vector<std::string>>& all_tokens,
									 std::vector<std::pair<HyperStates*, std::pair<Operation, std::pair<std::string, std::string>>>>& OpSet,
								     const size_t& count):
									_expressions{ expressions }, _all_tokens{ all_tokens }, _OpSet{ OpSet }, _count{count}
{}


// Depreciated code
EquationProcessor::operator MATHyperStates()
{
	if (this->_h_MAT.getNumRows() == 1 && this->_h_MAT.getNumColumns() == 1)
	{
		MATHyperStates temp(1, 1);
		temp[0][0] = this->_h_store;
		return temp;
	}
	else 
		return this->_h_MAT;
}

// Access the MATHyperStates element
HyperStates* EquationProcessor::operator[](const size_t& n) { return &this->_h_MAT[n][0]; }

// With respect to other Equation Processor objects (Elemenary Operations) => HyperStates operation
MATHyperStates EquationProcessor::operator+(const HyperStates& s){ return this->_h_MAT + s; }
MATHyperStates EquationProcessor::operator*(const HyperStates& s){ return this->_h_MAT*s; }
MATHyperStates EquationProcessor::operator-(const HyperStates& s){ return this->_h_MAT - s; }
MATHyperStates EquationProcessor::operator/(const HyperStates& s){ return this->_h_MAT / s; }
MATHyperStates EquationProcessor::operator^(const HyperStates& s){ return ((this->_h_MAT)^s); }
MATHyperStates EquationProcessor::operator<=(const HyperStates& s){ return this->_h_MAT <= s; }
MATHyperStates EquationProcessor::operator>=(const HyperStates& s){ return this->_h_MAT >= s; }

MATHyperStates EquationProcessor::operator+(const MATHyperStates& S){ return this->_h_MAT + S; }
MATHyperStates EquationProcessor::operator*(const MATHyperStates& S){ return this->_h_MAT*S; }
MATHyperStates EquationProcessor::operator-(const MATHyperStates& S){ return this->_h_MAT - S; }
MATHyperStates EquationProcessor::operator<=(const MATHyperStates& S){ return this->_h_MAT <= S; }
MATHyperStates EquationProcessor::operator>=(const MATHyperStates& S){ return this->_h_MAT >= S; }

// With respect to other Equation Processor objects (Elemenary Operations) => scalar operation
MATHyperStates EquationProcessor::operator+(const Real& val){ return this->_h_MAT + val; }
MATHyperStates EquationProcessor::operator*(const Real& val){ return this->_h_MAT + val; }
MATHyperStates EquationProcessor::operator-(const Real& val){ return this->_h_MAT - val; }
MATHyperStates EquationProcessor::operator/(const Real& val){ return this->_h_MAT / val; }
MATHyperStates EquationProcessor::operator^(const Real& val){ return ((this->_h_MAT) ^ val); }
MATHyperStates EquationProcessor::operator<=(const Real& val){ return this->_h_MAT <= val; }
MATHyperStates EquationProcessor::operator>=(const Real& val){ return this->_h_MAT >= val; }

// Negation operator
MATHyperStates EquationProcessor::operator- (){ return -this->_h_MAT; }

void EquationProcessor::replace(const HyperStates* s_old, const HyperStates* s_new)
{
	std::string::size_type pos = 0u; std::string str_temp;
	while ((pos = this->_h_store.getExpression().find(s_old->getName(), pos)) != std::string::npos) {
		this->_changes_vector.push_back(this->_h_store); str_temp = this->_h_store.getExpression();
		this->_h_store.setAliasExpression(str_temp.replace(pos, s_old->getName().length(), s_new->getName()));
		this->_h_store.setExpression(str_temp.replace(pos, s_old->getName().length(), s_new->getName()));
		pos += s_new->getName().length(); this->_changes++;
	}
}

void EquationProcessor::revert()
{
	if(this->_changes != 0)
		this->_h_store = this->_changes_vector[--this->_changes];
}

const HyperStates& EquationProcessor::getHyperStates() const
{
	return this->_h_store;
}

const MATHyperStates& EquationProcessor::getMATHyperStates() const
{
	return this->_h_MAT;
}