#include "pNMPC_headers.hpp"
#include "Database.hpp"
#include "States.hpp"
#include "Inputs.hpp"
#include "Params.hpp"

// Singleton
Database* Database::_instance = NULL;

Database* Database::getInstance()
{
	if (_instance == NULL)
		_instance = new Database{};
	return _instance;
}

// Hyperstates, states, inputs and parameters
std::vector<States*> Database::all_states{};
std::vector<Inputs*> Database::all_inputs{};
std::vector<Params*> Database::all_params{};
// string->Hyperstates map
std::map<std::string, HyperStates*> Database::str_hypermap{};

// Differential equations
std::vector<DiffEquation*> Database::DifferentialEquations{};
std::vector<std::string> Database::expressions_diff{};
std::vector<std::vector<std::string>>  Database::all_tokens_diff{};
std::vector<std::pair<HyperStates*, std::pair<Operation, std::pair<std::string, std::string>>>> Database::OpSet_diff{};

// Constraints equations
std::vector<ScalarConstraint*> Database::ScalarConstraintEquations{};
std::vector<std::string> Database::expressions_const{};
std::vector<std::vector<std::string>> Database::all_tokens_const{};
std::vector<std::pair<HyperStates*, std::pair<Operation, std::pair<std::string, std::string>>>> Database::OpSet_const{};

// Objective equations
std::vector<ScalarObjective*> Database::ScalarObjectiveEquations{};
std::vector<std::string> Database::expressions_obj{};
std::vector<std::vector<std::string>> Database::all_tokens_obj{};
std::vector<std::pair<HyperStates*, std::pair<Operation, std::pair<std::string, std::string>>>> Database::OpSet_obj{};

// Parameterization map equations
std::vector<ParameterizationMap*> Database::ParameterizationMapEquations{};
std::vector<std::string> Database::expressions_pme{};
std::vector<std::vector<std::string>> Database::all_tokens_pme{};
std::vector<std::pair<HyperStates*, std::pair<Operation, std::pair<std::string, std::string>>>> Database::OpSet_pme{};

// Get all states
std::vector<States*>& Database::accessAllStates() const
{
	return all_states;
}
// Get all inputs
std::vector<Inputs*>& Database::accessAllInputs() const
{
	return all_inputs;
}
// Get all parameters
std::vector<Params*>& Database::accessAllParams() const
{
	return all_params;
}
// Get hypermap
std::map<std::string, HyperStates*>& Database::accessAllHyperMaps() const
{
	return str_hypermap;
}
// Get Differential expressions, all_token, OpSet(Operation set)
std::vector<DiffEquation*>& Database::accessDifferentialEquations() const
{
	return DifferentialEquations;
}
// Get ScalarConstraint expressions, all_token, OpSet(Operation set)
std::vector<ScalarConstraint*>& Database::accessScalarConstraintEquations() const
{
	return ScalarConstraintEquations;
}
// Get ScalarObjective expressions, all_token, OpSet(Operation set)
std::vector<ScalarObjective*>& Database::accessScalarObjectiveEquations() const
{
	return ScalarObjectiveEquations;
}

std::vector<ParameterizationMap*>& Database::accessParameterizationMapEquations() const
{
	return ParameterizationMapEquations;
}

std::vector<std::string>& Database::accessExpressionsDiff() const
{
	return expressions_diff;
}
std::vector<std::vector<std::string>>& Database::accessTokensDiff() const
{
	return all_tokens_diff;
}
std::vector<std::pair<HyperStates*, std::pair<Operation, std::pair<std::string, std::string>>>>& Database::accessOpSetDiff() const
{
	return OpSet_diff;
}

// Get Constraint expressions, all_token, OpSet(Operation set)
std::vector<std::string>& Database::accessExpressionsConst() const
{
	return expressions_const;
}
std::vector<std::vector<std::string>>& Database::accessTokensConst() const
{
	return all_tokens_const;
}
std::vector<std::pair<HyperStates*, std::pair<Operation, std::pair<std::string, std::string>>>>& Database::accessOpSetConst() const
{
	return OpSet_const;
}

// Get Objective expressions, all_token, OpSet(Operation set)
std::vector<std::string>& Database::accessExpressionsObj() const
{
	return expressions_obj;
}
std::vector<std::vector<std::string>>& Database::accessTokensObj() const
{
	return all_tokens_obj;
}
std::vector<std::pair<HyperStates*, std::pair<Operation, std::pair<std::string, std::string>>>>& Database::accessOpSetObj() const
{
	return OpSet_obj;
}

// Get  ParameterizationMap expressions, all_token, OpSet(Operation set)
std::vector<std::string>& Database::accessExpressionsPME() const
{
	return expressions_pme;
}
std::vector<std::vector<std::string>>& Database::accessTokensPME() const
{
	return all_tokens_pme;
}
std::vector<std::pair<HyperStates*, std::pair<Operation, std::pair<std::string, std::string>>>>& Database::accessOpSetPME() const
{
	return OpSet_pme;
}

void Database::ObliterateDatabase()
{
	for (size_t i{ 0 }; i < OpSet_diff.size(); i++)
		delete OpSet_diff[i].first;
	for (size_t i{ 0 }; i < OpSet_const.size(); i++)
		delete OpSet_const[i].first;
	for (size_t i{ 0 }; i < OpSet_obj.size(); i++)
		delete OpSet_obj[i].first;
}

#if(DEBUG)
void Database::disp()
{
	// Display all expressions - Differential equations
	std::cout << "================= All expressions - Differential equations =================" << std::endl;
	for (auto& item : db->accessExpressionsDiff())
	{
		std::cout << " Expression: " << item << std::endl;
	}

	// Display all expressions - Constraints
	std::cout << "================= All expressions - Constraints =================" << std::endl;
	for (auto& item : db->accessExpressionsConst())
	{
		std::cout << " Expression: " << item << std::endl;
	}

	// Display all expressions - Objectives
	std::cout << "================= All expressions - Objectives =================" << std::endl;
	for (auto& item : db->accessExpressionsObj())
	{
		std::cout << " Expression: " << item << std::endl;
	}

	//Display all tokens - Differential equations
	std::cout << "================= All tokens - Differential equations =================" << std::endl;
	for (auto& item : db->accessTokensDiff())
	{
		for (auto& item2 : item)
			std::cout << "'" << item2 << "'" << " ";
		std::cout << std::endl;
	}

	//Display all tokens - Constraints
	std::cout << "================= All tokens - Constraints =================" << std::endl;
	for (auto& item : db->accessTokensConst())
	{
		for (auto& item2 : item)
			std::cout << "'" << item2 << "'" << " ";
		std::cout << std::endl;
	}

	//Display all tokens - Objectives
	std::cout << "================= All tokens - Objectives =================" << std::endl;
	for (auto& item : db->accessTokensObj())
	{
		for (auto& item2 : item)
			std::cout << "'" << item2 << "'" << " ";
		std::cout << std::endl;
	}


	// Order of operations - Differential equations
	std::cout << "================= Order of operations - Differential equations =================" << std::endl;
	for (int i{ 0 }; i < db->accessOpSetDiff().size(); i++)
	{
		//std::vector<std::pair<std::string, std::pair<Operation, std::pair<std::string, std::string>>>> Database::OpSet{};
		std::cout << db->accessOpSetDiff().at(i).first->getName() << "=" << db->accessOpSetDiff().at(i).second.second.first
			<< sOps[(db->accessOpSetDiff().at(i).second.first)] << db->accessOpSetDiff().at(i).second.second.second << std::endl;
		std::cout << "=====================" << std::endl;
	}

	// Order of operations - Constraints
	std::cout << "================= Order of operations - Constraints =================" << std::endl;
	for (int i{ 0 }; i < db->accessOpSetConst().size(); i++)
	{
		//std::vector<std::pair<std::string, std::pair<Operation, std::pair<std::string, std::string>>>> Database::OpSet{};
		std::cout << db->accessOpSetConst().at(i).first->getName() << "=" << db->accessOpSetConst().at(i).second.second.first
			<< sOps[(db->accessOpSetConst().at(i).second.first)] << db->accessOpSetConst().at(i).second.second.second << std::endl;
		std::cout << "=====================" << std::endl;
	}

	// Order of operations - Objectives
	std::cout << "================= Order of operations - Objectives =================" << std::endl;
	for (int i{ 0 }; i < db->accessOpSetObj().size(); i++)
	{
		//std::vector<std::pair<std::string, std::pair<Operation, std::pair<std::string, std::string>>>> Database::OpSet{};
		std::cout << db->accessOpSetObj().at(i).first->getName() << "=" << db->accessOpSetObj().at(i).second.second.first
			<< sOps[(db->accessOpSetObj().at(i).second.first)] << db->accessOpSetObj().at(i).second.second.second << std::endl;
		std::cout << "=====================" << std::endl;
	}

	// Display all states
	std::cout << "================= All states =================" << std::endl;
	for (auto& item : db->accessAllStates())
		std::cout << item->getName() << std::endl;

	// Display all inputs
	std::cout << "================= All inputs =================" << std::endl;
	for (auto& item : db->accessAllInputs())
		std::cout << item->getName() << std::endl;

	// Display all parameters
	std::cout << "================= All parameters =================" << std::endl;
	for (auto& item : db->accessAllParams())
		std::cout << item->getName() << std::endl;

	// Display all hyper states
	std::cout << "================= All hyper states =================" << std::endl;
	for (auto& item : db->accessAllHyperMaps())
		std::cout << item.first << std::endl;
}
#endif	