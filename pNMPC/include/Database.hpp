#pragma once
#include "pNMPC_headers.hpp"
using namespace pNMPC;

class Element;
class HyperStates;
class States;
class Inputs;
class Params;
class ScalarConstraint;
class ScalarObjective;
class DiffEquation;
class ScalarConstraint;
class ScalarObjective;
class ParameterizationMap;

class Database {
	private:
		// All hyperstates, states, inputs and params
		static std::vector<States*> all_states;
		static std::vector<Inputs*> all_inputs;
		static std::vector<Params*> all_params;
		// string->Hyperstates map
		static std::map<std::string, HyperStates*> str_hypermap;

		// Differential equation
		static std::vector<DiffEquation*> DifferentialEquations;
		static std::vector<std::string> expressions_diff;
		static std::vector<std::vector<std::string>> all_tokens_diff;
		static std::vector<std::pair<HyperStates*, std::pair<Operation, std::pair<std::string, std::string>>>> OpSet_diff;

		// Constraints equations
		static std::vector<ScalarConstraint*> ScalarConstraintEquations;
		static std::vector<std::string> expressions_const;
		static std::vector<std::vector<std::string>> all_tokens_const;
		static std::vector<std::pair<HyperStates*, std::pair<Operation, std::pair<std::string, std::string>>>> OpSet_const;

		// Objective equations
		static std::vector<ScalarObjective*> ScalarObjectiveEquations;
		static std::vector<std::string> expressions_obj;
		static std::vector<std::vector<std::string>> all_tokens_obj;
		static std::vector<std::pair<HyperStates*, std::pair<Operation, std::pair<std::string, std::string>>>> OpSet_obj;

		// Parameterization map equations
		static std::vector<ParameterizationMap*> ParameterizationMapEquations;
		static std::vector<std::string> expressions_pme;
		static std::vector<std::vector<std::string>> all_tokens_pme;
		static std::vector<std::pair<HyperStates*, std::pair<Operation, std::pair<std::string, std::string>>>> OpSet_pme;

		static Database* _instance;

	public:
		Database() = default;
		static Database* getInstance();
		~Database()
		{
			delete this->_instance;
		}

		// Obliterate Database
		static void ObliterateDatabase();

		// Access hyperstates, states, inputs, params database
		std::vector<States*>& accessAllStates() const;
		std::vector<Inputs*>& accessAllInputs() const;
		std::vector<Params*>& accessAllParams() const;

		// Access hypermap
		std::map<std::string, HyperStates*>& accessAllHyperMaps() const;

		// Access Differential expressions, all_token, OpSet(Operation set)
		std::vector<DiffEquation*>& accessDifferentialEquations() const;
		std::vector<std::string>& accessExpressionsDiff() const;
		std::vector<std::vector<std::string>>& accessTokensDiff() const;
		std::vector<std::pair<HyperStates*, std::pair<Operation, std::pair<std::string, std::string>>>>& accessOpSetDiff() const;

		// Access Constraint expressions, all_token, OpSet(Operation set)
		std::vector<ScalarConstraint*>& accessScalarConstraintEquations() const;
		std::vector<std::string>& accessExpressionsConst() const;
		std::vector<std::vector<std::string>>& accessTokensConst() const;
		std::vector<std::pair<HyperStates*, std::pair<Operation, std::pair<std::string, std::string>>>>& accessOpSetConst() const;

		// Access Objective expressions, all_token, OpSet(Operation set)
		std::vector<ScalarObjective*>& accessScalarObjectiveEquations() const;
		std::vector<std::string>& accessExpressionsObj() const;
		std::vector<std::vector<std::string>>& accessTokensObj() const;
		std::vector<std::pair<HyperStates*, std::pair<Operation, std::pair<std::string, std::string>>>>& accessOpSetObj() const;

		// Access ParameterizationMap expressions, all_token, OpSet(Operation set)
		std::vector<ParameterizationMap*>& accessParameterizationMapEquations() const;
		std::vector<std::string>& accessExpressionsPME() const;
		std::vector<std::vector<std::string>>& accessTokensPME() const;
		std::vector<std::pair<HyperStates*, std::pair<Operation, std::pair<std::string, std::string>>>>& accessOpSetPME() const;

		// Display Database
		#if(DEBUG)
			void disp();
		#endif	
};