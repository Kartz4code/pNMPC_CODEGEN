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

void EquationProcessor::parse()
{
	size_t j{ 0 }; std::vector<std::string> tokens;
	std::ostringstream oss; std::string item;
	{
		item = this->_expressions[_count];
		{
			for (size_t i{ 0 };i < item.size();i++)
			{
				switch (item[i]) {
				case 'x':
				{
					oss << item[i];
					j = i + 1;
					while (isdigit(item[j]))
						oss << item[j++];
					tokens.push_back(oss.str());
					oss.str("");
					i = j - 1;
					break;
				}
				case 'u':
				{
					oss << item[i];
					j = i + 1;
					while (isdigit(item[j]))
						oss << item[j++];
					tokens.push_back(oss.str());
					oss.str("");
					i = j - 1;
					break;
				}
				case 'p':
				{
					oss << item[i];
					j = i + 1;
					while (isdigit(item[j]))
						oss << item[j++];
					tokens.push_back(oss.str());
					oss.str("");
					i = j - 1;
					break;
				}
				case '+':
				{
					tokens.push_back(sOps[Operation::add]);
					break;
				}

				case '*':
				{
					tokens.push_back(sOps[Operation::multiply]);
					break;
				}
				case '/':
				{
					tokens.push_back(sOps[Operation::divide]);
					break;
				}
				case '^':
				{
					tokens.push_back(sOps[Operation::power]);
					break;
				}
				case 'o':
				{
					tokens.push_back(sOps[Operation::operate]);
					break;
				}

				case '(':
				{
					tokens.push_back(Paren[ParenOps::open]);
					break;
				}
				case ')':
				{
					tokens.push_back(Paren[ParenOps::close]);
					break;
				}

				case ' ':
				{
					break;
				}

				case '0': case '1': case '2': case '3': case '4':
				case '5': case '6': case '7': case '8': case '9':
					oss << item[i];
					j = i + 1;
					while (isdigit(item[j]) || item[j] == '.')
						oss << item[j++];
					tokens.push_back(oss.str());
					oss.str("");
					i = j - 1;
					break;

				case '-':
				{
					switch (item[i + 1]) {
					case '0': case '1': case '2': case '3': case '4':
					case '5': case '6': case '7': case '8': case '9':
						oss << item[i];
						j = i + 1;
						while (isdigit(item[j]) || item[j] == '.')
							oss << item[j++];
						tokens.push_back(oss.str());
						oss.str("");
						i = j - 1;
						break;
					default:
						tokens.push_back(sOps[Operation::subract]);
						break;
					}
					break;
				}


				case '<':
				{
					if (item[i + 1] == '=')
					{
						tokens.push_back(sOps[Operation::leq]);
						i++;
					}
					else if (item[i + 1] == '<')
					{
						tokens.push_back(sOps[Operation::min]);
						i++;
					}
					else
						tokens.push_back(sOps[Operation::less]);
					break;
				}

				case '>':
				{
					if (item[i + 1] == '=')
					{
						tokens.push_back(sOps[Operation::geq]);
						i++;
					}
					else if (item[i + 1] == '>')
					{
						tokens.push_back(sOps[Operation::max]);
						i++;
					}
					else
						tokens.push_back(sOps[Operation::greater]);
					break;
				}

				default:
					oss << item[i];
					j = i + 1;
					while ((item[j] >= 97) & (item[j] <= 122))
						oss << item[j++];
					tokens.push_back(oss.str());
					oss.str("");
					i = j - 1;
					break;
				}
			}
		}
		this->_all_tokens.push_back(tokens);
		tokens.clear();
	}
}