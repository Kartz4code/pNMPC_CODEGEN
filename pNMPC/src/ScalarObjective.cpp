#include "pNMPC_headers.hpp"
#include "ScalarObjective.hpp"


size_t ScalarObjective::_countobj{ 0 };
std::vector<std::unique_ptr<ScalarObjective>> ScalarObjective::_XObj(0);
size_t ScalarObjective::_keepObjSize_may{0};
size_t ScalarObjective::_keepObjSize_lag{0};

ScalarObjective::ScalarObjective() : EquationProcessor{ db->accessExpressionsObj(),
														db->accessTokensObj(),
														db->accessOpSetObj(),
														_countobj }
{
	if (Builder::getObjBuild())
	{
		if (Builder::getLagObjBuild())
			this->_ObjectiveType = OBJTYPE::LAGRANGIAN;
		if (Builder::getMayObjBuild())
			this->_ObjectiveType = OBJTYPE::MAYER;
	}
}

ScalarObjective::ScalarObjective(const HyperStates& S) : EquationProcessor{ db->accessExpressionsObj(),
																			db->accessTokensObj(),
																			db->accessOpSetObj(),
																			_countobj }
{
	if (Builder::getObjBuild())
	{
		if (Builder::getLagObjBuild())
			this->_ObjectiveType = OBJTYPE::LAGRANGIAN;
		if (Builder::getMayObjBuild())
			this->_ObjectiveType = OBJTYPE::MAYER;
		this->_h_store = S;  this->_h_MAT.resize(1, 1);
		this->_h_MAT[0][0] = this->_h_store;
		db->accessExpressionsObj().push_back(this->_h_store.getExpression());
		parse(); expression_tree();
		if (this->_ObjectiveType == OBJTYPE::LAGRANGIAN)
		{
			tree_reduction(_keepObjSize_lag);
			this->_keepObjSize_may = db->accessOpSetObj().size();
		}
		else if (this->_ObjectiveType == OBJTYPE::MAYER)
		{
			tree_reduction(_keepObjSize_may);
			this->_keepObjSize_lag = db->accessOpSetObj().size();
		}
		db->accessScalarObjectiveEquations().push_back(this); 
		++this->_countobj;
	}
}


ScalarObjective::ScalarObjective(HyperStates&& S) : EquationProcessor{ db->accessExpressionsObj(),
																	   db->accessTokensObj(),
																	   db->accessOpSetObj(),
																	   _countobj }
																							 
{
	if (Builder::getObjBuild())
	{
		if (Builder::getLagObjBuild())
			this->_ObjectiveType = OBJTYPE::LAGRANGIAN;
		if (Builder::getMayObjBuild())
			this->_ObjectiveType = OBJTYPE::MAYER;
		this->_h_store = std::move(S); this->_h_MAT.resize(1, 1); 
		this->_h_MAT[0][0] = this->_h_store;
		db->accessExpressionsObj().push_back(this->_h_store.getExpression());
		parse(); expression_tree();
		if (this->_ObjectiveType == OBJTYPE::LAGRANGIAN)
		{
			tree_reduction(_keepObjSize_lag);
			this->_keepObjSize_may = db->accessOpSetObj().size();
		}
		else if (this->_ObjectiveType == OBJTYPE::MAYER)
		{
			tree_reduction(_keepObjSize_may);
			this->_keepObjSize_lag = db->accessOpSetObj().size();
		}
		db->accessScalarObjectiveEquations().push_back(this); 
		++this->_countobj;
	}
}


ScalarObjective::ScalarObjective(const MATHyperStates& HSO) : EquationProcessor{ db->accessExpressionsObj(),
																				 db->accessTokensObj(),
																				 db->accessOpSetObj(),
																				 _countobj }
{
	if (Builder::getObjBuild())
	{
		if (Builder::getLagObjBuild())
			this->_ObjectiveType = OBJTYPE::LAGRANGIAN;
		if (Builder::getMayObjBuild())
			this->_ObjectiveType = OBJTYPE::MAYER;
		this->_h_MAT = std::move(HSO);
		if (HSO.getNumColumns() == 1)
		{
			for (size_t j{ 0 }; j < HSO.getNumRows(); j++)
				this->_XObj.push_back(std::make_unique<ScalarObjective>(HSO.getMatrixElement(j,0)));
			if (this->_ObjectiveType == OBJTYPE::LAGRANGIAN)
			{
				tree_reduction(_keepObjSize_lag);
				this->_keepObjSize_may = db->accessOpSetObj().size();
			}
			else if (this->_ObjectiveType == OBJTYPE::MAYER)
			{
				tree_reduction(_keepObjSize_may);
				this->_keepObjSize_lag = db->accessOpSetObj().size();
			}
		}
		else
			std::cerr << "Matrix forms not supported";
	}
}

ScalarObjective::ScalarObjective(const ScalarObjective& SOS) : EquationProcessor{ db->accessExpressionsObj(),
																				  db->accessTokensObj(),
																				  db->accessOpSetObj(),
																				  _countobj }
{
	if (Builder::getObjBuild())
	{
		if (SOS.getMATHyperStates().getNumRows() == 0 && SOS.getMATHyperStates().getNumColumns() == 0)
		{
			if (Builder::getLagObjBuild())
				this->_ObjectiveType = OBJTYPE::LAGRANGIAN;
			if (Builder::getMayObjBuild())
				this->_ObjectiveType = OBJTYPE::MAYER;
			this->_h_store = SOS.getHyperStates(); db->accessExpressionsObj().push_back(this->_h_store.getExpression());
			parse(); expression_tree();
			if (this->_ObjectiveType == OBJTYPE::LAGRANGIAN)
			{
				tree_reduction(_keepObjSize_lag);
				this->_keepObjSize_may = db->accessOpSetObj().size();
			}
			else if (this->_ObjectiveType == OBJTYPE::MAYER)
			{
				tree_reduction(_keepObjSize_may);
				this->_keepObjSize_lag = db->accessOpSetObj().size();
			}
			db->accessScalarObjectiveEquations().push_back(this);
			++this->_countobj;
		}
		else {
			this->_h_MAT = std::move(SOS.getMATHyperStates());
			if (this->_h_MAT.getNumColumns() == 1)
			{
				if (Builder::getLagObjBuild())
					this->_ObjectiveType = OBJTYPE::LAGRANGIAN;
				if (Builder::getMayObjBuild())
					this->_ObjectiveType = OBJTYPE::MAYER;
				for (size_t j{ 0 }; j < this->_h_MAT.getNumRows(); j++)
					this->_XObj.push_back(std::make_unique<ScalarObjective>(this->_h_MAT[j][0]));
				if (this->_ObjectiveType == OBJTYPE::LAGRANGIAN)
				{
					tree_reduction(_keepObjSize_lag);
					this->_keepObjSize_may = db->accessOpSetObj().size();
				}
				else if (this->_ObjectiveType == OBJTYPE::MAYER)
				{
					tree_reduction(_keepObjSize_may);
					this->_keepObjSize_lag = db->accessOpSetObj().size();
				}
			}
			else
				std::cerr << "Matrix forms not supported";
		}
	}
}


ScalarObjective::ScalarObjective(MATHyperStates&& HSO) : EquationProcessor{ db->accessExpressionsObj(),
																			db->accessTokensObj(),
																			db->accessOpSetObj(),							
																			_countobj }
{
	if (Builder::getObjBuild())
	{
		if (Builder::getLagObjBuild())
			this->_ObjectiveType = OBJTYPE::LAGRANGIAN;
		if (Builder::getMayObjBuild())
			this->_ObjectiveType = OBJTYPE::MAYER;
		this->_h_MAT = std::move(HSO);
		if (this->_h_MAT.getNumColumns() == 1 && this->_h_MAT.getNumRows() == 1)
			this->_h_store = this->_h_MAT[0][0];
		if (HSO.getNumColumns() == 1)
		{
		 	for (size_t j{ 0 }; j < HSO.getNumRows(); j++)
				this->_XObj.push_back(std::make_unique<ScalarObjective>(HSO[j][0]));
			if (this->_ObjectiveType == OBJTYPE::LAGRANGIAN)
			{
				tree_reduction(_keepObjSize_lag);
				this->_keepObjSize_may = db->accessOpSetObj().size();
			}
			else if (this->_ObjectiveType == OBJTYPE::MAYER)
			{
				tree_reduction(_keepObjSize_may);
				this->_keepObjSize_lag = db->accessOpSetObj().size();
			}
		}
		else
			std::cerr << "Matrix forms not supported";
	}
}

ScalarObjective::ScalarObjective(const Real&& val) : EquationProcessor{ db->accessExpressionsObj(),
																		db->accessTokensObj(),
																		db->accessOpSetObj(),
																		_countobj}

{
	if (Builder::getObjBuild())
	{
		if (Builder::getLagObjBuild())
			this->_ObjectiveType = OBJTYPE::LAGRANGIAN;
		if (Builder::getMayObjBuild())
			this->_ObjectiveType = OBJTYPE::MAYER;
		db->accessExpressionsObj().push_back(std::to_string(val));
		parse(); expression_tree();
		if (this->_ObjectiveType == OBJTYPE::LAGRANGIAN)
		{
			tree_reduction(_keepObjSize_lag);
			this->_keepObjSize_may = db->accessOpSetObj().size();
		}
		else if (this->_ObjectiveType == OBJTYPE::MAYER)
		{
			tree_reduction(_keepObjSize_may);
			this->_keepObjSize_lag = db->accessOpSetObj().size();
		}
		db->accessScalarObjectiveEquations().push_back(this);
		++this->_countobj;
	}
}

ScalarObjective::ScalarObjective(const EquationProcessor& EQ) : EquationProcessor{ db->accessExpressionsObj(),
																			       db->accessTokensObj(),
																			       db->accessOpSetObj(),
																		           _countobj }
{
	if (Builder::getObjBuild())
	{
		if (EQ.getMATHyperStates().getNumRows() == 0 && EQ.getMATHyperStates().getNumColumns() == 0)
		{
			if (Builder::getLagObjBuild())
				this->_ObjectiveType = OBJTYPE::LAGRANGIAN;
			if (Builder::getMayObjBuild())
				this->_ObjectiveType = OBJTYPE::MAYER;
			this->_h_store = EQ.getHyperStates(); db->accessExpressionsObj().push_back(this->_h_store.getExpression());
			parse(); expression_tree();
			if (this->_ObjectiveType == OBJTYPE::LAGRANGIAN)
			{
				tree_reduction(_keepObjSize_lag);
				this->_keepObjSize_may = db->accessOpSetObj().size();
			}
			else if (this->_ObjectiveType == OBJTYPE::MAYER)
			{
				tree_reduction(_keepObjSize_may);
				this->_keepObjSize_lag = db->accessOpSetObj().size();
			}
			db->accessScalarObjectiveEquations().push_back(this); 
			++this->_countobj;
		}
		else {
			this->_h_MAT = std::move(EQ.getMATHyperStates());
			if (this->_h_MAT.getNumColumns() == 1)
			{
				if (Builder::getLagObjBuild())
					this->_ObjectiveType = OBJTYPE::LAGRANGIAN;
				if (Builder::getMayObjBuild())
					this->_ObjectiveType = OBJTYPE::MAYER;
				for (size_t j{ 0 }; j < this->_h_MAT.getNumRows(); j++)
					this->_XObj.push_back(std::make_unique<ScalarObjective>(this->_h_MAT[j][0]));
				if (this->_ObjectiveType == OBJTYPE::LAGRANGIAN)
				{
					tree_reduction(_keepObjSize_lag);
					this->_keepObjSize_may = db->accessOpSetObj().size();
				}
				else if (this->_ObjectiveType == OBJTYPE::MAYER)
				{
					tree_reduction(_keepObjSize_may);
					this->_keepObjSize_lag = db->accessOpSetObj().size();
				}
			}
			else
				std::cerr << "Matrix forms not supported";
		}
	}
}

void ScalarObjective::operator=(HyperStates&& S)
{
	if (Builder::getObjBuild())
	{
		if (Builder::getLagObjBuild())
			this->_ObjectiveType = OBJTYPE::LAGRANGIAN;
		if (Builder::getMayObjBuild())
			this->_ObjectiveType = OBJTYPE::MAYER;
		this->_h_store = S;  this->_h_MAT.resize(1, 1);
		this->_h_MAT[0][0] = this->_h_store;
		db->accessExpressionsObj().push_back(this->_h_store.getExpression());
		parse(); expression_tree();
		if (this->_ObjectiveType == OBJTYPE::LAGRANGIAN)
		{
			tree_reduction(_keepObjSize_lag);
			this->_keepObjSize_may = db->accessOpSetObj().size();
		}
		else if (this->_ObjectiveType == OBJTYPE::MAYER)
		{
			tree_reduction(_keepObjSize_may);
			this->_keepObjSize_lag = db->accessOpSetObj().size();
		}
		db->accessScalarObjectiveEquations().push_back(this); 
		++this->_countobj;
	}
}

void ScalarObjective::operator=(MATHyperStates&& HSO)
{
	if (Builder::getObjBuild())
	{
		if (Builder::getLagObjBuild())
			this->_ObjectiveType = OBJTYPE::LAGRANGIAN;
		if (Builder::getMayObjBuild())
			this->_ObjectiveType = OBJTYPE::MAYER;
		this->_h_MAT = std::move(HSO);
		if (this->_h_MAT.getNumColumns() == 1 && this->_h_MAT.getNumRows() == 1)
			this->_h_store = this->_h_MAT[0][0];
		if (HSO.getNumColumns() == 1)
		{
			for (size_t j{ 0 }; j < HSO.getNumRows(); j++)
				this->_XObj.push_back(std::make_unique<ScalarObjective>(HSO[j][0]));
			if (this->_ObjectiveType == OBJTYPE::LAGRANGIAN)
			{
				tree_reduction(_keepObjSize_lag);
				this->_keepObjSize_may = db->accessOpSetObj().size();
			}
			else if (this->_ObjectiveType == OBJTYPE::MAYER)
			{
				tree_reduction(_keepObjSize_may);
				this->_keepObjSize_lag = db->accessOpSetObj().size();
			}
		}
		else
			std::cerr << "Matrix forms not supported";
	}
}

void ScalarObjective::operator=(Real&& val)
{
	if (Builder::getObjBuild())
	{
		if (Builder::getLagObjBuild())
			this->_ObjectiveType = OBJTYPE::LAGRANGIAN;
		if (Builder::getMayObjBuild())
			this->_ObjectiveType = OBJTYPE::MAYER;
		db->accessExpressionsObj().push_back(std::to_string(val));
		parse(); expression_tree();
		if (this->_ObjectiveType == OBJTYPE::LAGRANGIAN)
		{
			tree_reduction(_keepObjSize_lag);
			this->_keepObjSize_may = db->accessOpSetObj().size();
		}
		else if (this->_ObjectiveType == OBJTYPE::MAYER)
		{
			tree_reduction(_keepObjSize_may);
			this->_keepObjSize_lag = db->accessOpSetObj().size();
		}
		db->accessScalarObjectiveEquations().push_back(this); 
		++this->_countobj;
	}
}

const OBJTYPE& ScalarObjective::getObjectiveType() const
{
	return this->_ObjectiveType;
}