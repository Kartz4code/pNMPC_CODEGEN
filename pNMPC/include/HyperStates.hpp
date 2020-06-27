#pragma once
#include "pNMPC_headers.hpp"
using namespace pNMPC;
class HyperStates : public Element
{
private:
	// Value of HyperState, variable name and alias name, expression
	Real _h_value{ 0 };
	std::string _var_name{};
	std::string _var_alias_name{};
	std::string _expression{};

	// Static variable (counter variable)
	static size_t _count_h;

	// External function link
	bool _extern_bit{ false };
	std::string _func_name{};

public:
	// HyperStates Constructors
	HyperStates();
	HyperStates(const int& val);
	HyperStates(const Real& val);
	HyperStates(void** ptr);
	HyperStates(const std::string& str);
	HyperStates(const TYPE& ht, const std::string& str);

	// Setters and getters
	void setValue(const Real& val) override;
	const Real getValue() const override;
	const std::string& getName() const override;
	const std::string& getExpression() const override;
	void setExpression(const std::string& str) override;
	const std::string& getAliasExpression() const override;
	void setAliasExpression(const std::string& str) override;

	// With respect to other HyperStates object (Operations) => L-values
	HyperStates operator+(const HyperStates& s);
	HyperStates operator*(const HyperStates& s);
	HyperStates operator-(const HyperStates& s);
	HyperStates operator/(const HyperStates& s);
	HyperStates operator^(const HyperStates& s);
	HyperStates operator<=(const HyperStates& s);
	HyperStates operator>=(const HyperStates& s);

	// With respect to scalar values (Operations) => L-values
	HyperStates operator+(const Real& val);
	HyperStates operator*(const Real& val);
	HyperStates operator-(const Real& val);
	HyperStates operator/(const Real& val);
	HyperStates operator^(const Real& val);
	HyperStates operator<=(const Real& val);
	HyperStates operator>=(const Real& val);
	// Negation operator
	HyperStates operator- ();
	
	// Equal to operator 
	void operator=(const Real& val);
	HyperStates& operator=(const HyperStates& H);

	// Logical " and "
	template<typename T = HyperStates>
	friend HyperStates operator&&(const HyperStates& s_lhs, const HyperStates& s_rhs)
	{
		std::string before, after, lhs_new{ s_lhs.getAliasExpression() }, rhs_new{ s_rhs.getAliasExpression() };
		size_t pos_leq, pos_geq;

		pos_leq = s_lhs.find_string("<=");
		pos_geq = s_lhs.find_string(">=");
		// LHS side
		if (pos_leq)
		{
			before = s_lhs.getExpression().substr(1, pos_leq-1);
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
		return{ HyperStates{maximum(HyperStates{ lhs_new },0)} * HyperStates{maximum(HyperStates{ rhs_new },0)} };
	}

	// Logical " or "
	template<typename T = HyperStates>
	friend HyperStates operator||(const HyperStates& s_lhs, const HyperStates& s_rhs)
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
	template<typename T = HyperStates>
	friend HyperStates operator+(const Real& lhs, HyperStates& rhs)
	{
		rhs.setExpression(rhs.getAliasExpression());
		return{ HyperStates(Paren[ParenOps::open]
			+ std::to_string(lhs) + sOps[Operation::add] + rhs.getExpression() + Paren[ParenOps::close]) };
	}

	template<typename T = HyperStates>
	friend HyperStates operator*(const Real& lhs, HyperStates& rhs) {
		rhs.setExpression(rhs.getAliasExpression());
		return{ HyperStates(Paren[ParenOps::open] + std::to_string(lhs) + sOps[Operation::multiply] + rhs.getExpression() + Paren[ParenOps::close]) };
	}

	template<typename T = HyperStates>
	friend HyperStates operator-(const Real& lhs, HyperStates& rhs) {
		rhs.setExpression(rhs.getAliasExpression());
		return{ HyperStates(Paren[ParenOps::open]
			+ std::to_string(lhs) + sOps[Operation::subract] + rhs.getExpression() + Paren[ParenOps::close]) };
	}

	template<typename T = HyperStates>
	friend HyperStates operator/(const Real& lhs, HyperStates& rhs) {
		rhs.setExpression(rhs.getAliasExpression());
		return{ HyperStates(Paren[ParenOps::open]
			+ std::to_string(lhs) + sOps[Operation::divide] + rhs.getExpression() + Paren[ParenOps::close]) };
	}

	template<typename T = HyperStates>
	friend HyperStates operator^(const Real& lhs, HyperStates& rhs) {
		rhs.setExpression(rhs.getAliasExpression());
		return{ HyperStates(Paren[ParenOps::open]
			+ std::to_string(lhs) + sOps[Operation::power] + rhs.getExpression() + Paren[ParenOps::close]) };
	}

	template<typename T = HyperStates>
	friend HyperStates operator<=(const Real& lhs, HyperStates& rhs) {
		rhs.setExpression(rhs.getAliasExpression());
		return{ HyperStates(Paren[ParenOps::open]
			+ std::to_string(lhs) + sOps[Operation::leq] + rhs.getExpression() + Paren[ParenOps::close]) };
	}

	template<typename T = HyperStates>
	friend HyperStates operator>=(const Real& lhs, HyperStates& rhs) {
		rhs.setExpression(rhs.getAliasExpression());
		return{ HyperStates(Paren[ParenOps::open]
			+ std::to_string(lhs) + sOps[Operation::geq] + rhs.getExpression() + Paren[ParenOps::close]) };
	}

	// Functions (Trigonometry, log, etc)
	// Trignometric functions (L-Value)
	template<typename T = HyperStates> 
	friend HyperStates sin(const HyperStates& s)
	{
		return{ HyperStates(Paren[ParenOps::open] + Functions[0] + Paren[ParenOps::space] + sOps[Operation::operate] + Paren[ParenOps::space] + s.getAliasExpression() + Paren[ParenOps::close]) };
	}

	template<typename T = HyperStates>
	friend HyperStates cos(const HyperStates& s)
	{
		return{ HyperStates(Paren[ParenOps::open] + Functions[1] + Paren[ParenOps::space] + sOps[Operation::operate] + Paren[ParenOps::space] + s.getAliasExpression() + Paren[ParenOps::close]) };
	}

	template<typename T = HyperStates>
	friend HyperStates tan(const HyperStates& s)
	{
		return{ HyperStates(Paren[ParenOps::open] + Functions[2] + Paren[ParenOps::space] + sOps[Operation::operate] + Paren[ParenOps::space] + s.getAliasExpression() + Paren[ParenOps::close]) };
	}

	template<typename T = HyperStates>
	friend HyperStates sinh(const HyperStates& s)
	{
		return{ HyperStates(Paren[ParenOps::open] + Functions[3] + Paren[ParenOps::space] + sOps[Operation::operate] + Paren[ParenOps::space] + s.getAliasExpression() + Paren[ParenOps::close]) };
	}

	template<typename T = HyperStates>
	friend HyperStates cosh(const HyperStates& s)
	{
		return{ HyperStates(Paren[ParenOps::open] + Functions[4] + Paren[ParenOps::space] + sOps[Operation::operate] + Paren[ParenOps::space] + s.getAliasExpression() + Paren[ParenOps::close]) };
	}

	template<typename T = HyperStates>
	friend HyperStates tanh(const HyperStates& s)
	{
		return{ HyperStates(Paren[ParenOps::open] + Functions[5] + Paren[ParenOps::space] + sOps[Operation::operate] + Paren[ParenOps::space] + s.getAliasExpression() + Paren[ParenOps::close]) };
	}

	template<typename T = HyperStates>
	friend HyperStates exp(const HyperStates& s)
	{
		return{ HyperStates(Paren[ParenOps::open] + Functions[6] + Paren[ParenOps::space] + sOps[Operation::operate] + Paren[ParenOps::space] + s.getAliasExpression() + Paren[ParenOps::close]) };
	}

	template<typename T = HyperStates>
	friend HyperStates log(const HyperStates& s)
	{
		return{ HyperStates(Paren[ParenOps::open] + Functions[7] + Paren[ParenOps::space] + sOps[Operation::operate] + Paren[ParenOps::space] + s.getAliasExpression() + Paren[ParenOps::close]) };
	}

	template<typename T = HyperStates>
	friend HyperStates abs(const HyperStates& s)
	{
		return{ HyperStates(Paren[ParenOps::open] + Functions[8] + Paren[ParenOps::space] + sOps[Operation::operate] + Paren[ParenOps::space] + s.getAliasExpression() + Paren[ParenOps::close]) };
	}

	template<typename T = HyperStates>
	friend HyperStates asin(const HyperStates& s)
	{
		return{ HyperStates(Paren[ParenOps::open] + Functions[9] + Paren[ParenOps::space] + sOps[Operation::operate] + Paren[ParenOps::space] + s.getAliasExpression() + Paren[ParenOps::close]) };
	}

	template<typename T = HyperStates>
	friend HyperStates acos(const HyperStates& s)
	{
		return{ T(Paren[ParenOps::open] + Functions[10] + Paren[ParenOps::space] + sOps[Operation::operate] + Paren[ParenOps::space] + s.getAliasExpression() + Paren[ParenOps::close]) };
	}

	template<typename T = HyperStates>
	friend HyperStates atan(const HyperStates& s)
	{
		return{ HyperStates(Paren[ParenOps::open] + Functions[11] + Paren[ParenOps::space] + sOps[Operation::operate] + Paren[ParenOps::space] + s.getAliasExpression() + Paren[ParenOps::close]) };
	}

	template<typename T = HyperStates>
	friend HyperStates asinh(const HyperStates& s)
	{
		return{ HyperStates(Paren[ParenOps::open] + Functions[12] + Paren[ParenOps::space] + sOps[Operation::operate] + Paren[ParenOps::space] + s.getAliasExpression() + Paren[ParenOps::close]) };
	}

	template<typename T = HyperStates>
	friend HyperStates acosh(const HyperStates& s)
	{
		return{ HyperStates(Paren[ParenOps::open] + Functions[13] + Paren[ParenOps::space] + sOps[Operation::operate] + Paren[ParenOps::space] + s.getAliasExpression() + Paren[ParenOps::close]) };
	}

	template<typename T = HyperStates>
	friend HyperStates atanh(const HyperStates& s)
	{
		return{ HyperStates(Paren[ParenOps::open] + Functions[14] + Paren[ParenOps::space] + sOps[Operation::operate] + Paren[ParenOps::space] + s.getAliasExpression() + Paren[ParenOps::close]) };
	}

	template<typename T = HyperStates>
	friend HyperStates sign(const HyperStates& s)
	{
		return{ HyperStates(Paren[ParenOps::open] + Functions[18] + Paren[ParenOps::space] + sOps[Operation::operate] + Paren[ParenOps::space] + s.getAliasExpression() + Paren[ParenOps::close]) };
	}

	// All max functions
	template<typename T = HyperStates>
	friend HyperStates maximum(const HyperStates& s_lhs, const HyperStates& s_rhs)
	{
		return{ HyperStates(Paren[ParenOps::open] + s_lhs.getAliasExpression() + sOps[Operation::max] + s_rhs.getAliasExpression() + Paren[ParenOps::close]) };
	}

	template<typename T = HyperStates>
	friend HyperStates maximum(const Real& s_lhs, const HyperStates& s_rhs)
	{
		return{ HyperStates(Paren[ParenOps::open] + std::to_string(s_lhs) + sOps[Operation::max] + s_rhs.getAliasExpression() + Paren[ParenOps::close]) };
	}

	template<typename T = HyperStates>
	friend HyperStates maximum(const HyperStates& s_lhs, const Real& s_rhs)
	{
		return{ HyperStates(Paren[ParenOps::open] + s_lhs.getAliasExpression() + sOps[Operation::max] + std::to_string(s_rhs) + Paren[ParenOps::close]) };
	}

	template<typename T = HyperStates>
	friend HyperStates maximum(const Real& s_lhs, const Real& s_rhs)
	{
		return{ HyperStates(Paren[ParenOps::open] + std::to_string(s_lhs) + sOps[Operation::max] + std::to_string(s_rhs) + Paren[ParenOps::close]) };
	}

	// All min functions
	template<typename T = HyperStates>
	friend HyperStates minimum(const HyperStates& s_lhs, const HyperStates& s_rhs)
	{
		return{ HyperStates(Paren[ParenOps::open] + s_lhs.getAliasExpression() + sOps[Operation::min] + s_rhs.getAliasExpression() + Paren[ParenOps::close]) };
	}

	template<typename T = HyperStates>
	friend HyperStates minimum(const Real& s_lhs, const HyperStates& s_rhs)
	{
		return{ HyperStates(Paren[ParenOps::open] + std::to_string(s_lhs) + sOps[Operation::min] + s_rhs.getAliasExpression() + Paren[ParenOps::close]) };
	}

	template<typename T = HyperStates>
	friend HyperStates minimum(const HyperStates& s_lhs, const Real& s_rhs)
	{
		return{ HyperStates(Paren[ParenOps::open] + s_lhs.getAliasExpression() + sOps[Operation::min] + std::to_string(s_rhs) + Paren[ParenOps::close]) };
	}

	template<typename T = HyperStates>
	friend HyperStates minimum(const Real& s_lhs, const Real& s_rhs)
	{
		return{ HyperStates(Paren[ParenOps::open] + std::to_string(s_lhs) + sOps[Operation::min] + std::to_string(s_rhs) + Paren[ParenOps::close]) };
	}

	// Output stream
	friend std::ostream& operator<<(std::ostream& os, const HyperStates& S);
	// Return current hyperstate
	HyperStates& getCurrentHyperState();
	// Check whether an expression exists 
	bool replace_check(const HyperStates* s_old) const;
	// Replace with new HyperState and return a fresh HyperState
	HyperStates replace(const HyperStates* s_old, const HyperStates* s_new);
	// Find string
	size_t find_string(const std::string& str) const;
	// Get Count 
	const size_t getCountHyperStates() const;
	// Flush HyperStates content
	void flush();
	// Is external
	const bool isExtern() const;
	// Get function name
	const std::string& getFunctionName() const;
	// Destructor
	~HyperStates() = default;
};
