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
void PNMPCGEN::genCCodeSQPInterface()
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
	// Header files
	#if(!ENABLE_CUDA && ENABLE_CPP)
		// C++ BEGIN INTERFACE BEGIN
		__CPP_INTERFACE_BEGIN__
	#endif

	HEADER_STREAM(HEADER_GUARD);
	HEADER_STREAM(INCLUDE(FILE_PNMPC_SQP_H_));
	HEADER_STREAM(INCLUDE(FILE_SQP_BBOSQPOBJCall_H_));
	HEADER_STREAM(INCLUDE(FILE_SQP_BBOSQPCONSTCall_H_));
	#if(!ENABLE_CUDA)
		HEADER_STREAM(INCLUDE(FILE_SQP_H_));
	#elif(ENABLE_CUDA)
		HEADER_STREAM(INCLUDE(FILE_SQPCUDA_H_));
	#endif
	if(this->_NPME)
	HEADER_STREAM(INCLUDE(FILE_PARAMETERIZATION_MAP_H_));

	// Function prototype for SQP Interface
	HEADER_STREAM(COMMENT("Function prototype for SQP Interface"));
	#if(!ENABLE_CUDA)
		HEADER_STREAM_NT("void BBO_SQP(" << VAR_ARRAY(X, nx) << Paren[ParenOps::comma]);
	#elif(ENABLE_CUDA)
		HEADER_STREAM_NT("void BBO_SQPCUDA(" << VAR_ARRAY(X, nx) << Paren[ParenOps::comma]);
	#endif
	HEADER_STREAM_NT(VAR_ARRAY(U_IN,NMAX) << Paren[ParenOps::comma]);
	if (this->_np)
	{
		if (this->_ParamPredict)
			HEADER_STREAM_NT(VAR_ARRAY2D(P_horizon, Nh, np) << Paren[ParenOps::comma]);
		else
			HEADER_STREAM_NT(VAR_ARRAY(P, np) << Paren[ParenOps::comma]);
	}
	if(this->_NPME)
	// Parameterized output
	HEADER_STREAM_NT(VAR_ARRAY(U_ACT,NPME) << Paren[ParenOps::comma]);
	else if(this->_nu)
	// Parameterized output
	HEADER_STREAM_NT(VAR_ARRAY(U_ACT, nu) << Paren[ParenOps::comma]);
	// Past parameterized input 
	HEADER_STREAM_NT(VAR_ARRAY(U_OUT, NMAX) << Paren[ParenOps::comma]);
	// Computation time
	HEADER_STREAM_NT(VAR_PTR(compTime) << Paren[ParenOps::comma]);
	// Objecive value
	HEADER_STREAM_T(VAR_PTR(fxSOL) << Paren[ParenOps::close]);

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

	// C code
	CCODE_STREAM(INCLUDE(FILE_SQP_INTERFACE_H_));

	#if(ENABLE_CUDA)
		// Device function pointers
		CCODE_STREAM(COMMENT("Device function pointers"));
		CCODE_STREAM_T("__device__ funcPointer funcJDevice = func2BBOOBJ_SQP");
		CCODE_STREAM_T("__device__ funcPointer funcGDevice = func2BBOCONST_SQP");

		// Function pointers for objective and constraints host side
		CCODE_STREAM(COMMENT("Function pointers for objective and constraints host side"));
		CCODE_STREAM_T("funcPointer funcJHost, funcGHost");
	#endif

	// Interface variables (Optimization settings, Computation time, Cost function parameters, 
	//                      Tempory actual input array, Parameter vector P at current time 0)
	InterfaceVariables(this->oss_c);

	// SQP interface function
	CCODE_STREAM(COMMENT("SQP interface function"));
	#if(!ENABLE_CUDA)
		CCODE_STREAM_NT("void BBO_SQP(" << VAR_ARRAY(X, nx) << Paren[ParenOps::comma]);
	#elif(ENABLE_CUDA)
		CCODE_STREAM_NT("void BBO_SQPCUDA(" << VAR_ARRAY(X, nx) << Paren[ParenOps::comma]);
	#endif
	CCODE_STREAM_NT(VAR_ARRAY(U_IN, NMAX) << Paren[ParenOps::comma]);
	if (this->_np)
	{
		if (this->_ParamPredict)
			CCODE_STREAM_NT(VAR_ARRAY2D(P_horizon, Nh, np) << Paren[ParenOps::comma]);
		else
			CCODE_STREAM_NT(VAR_ARRAY(P, np) << Paren[ParenOps::comma]);
	}
	if (this->_NPME)
		// Parameterized output
		CCODE_STREAM_NT(VAR_ARRAY(U_ACT, NPME) << Paren[ParenOps::comma]);
	else if (this->_nu)
		// Parameterized output
		CCODE_STREAM_NT(VAR_ARRAY(U_ACT, nu) << Paren[ParenOps::comma]);
	// Past parameterized input 
	CCODE_STREAM_NT(VAR_ARRAY(U_OUT, NMAX) << Paren[ParenOps::comma]);
	// Computation time
	CCODE_STREAM_NT(VAR_PTR(compTime) << Paren[ParenOps::comma]);
	// Objecive value
	CCODE_STREAM(VAR_PTR(fxSOL) << Paren[ParenOps::close]);

	// Body begin
	__BODY_BEGIN__
	
	// Setting up the optimization structure
	CCODE_STREAM(COMMENT("Setting up the optimization structure"));
	CCODE_STREAM(FOR_UNSIGNED_FORWARD(i, 0, nx));
	CCODE_STREAM_T(ASSIGN(paramsSQP.X[i], X[i]));
	if (!this->_ParamPredict)
	{
		CCODE_STREAM(FOR_UNSIGNED_FORWARD(i, 0, np));
		if (this->_np)
			CCODE_STREAM_T(ASSIGN(paramsSQP.P[i], P[i]));
		else
			CCODE_STREAM_T(ASSIGN(paramsSQP.P[i], 0));
	}
	else {
		CCODE_STREAM(FOR_UNSIGNED_FORWARD(i, 0, Nh));
		CCODE_STREAM(FOR_UNSIGNED_FORWARD(j, 0, np));
		CCODE_STREAM_T(ASSIGN(paramsSQP.P_horizon[i][j], P_horizon[i][j]));
	}

	if (this->_ParamPredict)
	{
		// Set parameter vector P at current time 0
		CCODE_STREAM(COMMENT("Set parameter vector P at current time 0"));
		CCODE_STREAM(FOR_UNSIGNED_FORWARD(j, 0, np));
		CCODE_STREAM_T(ASSIGN(P[j], P_horizon[0][j]));
	}

	// Set optimization parameters
	OptimizationParameters(this->oss_c);

	#if(ENABLE_CUDA)
		// Device in-data and out-data
		CCODE_STREAM(COMMENT("Device in-data and out-data"));
		CCODE_STREAM_T("Real_C* P_inDevice, *P_outDevice");
		CCODE_STREAM_T("optimset_SQP* opt_device; ParaStructC* params_device");
		CCODE_STREAM_T("cudaMalloc(&P_inDevice, NMAX*sizeof(Real_C))");
		CCODE_STREAM_T("cudaMalloc(&P_outDevice, NMAX*sizeof(Real_C))");
		CCODE_STREAM_T("cudaMalloc(&opt_device, sizeof(optimset_SQP))");
		CCODE_STREAM_T("cudaMalloc(&params_device, sizeof(ParaStructC))");
	#endif

	// Start clock
	CCODE_STREAM(COMMENT("Start clock"));
	CCODE_STREAM_T(ASSIGN(start, clock()));

	#if(ENABLE_CUDA)
		// Cuda memcpyies
		CCODE_STREAM(COMMENT("Cuda memcpyies"));
		CCODE_STREAM_T("cudaMemcpyFromSymbol(&funcJHost, funcJDevice, sizeof(funcPointer))");
		CCODE_STREAM_T("cudaMemcpyFromSymbol(&funcGHost, funcGDevice, sizeof(funcPointer))");
		CCODE_STREAM_T("cudaMemcpy(P_inDevice, U_IN, NMAX*sizeof(Real_C), cudaMemcpyHostToDevice)");
		CCODE_STREAM_T("cudaMemcpy(opt_device, &opt, sizeof(optimset_SQP), cudaMemcpyHostToDevice)");
		CCODE_STREAM_T("cudaMemcpy(params_device, &paramsSQP, sizeof(ParaStructC), cudaMemcpyHostToDevice)");
	#endif

	// SQP function call
	CCODE_STREAM(COMMENT("SQP function call"));
	#if(!ENABLE_CUDA)
		CCODE_STREAM_T("BBO_SQPFunction(U_IN, &paramsSQP, &func2BBOOBJ_SQP, &func2BBOCONST_SQP, &opt)");
	#elif(ENABLE_CUDA)
		CCODE_STREAM_T("SQPCUDA_BBO <<< (NMAX + TPB - 1) / TPB, TPB >>> (P_inDevice, P_outDevice, params_device, funcJHost, funcGHost, opt_device)");
		CCODE_STREAM_T("cudaMemcpy(U_OUT, P_outDevice, NMAX*sizeof(Real_C), cudaMemcpyDeviceToHost)");
	#endif

	// End clock
	CCODE_STREAM(COMMENT("End clock"));
	CCODE_STREAM_T(ASSIGN(end, clock()));

	#if(!ENABLE_CUDA)
		// Results assignment
		CCODE_STREAM(COMMENT("Result assignment"));
		CCODE_STREAM(FOR_UNSIGNED_FORWARD(i, 0, NMAX));
		CCODE_STREAM_T(ASSIGN(U_OUT[i],U_IN[i]));
	#endif

	if (this->_NPME)
	{
		// Parameterized Mapped input
		CCODE_STREAM(COMMENT("Parameterized Mapped input"));
		for (size_t k{ 0 }; k < this->_nu; k++)
		{
			if (k == 0)
				#if(!ENABLE_CUDA)
					CCODE_STREAM_T("U_TEMP[" << k << "]" << EQUALS << "U_IN[0]");
				#elif(ENABLE_CUDA)
					CCODE_STREAM_T("U_TEMP[" << k << "]" << EQUALS << "U_OUT[0]");
				#endif
			else
				#if(!ENABLE_CUDA)
					CCODE_STREAM_T("U_TEMP[" << k << "]" << EQUALS << "U_IN[" << this->_arrSizes[k - 1] << "]");
				#elif(ENABLE_CUDA)
					CCODE_STREAM_T("U_TEMP[" << k << "]" << EQUALS << "U_OUT[" << this->_arrSizes[k - 1] << "]");
				#endif
		}
		if(this->_np)
			#if(!ENABLE_CUDA)
					CCODE_STREAM_T(ASSIGN(U_ACT_TEMP, ParameterInputMap(X, U_TEMP, P)));
			#elif(ENABLE_CUDA)
					CCODE_STREAM_T(ASSIGN(U_ACT_TEMP, ParameterInputMapHost(X, U_TEMP, P)));
			#endif	
		else
			#if(!ENABLE_CUDA)
					CCODE_STREAM_T(ASSIGN(U_ACT_TEMP, ParameterInputMap(X, U_TEMP, NULL)));
			#elif(ENABLE_CUDA)
					CCODE_STREAM_T(ASSIGN(U_ACT_TEMP, ParameterInputMapHost(X, U_TEMP, NULL)));
			#endif

		// Set Values
		CCODE_STREAM(FOR_UNSIGNED_FORWARD(i, 0, NPME));
		CCODE_STREAM_T(ASSIGN(U_ACT[i], U_ACT_TEMP[i]));
	}
	else {
		// Results assignment
		CCODE_STREAM(COMMENT("Result assignment"));
		for (size_t k{ 0 }; k < this->_nu; k++)
		{
			if (k == 0)
				CCODE_STREAM_T("U_ACT[" << k << "]" << EQUALS << "U_IN[0]");
			else
				CCODE_STREAM_T("U_ACT[" << k << "]" << EQUALS << "U_IN[" << this->_arrSizes[k - 1] << "]");
		}
	}

	CCODE_STREAM(IF_NOT_EQUALS(compTime, NULL));
	// Computation time
	CCODE_STREAM(COMMENT("Computation time"));
	CCODE_STREAM_T(ASSIGN(compTime[0], ((Real_C)(end - start)) / (Real_C)(CLOCKS_PER_SEC)));

	CCODE_STREAM(IF_NOT_EQUALS(fxSOL, NULL));
	// Optimized objective
	CCODE_STREAM(COMMENT("Optimized objective"));
	#if(ENABLE_CUDA)
		CCODE_STREAM_T(COMMENT(ASSIGN(fxSOL[0], func2BBOOBJ_SQP(U_IN, &paramsSQP))));
	#elif(!ENABLE_CUDA)
		CCODE_STREAM_T(ASSIGN(fxSOL[0], func2BBOOBJ_SQP(U_IN, &paramsSQP)));
	#endif

	#if(ENABLE_CUDA)
		// Free memory 
		CCODE_STREAM(COMMENT("CUDA free memory"));
		CCODE_STREAM_T("cudaFree(P_inDevice)");
		CCODE_STREAM_T("cudaFree(P_outDevice)");
		CCODE_STREAM_T("cudaFree(opt_device)");
		CCODE_STREAM_T("cudaFree(params_device)");
	#endif

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
	EXPORT_CCODE(SRC_DIR, FILE_SQP_INTERFACE_C_);
	EXPORT_HEADER(INCLUDE_DIR, FILE_SQP_INTERFACE_H_);
}
