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

void EquationProcessor::expression_tree()
{
	std::pair<Operation, std::pair<std::string, std::string>> pair_in;
	std::pair<HyperStates*, std::pair<Operation, std::pair<std::string, std::string>>> pair_out;
	bool skipper{ false }; HyperStates* s_temp;
	{
		// If the RHS is a number, don't enter this loop
		if (this->_all_tokens[_count].size() != 1)
		{
			// Outmost loop is for the number of brackets
			for (size_t j{ 0 }; j < this->_all_tokens[_count].size(); j++)
			{
				if (this->_all_tokens[this->_count].size() < 2)
					break;
				if (this->_all_tokens[this->_count][j] == Paren[ParenOps::close])
				{
					for (size_t ops{ 0 }; ops < sizeof(sOps) / sizeof(std::string); ops++)
					{
						if (this->_all_tokens[this->_count][j - 2] == sOps[ops])
						{
							s_temp = new HyperStates(nullptr);
							pair_in.first = static_cast<Operation>(ops);
							pair_in.second = std::make_pair(this->_all_tokens[_count][j - 3], this->_all_tokens[_count][j - 1]);
							pair_out = std::make_pair(s_temp, pair_in);
							this->_OpSet.push_back(pair_out);
							this->_all_tokens[this->_count].erase(this->_all_tokens[this->_count].begin() + j - 4, this->_all_tokens[this->_count].begin() + j + 1);
							if (this->_all_tokens[this->_count].size() == 4)
							{
								for (size_t ops{ 0 }; ops < sizeof(sOps) / sizeof(std::string); ops++)
									if (this->_all_tokens[_count][1] == sOps[ops])
										skipper = true;
								if (!skipper)
									this->_all_tokens[this->_count].insert(this->_all_tokens[this->_count].end() - 1, s_temp->getName());
								else
									this->_all_tokens[_count].insert(this->_all_tokens[this->_count].begin() + 1, s_temp->getName());
							}
							else if (this->_all_tokens[_count].size() == 0)
								this->_all_tokens[_count].push_back(s_temp->getName());
							else
								this->_all_tokens[_count].insert(this->_all_tokens[this->_count].begin() + (j - 4), s_temp->getName());
							j = 0;
							break;
						}
					}
				}
			}
		}
		else
		{
			s_temp = new HyperStates(nullptr);
			pair_in.first = static_cast<Operation>(Operation::multiply);
			pair_in.second = std::make_pair(this->_all_tokens[this->_count][0], "1");
			pair_out = std::make_pair(s_temp, pair_in);
			this->_OpSet.push_back(pair_out);
			this->_all_tokens[this->_count][0] = s_temp->getName();
		}
	}
}