#include "pNMPC_headers.hpp"
#include "States.hpp"

size_t States::_count_x{ 0 };

States::States() : HyperStates{ nullptr }, 
				   _var_name{ Var[TYPE::STATE] + std::to_string(_count_x) },
				   _var_alias_name{ Var[TYPE::STATE] + std::to_string(_count_x) }
{
	db->accessAllStates().push_back(this); _count_x++;
	db->accessAllHyperMaps()[this->getName()] = &this->getCurrentHyperState();
}

States::States(const Real& val) : HyperStates{ nullptr }, 
								  _x_value{ val }, 
								  _var_name{ Var[TYPE::STATE] + std::to_string(_count_x)},
							      _var_alias_name{ Var[TYPE::STATE] + std::to_string(_count_x) }
{
	db->accessAllStates().push_back(this); _count_x++;
	db->accessAllHyperMaps()[this->getName()] = &this->getCurrentHyperState();
}


States::States(const std::string& str) : _expression{ str }
{
	this->_var_alias_name = this->_expression;
}

void States::setValue(const Real& val)
{
	this->_x_value = val;
}

const Real States::getValue() const
{
	return this->_x_value;
}

const std::string& States::getName() const
{
	return this->_var_name;
}

const std::string& States::getExpression() const
{
	if (!this->_expression.empty())
		return this->_expression;
	else
		return this->_var_name;
}

const std::string& States::getAliasExpression() const
{
	return this->_var_alias_name;
}

void States::setExpression(const std::string& str)
{
	this->_expression = str;
}

void States::operator=(const Real& val)
{
	this->setValue(val);
}

void States::operator=(const HyperStates& S)
{
	if (!S.replace_check(this))
	{
		// Delete from the States vector database
		for (auto item = db->accessAllStates().begin(); item != db->accessAllStates().end(); item++)
		{
			if ((*item)->getName() == this->getName())
			{
				db->accessAllStates().erase(item);
				break;
			}
		}
		// Delete from HyperStates map
		db->accessAllHyperMaps().erase(this->getName());
	}
	this->_var_alias_name = S.getExpression();
}

States& States::operator=(const States& In)
{
	if (this->getName() != In.getName())
		this->operator=(static_cast<HyperStates>(In));
	return *this;
}

States::operator States()
{
	return *this;
}