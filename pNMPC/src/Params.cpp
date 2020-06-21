#include "pNMPC_headers.hpp"
#include "Params.hpp"

size_t Params::_count_p{ 0 };

Params::Params() : HyperStates{ nullptr }, 
				   _var_name{ Var[TYPE::PARAMETER] + std::to_string(_count_p) },
				   _var_alias_name{ Var[TYPE::PARAMETER] + std::to_string(_count_p)}
{
	db->accessAllParams().push_back(this); _count_p++;
	db->accessAllHyperMaps()[this->getName()] = &this->getCurrentHyperState();
};

Params::Params(const Real& val) : HyperStates{ nullptr }, 
								  _p_value{ val }, 
								  _var_name{ Var[TYPE::PARAMETER] + std::to_string(_count_p) },
								  _var_alias_name{ Var[TYPE::PARAMETER] + std::to_string(_count_p)}
{
	db->accessAllParams().push_back(this); _count_p++;
	db->accessAllHyperMaps()[this->getName()] = &this->getCurrentHyperState();
}

Params::Params(const std::string& str) : _expression{ str }
{
	this->_var_alias_name = this->_expression;
}

void Params::setValue(const Real& val)
{
	this->_p_value = val;
}

const Real Params::getValue() const
{
	return this->_p_value;
}

const std::string& Params::getName() const
{
	return this->_var_name;
}

const std::string& Params::getExpression() const
{
	if (!this->_expression.empty())
		return this->_expression;
	else
		return this->_var_name;
}

const std::string& Params::getAliasExpression() const
{
	return this->_var_alias_name;
}

void Params::setExpression(const std::string& str)
{
	this->_expression = str;
}

void Params::operator=(const Real& val)
{
	this->setValue(val);
}

void Params::operator=(const HyperStates& S)
{
	if (!S.replace_check(this))
	{
		// Delete from the Parameter vector database
		for (auto item = db->accessAllParams().begin(); item != db->accessAllParams().end(); item++)
		{
			if ((*item)->getName() == this->getName())
			{
				db->accessAllParams().erase(item);
				break;
			}
		}
		// Delete from HyperStates map
		db->accessAllHyperMaps().erase(this->getName());
	}
	this->_var_alias_name = S.getExpression();
}

Params& Params::operator=(const Params& In)
{
	if (this->getName() != In.getName())
		this->operator=(static_cast<HyperStates>(In));
	return *this;
}