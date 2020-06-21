#include "pNMPC_headers.hpp"
#include "Inputs.hpp"

size_t Inputs::_count_u{ 0 };

Inputs::Inputs() : HyperStates{ nullptr }, 
				   _var_name{ Var[TYPE::INPUT] + std::to_string(_count_u)},
				   _var_alias_name{ Var[TYPE::INPUT] + std::to_string(_count_u)}
{
	db->accessAllInputs().push_back(this); _count_u++;
	db->accessAllHyperMaps()[this->getName()] = &this->getCurrentHyperState();
}

Inputs::Inputs(const Real& val) : HyperStates{ nullptr }, 
								  _u_value{ val },
								  _var_name{ Var[TYPE::INPUT] + std::to_string(_count_u) },
								  _var_alias_name{ Var[TYPE::INPUT] + std::to_string(_count_u)}
{
	db->accessAllInputs().push_back(this); _count_u++;
	db->accessAllHyperMaps()[this->getName()] = &this->getCurrentHyperState();
}


Inputs::Inputs(const std::string& str) : _expression{ str }
{
	this->_var_alias_name = this->_expression;
}

void Inputs::setValue(const Real& val)
{
	this->_u_value = val;
}

const Real Inputs::getValue() const
{
	return this->_u_value;
}

const std::string& Inputs::getName() const
{
	return this->_var_name;
}

const std::string& Inputs::getExpression() const
{
	if (!this->_expression.empty())
		return this->_expression;
	else
		return this->_var_name;
}

const std::string& Inputs::getAliasExpression() const
{
	return this->_var_alias_name;
}

void Inputs::setExpression(const std::string& str)
{
	this->_expression = str;
}

void Inputs::operator=(const Real& val)
{
	this->setValue(val);
}

void Inputs::operator=(const HyperStates& S)
{
	if(!S.replace_check(this))
	{
		// Delete from the Input vector database
		for (auto item = db->accessAllInputs().begin(); item != db->accessAllInputs().end(); item++)
		{
			if ((*item)->getName() == this->getName())
			{
				db->accessAllInputs().erase(item);
				break;
			}
		}
		// Delete from HyperStates map
		db->accessAllHyperMaps().erase(this->getName());
	}
	this->_var_alias_name = S.getExpression();
}

Inputs& Inputs::operator=(const Inputs& In)
{
	if(this->getName() != In.getName())
		this->operator=(static_cast<HyperStates>(In));
	return *this;
}

void Inputs::setControlParameterization(const size_t& val)
{
	this->_nPC = val;
}

const size_t Inputs::getControlParameterization() const
{
	return this->_nPC;
}

void Inputs::setCPType(const CONTROLPARAM& CP)
{
	this->_CP = CP;
}
const CONTROLPARAM& Inputs::getCPType() const
{
	return this->_CP;
}

void Inputs::setCPTime(const std::vector<Real>& CP_time)
{
	this->_CP_time.clear();
	for (auto it = CP_time.begin(); it != CP_time.end(); it++)
		this->_CP_time.push_back(*it);
}
void Inputs::setCPValue(const std::vector<Real>& CP_value)
{
	this->_CP_value.clear();
	for (auto it = CP_value.begin(); it != CP_value.end(); it++)
		this->_CP_value.push_back(*it);
}
const std::vector<Real>& Inputs::getCPTime() const
{
	return this->_CP_time;
}
const std::vector<Real>& Inputs::getCPValue() const
{
	return this->_CP_value;
}

// Setters and getters for upper and lowerbounds
void Inputs::setUpperBound(const Real& val)
{
	this->_U_up = val;
}

const Real Inputs::getUpperBound() const
{
	return this->_U_up;
}

void Inputs::setLowerBound(const Real& val)
{
	this->_U_low = val;
}

const Real Inputs::getLowerBound() const
{
	return this->_U_low;
}
