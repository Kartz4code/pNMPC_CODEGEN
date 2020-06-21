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
		this->_h_MAT = std::move(HS);
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
			this->_h_MAT = std::move(EQ.getMATHyperStates());
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