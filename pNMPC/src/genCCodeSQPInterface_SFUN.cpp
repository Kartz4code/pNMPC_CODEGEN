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

#include "PNMPCGEN.hpp"

void PNMPCGEN::genCCodeSQPInterface_SFUN()
{
/*
=============================================================================================================
	  __    __   _______     ___       _______   _______ .______          _______  __   __       _______ 
	 |  |  |  | |   ____|   /   \     |       \ |   ____||   _  \        |   ____||  | |  |     |   ____|
	 |  |__|  | |  |__     /  ^  \    |  .--.  ||  |__   |  |_)  |       |  |__   |  | |  |     |  |__   
	 |   __   | |   __|   /  /_\  \   |  |  |  ||   __|  |      /        |   __|  |  | |  |     |   __|  
	 |  |  |  | |  |____ /  _____  \  |  '--'  ||  |____ |  |\  \----.   |  |     |  | |  `----.|  |____ 
	 |__|  |__| |_______/__/     \__\ |_______/ |_______|| _| `._____|   |__|     |__| |_______||_______|
 
=============================================================================================================
 */
	HEADER_STREAM(license);
	// Header files
	// C++ BEGIN INTERFACE BEGIN
	__CPP_INTERFACE_BEGIN__

	// Header guard
	HEADER_STREAM(C_HEADER_GUARD_BEGIN(FILE_SQP_INTERFACE_SFUN_H_));
	HEADER_STREAM(INCLUDE(FILE_PNMPC_SQP_H_));
	HEADER_STREAM(INCLUDE(FILE_SQP_BBOSQPOBJCall_H_));
	HEADER_STREAM(INCLUDE(FILE_SQP_BBOSQPCONSTCall_H_));
	HEADER_STREAM(INCLUDE(FILE_SQP_H_));
	if (this->_NPME)
		HEADER_STREAM(INCLUDE(FILE_PARAMETERIZATION_MAP_H_));

	// Get the total number of parameterization variables
	size_t sumParams{ 0 };
	for (size_t i{ 0 }; i < this->_nu; i++)
		sumParams += db->accessAllInputs()[i]->getControlParameterization();

/*
=============================================================================================================
			 __  .__   __. .___________. _______ .______       _______    ___       ______  _______ 
			|  | |  \ |  | |           ||   ____||   _  \     |   ____|  /   \     /      ||   ____|
			|  | |   \|  | `---|  |----`|  |__   |  |_)  |    |  |__    /  ^  \   |  ,----'|  |__   
			|  | |  . `  |     |  |     |   __|  |      /     |   __|  /  /_\  \  |  |     |   __|  
			|  | |  |\   |     |  |     |  |____ |  |\  \----.|  |    /  _____  \ |  `----.|  |____ 
			|__| |__| \__|     |__|     |_______|| _| `._____||__|   /__/     \__\ \______||_______|
                                                                                        
=============================================================================================================
 */

	// Interface stream
	INTERFACE_STREAM_NT("Name: BBO_SQP(Real_C u1" << "[" << this->_nx << "]" << Paren[ParenOps::comma]);
	INTERFACE_STREAM_NT(TYPE_C << "u2" << "[" << sumParams << "]" << Paren[ParenOps::comma]);
	
	// External parameter vector 
	if (this->_np)
	{
		if (this->_ParamPredict)
			INTERFACE_STREAM_NT(TYPE_C << "u3" << "[" << this->_Nh << "]" << "[" << this->_np << "]" << Paren[ParenOps::comma]);
		else
			INTERFACE_STREAM_NT(TYPE_C << "u3" << "[" << this->_np << "]" << Paren[ParenOps::comma]);
	}
	if(this->_NPME)
		INTERFACE_STREAM_NT(TYPE_C << "y1" << "[" << this->_NPME << "]" << Paren[ParenOps::comma]);
	else
		INTERFACE_STREAM_NT(TYPE_C << "y1" << "[" << this->_nu << "]" << Paren[ParenOps::comma]);
	INTERFACE_STREAM_NT(TYPE_C << "y2" << "[" << sumParams << "]" << Paren[ParenOps::comma]);
	INTERFACE_STREAM_NT(TYPE_C << "y3" << "[1]" << Paren[ParenOps::comma]);
	INTERFACE_STREAM(TYPE_C << "y4" << "[1]" << Paren[ParenOps::close]);

	// Input data
	INTERFACE_STREAM("In: X0[" << this->_nx << "]");
	INTERFACE_STREAM("In: U_PARAM_W[" << sumParams << "]");
	if (this->_np)
	{
		if (this->_ParamPredict)
			INTERFACE_STREAM("In: P[" << this->_Nh << "]" << "[" << this->_np << "]")
		else
			INTERFACE_STREAM("In: P[" << this->_np << "]");
	}

	// Output data
	if(this->_NPME)
		INTERFACE_STREAM("Out: U_ACT[" << this->_NPME << "]")
	else
		INTERFACE_STREAM("Out: U_ACT[" << this->_nu << "]");

	INTERFACE_STREAM("Out: U_PARAM[" << sumParams << "]");
	INTERFACE_STREAM("Out: CompTime");
	INTERFACE_STREAM_NT("Out: OBJ");


	// Function prototype for SQP Interface
	HEADER_STREAM(COMMENT("Function prototype for SQP Interface"));
	HEADER_STREAM_NT("void BBO_SQP(Real_C u1" << "[" << this->_nx << "]" << Paren[ParenOps::comma]);
	HEADER_STREAM_NT(TYPE_C << "u2" << "[" << sumParams << "]" << Paren[ParenOps::comma]);
	
	// External parameter vector 
	if (this->_np)
	{
		if (this->_ParamPredict)
			HEADER_STREAM_NT(TYPE_C << "u3" << "[" << this->_Nh << "]" << "[" << this->_np << "]" << Paren[ParenOps::comma]);
		else
			HEADER_STREAM_NT(TYPE_C << "u3" << "[" << this->_np << "]" << Paren[ParenOps::comma]);
	}
	
	// Outputs
	if(this->_NPME)
		HEADER_STREAM_NT(TYPE_C << "y1" << "[" << this->_NPME << "]" << Paren[ParenOps::comma]);
	else
		HEADER_STREAM_NT(TYPE_C << "y1" << "[" << this->_nu << "]" << Paren[ParenOps::comma]);

	HEADER_STREAM_NT(TYPE_C << "y2" << "[" << sumParams << "]" << Paren[ParenOps::comma]);
	HEADER_STREAM_NT(TYPE_C << "y3[1]" << Paren[ParenOps::comma]);
	HEADER_STREAM_T(TYPE_C << "y4[1]" << Paren[ParenOps::close]);

	HEADER_STREAM(C_HEADER_GUARD_END);
	// C++ END INTERFACE
	__CPP_INTERFACE_END__

/*
==========================================================================================================
							   ______      ______   ______    _______   _______
							  /      |    /      | /  __  \  |       \ |   ____|
							 |  ,----'   |  ,----'|  |  |  | |  .--.  ||  |__
							 |  |        |  |     |  |  |  | |  |  |  ||   __|
							 |  `----.   |  `----.|  `--'  | |  '--'  ||  |____
							  \______|    \______| \______/  |_______/ |_______|

 ==========================================================================================================
*/
	CCODE_STREAM(license);
	// C code
	CCODE_STREAM(INCLUDE(FILE_SQP_INTERFACE_SFUN_H_));
	
	// Interface variables (Optimization settings, Computation time, Cost function parameters, 
	//                      Tempory actual input array, Parameter vector P at current time 0)
	InterfaceVariables(this->oss_c);

	// SQP interface function
	CCODE_STREAM(COMMENT("SQP interface function"));
	CCODE_STREAM_NT("void BBO_SQP(Real_C u1" << "[" << this->_nx << "]" << Paren[ParenOps::comma]);
	CCODE_STREAM_NT(TYPE_C << "u2" << "[" << sumParams << "]" << Paren[ParenOps::comma]);
	
	// External parameter vector 
	if (this->_np)
	{
		if (this->_ParamPredict)
			CCODE_STREAM_NT(TYPE_C << "u3" << "[" << this->_Nh << "]" << "[" << this->_np << "]" << Paren[ParenOps::comma]);
		else
			CCODE_STREAM_NT(TYPE_C << "u3" << "[" << this->_np << "]" << Paren[ParenOps::comma]);
	}
	
	// Outputs
	if(this->_NPME)
		CCODE_STREAM_NT(TYPE_C << "y1" << "[" << this->_NPME << "]" << Paren[ParenOps::comma]);
	else
		CCODE_STREAM_NT(TYPE_C << "y1" << "[" << this->_nu << "]" << Paren[ParenOps::comma]);
	CCODE_STREAM_NT(TYPE_C << "y2" << "[" << sumParams << "]" << Paren[ParenOps::comma]);
	CCODE_STREAM_NT(TYPE_C << "y3[1]" << Paren[ParenOps::comma]);
	CCODE_STREAM(TYPE_C << "y4[1]" << Paren[ParenOps::close]);

	// Body begin
	__BODY_BEGIN__

	// Setting up the optimization structure
	CCODE_STREAM(COMMENT("Setting up the optimization structure"));
	CCODE_STREAM(FOR_UNSIGNED_FORWARD(i, 0, nx));
	CCODE_STREAM_T(ASSIGN(paramsSQP.X[i], u1[i]));
	if (!this->_ParamPredict)
	{
		CCODE_STREAM(FOR_UNSIGNED_FORWARD(i, 0, np));
		if (this->_np)
			CCODE_STREAM_T(ASSIGN(paramsSQP.P[i], u3[i]));
		else
			CCODE_STREAM_T(ASSIGN(paramsSQP.P[i], 0));
	}
	else {
		CCODE_STREAM(FOR_UNSIGNED_FORWARD(i, 0, Nh));
		CCODE_STREAM(FOR_UNSIGNED_FORWARD(j, 0, np));
		CCODE_STREAM_T(ASSIGN(paramsSQP.P_horizon[i][j], u3[i][j]));
	}

	if (this->_ParamPredict)
	{
		// Set parameter vector P at current time 0
		CCODE_STREAM(COMMENT("Set parameter vector P at current time 0"));
		CCODE_STREAM(FOR_UNSIGNED_FORWARD(j, 0, np));
		CCODE_STREAM_T(ASSIGN(P[j], u3[0][j]));
	}

	// Set alpha for hyperparameters
	CCODE_STREAM(COMMENT("Set alpha for hyperparameters"));
	CCODE_STREAM(FOR_UNSIGNED_FORWARD(i, 0, NMAX));
	CCODE_STREAM_T("opt.alpha[i]" << EQUALS << this->_SCG.getAlpha());

	// Set optimization parameters
	OptimizationParameters(this->oss_c);

	// Start clock
	CCODE_STREAM(COMMENT("Start clock"));
	CCODE_STREAM_T(ASSIGN(start, clock()));
	CCODE_STREAM(COMMENT("SQP function call"));
	CCODE_STREAM_T("BBO_SQPFunction(u2, &paramsSQP, &func2BBOOBJ_SQP, &func2BBOCONST_SQP, &opt)");
	
	// End clock
	CCODE_STREAM(COMMENT("End clock"));
	CCODE_STREAM_T(ASSIGN(end, clock()));

	// Results assignment
	CCODE_STREAM(COMMENT("Result assignment"));
	CCODE_STREAM(FOR_UNSIGNED_FORWARD(i, 0, NMAX));
	CCODE_STREAM_T(ASSIGN(y2[i],u2[i]));

	if (this->_NPME)
	{
		// Parameterized Mapped input
		CCODE_STREAM(COMMENT("Parameterized Mapped input"));
		for (size_t k{ 0 }; k < this->_nu; k++)
		{
			if (k == 0)
				CCODE_STREAM_T("U_TEMP[" << k << "]" << EQUALS << "u2[0]");
			else
				CCODE_STREAM_T("U_TEMP[" << k << "]" << EQUALS << "u2[" << this->_arrSizes[k - 1] << "]");
		}
		if(this->_np)
			CCODE_STREAM_T(ASSIGN(U_ACT_TEMP, ParameterInputMap(u1, U_TEMP, u3)));
		else
			CCODE_STREAM_T(ASSIGN(U_ACT_TEMP, ParameterInputMap(u1, U_TEMP, 0)));
		// Set Values
		CCODE_STREAM(FOR_UNSIGNED_FORWARD(i, 0, NPME));
		CCODE_STREAM_T(ASSIGN(y1[i], U_ACT_TEMP[i]));
	}
	else {
		// Results assignment
		CCODE_STREAM(COMMENT("Result assignment"));
		for (size_t k{ 0 }; k < this->_nu; k++)
		{
			if (k == 0)
				CCODE_STREAM_T("y1["<< k <<"]" << EQUALS << "u2[0]");
			else
				CCODE_STREAM_T("y1[" << k << "]" << EQUALS << "u2[" << this->_arrSizes[k - 1] << "]");
		}
	}

	// Computation time 
	CCODE_STREAM(COMMENT("Computation time"));
	CCODE_STREAM_T(ASSIGN(y3[0], ((Real_C)(end - start)) / (Real_C)(CLOCKS_PER_SEC)));

	// Optimized objective
	if (this->_needObjective)
	{
		CCODE_STREAM(COMMENT("Optimized objective"));
		CCODE_STREAM_T(ASSIGN(y4[0], func2BBOOBJ_SQP(u2, &paramsSQP)));
	}
	else
	{
		CCODE_STREAM(COMMENT("Optimized objective"));
		CCODE_STREAM_T(ASSIGN(y4[0], 0));
	}

	// Body end
	__BODY_END__

/*
==========================================================================================================
 __________   ___ .______     ______   .______     .___________.     ______   ______    _______   _______ 
|   ____\  \ /  / |   _  \   /  __  \  |   _  \    |           |    /      | /  __  \  |       \ |   ____|
|  |__   \  V  /  |  |_)  | |  |  |  | |  |_)  |   `---|  |----`   |  ,----'|  |  |  | |  .--.  ||  |__   
|   __|   >   <   |   ___/  |  |  |  | |      /        |  |        |  |     |  |  |  | |  |  |  ||   __|  
|  |____ /  .  \  |  |      |  `--'  | |  |\  \----.   |  |        |  `----.|  `--'  | |  '--'  ||  |____ 
|_______/__/ \__\ | _|       \______/  | _| `._____|   |__|         \______| \______/  |_______/ |_______|

==========================================================================================================
*/

	// Export files
	EXPORT_CCODE(SRC_DIR, FILE_SQP_INTERFACE_SFUN_C_);
	EXPORT_HEADER(INCLUDE_DIR, FILE_SQP_INTERFACE_SFUN_H_);

	// MATLAB Interface
	EXPORT_INTERFACE(INTERFACE_DIR, MATLAB_INTERFACE_);
}