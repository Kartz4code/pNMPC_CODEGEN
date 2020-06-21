#include "pNMPC_headers.hpp"
#include "BlockConstraint.hpp"

BlockConstraint::BlockConstraint(HyperStates&& S) : BlockConstraint{S,false}
{
	if(this->_activation_flag)
		this->_BCptr = std::make_unique<ScalarConstraint>((this->_BC) <= 0);
}

BlockConstraint::BlockConstraint(const HyperStates& S) : BlockConstraint{ S,false }
{
	if (this->_activation_flag)
		this->_BCptr = std::make_unique<ScalarConstraint>((this->_BC) <= 0);
}


BlockConstraint::BlockConstraint(const HyperStates& S, bool activation_flag) : _BC{ S }, _activation_flag{ activation_flag }
{
	if (this->_activation_flag)
		this->_BCptr = std::make_unique<ScalarConstraint>((this->_BC) <= 0);
}

BlockConstraint::BlockConstraint(HyperStates&& S, bool activation_flag) : _BC{ S }, _activation_flag{activation_flag}
{
	if (this->_activation_flag)
		this->_BCptr = std::make_unique<ScalarConstraint>((this->_BC) <= 0);
}

void BlockConstraint::Intersect(const BlockConstraint& bc2int)
{
	this->_BCptr = std::make_unique<ScalarConstraint>((this->_BC && bc2int._BC) <= 0);
}

void BlockConstraint::Union(const BlockConstraint& bc2int)
{
	this->_BCptr = std::make_unique<ScalarConstraint>((this->_BC || bc2int._BC) <= 0);
}

BlockConstraint::~BlockConstraint()
{
}
