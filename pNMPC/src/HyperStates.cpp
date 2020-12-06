#include "pNMPC_headers.hpp"
#include "HyperStates.hpp"


size_t HyperStates::_count_h{ 0 };

HyperStates::HyperStates() : _var_name{ Paren[ParenOps::open] + "0*0" + Paren[ParenOps::close] },
							 _var_alias_name{ Paren[ParenOps::open] + "0*0" + Paren[ParenOps::close] },
							 _expression{ Paren[ParenOps::open] + "0*0" + Paren[ParenOps::close] }
{
 // A fresh hyperstate not loaded into the Database
}

HyperStates::HyperStates(void** ptr) : _var_name{Var[TYPE::STATE] + std::to_string(_count_h)},
									   _var_alias_name{Var[TYPE::STATE] + std::to_string(_count_h)}
{ 
	db->accessAllHyperMaps()[this->getName()] = &this->getCurrentHyperState(); 
	_count_h++;
}


HyperStates::HyperStates(const int& val) : HyperStates{static_cast<Real>(val)}
{
	// not loaded into the Database
}

HyperStates::HyperStates(const Real& val) : _h_value{ val },
											_var_name{ Paren[ParenOps::open] + std::to_string(val) + "*1" + Paren[ParenOps::close] },
											_var_alias_name{ Paren[ParenOps::open] + std::to_string(val) + "*1" + Paren[ParenOps::close] },
											_expression{ Paren[ParenOps::open] + std::to_string(val) + "*1" + Paren[ParenOps::close] }
{	
	//  not loaded into the Database
}

HyperStates::HyperStates(const std::string& str) : _expression{ str }
				
{
	this->_var_alias_name = this->_expression;
}


HyperStates::HyperStates(const TYPE& ht, const std::string& str) : _func_name{ str },
																   _var_name{ Var[TYPE::STATE] + std::to_string(_count_h) },
																   _var_alias_name{ Var[TYPE::STATE] + std::to_string(_count_h) }
{
	this->_extern_bit = (ht == TYPE::EXTERNAL) ? true : false;
	db->accessAllHyperMaps()[this->getName()] = &this->getCurrentHyperState();
	_count_h++;
}


void HyperStates::setValue(const Real& val)
{
	this->_h_value = val;
}

const Real HyperStates::getValue() const
{
	return this->_h_value;
}

const std::string& HyperStates::getName() const
{
	return this->_var_name;
}

const std::string& HyperStates::getExpression() const
{
	if (!this->_expression.empty())
		return this->_expression;
	else
		return this->_var_name;
}

const std::string& HyperStates::getAliasExpression() const
{
	return this->_var_alias_name;
}

void HyperStates::setExpression(const std::string& str)
{
	this->_expression = str;
}

void HyperStates::setAliasExpression(const std::string& str)
{
	this->_var_alias_name = str;
}

HyperStates HyperStates::operator+ (const HyperStates& s)
{
	this->setExpression(this->getAliasExpression());
	return{ HyperStates(Paren[ParenOps::open]
		+ this->getExpression() + sOps[Operation::add] + s.getAliasExpression() + Paren[ParenOps::close]) };
}

HyperStates HyperStates::operator*(const HyperStates& s)
{
	this->setExpression(this->getAliasExpression());
	return{ HyperStates(Paren[ParenOps::open]
		+ this->getExpression() + sOps[Operation::multiply] + s.getAliasExpression() + Paren[ParenOps::close])};
}

HyperStates HyperStates::operator-(const HyperStates& s)
{
	this->setExpression(this->getAliasExpression());
	return{ HyperStates(Paren[ParenOps::open]
		+ this->getExpression() + sOps[Operation::subract] + s.getAliasExpression() + Paren[ParenOps::close]) };
}

HyperStates HyperStates::operator/(const HyperStates& s)
{
	this->setExpression(this->getAliasExpression());
	return{ HyperStates(Paren[ParenOps::open] 
		+ this->getExpression() + sOps[Operation::divide] + s.getAliasExpression() + Paren[ParenOps::close]) };
}

HyperStates HyperStates::operator^(const HyperStates& s)
{
	this->setExpression(this->getAliasExpression());
	return{ HyperStates(Paren[ParenOps::open]
		+ this->getExpression() + sOps[Operation::power] + s.getAliasExpression() + Paren[ParenOps::close]) };
}

HyperStates HyperStates::operator<=(const HyperStates& s)
{
	this->setExpression(this->getAliasExpression());
	return{ HyperStates(Paren[ParenOps::open]
		+ this->getExpression() + sOps[Operation::leq] + s.getAliasExpression() + Paren[ParenOps::close]) };
}

HyperStates HyperStates::operator>=(const HyperStates& s)
{
	this->setExpression(this->getAliasExpression());
	return{ HyperStates(Paren[ParenOps::open]
		+ this->getExpression() + sOps[Operation::geq] + s.getAliasExpression() + Paren[ParenOps::close]) };
}

// With respect to scalar values (Operations) => L-values
HyperStates HyperStates::operator+(const Real& val)
{
	this->setExpression(this->getAliasExpression());
	if (val > 0)
		return{ HyperStates(Paren[ParenOps::open]
			+ this->getExpression() + sOps[Operation::add] + std::to_string(val) + Paren[ParenOps::close]) };
	else
		return{ HyperStates(Paren[ParenOps::open]
			+ this->getExpression() + Paren[ParenOps::space] + sOps[Operation::subract] + Paren[ParenOps::space] + std::to_string(-val) + Paren[ParenOps::close]) };
}

HyperStates HyperStates::operator-(const Real& val)
{
	this->setExpression(this->getAliasExpression());
	if (val > 0)
		return{ HyperStates(Paren[ParenOps::open]
			+ this->getExpression() + Paren[ParenOps::space] + sOps[Operation::subract] + Paren[ParenOps::space] + std::to_string(val) + Paren[ParenOps::close]) };
	else {
		return{ HyperStates(Paren[ParenOps::open]
			+ this->getExpression() + sOps[Operation::add] + std::to_string(-val) + Paren[ParenOps::close]) };
	}
}

HyperStates HyperStates::operator*(const Real& val)
{
	this->setExpression(this->getAliasExpression());
	return{ HyperStates(Paren[ParenOps::open]
		+ this->getExpression() + sOps[Operation::multiply] + std::to_string(val) + Paren[ParenOps::close]) };
}

HyperStates HyperStates::operator/(const Real& val)
{
	this->setExpression(this->getAliasExpression());
	return{ HyperStates(Paren[ParenOps::open]
		+ this->getExpression() + sOps[Operation::divide] + std::to_string(val) + Paren[ParenOps::close]) };
}

HyperStates HyperStates::operator^(const Real& val)
{
	this->setExpression(this->getAliasExpression());
	return{ HyperStates(Paren[ParenOps::open]
		+ this->getExpression() + sOps[Operation::power] + std::to_string(val) +  Paren[ParenOps::close]) };
}

HyperStates HyperStates::operator<=(const Real& val)
{
	this->setExpression(this->getAliasExpression());
	return{ HyperStates(Paren[ParenOps::open]
		+ this->getExpression() + sOps[Operation::leq] + std::to_string(val) + Paren[ParenOps::close]) };
}

HyperStates HyperStates::operator>=(const Real& val)
{
	this->setExpression(this->getAliasExpression());
	return{ HyperStates(Paren[ParenOps::open]
		+ this->getExpression() + sOps[Operation::geq] + std::to_string(val) + Paren[ParenOps::close]) };
}

// Negation operator
HyperStates HyperStates::operator-()
{
	this->setExpression(this->getAliasExpression());
	return{ HyperStates(std::string(Paren[ParenOps::open])
		+ "-1" + sOps[Operation::multiply] + this->getExpression() + Paren[ParenOps::close]) };
}

// Output stream operator
std::ostream& operator<<(std::ostream& os, const HyperStates& S)
{
	return os << S.getExpression();
}

// Return current HyperStates
HyperStates& HyperStates::getCurrentHyperState()
{
	return *this;
}

HyperStates HyperStates::replace(const HyperStates* s_old, const HyperStates* s_new)
{
	std::string::size_type pos = 0u; std::string str_temp;
	while ((pos = this->getExpression().find(s_old->getName(), pos)) != std::string::npos) {
		str_temp = this->getExpression();
		this->setAliasExpression(str_temp.replace(pos, s_old->getName().length(), s_new->getExpression()));
		this->setExpression(str_temp.replace(pos, s_old->getName().length(), s_new->getExpression()));
		pos += s_new->getExpression().length();
	}
	return HyperStates{ *this };
}

bool HyperStates::replace_check(const HyperStates* s_old) const 
{
	std::string::size_type pos = 0u;
	return ((pos = this->getExpression().find(s_old->getName(), pos)) != std::string::npos);
}

// Find string
size_t HyperStates::find_string(const std::string& str) const
{
	std::string::size_type pos = 0u;
	if ((pos = this->getExpression().find(str, pos)) != std::string::npos)
		return pos;
	else
		return 0;
}

// Flush content
void HyperStates::flush()
{
	this->_var_name = Paren[ParenOps::open] + "0*0" + Paren[ParenOps::close];
	this->_var_alias_name = Paren[ParenOps::open] + "0*0" + Paren[ParenOps::close];
	this->_expression = Paren[ParenOps::open] + "0*0" + Paren[ParenOps::close];
}

void HyperStates::operator=(const Real& val)
{
	this->_var_name = std::to_string(val);
	this->_var_alias_name = Paren[ParenOps::open] + std::to_string(val) + "*1" + Paren[ParenOps::close];
	this->_expression = Paren[ParenOps::open] + std::to_string(val) + "*1" + Paren[ParenOps::close];
}


HyperStates& HyperStates::operator=(const HyperStates& H)
{
	if (auto it = dynamic_cast<const States*>(&H))
	{
		this->_h_value = it->getValue();
		this->_var_alias_name = Paren[ParenOps::open] + it->getAliasExpression() + "*1" + Paren[ParenOps::close];
		this->_expression = Paren[ParenOps::open] + it->getExpression() + "*1" + Paren[ParenOps::close];
		return *this;
	}
	else if (auto it = dynamic_cast<const Inputs*>(&H))
	{
		this->_h_value = it->getValue();
		this->_var_alias_name = Paren[ParenOps::open] + it->getAliasExpression() + "*1" + Paren[ParenOps::close];
		this->_expression = Paren[ParenOps::open] + it->getExpression() + "*1" + Paren[ParenOps::close];
		return *this;
	}
	else if (auto it = dynamic_cast<const Params*>(&H))
	{
		this->_h_value = it->getValue();
		this->_var_alias_name = Paren[ParenOps::open] + it->getAliasExpression() + "*1" + Paren[ParenOps::close];
		this->_expression = Paren[ParenOps::open] + it->getExpression() + "*1" + Paren[ParenOps::close];
		return *this;
	}
	else if (auto it = dynamic_cast<const External*>(&H))
	{
		this->_h_value = it->getValue();
		this->_var_alias_name = Paren[ParenOps::open] + it->getAliasExpression() + "*1" + Paren[ParenOps::close];
		this->_expression = Paren[ParenOps::open] + it->getExpression() + "*1" + Paren[ParenOps::close];
		return *this;
	}
	else
	{
		this->_h_value = H.getValue();
		this->_var_alias_name = H.getAliasExpression();
		this->_expression = H.getExpression();
		return *this;
	}
}

// Is external
const bool HyperStates::isExtern() const
{
	return this->_extern_bit;
}
// Get function name
const std::string& HyperStates::getFunctionName() const
{
	return this->_func_name;
}

// Get Count 
const size_t HyperStates::getCountHyperStates() const
{
	return this->_count_h;
}

// Logical " and "
HyperStates operator&&(const HyperStates& s_lhs, const HyperStates& s_rhs)
{
	std::string before, after, lhs_new{ s_lhs.getAliasExpression() }, rhs_new{ s_rhs.getAliasExpression() };
	size_t pos_leq, pos_geq;

	pos_leq = s_lhs.find_string("<=");
	pos_geq = s_lhs.find_string(">=");
	// LHS side
	if (pos_leq)
	{
		before = s_lhs.getExpression().substr(1, pos_leq - 1);
		after = s_lhs.getExpression().substr(pos_leq + 2, s_lhs.getExpression().length() - pos_leq - 3);
		if (is_number(after) && std::atof(after.c_str()) < 0)
		{
			after = std::to_string(-1 * std::atof(after.c_str()));
			lhs_new = Paren[ParenOps::open] + before + Paren[ParenOps::space] + sOps[Operation::add] + Paren[ParenOps::space] + after + Paren[ParenOps::close];
		}
		else
			lhs_new = Paren[ParenOps::open] + before + Paren[ParenOps::space] + sOps[Operation::subract] + Paren[ParenOps::space] + after + Paren[ParenOps::close];
	}

	if (pos_geq)
	{
		after = s_lhs.getExpression().substr(pos_geq + 2, s_lhs.getExpression().length() - pos_geq - 3);
		before = s_lhs.getExpression().substr(1, pos_geq - 1);
		if (is_number(before) && std::atof(before.c_str()) < 0)
		{
			before = std::to_string(-1 * std::atof(before.c_str()));
			lhs_new = Paren[ParenOps::open] + after + Paren[ParenOps::space] + sOps[Operation::add] + Paren[ParenOps::space] + before + Paren[ParenOps::close];
		}
		else
			lhs_new = Paren[ParenOps::open] + after + Paren[ParenOps::space] + sOps[Operation::subract] + Paren[ParenOps::space] + before + Paren[ParenOps::close];
	}

	// RHS side
	pos_leq = s_rhs.find_string("<=");
	pos_geq = s_rhs.find_string(">=");
	if (pos_leq)
	{
		before = s_rhs.getExpression().substr(1, pos_leq - 1);
		after = s_rhs.getExpression().substr(pos_leq + 2, s_rhs.getExpression().length() - pos_leq - 3);
		if (is_number(after) && std::atof(after.c_str()) < 0)
		{
			after = std::to_string(-1 * std::atof(after.c_str()));
			rhs_new = Paren[ParenOps::open] + before + Paren[ParenOps::space] + sOps[Operation::add] + Paren[ParenOps::space] + after + Paren[ParenOps::close];
		}
		else
			rhs_new = Paren[ParenOps::open] + before + Paren[ParenOps::space] + sOps[Operation::subract] + Paren[ParenOps::space] + after + Paren[ParenOps::close];
	}

	if (pos_geq)
	{
		after = s_rhs.getExpression().substr(pos_geq + 2, s_rhs.getExpression().length() - pos_geq - 3);
		before = s_rhs.getExpression().substr(1, pos_geq - 1);
		if (is_number(before) && std::atof(before.c_str()) < 0)
		{
			before = std::to_string(-1 * std::atof(before.c_str()));
			rhs_new = Paren[ParenOps::open] + after + Paren[ParenOps::space] + sOps[Operation::add] + Paren[ParenOps::space] + before + Paren[ParenOps::close];
		}
		else
			rhs_new = Paren[ParenOps::open] + after + Paren[ParenOps::space] + sOps[Operation::subract] + Paren[ParenOps::space] + before + Paren[ParenOps::close];
	}
	return{ HyperStates{maximum(HyperStates{ lhs_new },0)} *HyperStates{maximum(HyperStates{ rhs_new },0)} };
}

// Logical " or "
HyperStates operator||(const HyperStates& s_lhs, const HyperStates& s_rhs)
{
	std::string before, after, lhs_new{ s_lhs.getAliasExpression() }, rhs_new{ s_rhs.getAliasExpression() };
	size_t pos_leq, pos_geq;

	pos_leq = s_lhs.find_string("<=");
	pos_geq = s_lhs.find_string(">=");
	// LHS side
	if (pos_leq)
	{
		before = s_lhs.getExpression().substr(1, pos_leq - 1);
		after = s_lhs.getExpression().substr(pos_leq + 2, s_lhs.getExpression().length() - pos_leq - 3);
		if (is_number(after) && std::atof(after.c_str()) < 0)
		{
			after = std::to_string(-1 * std::atof(after.c_str()));
			lhs_new = Paren[ParenOps::open] + before + Paren[ParenOps::space] + sOps[Operation::add] + Paren[ParenOps::space] + after + Paren[ParenOps::close];
		}
		else
			lhs_new = Paren[ParenOps::open] + before + Paren[ParenOps::space] + sOps[Operation::subract] + Paren[ParenOps::space] + after + Paren[ParenOps::close];
	}

	if (pos_geq)
	{
		after = s_lhs.getExpression().substr(pos_geq + 2, s_lhs.getExpression().length() - pos_geq - 3);
		before = s_lhs.getExpression().substr(1, pos_geq - 1);
		if (is_number(before) && std::atof(before.c_str()) < 0)
		{
			before = std::to_string(-1 * std::atof(before.c_str()));
			lhs_new = Paren[ParenOps::open] + after + Paren[ParenOps::space] + sOps[Operation::add] + Paren[ParenOps::space] + before + Paren[ParenOps::close];
		}
		else
			lhs_new = Paren[ParenOps::open] + after + Paren[ParenOps::space] + sOps[Operation::subract] + Paren[ParenOps::space] + before + Paren[ParenOps::close];
	}

	// RHS side
	pos_leq = s_rhs.find_string("<=");
	pos_geq = s_rhs.find_string(">=");
	if (pos_leq)
	{
		before = s_rhs.getExpression().substr(1, pos_leq - 1);
		after = s_rhs.getExpression().substr(pos_leq + 2, s_rhs.getExpression().length() - pos_leq - 3);
		if (is_number(after) && std::atof(after.c_str()) < 0)
		{
			after = std::to_string(-1 * std::atof(after.c_str()));
			rhs_new = Paren[ParenOps::open] + before + Paren[ParenOps::space] + sOps[Operation::add] + Paren[ParenOps::space] + after + Paren[ParenOps::close];
		}
		else
			rhs_new = Paren[ParenOps::open] + before + Paren[ParenOps::space] + sOps[Operation::subract] + Paren[ParenOps::space] + after + Paren[ParenOps::close];
	}

	if (pos_geq)
	{
		after = s_rhs.getExpression().substr(pos_geq + 2, s_rhs.getExpression().length() - pos_geq - 3);
		before = s_rhs.getExpression().substr(1, pos_geq - 1);
		if (is_number(before) && std::atof(before.c_str()) < 0)
		{
			before = std::to_string(-1 * std::atof(before.c_str()));
			rhs_new = Paren[ParenOps::open] + after + Paren[ParenOps::space] + sOps[Operation::add] + Paren[ParenOps::space] + before + Paren[ParenOps::close];
		}
		else
			rhs_new = Paren[ParenOps::open] + after + Paren[ParenOps::space] + sOps[Operation::subract] + Paren[ParenOps::space] + before + Paren[ParenOps::close];
	}

	return{ HyperStates{ maximum(HyperStates{ lhs_new },0) } + HyperStates{ maximum(HyperStates{ rhs_new },0) } };
}

// Friend functions (L-value)
HyperStates operator+(const Real& lhs, const HyperStates& rhs)
{
	return{ HyperStates(Paren[ParenOps::open] + std::to_string(lhs) + sOps[Operation::add] + 
		    rhs.getAliasExpression() + Paren[ParenOps::close]) };
}

HyperStates operator*(const Real& lhs, const HyperStates& rhs) {
	return{ HyperStates(Paren[ParenOps::open] + std::to_string(lhs) + sOps[Operation::multiply] + 
		    rhs.getAliasExpression() + Paren[ParenOps::close]) };
}

HyperStates operator-(const Real& lhs, const HyperStates& rhs) {
	return{ HyperStates(Paren[ParenOps::open] + std::to_string(lhs) + sOps[Operation::subract] + 
		    rhs.getAliasExpression() + Paren[ParenOps::close]) };
}

HyperStates operator/(const Real& lhs, const HyperStates& rhs) {
	return{ HyperStates(Paren[ParenOps::open] + std::to_string(lhs) + sOps[Operation::divide] + 
		    rhs.getAliasExpression() + Paren[ParenOps::close]) };
}

HyperStates operator^(const Real& lhs, const HyperStates& rhs) {
	return{ HyperStates(Paren[ParenOps::open] + std::to_string(lhs) + sOps[Operation::power] + 
		    rhs.getAliasExpression() + Paren[ParenOps::close]) };
}

HyperStates operator<=(const Real& lhs, const HyperStates& rhs) {
	return{ HyperStates(Paren[ParenOps::open] + std::to_string(lhs) + sOps[Operation::leq] + 
		    rhs.getAliasExpression() + Paren[ParenOps::close]) };
}

HyperStates operator>=(const Real& lhs, const HyperStates& rhs) {
	return{ HyperStates(Paren[ParenOps::open] + std::to_string(lhs) + sOps[Operation::geq] + 
		    rhs.getAliasExpression() + Paren[ParenOps::close]) };
}

// Functions (Trigonometry, log, etc)
// Trignometric functions (L-Value)
HyperStates sin(const HyperStates& s)
{
	return{ HyperStates(Paren[ParenOps::open] + Functions[0] + Paren[ParenOps::space] + sOps[Operation::operate] + Paren[ParenOps::space] + s.getAliasExpression() + Paren[ParenOps::close]) };
}

HyperStates cos(const HyperStates& s)
{
	return{ HyperStates(Paren[ParenOps::open] + Functions[1] + Paren[ParenOps::space] + sOps[Operation::operate] + Paren[ParenOps::space] + s.getAliasExpression() + Paren[ParenOps::close]) };
}

HyperStates tan(const HyperStates& s)
{
	return{ HyperStates(Paren[ParenOps::open] + Functions[2] + Paren[ParenOps::space] + sOps[Operation::operate] + Paren[ParenOps::space] + s.getAliasExpression() + Paren[ParenOps::close]) };
}

HyperStates sinh(const HyperStates& s)
{
	return{ HyperStates(Paren[ParenOps::open] + Functions[3] + Paren[ParenOps::space] + sOps[Operation::operate] + Paren[ParenOps::space] + s.getAliasExpression() + Paren[ParenOps::close]) };
}

HyperStates cosh(const HyperStates& s)
{
	return{ HyperStates(Paren[ParenOps::open] + Functions[4] + Paren[ParenOps::space] + sOps[Operation::operate] + Paren[ParenOps::space] + s.getAliasExpression() + Paren[ParenOps::close]) };
}

HyperStates tanh(const HyperStates& s)
{
	return{ HyperStates(Paren[ParenOps::open] + Functions[5] + Paren[ParenOps::space] + sOps[Operation::operate] + Paren[ParenOps::space] + s.getAliasExpression() + Paren[ParenOps::close]) };
}

HyperStates exp(const HyperStates& s)
{
	return{ HyperStates(Paren[ParenOps::open] + Functions[6] + Paren[ParenOps::space] + sOps[Operation::operate] + Paren[ParenOps::space] + s.getAliasExpression() + Paren[ParenOps::close]) };
}

HyperStates log(const HyperStates& s)
{
	return{ HyperStates(Paren[ParenOps::open] + Functions[7] + Paren[ParenOps::space] + sOps[Operation::operate] + Paren[ParenOps::space] + s.getAliasExpression() + Paren[ParenOps::close]) };
}

HyperStates abs(const HyperStates& s)
{
	return{ HyperStates(Paren[ParenOps::open] + Functions[8] + Paren[ParenOps::space] + sOps[Operation::operate] + Paren[ParenOps::space] + s.getAliasExpression() + Paren[ParenOps::close]) };
}

HyperStates asin(const HyperStates& s)
{
	return{ HyperStates(Paren[ParenOps::open] + Functions[9] + Paren[ParenOps::space] + sOps[Operation::operate] + Paren[ParenOps::space] + s.getAliasExpression() + Paren[ParenOps::close]) };
}

HyperStates acos(const HyperStates& s)
{
	return{ HyperStates(Paren[ParenOps::open] + Functions[10] + Paren[ParenOps::space] + sOps[Operation::operate] + Paren[ParenOps::space] + s.getAliasExpression() + Paren[ParenOps::close]) };
}

HyperStates atan(const HyperStates& s)
{
	return{ HyperStates(Paren[ParenOps::open] + Functions[11] + Paren[ParenOps::space] + sOps[Operation::operate] + Paren[ParenOps::space] + s.getAliasExpression() + Paren[ParenOps::close]) };
}

HyperStates asinh(const HyperStates& s)
{
	return{ HyperStates(Paren[ParenOps::open] + Functions[12] + Paren[ParenOps::space] + sOps[Operation::operate] + Paren[ParenOps::space] + s.getAliasExpression() + Paren[ParenOps::close]) };
}

HyperStates acosh(const HyperStates& s)
{
	return{ HyperStates(Paren[ParenOps::open] + Functions[13] + Paren[ParenOps::space] + sOps[Operation::operate] + Paren[ParenOps::space] + s.getAliasExpression() + Paren[ParenOps::close]) };
}

HyperStates atanh(const HyperStates& s)
{
	return{ HyperStates(Paren[ParenOps::open] + Functions[14] + Paren[ParenOps::space] + sOps[Operation::operate] + Paren[ParenOps::space] + s.getAliasExpression() + Paren[ParenOps::close]) };
}

HyperStates sign(const HyperStates& s)
{
	return{ HyperStates(Paren[ParenOps::open] + Functions[18] + Paren[ParenOps::space] + sOps[Operation::operate] + Paren[ParenOps::space] + s.getAliasExpression() + Paren[ParenOps::close]) };
}

// All max functions
HyperStates maximum(const HyperStates& s_lhs, const HyperStates& s_rhs)
{
	return{ HyperStates(Paren[ParenOps::open] + s_lhs.getAliasExpression() + sOps[Operation::max] + s_rhs.getAliasExpression() + Paren[ParenOps::close]) };
}

HyperStates maximum(const Real& s_lhs, const HyperStates& s_rhs)
{
	return{ HyperStates(Paren[ParenOps::open] + std::to_string(s_lhs) + sOps[Operation::max] + s_rhs.getAliasExpression() + Paren[ParenOps::close]) };
}

HyperStates maximum(const HyperStates& s_lhs, const Real& s_rhs)
{
	return{ HyperStates(Paren[ParenOps::open] + s_lhs.getAliasExpression() + sOps[Operation::max] + std::to_string(s_rhs) + Paren[ParenOps::close]) };
}

HyperStates maximum(const Real& s_lhs, const Real& s_rhs)
{
	return{ HyperStates(Paren[ParenOps::open] + std::to_string(s_lhs) + sOps[Operation::max] + std::to_string(s_rhs) + Paren[ParenOps::close]) };
}

// All min functions
HyperStates minimum(const HyperStates& s_lhs, const HyperStates& s_rhs)
{
	return{ HyperStates(Paren[ParenOps::open] + s_lhs.getAliasExpression() + sOps[Operation::min] + s_rhs.getAliasExpression() + Paren[ParenOps::close]) };
}

HyperStates minimum(const Real& s_lhs, const HyperStates& s_rhs)
{
	return{ HyperStates(Paren[ParenOps::open] + std::to_string(s_lhs) + sOps[Operation::min] + s_rhs.getAliasExpression() + Paren[ParenOps::close]) };
}

HyperStates minimum(const HyperStates& s_lhs, const Real& s_rhs)
{
	return{ HyperStates(Paren[ParenOps::open] + s_lhs.getAliasExpression() + sOps[Operation::min] + std::to_string(s_rhs) + Paren[ParenOps::close]) };
}

HyperStates minimum(const Real& s_lhs, const Real& s_rhs)
{
	return{ HyperStates(Paren[ParenOps::open] + std::to_string(s_lhs) + sOps[Operation::min] + std::to_string(s_rhs) + Paren[ParenOps::close]) };
}

