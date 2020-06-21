#pragma once
#include "pNMPC_headers.hpp"
class EquationProcessor
{
private:
	// EquationProcessor function variables
	HyperStates* _temper;
	// Underived
	std::vector<std::string>& _expressions;
	std::vector<std::vector<std::string>>& _all_tokens;
	std::vector<std::pair<HyperStates*, std::pair<Operation, std::pair<std::string, std::string>>>>& _OpSet;
	size_t _count; 
protected:
	// Protected method for derived and underived
	void parse();
	void expression_tree();
	void tree_reduction(const size_t& start);
	// Protected variables
	HyperStates _h_store; MATHyperStates _h_MAT;
	std::vector<HyperStates> _changes_vector;
	size_t _changes{0};
public:

	EquationProcessor() = default;

	EquationProcessor(std::vector<std::string>& expressions,
					  std::vector<std::vector<std::string>>& all_tokens,
					  std::vector<std::pair<HyperStates*, std::pair<Operation, std::pair<std::string, std::string>>>>& OpSet,
		              const size_t& count = 0);

	// Depreciated code
	operator MATHyperStates();

	// Access the MATHyperStates element
	HyperStates* operator[](const size_t& n);

	// With respect to other States object (Scalar Operations)
	MATHyperStates operator+(const HyperStates& s);
	MATHyperStates operator*(const HyperStates& s);
	MATHyperStates operator-(const HyperStates& s);
	MATHyperStates operator/(const HyperStates& s);
	MATHyperStates operator^(const HyperStates& s);
	MATHyperStates operator<=(const HyperStates& s);
	MATHyperStates operator>=(const HyperStates& s);

	// With respect to other States object (Matrix Operations)
	MATHyperStates operator+(const MATHyperStates& S);
	MATHyperStates operator*(const MATHyperStates& S);
	MATHyperStates operator-(const MATHyperStates& S);
	MATHyperStates operator<=(const MATHyperStates& S);
	MATHyperStates operator>=(const MATHyperStates& S);

	// With respect to scalar values (Operations)
	MATHyperStates operator+(const Real& val);
	MATHyperStates operator*(const Real& val);
	MATHyperStates operator-(const Real& val);
	MATHyperStates operator/(const Real& val);
	MATHyperStates operator^(const Real& val);
	MATHyperStates operator<=(const Real& val);
	MATHyperStates operator>=(const Real& val);

	// Negation operator
	MATHyperStates operator- ();

	// Replace and revert back functions
	void replace(const HyperStates* s_old, const HyperStates* s_new);
	void revert();

	// Getters for HyperState and MATHyperStates 
	const HyperStates& getHyperStates() const;
	const MATHyperStates& getMATHyperStates() const;

	// Destructor
	~EquationProcessor() = default;
};

