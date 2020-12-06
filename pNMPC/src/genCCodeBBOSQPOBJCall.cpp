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
void PNMPCGEN::genCCodeBBOSQPOBJCall()
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
	#if(!ENABLE_CUDA && ENABLE_CPP)
		// C++ BEGIN INTERFACE
		__CPP_INTERFACE_BEGIN__
	#endif

	// Header guard
	HEADER_STREAM(C_HEADER_GUARD_BEGIN(FILE_SQP_BBOSQPOBJCall_H_));
	HEADER_STREAM(INCLUDE(FILE_PNMPC_H_));

	// Function prototype for SQP Objective
	HEADER_STREAM(COMMENT("Function prototype for SQP - Objective"));
	#if(ENABLE_CUDA)
		__CUDA_DEVICE_H__
	#endif
	HEADER_STREAM_T(TYPE_C << "func2BBOOBJ_SQP(Real_C X_POINT[NMAX], const ParaStructC* XP_SQP)");
	
	HEADER_STREAM(C_HEADER_GUARD_END);
	#if(!ENABLE_CUDA && ENABLE_CPP)
		// C++ END INTERFACE
		__CPP_INTERFACE_END__
	#endif

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
	CCODE_STREAM(INCLUDE(FILE_SQP_BBOSQPOBJCall_H_));
	CCODE_STREAM(INCLUDE(FILE_PNMPC_SQP_H_));
    
	// SQP function call - Objective
	CCODE_STREAM(COMMENT("SQP function call - Objective"));
	#if(ENABLE_CUDA)
		__CUDA_DEVICE_C__
	#endif
	CCODE_STREAM(TYPE_C << "func2BBOOBJ_SQP(Real_C X_POINT[NMAX], const ParaStructC* XP_SQP)");

	// Begin body
	__BODY_BEGIN__

	// #if(ENABLE_CUDA || ENABLE_DSPACE)
		// Local variables for BBOCallFunc
		CCODE_STREAM(COMMENT("Local variables for BBOCallFunc"));
		CCODE_STREAM_T(VAR_ARRAY(X_SQP, nx));
		if (!this->_ParamPredict)
			CCODE_STREAM_T(VAR_ARRAY(P_SQP, np));
		else
			CCODE_STREAM_T(VAR_ARRAY2D(P_horizon_SQP, Nh, np));
		for (auto& item : db->accessAllInputs())
			CCODE_STREAM_T(TYPE_C << "U" << item->getName() << "[N" << item->getName() << "]");
	// #endif

	// Assign the states, parameters and inputs
	CCODE_STREAM(COMMENT("Assign the states, parameters and inputs"));
	CCODE_STREAM(FOR_UNSIGNED_FORWARD(i, 0, nx));
	CCODE_STREAM_T(ASSIGN(X_SQP[i], XP_SQP->X[i]));

	if (!this->_ParamPredict)
	{
		CCODE_STREAM(FOR_UNSIGNED_FORWARD(i, 0, np));
		CCODE_STREAM_T(ASSIGN(P_SQP[i], XP_SQP->P[i]));
	}
	else
	{
		CCODE_STREAM(FOR_UNSIGNED_FORWARD(i, 0, Nh));
		CCODE_STREAM(FOR_UNSIGNED_FORWARD(j, 0, np));
		CCODE_STREAM_T(ASSIGN(P_horizon_SQP[i][j], XP_SQP->P_horizon[i][j]));
	}

	// Set inputs
	CCODE_STREAM(COMMENT("Set input values"));
	for (size_t k{ 0 }; k < this->_nu; k++)
	{
		if (k == 0)
		{
			CCODE_STREAM("for (uint16_t i=0; i < " << this->_arrSizes[0] << "; i++)");
			CCODE_STREAM_T("U" << db->accessAllInputs()[k]->getName() << "[i]" << EQUALS << "X_POINT[i]");
		}
		else
		{
			CCODE_STREAM("for (uint16_t i=" << this->_arrSizes[k - 1] << "; i < " << this->_arrSizes[k] << "; i++)");
			CCODE_STREAM_T("U" << db->accessAllInputs()[k]->getName() << "[i-(" << this->_arrSizes[k - 1] << ")]" << EQUALS << "X_POINT[i]");
		}

	}

	// Call to BBO Function - Objective
	CCODE_STREAM(COMMENT("Call to BBO Function - Objective"));
	CCODE_STREAM_NT("return BBO_FunctionOBJ(X_SQP" << Paren[ParenOps::comma]);
	for (auto& item : db->accessAllInputs())
		CCODE_STREAM_NT("U" << item->getName() << Paren[ParenOps::comma]);
	if (this->_ParamPredict)
		CCODE_STREAM_T("P_horizon_SQP)");
	else
		CCODE_STREAM_T("P_SQP)");

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
	EXPORT_CCODE(SRC_DIR, FILE_SQP_BBOSQPOBJCall_C_);
	EXPORT_HEADER(INCLUDE_DIR, FILE_SQP_BBOSQPOBJCall_H_);
}
