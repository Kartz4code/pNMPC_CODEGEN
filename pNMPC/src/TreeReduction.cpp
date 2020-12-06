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
#include "EquationProcessor.hpp"

void EquationProcessor::tree_reduction(const size_t& start)
{
	// Graph optimization has two loops 
	// 1. For identity and inverse cases
	// 2. For redunant expressions case;
	std::string rep, lhs, rhs, res_in; pNMPC::Operation ops; bool has_replaced{ false };
	for (size_t i = start; i < this->_OpSet.size(); i++)
	{
		// First name of the assignment 
		rep = this->_OpSet[i].first->getName();
		// lhs element
		lhs = this->_OpSet[i].second.second.first;
		// rhs element 
		rhs = this->_OpSet[i].second.second.second;
		// Operation between lhs and rhs
		ops = static_cast<pNMPC::Operation>(this->_OpSet[i].second.first);

		// When the operation is x*1
		if (is_number(rhs) && std::atof(rhs.c_str()) == 1.0 && ops == pNMPC::Operation::multiply)
		{
			has_replaced = false;
			for (size_t k{ i + 1 }; k < this->_OpSet.size(); k++)
			{
				if (this->_OpSet[k].second.second.first == rep)
				{
					this->_OpSet[k].second.second.first = lhs;
					has_replaced = true;
				}
				if (this->_OpSet[k].second.second.second == rep)
				{
					this->_OpSet[k].second.second.second = lhs;
					has_replaced = true;
				}
			}
			if (has_replaced)
			{
				delete this->_OpSet[i].first;
				this->_OpSet.erase(this->_OpSet.begin() + i);
				i = start-1;
			}
		}

		// Self negation operations
		if ((lhs == rhs) && ops == pNMPC::Operation::subract)
		{
			has_replaced = false;
			for (size_t k{ i + 1 }; k < this->_OpSet.size(); k++)
			{
				if (this->_OpSet[k].second.second.first == rep)
				{
					this->_OpSet[k].second.second.first = "0";
					has_replaced = true;
				}
				if (this->_OpSet[k].second.second.second == rep)
				{
					this->_OpSet[k].second.second.second = "0";
					has_replaced = true;
				}
			}
			if (has_replaced)
			{
				delete this->_OpSet[i].first;
				this->_OpSet.erase(this->_OpSet.begin() + i);
				i = start-1;
			}
		}

		/// Multiplication operator on 0 & 1 
		// When the operation is 1*x
		if (is_number(lhs) && std::atof(lhs.c_str()) == 1.0 && !is_number(rhs) && ops == pNMPC::Operation::multiply)
		{
			has_replaced = false;
			for (size_t k{ i + 1 }; k < this->_OpSet.size(); k++)
			{
				if (this->_OpSet[k].second.second.first == rep)
				{
					this->_OpSet[k].second.second.first = rhs;
					has_replaced = true;
				}
				if (this->_OpSet[k].second.second.second == rep)
				{
					this->_OpSet[k].second.second.second = rhs;
					has_replaced = true;
				}
			}
			if (has_replaced)
			{
				delete this->_OpSet[i].first;
				this->_OpSet.erase(this->_OpSet.begin() + i);
				i = start-1;
			}
		}

		// When the operation is 0*x or x*0
		if ((is_number(lhs) && std::atof(lhs.c_str()) == 0.0 || is_number(rhs) && std::atof(rhs.c_str()) == 0.0) && ops == Operation::multiply)
		{
			has_replaced = false;
			for (size_t k{ i + 1 }; k < this->_OpSet.size(); k++)
			{
				if (this->_OpSet[k].second.second.first == rep)
				{
					this->_OpSet[k].second.second.first = "0";
					has_replaced = true;
				}
				if (this->_OpSet[k].second.second.second == rep)
				{
					this->_OpSet[k].second.second.second = "0";
					has_replaced = true;
				}
			}
			if (has_replaced)
			{
				delete this->_OpSet[i].first;
				this->_OpSet.erase(this->_OpSet.begin() + i);
				i = start-1;
			}
		}

		/// Division operator on 0 & 1 
		// When the operation is 0/x
		if (is_number(lhs) && std::atof(lhs.c_str()) == 0.0 && ops == Operation::divide)
		{
			has_replaced = false;
			for (size_t k{ i + 1 }; k < this->_OpSet.size(); k++)
			{
				if (this->_OpSet[k].second.second.first == rep)
				{
					this->_OpSet[k].second.second.first = "0";
					has_replaced = true;
				}
				if (this->_OpSet[k].second.second.second == rep)
				{
					this->_OpSet[k].second.second.second = "0";
					has_replaced = true;
				}
			}
			if (has_replaced)
			{
				delete this->_OpSet[i].first;
				this->_OpSet.erase(this->_OpSet.begin() + i);
				i = start-1;
			}
		}

		// When the operation is x/1
		if (is_number(rhs) && std::atof(rhs.c_str()) == 1.0 && ops == Operation::divide)
		{
			has_replaced = false;
			for (size_t k{ i + 1 }; k < this->_OpSet.size(); k++)
			{
				if (this->_OpSet[k].second.second.first == rep)
				{
					this->_OpSet[k].second.second.first = lhs;
					has_replaced = true;
				}
				if (this->_OpSet[k].second.second.second == rep)
				{
					this->_OpSet[k].second.second.second = lhs;
					has_replaced = true;
				}
			}
			if (has_replaced)
			{
				delete this->_OpSet[i].first;
				this->_OpSet.erase(this->_OpSet.begin() + i);
				i = start-1;
			}
		}
		/// Addition operator on 0 
		// When the operation is 0+x
		if (is_number(lhs) && std::atof(lhs.c_str()) == 0.0 && ops == pNMPC::Operation::add)
		{
			has_replaced = false;
			for (size_t k{ i + 1 }; k < this->_OpSet.size(); k++)
			{
				if (this->_OpSet[k].second.second.first == rep)
				{
					this->_OpSet[k].second.second.first = rhs;
					has_replaced = true;
				}
				if (this->_OpSet[k].second.second.second == rep)
				{
					this->_OpSet[k].second.second.second = rhs;
					has_replaced = true;
				}
			}
			if (has_replaced)
			{
				delete this->_OpSet[i].first;
				this->_OpSet.erase(this->_OpSet.begin() + i);
				i = start-1;
			}
		}

		// When the operation is x+0
		if (is_number(rhs) && std::atof(rhs.c_str()) == 0.0 && ops == Operation::add)
		{
			has_replaced = false;
			for (size_t k{ i + 1 }; k < this->_OpSet.size(); k++)
			{
				if (this->_OpSet[k].second.second.first == rep)
				{
					this->_OpSet[k].second.second.first = lhs;
					has_replaced = true;
				}
				if (this->_OpSet[k].second.second.second == rep)
				{
					this->_OpSet[k].second.second.second = lhs;
					has_replaced = true;
				}
			}
			if (has_replaced)
			{
				delete this->_OpSet[i].first;
				this->_OpSet.erase(this->_OpSet.begin() + i);
				i = start-1;
			}
		}

		/// Subraction operator on 0 
		// When the operation is x-0
		if (is_number(rhs) && std::atof(rhs.c_str()) == 0.0 && !is_number(lhs) && ops == Operation::subract)
		{
			has_replaced = false;
			for (size_t k{ i + 1 }; k < this->_OpSet.size(); k++)
			{
				if (this->_OpSet[k].second.second.first == rep)
				{
					this->_OpSet[k].second.second.first = lhs;
					has_replaced = true;
				}
				if (this->_OpSet[k].second.second.second == rep)
				{
					this->_OpSet[k].second.second.second = lhs;
					has_replaced = true;
				}
			}
			if (has_replaced)
			{
				delete this->_OpSet[i].first;
				this->_OpSet.erase(this->_OpSet.begin() + i);
				i = start-1;
			}
		}
	}

	// Remove repeating elements
	for (size_t i = start; i < this->_OpSet.size(); i++)
	{
		// First name of the assignment 
		rep = this->_OpSet[i].first->getName();
		// lhs element
		lhs = this->_OpSet[i].second.second.first;
		// rhs element 
		rhs = this->_OpSet[i].second.second.second;
		// Operation between lhs and rhs
		ops = static_cast<pNMPC::Operation>(this->_OpSet[i].second.first);
		// Boolean trust
		bool trust{ false };
		for (size_t j{ i + 1 }; j < this->_OpSet.size(); j++)
		{
			if (lhs == this->_OpSet[j].second.second.first && rhs == this->_OpSet[j].second.second.second && ops == this->_OpSet[j].second.first)
			{
				trust = false;
				for (auto &item : this->_all_tokens)
					if (rep == item[0])
						trust = true;

				if (!trust)
				{
					res_in = this->_OpSet[j].first->getName();
					for (size_t k{ j + 1 }; k < this->_OpSet.size(); k++)
					{
						has_replaced = false;
						if (this->_OpSet[k].second.second.first == res_in)
						{
							this->_OpSet[k].second.second.first = rep;
							has_replaced = true;
						}
						if (this->_OpSet[k].second.second.second == res_in)
						{
							this->_OpSet[k].second.second.second = rep;
							has_replaced = true;
						}
					}
						has_replaced = true;
						delete this->_OpSet[j].first;
						this->_OpSet.erase(this->_OpSet.begin() + j);
						i = start-1;
				}
			}
		}
	}
}
