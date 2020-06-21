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
#include "PNMPCGEN.hpp"

// Singleton
PNMPCGEN* PNMPCGEN::_instance = nullptr;

// pNMPC singleton object
PNMPCGEN* PNMPCGEN::getSton()
{
	if (_instance == nullptr)
		_instance = new PNMPCGEN{};
	return _instance;
}

void PNMPCGEN::termConstExist()
{
	for (size_t i{ 0 }; i < db->accessTokensConst().size(); i++)
	{
		if (db->accessScalarConstraintEquations()[i]->getConstraintType() == CONSTTYPE::TERMINAL)
			this->_termExist = true;
		if (db->accessScalarConstraintEquations()[i]->getConstraintType() == CONSTTYPE::REGULAR)
			this->_regularExist = true;
	}
}

void PNMPCGEN::lagMayBool()
{
	for (size_t i{ 0 }; i < this->_NO; i++)
	{
		if (db->accessScalarObjectiveEquations()[i]->getObjectiveType() == OBJTYPE::LAGRANGIAN)
			this->_lagBool = true;
		else if (db->accessScalarObjectiveEquations()[i]->getObjectiveType() == OBJTYPE::MAYER)
			this->_mayBool = true;
	}
}

void PNMPCGEN::setArrayVector() {
	std::string temp{ "N" + db->accessAllInputs()[0]->getName() };
	this->_arrSizes.push_back(temp);
	for (size_t k{ 1 }; k < this->_nu; k++)
	{
		temp = temp + std::string{ "+N" + db->accessAllInputs()[k]->getName() };
		this->_arrSizes.push_back(temp);
	}
}

const size_t PNMPCGEN::getRegularConstraintSize() const
{
	// Count the number of constraints
	size_t counter{ 0 }; size_t j{ 0 };
	for (size_t i{ 0 }; i < db->accessOpSetConst().size(); i++)
	{
		if (db->accessScalarConstraintEquations()[j]->getConstraintType() == CONSTTYPE::REGULAR)
			if (sOps[(db->accessOpSetConst().at(i).second.first)] == sOps[Operation::leq] || sOps[(db->accessOpSetConst().at(i).second.first)] == sOps[Operation::geq])
				counter++;
		if (db->accessTokensConst()[j][0] == db->accessOpSetConst()[i].first->getName())
			j++;
	}
	return counter;
}

const size_t PNMPCGEN::getTerminalConstraintSize() const
{
	// Count the number of constraints
	size_t counter{ 0 }; size_t j{ 0 };
	for (size_t i{ 0 }; i < db->accessOpSetConst().size(); i++)
	{
		if (db->accessScalarConstraintEquations()[j]->getConstraintType() == CONSTTYPE::TERMINAL)
			if (sOps[(db->accessOpSetConst().at(i).second.first)] == sOps[Operation::leq] || sOps[(db->accessOpSetConst().at(i).second.first)] == sOps[Operation::geq])
				counter++;
		if (db->accessTokensConst()[j][0] == db->accessOpSetConst()[i].first->getName())
			j++;
	}
	return counter;
}

PNMPCGEN::PNMPCGEN()
{
	lagMayBool(); 
	termConstExist();
	setArrayVector();
}

void PNMPCGEN::setInitialTime(const Real& init_time)
{
	this->_init_time = init_time;
}
void PNMPCGEN::setFinalTime(const Real& final_time)
{
	this->_final_time = final_time;
}
void PNMPCGEN::setStepSize(const Real& step_size)
{
	this->_step_size = step_size;
}

SQPSolverCodeGen* PNMPCGEN::getSolver()
{
	return &this->_SCG;
}
void PNMPCGEN::setConstForm(const CONST_FORM& CN)
{
	this->__CN = CN;
}
void PNMPCGEN::setIntegrator(const INTEGRATOR& itype)
{
	this->_itype = itype;
}
void PNMPCGEN::setParameterPredict(const bool& ParamPredict)
{
	this->_ParamPredict = ParamPredict;
}

void PNMPCGEN::setThreadsPerBlock(const size_t& TPB)
{
	this->_TPB = TPB;
}

void PNMPCGEN::setNeedObjective(const bool& sno)
{
	this->_needObjective = sno;
}


// C - code generation helper functions
void PNMPCGEN::StateControlParamdecl(const std::stringstream& oss)
{
	// State declaration
	if (this->_nx)
	{
		CCODE_STREAM(COMMENT("States declaration"));
		for (size_t i{ 0 }; i < this->_nx; i++)
			CCODE_STREAM_T(TYPE_C << db->accessAllStates()[i]->getName() << EQUALS << "X[" << i << "]");
	}

	__BREAK_C__
		// Input and Input HyperStates declaration
		if (this->_nu)
		{
			CCODE_STREAM(COMMENT("Inputs declaration"));
			for (size_t i{ 0 }; i < this->_nu; i++)
				CCODE_STREAM_T(TYPE_C << db->accessAllInputs()[i]->getName() << EQUALS << "U[" << i << "]");
		}

	__BREAK_C__
		// Parameter declaration
		if (this->_np)
		{
			CCODE_STREAM(COMMENT("Parameters declaration"));
			for (size_t i{ 0 }; i < this->_np; i++)
				CCODE_STREAM_T(TYPE_C << db->accessAllParams()[i]->getName() << EQUALS << "P[" << i << "]");
		}
}

void PNMPCGEN::InterfaceVariables(const std::stringstream& oss)
{
	// Optimization settings
	CCODE_STREAM(COMMENT("Optimization settings"));
	CCODE_STREAM_T("optimset_SQP opt");

	// Computation time
	CCODE_STREAM(COMMENT("Computation time"));
	CCODE_STREAM_T("clock_t start, end");

	// Cost function parameters
	CCODE_STREAM(COMMENT("Cost function parameters"));
	CCODE_STREAM_T("ParaStructC paramsSQP");

	// Tempory actual input array
	CCODE_STREAM(COMMENT("Tempory actual input array"));
	CCODE_STREAM_T(VAR_PTR(U_ACT_TEMP));
	CCODE_STREAM_T(VAR_ARRAY(U_TEMP, nu));

	if (this->_ParamPredict)
	{
		// Parameter vector P at current time 0
		CCODE_STREAM(COMMENT("Parameter vector P at current time 0"));
		CCODE_STREAM_T(VAR_ARRAY(P, np));
	}
}

void PNMPCGEN::OptimizationParameters(const std::stringstream& oss)
{
	// Set alpha for hyperparameters
	CCODE_STREAM(COMMENT("Set alpha for hyperparameters"));
	CCODE_STREAM(FOR_UNSIGNED_FORWARD(i, 0, NMAX));
	#if(!ENABLE_CUDA)
		CCODE_STREAM_T("opt.alpha[i]" << EQUALS << this->_SCG.getAlpha());
	#elif(ENABLE_CUDA)
			CCODE_STREAM_T("opt.alpha[i]" << EQUALS << std::to_string(this->_SCG.getAlpha()) << "f");
	#endif

	// Set number of iterations
	CCODE_STREAM(COMMENT("Set number of iterations"));
	CCODE_STREAM_T("opt.Niter" << EQUALS << this->_SCG.getNiter());

	// Set betaminus and betaplus
	CCODE_STREAM(COMMENT("Set betaminus and betaplus"));
	#if(!ENABLE_CUDA)
		CCODE_STREAM_T("opt.betaMinus" << EQUALS << this->_SCG.getBetaMinus());
	#elif(ENABLE_CUDA)
		CCODE_STREAM_T("opt.betaMinus" << EQUALS << std::to_string(this->_SCG.getBetaMinus()) << "f");
	#endif

	#if(!ENABLE_CUDA)
		CCODE_STREAM_T("opt.betaPlus" << EQUALS << this->_SCG.getBetaPlus());
	#elif(ENABLE_CUDA)
		CCODE_STREAM_T("opt.betaPlus" << EQUALS << std::to_string(this->_SCG.getBetaPlus()) << "f");
	#endif

	// Set interval min/max
	CCODE_STREAM(COMMENT("Set interval min/max"));
	for (size_t i{ 0 }; i < this->_nu; i++)
	{
		if (i == 0)
		{
			CCODE_STREAM("for(uint16_t i=0; i<" << this->_arrSizes[i] << ";i++)");
			__BODY_BEGIN__
				CCODE_STREAM_T("opt.I_init.Imax[i]" << EQUALS << db->accessAllInputs()[i]->getUpperBound());
			CCODE_STREAM_T("opt.I_init.Imin[i]" << EQUALS << db->accessAllInputs()[i]->getLowerBound());
			__BODY_END__
		}
		else
		{
			CCODE_STREAM("for(uint16_t i=" << this->_arrSizes[i - 1] << "; i<" << this->_arrSizes[i] << ";i++)");
			__BODY_BEGIN__
				CCODE_STREAM_T("opt.I_init.Imax[i]" << EQUALS << db->accessAllInputs()[i]->getUpperBound());
			CCODE_STREAM_T("opt.I_init.Imin[i]" << EQUALS << db->accessAllInputs()[i]->getLowerBound());
			__BODY_END__
		}
	}
}


void PNMPCGEN::genCCode()
{
	// Set horizon length
	this->_Nh = static_cast<size_t>((this->_final_time - this->_init_time) / this->_step_size);
	// All C-code generation files
	// Generate generic interface C-code/CUDA code
	genCCodeInterface();
	// Generate dynamics C-code/CUDA code
	genCCodeDynamics();
	// Generate parametrization map C-code/CUDA code
	genCCodeParameterizationMap();
	// Generate regular constraints C-code/CUDA code
	genCCodeRegularConstraints();
	// Generate terminal constraints C-code/CUDA code
	genCCodeTerminalConstraints();
	// Generate Lagrangian C-code/CUDA code
	genCCodeLagrangian();
	// Generate Mayer C-code/CUDA code
	genCCodeMayer();
	// Generate pNMPC_SQP C-code/CUDA code
	genCCodePNMPC_SQP();
	// Generate Objective and constraint calls C-code/CUDA code
	genCCodeBBOSQPOBJCall();
	genCCodeBBOSQPCONSTCall();
	
	// SQP interface	
	#if(!ENABLE_SFUNCTION && !ENABLE_CUDA && !ENABLE_MEX)
		genCCodeSQPInterface();
	#endif

	// SQP interface for S-function Simulink
	#if(ENABLE_SFUNCTION)
		genCCodeSQPInterface_SFUN();
	#endif
	
	// SQP interface for MEX-function MATLAB/CUDA code
	#if(ENABLE_MEX || ENABLE_CUDA)
		genCCodeSQPInterface_MEX();
	#endif
}

PNMPCGEN::~PNMPCGEN()
{
	delete this->_instance;
}
