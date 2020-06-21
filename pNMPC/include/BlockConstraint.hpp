#pragma once
#include "pNMPC_headers.hpp"
class BlockConstraint
{
private:
	HyperStates _BC;
	std::unique_ptr<ScalarConstraint> _BCptr;
	bool _activation_flag{ false };
public:
	BlockConstraint(HyperStates&& S);
	BlockConstraint(HyperStates&& S, bool activation_flag);
	BlockConstraint(const HyperStates& S);
	BlockConstraint(const HyperStates& S, bool activation_flag);
	void Intersect(const BlockConstraint& bc2int);
	void Union(const BlockConstraint& bc2int);
	~BlockConstraint();
};

