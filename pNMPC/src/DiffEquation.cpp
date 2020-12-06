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
#include "DiffEquation.hpp"
size_t DiffEquation::_countdiff{ 0 };
std::vector<std::unique_ptr<DiffEquation>> DiffEquation::_XDiff(0);

DiffEquation::DiffEquation() : EquationProcessor{db->accessExpressionsDiff(),
												 db->accessTokensDiff(),
												 db->accessOpSetDiff(),
												 _countdiff }
{}


DiffEquation::DiffEquation(const HyperStates& S) : EquationProcessor{ db->accessExpressionsDiff(),
																	  db->accessTokensDiff(),
																	  db->accessOpSetDiff(),
																	  _countdiff }
{
	if (Builder::getDiffBuild())
	{
		this->_h_store = S; db->accessExpressionsDiff().push_back(this->_h_store.getExpression());
		this->_h_MAT.resize(1, 1); this->_h_MAT[0][0] = this->_h_store;
		parse(); expression_tree(); tree_reduction(0); 
		db->accessDifferentialEquations().push_back(this); 
		++this->_countdiff;
	}
}

DiffEquation::DiffEquation(HyperStates&& S) : EquationProcessor{ db->accessExpressionsDiff(),
																 db->accessTokensDiff(),
																 db->accessOpSetDiff(),
																 _countdiff }
{
	if (Builder::getDiffBuild())
	{
		this->_h_store = std::move(S); db->accessExpressionsDiff().push_back(this->_h_store.getExpression());
		this->_h_MAT.resize(1, 1); this->_h_MAT[0][0] = this->_h_store;
		parse(); expression_tree(); tree_reduction(0); 
		db->accessDifferentialEquations().push_back(this); 
		++this->_countdiff;
	}
}

DiffEquation::DiffEquation(const MATHyperStates& HS) : EquationProcessor{ db->accessExpressionsDiff(),
																		  db->accessTokensDiff(),
																		  db->accessOpSetDiff(),
																		  _countdiff }
{
	if (Builder::getDiffBuild())
	{
		this->_h_MAT = HS;
		if (HS.getNumColumns() == 1)
		{
			for (size_t j{ 0 }; j < HS.getNumRows(); j++)
				this->_XDiff.push_back(std::make_unique<DiffEquation>(HS.getMatrixElement(j,0)));
			tree_reduction(0);
		}
		else
			std::cerr << "Matrix forms not supported";
	}

}

DiffEquation::DiffEquation(MATHyperStates&& HS) : EquationProcessor{ db->accessExpressionsDiff(),
																	 db->accessTokensDiff(),
																	 db->accessOpSetDiff(),
																	 _countdiff }
{
	if (Builder::getDiffBuild())
	{
		this->_h_MAT = std::move(HS);
		if (this->_h_MAT.getNumColumns() == 1 && this->_h_MAT.getNumRows() == 1)
			this->_h_store = this->_h_MAT[0][0];
		if (HS.getNumColumns() == 1)
		{
			for (size_t j{ 0 }; j < HS.getNumRows(); j++)
				this->_XDiff.push_back(std::make_unique<DiffEquation>(HS[j][0]));
			tree_reduction(0);
		}
		else 
			std::cerr << "Matrix forms not supported";
	}

}

DiffEquation::DiffEquation(const DiffEquation& DES) : EquationProcessor{ db->accessExpressionsDiff(),
																		 db->accessTokensDiff(),
																		 db->accessOpSetDiff(),
																		 _countdiff }
{
	if (Builder::getDiffBuild())
	{
		this->_h_store = DES._h_store; db->accessExpressionsDiff().push_back(this->_h_store.getExpression());
		parse(); expression_tree(); tree_reduction(0); db->accessDifferentialEquations().push_back(this); 
		++this->_countdiff;
	}
}

DiffEquation::DiffEquation(const EquationProcessor& EQ) : EquationProcessor{ db->accessExpressionsDiff(),
																			 db->accessTokensDiff(),
																			 db->accessOpSetDiff(),
																			 _countdiff }
{
	if (Builder::getDiffBuild())
	{
		if (EQ.getMATHyperStates().getNumRows() == 0 && EQ.getMATHyperStates().getNumColumns() == 0)
		{
			this->_h_store = EQ.getHyperStates(); db->accessExpressionsDiff().push_back(this->_h_store.getExpression());
			parse(); expression_tree(); tree_reduction(0); db->accessDifferentialEquations().push_back(this);
			++this->_countdiff;
		}
		else
		{
			this->_h_MAT = EQ.getMATHyperStates();
			if (this->_h_MAT.getNumColumns() == 1)
			{
				for (size_t j{ 0 }; j < this->_h_MAT.getNumRows(); j++)
					this->_XDiff.push_back(std::make_unique<DiffEquation>(this->_h_MAT[j][0]));
				tree_reduction(0);
			}
			else
				std::cerr << "Matrix forms not supported";
		}
	}
}

DiffEquation::DiffEquation(const Real&& val) : EquationProcessor{ db->accessExpressionsDiff(),
																  db->accessTokensDiff(),
																  db->accessOpSetDiff(),
																  _countdiff }
{
	if (Builder::getDiffBuild())
	{
		this->_h_store.setValue(val); this->_h_store.setExpression(std::to_string(val));
		this->_h_store.setAliasExpression(std::to_string(val)); db->accessExpressionsDiff().push_back(this->_h_store.getExpression());
		parse(); expression_tree(); tree_reduction(0);  db->accessDifferentialEquations().push_back(this); 
		++this->_countdiff;
	}
}

void DiffEquation::operator=(HyperStates&& S)
{
	if (Builder::getDiffBuild())
	{
		this->_h_store = S; db->accessExpressionsDiff().push_back(this->_h_store.getExpression());
		this->_h_MAT.resize(1, 1); this->_h_MAT[0][0] = this->_h_store;
		parse(); expression_tree(); tree_reduction(0);  db->accessDifferentialEquations().push_back(this); 
		++this->_countdiff;
	}
}


void DiffEquation::operator=(MATHyperStates&& HS)
{
	if (Builder::getDiffBuild())
	{
		this->_h_MAT = std::move(HS);
		if (this->_h_MAT.getNumColumns() == 1 && this->_h_MAT.getNumRows() == 1)
			this->_h_store = this->_h_MAT[0][0];
		if (HS.getNumColumns() == 1)
		{
			for (size_t j{ 0 }; j < HS.getNumRows(); j++)
				this->_XDiff.push_back(std::make_unique<DiffEquation>(HS[j][0]));
			tree_reduction(0);
		}
		else
			std::cerr << "Matrix forms not supported";
	}
}

void DiffEquation::operator=(Real&& val)
{
	if (Builder::getDiffBuild())
	{
		this->_h_store.setValue(val); this->_h_store.setExpression(std::to_string(val));
		this->_h_store.setAliasExpression(std::to_string(val)); db->accessExpressionsDiff().push_back(this->_h_store.getExpression());
		parse(); expression_tree(); tree_reduction(0); db->accessDifferentialEquations().push_back(this); 
		++this->_countdiff;
	}
}