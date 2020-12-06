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
#include "ParameterizationMap.hpp"
size_t ParameterizationMap::_countpme{ 0 };
std::vector<std::unique_ptr<ParameterizationMap>> ParameterizationMap::_XPME(0);

ParameterizationMap::ParameterizationMap() : EquationProcessor{ db->accessExpressionsPME(),
																db->accessTokensPME(),
																db->accessOpSetPME(),
																_countpme }
{}

ParameterizationMap::ParameterizationMap(const HyperStates& S) : EquationProcessor{ db->accessExpressionsPME(),
																					db->accessTokensPME(),
																					db->accessOpSetPME(),
																					_countpme }
{
	if (Builder::getPMBuild())
	{
		this->_h_store = S; db->accessExpressionsPME().push_back(this->_h_store.getExpression());
		this->_h_MAT.resize(1, 1); this->_h_MAT[0][0] = this->_h_store;
		parse(); expression_tree(); tree_reduction(0); db->accessParameterizationMapEquations().push_back(this); 
		++this->_countpme;
	}
}

ParameterizationMap::ParameterizationMap(HyperStates&& S) : EquationProcessor{ db->accessExpressionsPME(),
																			   db->accessTokensPME(),
																			   db->accessOpSetPME(),
																			   _countpme }
{
	if (Builder::getPMBuild())
	{
		this->_h_store = std::move(S);
		db->accessExpressionsPME().push_back(this->_h_store.getExpression());
		this->_h_MAT.resize(1, 1); this->_h_MAT[0][0] = this->_h_store;
		parse(); expression_tree(); tree_reduction(0); db->accessParameterizationMapEquations().push_back(this); 
		++this->_countpme;
	}
}

ParameterizationMap::ParameterizationMap(const MATHyperStates& HP) : EquationProcessor{ db->accessExpressionsPME(),
																						db->accessTokensPME(),
																						db->accessOpSetPME(),
																						_countpme }
{
	if (Builder::getPMBuild())
	{
		this->_h_MAT = HP;
		if (HP.getNumColumns() == 1)
		{
			for (size_t j{ 0 }; j < HP.getNumRows(); j++)
				this->_XPME.push_back(std::make_unique<ParameterizationMap>(HP.getMatrixElement(j,0)));
			tree_reduction(0);
		}
		else
			std::cerr << "Matrix forms not supported";
	}
}

ParameterizationMap::ParameterizationMap(MATHyperStates&& HP) : EquationProcessor{ db->accessExpressionsPME(),
																				   db->accessTokensPME(),
																				   db->accessOpSetPME(),
																			       _countpme }
{
	if (Builder::getPMBuild())
	{
		this->_h_MAT = std::move(HP);
		if (HP.getNumColumns() == 1)
		{
			for (size_t j{ 0 }; j < HP.getNumRows(); j++)
				this->_XPME.push_back(std::make_unique<ParameterizationMap>(HP[j][0]));
			tree_reduction(0);
		}
		else
			std::cerr << "Matrix forms not supported";
	}
}

ParameterizationMap::ParameterizationMap(const ParameterizationMap& PME) : EquationProcessor{ db->accessExpressionsPME(),
																							  db->accessTokensPME(),
																							  db->accessOpSetPME(),
																							  _countpme }
{
	if (Builder::getPMBuild())
	{
		this->_h_store = PME._h_store;
		db->accessExpressionsPME().push_back(this->_h_store.getExpression());
		parse(); expression_tree(); tree_reduction(0); db->accessParameterizationMapEquations().push_back(this);
		++this->_countpme;
	}
}

ParameterizationMap::ParameterizationMap(const EquationProcessor& EQ) : EquationProcessor{ db->accessExpressionsPME(),
																						   db->accessTokensPME(),
																						   db->accessOpSetPME(),
																						   _countpme }
{
	if (Builder::getPMBuild())
	{
		if (EQ.getMATHyperStates().getNumRows() == 0 && EQ.getMATHyperStates().getNumColumns() == 0)
		{
			this->_h_store = EQ.getHyperStates(); db->accessExpressionsPME().push_back(this->_h_store.getExpression());
			parse(); expression_tree(); tree_reduction(0); db->accessParameterizationMapEquations().push_back(this); 
			++this->_countpme;
		}
		else
		{
			this->_h_MAT = EQ.getMATHyperStates();
			if (this->_h_MAT.getNumColumns() == 1)
			{
				for (size_t j{ 0 }; j < this->_h_MAT.getNumRows(); j++)
					this->_XPME.push_back(std::make_unique<ParameterizationMap>(this->_h_MAT[j][0]));
				tree_reduction(0);
			}
			else
				std::cerr << "Matrix forms not supported";
		}
	}
}

ParameterizationMap::ParameterizationMap(const Real&& val) : EquationProcessor{ db->accessExpressionsPME(),
																				db->accessTokensPME(),
																				db->accessOpSetPME(),
																				_countpme }
{
	if (Builder::getPMBuild())
	{
		this->_h_store.setValue(val); this->_h_store.setExpression(std::to_string(val));
		this->_h_store.setAliasExpression(std::to_string(val));
		db->accessExpressionsPME().push_back(this->_h_store.getExpression());
		parse(); expression_tree(); tree_reduction(0); db->accessParameterizationMapEquations().push_back(this); 
		++this->_countpme;
	}
}

void ParameterizationMap::operator=(HyperStates&& S)
{
	if (Builder::getPMBuild())
	{
		this->_h_store = S; db->accessExpressionsPME().push_back(this->_h_store.getExpression());
		this->_h_MAT.resize(1, 1); this->_h_MAT[0][0] = this->_h_store;
		parse(); expression_tree(); tree_reduction(0); db->accessParameterizationMapEquations().push_back(this);
		++this->_countpme;
	}
}

void ParameterizationMap::operator=(MATHyperStates&& HP)
{
	if (Builder::getPMBuild())
	{
		this->_h_MAT = std::move(HP);
		if (HP.getNumColumns() == 1)
		{
			for (size_t j{ 0 }; j < HP.getNumRows(); j++)
				this->_XPME.push_back(std::make_unique<ParameterizationMap>(HP[j][0]));
			tree_reduction(0);
		}
		else
			std::cerr << "Matrix forms not supported";
	}
}

void ParameterizationMap::operator=(Real&& val)
{
	if (Builder::getPMBuild())
	{
		this->_h_store.setValue(val); this->_h_store.setExpression(std::to_string(val));
		this->_h_store.setAliasExpression(std::to_string(val));
		db->accessExpressionsPME().push_back(this->_h_store.getExpression());
		parse(); expression_tree(); tree_reduction(0); db->accessParameterizationMapEquations().push_back(this); 
		++this->_countpme;
	}
}