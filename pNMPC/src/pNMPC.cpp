/*
*    This file is part of pNMPC software.
*    Copyright (c) 2020 GIPSA lab [https://github.com/Kartz4code/pNMPC_CODEGEN]
*
*    Main developer - Karthik Murali Madhavan Rathai
*
*    pNMPC - A Code Generation Software Tool For Implementation of Derivative Free
*    Parameterized NMPC Scheme for Embedded Control Systems
*    The software was developed by Karthik Murali Madhavan Rathai under the supervision of
*    Mazen Alamir and Olivier Sename affiliated to Univ. Grenoble Alpes, CNRS, Grenoble INP,
*    GIPSA lab, 38000 Grenoble, France.
*
* 	 pNMPC software is free software: you can redistribute it and/or modify
*    it under the terms of the GNU General Public License as published by
*    the Free Software Foundation, either version 3 of the License, or
*    (at your option) any later version.
*
*	 pNMPC software is distributed in the hope that it will be useful,
*	 but WITHOUT ANY WARRANTY; without even the implied warranty of
*	 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*	 GNU General Public License for more details.
*
*	 You should have received a copy of the GNU General Public License
* 	 along with pNMPC software.  If not, see <http://www.gnu.org/licenses/>.
*
*/

#include "pNMPC_headers.hpp"
#include "pNMPC.hpp"
using namespace pNMPC;
void pNMPC::pNMPC_free()
{
	Database::ObliterateDatabase();
}

// Is the string a number
bool pNMPC::is_number(const std::string& s)
{
	return !s.empty() && s.find_first_not_of("-.0123456789") == std::string::npos;
}

// Is the string a integer
bool pNMPC::is_integer(const std::string& val)
{
	return (val.find('.') == -1);
}

// Is the string a function from the array of supported functions
bool pNMPC::is_function(const std::string& token)
{
	bool ret{ false };
	for (auto& item : Functions)
		if (item == token)
			ret = true;
	return ret;
}

// Count number of spaces in string 
size_t pNMPC::countStringSpaces(const std::string& str)
{
	size_t count{ 0 };
	for (size_t j{ 0 }; j < str.length(); j++)
		if (str[j] == ' ')
			count++;
	return count;
}

// Check whether the function name is proper in nature
bool pNMPC::isProperFunctionName(const std::string& str)
{
	bool isproperFunc{ true };
	if (!(str[0] == '_' || isalpha(str[0])))
		isproperFunc = false;
	for (size_t j{ 1 }; j < str.length(); j++)
	{
		if (!isalnum(str[j]) && str[j] != '_')
			isproperFunc = false;
	}
	return isproperFunc;
}


MATReal pNMPC::eye(const size_t& n)
{
	MATReal X(n, n);
	for (size_t i{ 0 }; i < n; i++)
		for (size_t j{ 0 }; j < n; j++)
			if (i == j)
				X[i][j] = 1;
	return X;
}

MATReal pNMPC::ones(const size_t& n, const size_t& m)
{
	MATReal X(n, m);
	for (size_t i{ 0 }; i < n; i++)
		for (size_t j{ 0 }; j < m; j++)
			X[i][j] = 1.0;
	return X;
}

MATReal pNMPC::zeros(const size_t& n, const size_t& m)
{
	MATReal X(n, m);
	for (size_t i{ 0 }; i < n; i++)
		for (size_t j{ 0 }; j < m; j++)
			X[i][j] = 0;
	return X;
}


MATReal pNMPC::diag(const std::initializer_list<Real>& X)
{
	size_t n{ X.size() };
	MATReal Xret(n, n);
	for (size_t i{ 0 }; i < n; i++)
		for (size_t j{ 0 }; j < n; j++)
			if (i == j)
				Xret[i][j] = *(X.begin() + i);
	return Xret;
}

MATHyperStates pNMPC::operator*(const Real& x, MATHyperStates& X)
{
	MATHyperStates Y(X.getNumRows(), X.getNumColumns());
	for (size_t i{ 0 }; i < Y.getNumRows(); i++)
		for (size_t j{ 0 }; j < Y.getNumColumns(); j++)
			Y[i][j] = x*X[i][j];
	return Y;
}

MATHyperStates pNMPC::operator*(MATHyperStates& X, const Real& x)
{
	MATHyperStates Y(X.getNumRows(), X.getNumColumns());
	for (size_t i{ 0 }; i < Y.getNumRows(); i++)
		for (size_t j{ 0 }; j < Y.getNumColumns(); j++)
			Y[i][j] = x*X[i][j];
	return Y;
}

// Multiplication operator
MATReal pNMPC::operator*(const Real& x, MATReal X)
{
	MATReal Y(X.getNumRows(), X.getNumColumns());
	for (size_t i{ 0 }; i < Y.getNumRows(); i++)
		for (size_t j{ 0 }; j < Y.getNumColumns(); j++)
			Y[i][j] = x*X[i][j];
	return Y;
}

// Addition operator
MATReal pNMPC::operator+(const Real& x, MATReal X)
{
	MATReal Y(X.getNumRows(), X.getNumColumns());
	for (size_t i{ 0 }; i < Y.getNumRows(); i++)
		for (size_t j{ 0 }; j < Y.getNumColumns(); j++)
			Y[i][j] = x + X[i][j];
	return Y;
}

// Subraction operator
MATReal pNMPC::operator-(const Real& x, MATReal X)
{
	MATReal Y(X.getNumRows(), X.getNumColumns());
	for (size_t i{ 0 }; i < Y.getNumRows(); i++)
		for (size_t j{ 0 }; j < Y.getNumColumns(); j++)
			Y[i][j] = x - X[i][j];
	return Y;
}

// Multiplication operator
MATHyperStates pNMPC::operator*(const Real& x, MATHyperStates X)
{
	MATHyperStates Y(X.getNumRows(), X.getNumColumns());
	for (size_t i{ 0 }; i < Y.getNumRows(); i++)
		for (size_t j{ 0 }; j < Y.getNumColumns(); j++)
			Y[i][j] = x*X[i][j];
	return Y;
}

// Addition operator
MATHyperStates pNMPC::operator+(const Real& x, MATHyperStates X)
{
	MATHyperStates Y(X.getNumRows(), X.getNumColumns());
	for (size_t i{ 0 }; i < Y.getNumRows(); i++)
		for (size_t j{ 0 }; j < Y.getNumColumns(); j++)
			Y[i][j] = x + X[i][j];
	return Y;
}

// Subraction operator
MATHyperStates pNMPC::operator-(const Real& x, MATHyperStates X)
{
	MATHyperStates Y(X.getNumRows(), X.getNumColumns());
	for (size_t i{ 0 }; i < Y.getNumRows(); i++)
		for (size_t j{ 0 }; j < Y.getNumColumns(); j++)
			Y[i][j] = x - X[i][j];
	return Y;
}

// Less than or equal to operator
MATHyperStates pNMPC::operator<=(const Real& x, MATHyperStates X)
{
	MATHyperStates Y(X.getNumRows(), X.getNumColumns());
	for (size_t i{ 0 }; i < Y.getNumRows(); i++)
		for (size_t j{ 0 }; j < Y.getNumColumns(); j++)
			Y[i][j] = (x <= X[i][j]);
	return Y;
}

// Greater than or equal to operator
MATHyperStates pNMPC::operator>=(const Real& x, MATHyperStates X)
{
	MATHyperStates Y(X.getNumRows(), X.getNumColumns());
	for (size_t i{ 0 }; i < Y.getNumRows(); i++)
		for (size_t j{ 0 }; j < Y.getNumColumns(); j++)
			Y[i][j] = (x >= X[i][j]);
	return Y;
}

// Multiplication
MATHyperStates pNMPC::operator*(HyperStates& x, MATHyperStates X)
{
	MATHyperStates Y(X.getNumRows(), X.getNumColumns());
	for (size_t i{ 0 }; i < Y.getNumRows(); i++)
		for (size_t j{ 0 }; j < Y.getNumColumns(); j++)
			Y[i][j] = x*X[i][j];
	return Y;
}

// Addition operator
MATHyperStates pNMPC::operator+(HyperStates& x, MATHyperStates X)
{
	MATHyperStates Y(X.getNumRows(), X.getNumColumns());
	for (size_t i{ 0 }; i < Y.getNumRows(); i++)
		for (size_t j{ 0 }; j < Y.getNumColumns(); j++)
			Y[i][j] = x+X[i][j];
	return Y;
}

// Subraction operator
MATHyperStates pNMPC::operator-(HyperStates& x, MATHyperStates X)
{
	MATHyperStates Y(X.getNumRows(), X.getNumColumns());
	for (size_t i{ 0 }; i < Y.getNumRows(); i++)
		for (size_t j{ 0 }; j < Y.getNumColumns(); j++)
			Y[i][j] = x-X[i][j];
	return Y;
}

// Less than or equal to operator
MATHyperStates pNMPC::operator<=(HyperStates& x, MATHyperStates X)
{
	MATHyperStates Y(X.getNumRows(), X.getNumColumns());
	for (size_t i{ 0 }; i < Y.getNumRows(); i++)
		for (size_t j{ 0 }; j < Y.getNumColumns(); j++)
			Y[i][j] = (x <= X[i][j]);
	return Y;
}

// Greater than or equal to operator
MATHyperStates  pNMPC::operator>=(HyperStates& x, MATHyperStates X)
{
	MATHyperStates Y(X.getNumRows(), X.getNumColumns());
	for (size_t i{ 0 }; i < Y.getNumRows(); i++)
		for (size_t j{ 0 }; j < Y.getNumColumns(); j++)
			Y[i][j] = (x >= X[i][j]);
	return Y;
}

// Otherfunctions
MATHyperStates pNMPC::sin(MATHyperStates X)
{
	MATHyperStates Y(X.getNumRows(), X.getNumColumns());
	for (size_t i{ 0 }; i < Y.getNumRows(); i++)
		for (size_t j{ 0 }; j < Y.getNumColumns(); j++)
			Y[i][j] = sin(X[i][j]);
	return Y;
}

MATHyperStates pNMPC::cos(MATHyperStates X)
{
	MATHyperStates Y(X.getNumRows(), X.getNumColumns());
	for (size_t i{ 0 }; i < Y.getNumRows(); i++)
		for (size_t j{ 0 }; j < Y.getNumColumns(); j++)
			Y[i][j] = cos(X[i][j]);
	return Y;
}
MATHyperStates pNMPC::tan(MATHyperStates X)
{
	MATHyperStates Y(X.getNumRows(), X.getNumColumns());
	for (size_t i{ 0 }; i < Y.getNumRows(); i++)
		for (size_t j{ 0 }; j < Y.getNumColumns(); j++)
			Y[i][j] = tan(X[i][j]);
	return Y;
}
MATHyperStates pNMPC::sinh(MATHyperStates X)
{
	MATHyperStates Y(X.getNumRows(), X.getNumColumns());
	for (size_t i{ 0 }; i < Y.getNumRows(); i++)
		for (size_t j{ 0 }; j < Y.getNumColumns(); j++)
			Y[i][j] = sinh(X[i][j]);
	return Y;
}
MATHyperStates pNMPC::cosh(MATHyperStates X)
{
	MATHyperStates Y(X.getNumRows(), X.getNumColumns());
	for (size_t i{ 0 }; i < Y.getNumRows(); i++)
		for (size_t j{ 0 }; j < Y.getNumColumns(); j++)
			Y[i][j] = cosh(X[i][j]);
	return Y;
}
MATHyperStates pNMPC::tanh(MATHyperStates X)
{
	MATHyperStates Y(X.getNumRows(), X.getNumColumns());
	for (size_t i{ 0 }; i < Y.getNumRows(); i++)
		for (size_t j{ 0 }; j < Y.getNumColumns(); j++)
			Y[i][j] = tanh(X[i][j]);
	return Y;
}
MATHyperStates pNMPC::exp(MATHyperStates X)
{
	MATHyperStates Y(X.getNumRows(), X.getNumColumns());
	for (size_t i{ 0 }; i < Y.getNumRows(); i++)
		for (size_t j{ 0 }; j < Y.getNumColumns(); j++)
			Y[i][j] = exp(X[i][j]);
	return Y;
}
MATHyperStates pNMPC::log(MATHyperStates X)
{
	MATHyperStates Y(X.getNumRows(), X.getNumColumns());
	for (size_t i{ 0 }; i < Y.getNumRows(); i++)
		for (size_t j{ 0 }; j < Y.getNumColumns(); j++)
			Y[i][j] = log(X[i][j]);
	return Y;
}
MATHyperStates pNMPC::abs(MATHyperStates X)
{
	MATHyperStates Y(X.getNumRows(), X.getNumColumns());
	for (size_t i{ 0 }; i < Y.getNumRows(); i++)
		for (size_t j{ 0 }; j < Y.getNumColumns(); j++)
			Y[i][j] = abs(X[i][j]);
	return Y;
}
MATHyperStates pNMPC::asin(MATHyperStates X)
{
	MATHyperStates Y(X.getNumRows(), X.getNumColumns());
	for (size_t i{ 0 }; i < Y.getNumRows(); i++)
		for (size_t j{ 0 }; j < Y.getNumColumns(); j++)
			Y[i][j] = asin(X[i][j]);
	return Y;
}
MATHyperStates pNMPC::acos(MATHyperStates X)
{
	MATHyperStates Y(X.getNumRows(), X.getNumColumns());
	for (size_t i{ 0 }; i < Y.getNumRows(); i++)
		for (size_t j{ 0 }; j < Y.getNumColumns(); j++)
			Y[i][j] = acos(X[i][j]);
	return Y;
}
MATHyperStates pNMPC::atan(MATHyperStates X)
{
	MATHyperStates Y(X.getNumRows(), X.getNumColumns());
	for (size_t i{ 0 }; i < Y.getNumRows(); i++)
		for (size_t j{ 0 }; j < Y.getNumColumns(); j++)
			Y[i][j] = atan(X[i][j]);
	return Y;
}
MATHyperStates pNMPC::asinh(MATHyperStates X)
{
	MATHyperStates Y(X.getNumRows(), X.getNumColumns());
	for (size_t i{ 0 }; i < Y.getNumRows(); i++)
		for (size_t j{ 0 }; j < Y.getNumColumns(); j++)
			Y[i][j] = asinh(X[i][j]);
	return Y;
}
MATHyperStates pNMPC::acosh(MATHyperStates X)
{
	MATHyperStates Y(X.getNumRows(), X.getNumColumns());
	for (size_t i{ 0 }; i < Y.getNumRows(); i++)
		for (size_t j{ 0 }; j < Y.getNumColumns(); j++)
			Y[i][j] = acosh(X[i][j]);
	return Y;
}
MATHyperStates pNMPC::atanh(MATHyperStates X)
{
	MATHyperStates Y(X.getNumRows(), X.getNumColumns());
	for (size_t i{ 0 }; i < Y.getNumRows(); i++)
		for (size_t j{ 0 }; j < Y.getNumColumns(); j++)
			Y[i][j] = atanh(X[i][j]);
	return Y;
}
MATHyperStates pNMPC::sign(MATHyperStates X)
{
	MATHyperStates Y(X.getNumRows(), X.getNumColumns());
	for (size_t i{ 0 }; i < Y.getNumRows(); i++)
		for (size_t j{ 0 }; j < Y.getNumColumns(); j++)
			Y[i][j] = sign(X[i][j]);
	return Y;
}

MATHyperStates pNMPC::maximum(MATHyperStates X, HyperStates& x)
{
	MATHyperStates Y(X.getNumRows(), X.getNumColumns());
	for (size_t i{ 0 }; i < Y.getNumRows(); i++)
		for (size_t j{ 0 }; j < Y.getNumColumns(); j++)
			Y[i][j] = maximum(X[i][j],x);
	return Y;
}

MATHyperStates pNMPC::maximum(HyperStates& x, MATHyperStates X)
{
	MATHyperStates Y(X.getNumRows(), X.getNumColumns());
	for (size_t i{ 0 }; i < Y.getNumRows(); i++)
		for (size_t j{ 0 }; j < Y.getNumColumns(); j++)
			Y[i][j] = maximum(x, X[i][j]);
	return Y;
}

MATHyperStates pNMPC::maximum(MATHyperStates X, const Real& x)
{
	MATHyperStates Y(X.getNumRows(), X.getNumColumns());
	for (size_t i{ 0 }; i < Y.getNumRows(); i++)
		for (size_t j{ 0 }; j < Y.getNumColumns(); j++)
			Y[i][j] = maximum(X[i][j], x);
	return Y;
}

MATHyperStates pNMPC::maximum(const Real& x, MATHyperStates X)
{
	MATHyperStates Y(X.getNumRows(), X.getNumColumns());
	for (size_t i{ 0 }; i < Y.getNumRows(); i++)
		for (size_t j{ 0 }; j < Y.getNumColumns(); j++)
			Y[i][j] = maximum(x, X[i][j]);
	return Y;
}

MATHyperStates pNMPC::minimum(MATHyperStates X, HyperStates& x)
{
	MATHyperStates Y(X.getNumRows(), X.getNumColumns());
	for (size_t i{ 0 }; i < Y.getNumRows(); i++)
		for (size_t j{ 0 }; j < Y.getNumColumns(); j++)
			Y[i][j] = minimum(X[i][j], x);
	return Y;
}

MATHyperStates pNMPC::minimum(HyperStates& x, MATHyperStates X)
{
	MATHyperStates Y(X.getNumRows(), X.getNumColumns());
	for (size_t i{ 0 }; i < Y.getNumRows(); i++)
		for (size_t j{ 0 }; j < Y.getNumColumns(); j++)
			Y[i][j] = minimum(x, X[i][j]);
	return Y;
}

MATHyperStates pNMPC::minimum(MATHyperStates X, const Real& x)
{
	MATHyperStates Y(X.getNumRows(), X.getNumColumns());
	for (size_t i{ 0 }; i < Y.getNumRows(); i++)
		for (size_t j{ 0 }; j < Y.getNumColumns(); j++)
			Y[i][j] = minimum(X[i][j], x);
	return Y;
}

MATHyperStates pNMPC::minimum(const Real& x, MATHyperStates X)
{
	MATHyperStates Y(X.getNumRows(), X.getNumColumns());
	for (size_t i{ 0 }; i < Y.getNumRows(); i++)
		for (size_t j{ 0 }; j < Y.getNumColumns(); j++)
			Y[i][j] = minimum(x, X[i][j]);
	return Y;
}

MATHyperStates pNMPC::transpose(EquationProcessor& EQ)
{
	return transpose(EQ.getMATHyperStates());
}