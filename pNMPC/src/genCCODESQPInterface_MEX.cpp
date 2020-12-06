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
void PNMPCGEN::genCCodeSQPInterface_MEX()
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
		// C++ BEGIN INTERFACE BEGIN
		__CPP_INTERFACE_BEGIN__
	#endif

	// Header guard
	HEADER_STREAM(C_HEADER_GUARD_BEGIN(FILE_SQP_INTERFACE_MEX_H_));
	HEADER_STREAM(INCLUDE(FILE_MEX));
	HEADER_STREAM(INCLUDE(FILE_PNMPC_SQP_H_));
	HEADER_STREAM(INCLUDE(FILE_SQP_BBOSQPOBJCall_H_));
	HEADER_STREAM(INCLUDE(FILE_SQP_BBOSQPCONSTCall_H_));
	#if(!ENABLE_CUDA)
		HEADER_STREAM(INCLUDE(FILE_SQP_H_));
	#elif(ENABLE_CUDA)
		HEADER_STREAM(INCLUDE(FILE_SQPCUDA_H_));
	#endif
	if (this->_NPME)
		HEADER_STREAM(INCLUDE(FILE_PARAMETERIZATION_MAP_H_));

	// Function prototype for SQP Interface MEX
	HEADER_STREAM(COMMENT("Function prototype for SQP Interface MEX function"));
	HEADER_STREAM_T("void mexFunction(int nlhs, mxArray* plhs[], int nrhs, const mxArray* prhs[])");

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
	CCODE_STREAM(INCLUDE(FILE_SQP_INTERFACE_MEX_H_));

	// MEX lock
 	CCODE_STREAM_T(VAR_ARRAY(X,nx));
	CCODE_STREAM_T(VAR_ARRAY(U_IN,NMAX));
	if (this->_np)
	{
		if (this->_ParamPredict)
		{
			CCODE_STREAM_T(VAR_PTR(P_horizon_mex));
			CCODE_STREAM_T(VAR_ARRAY2D(P_horizon, Nh, np));
		}
		else
			CCODE_STREAM_T(VAR_ARRAY(P, np));
	}
	if (this->_NPME)
		// Parameterized output
		CCODE_STREAM_T(VAR_PTR(U_ACT));
	else if (this->_nu)
		// Parameterized output
		CCODE_STREAM_T(VAR_PTR(U_ACT));
	// Past parameterized input 
	CCODE_STREAM_T(VAR_PTR(U_OUT));
	// Computation time
	CCODE_STREAM_T(VAR_PTR(compTime));
	// Objecive value
	CCODE_STREAM_T(VAR_PTR(fxSOL));

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
	CCODE_STREAM(COMMENT("SQP interface MEX function"));
	CCODE_STREAM_NT("void mexFunction(int nlhs, mxArray* plhs[], int nrhs, const mxArray* prhs[])");

	// Body begin
	__BODY_BEGIN__

	// Check for number of outputs validity
	CCODE_STREAM(COMMENT("Validate number of outputs"));
	CCODE_STREAM(IF_EQUALS(nlhs != 4, true));
		CCODE_STREAM_T(MEX_EXCEPTION(MEX_NLHS, "Variables: Number of output variables is not 4"));

	if (this->_np)
	{
		CCODE_STREAM(COMMENT("Validate number of inputs"));
		CCODE_STREAM(IF_EQUALS(nrhs != 3, true));
			CCODE_STREAM_T(MEX_EXCEPTION(MEX_NRHS, "Variables: Number of input variables is not 3"));
	}
	else
	{
		CCODE_STREAM(COMMENT("Validate number of inputs"));
		CCODE_STREAM(IF_EQUALS(nrhs != 2, true));
			CCODE_STREAM_T(MEX_EXCEPTION(MEX_NRHS, "Variables: Number of input variables is not 2"));
	}

	// MEX lock with inputs
	CCODE_STREAM(COMMENT("MEX lock with inputs"));
	// Is state vector double?
	CCODE_STREAM(COMMENT("Is state vector double?"));
	CCODE_STREAM(IF_EQUALS(!mxIsDouble(prhs[0]) || mxIsComplex(prhs[0]), true));
		CCODE_STREAM_T(MEX_EXCEPTION(MEX_NOTDOUBLE, "Input: State vector is not a double"));
	// Is state vector of right dimensions?
	CCODE_STREAM(COMMENT("Is state vector of right dimensions?"));
	CCODE_STREAM(IF_EQUALS(mxGetNumberOfElements(prhs[0]) != nx, true));
		CCODE_STREAM_T(MEX_EXCEPTION(MEX_NOTRIGHTDIMENSION, "Input: State vector is not of right dimensions"));
	CCODE_STREAM(ELSE);
	// Assign states (Deprecated)
	//CCODE_STREAM(COMMENT("Assign states"));
	//CCODE_STREAM_T(ASSIGN(X, (Real_C*)mxGetPr(prhs[0])));
	CCODE_STREAM(FOR_UNSIGNED_FORWARD(i, 0, nx));
	CCODE_STREAM_T(ASSIGN(X[i], (Real_C)(mxGetPr(prhs[0])[i])));

	// Is previous input vector double?
	CCODE_STREAM(COMMENT("Is previous input vector double?"));
	CCODE_STREAM(IF_EQUALS(!mxIsDouble(prhs[1]) || mxIsComplex(prhs[1]), true));
		CCODE_STREAM_T(MEX_EXCEPTION(MEX_NOTDOUBLE, "Input: Previous input vector is not a double"));
	// Is previous input vector of right dimensions?
	CCODE_STREAM(COMMENT("Is previous input vector of right dimensions?"));
	CCODE_STREAM(IF_EQUALS(mxGetNumberOfElements(prhs[1]) != NMAX, true));
		CCODE_STREAM_T(MEX_EXCEPTION(MEX_NOTRIGHTDIMENSION, "Input: Previous input vector is not of right dimensions"));
	CCODE_STREAM(ELSE);
	// Assign previous inputs (Deprecated)
	//CCODE_STREAM(COMMENT("Assign previous inputs"));
	//CCODE_STREAM_T(ASSIGN(U_IN, (Real_C*)mxGetPr(prhs[1])));
	CCODE_STREAM(FOR_UNSIGNED_FORWARD(i, 0, NMAX));
	CCODE_STREAM_T(ASSIGN(U_IN[i], (Real_C)(mxGetPr(prhs[1])[i])));

	if (this->_np)
	{
		if (this->_ParamPredict)
		{
			// Is parameter matrix over horizon double?
			CCODE_STREAM(COMMENT("Is parameter matrix over horizon double?"));
			CCODE_STREAM(IF_EQUALS(!mxIsDouble(prhs[2]) || mxIsComplex(prhs[2]), true));
				CCODE_STREAM_T(MEX_EXCEPTION(MEX_NOTDOUBLE, "Input: Parameter matrix over the horizon is not a double"));
			// Is parameter matrix over horizon of right dimensions?
			CCODE_STREAM(COMMENT("Is previous input vector of right dimensions?"));
			CCODE_STREAM(IF_EQUALS((mxGetM(prhs[2]) != Nh) || (mxGetN(prhs[2]) != np), true));
				CCODE_STREAM_T(MEX_EXCEPTION(MEX_NOTRIGHTDIMENSION, "Input: Parameter matrix over the horizo is not of right dimensions"));
			CCODE_STREAM(ELSE);
				// Assign parameter matrix 
				CCODE_STREAM(COMMENT("Assign parameter matrix"));
				CCODE_STREAM_T(ASSIGN(P_horizon_mex, (Real_C*)mxGetPr(prhs[2])));
			// Assign P_horizon
			CCODE_STREAM(COMMENT("Assign P_horizon"));
			CCODE_STREAM(FOR_UNSIGNED_FORWARD(i, 0, Nh));
			CCODE_STREAM(FOR_UNSIGNED_FORWARD(j, 0, np));
			CCODE_STREAM_T(ASSIGN(P_horizon[i][j], (Real_C)P_horizon_mex[Nh*j + i]));
		}
		else
		{
			// Is parameter vector double?
			CCODE_STREAM(COMMENT("Is parameter vector double?"));
			CCODE_STREAM(IF_EQUALS(!mxIsDouble(prhs[2]) || mxIsComplex(prhs[2]), true));
				CCODE_STREAM_T(MEX_EXCEPTION(MEX_NOTDOUBLE, "Input: Parameter vector is not a double"));
			// Is parameter vector of right dimensions?
			CCODE_STREAM(COMMENT("Is parameter vector of right dimensions?"));
			CCODE_STREAM(IF_EQUALS(mxGetNumberOfElements(prhs[2]) != np, true));
				CCODE_STREAM_T(MEX_EXCEPTION(MEX_NOTRIGHTDIMENSION, "Input: Parameter vector is not of right dimensions"));
			CCODE_STREAM(ELSE);
			// Assign paramters (Deprecated)
			//CCODE_STREAM(COMMENT("Assign parameters"));
			//CCODE_STREAM_T(ASSIGN(P, (Real_C*)mxGetPr(prhs[2])));
			CCODE_STREAM(FOR_UNSIGNED_FORWARD(i, 0, np));
			CCODE_STREAM_T(ASSIGN(P[i], (Real_C)(mxGetPr(prhs[2])[i])));
		}
	}

	// MEX lock with outputs
	CCODE_STREAM(COMMENT("MEX lock with outputs"));
	if (this->_NPME)	
	{
		// Parameterized output
		#if(!ENABLE_CUDA)
			CCODE_STREAM_T(ASSIGN(plhs[0], mxCreateDoubleMatrix(NPME, 1, mxREAL)));
			CCODE_STREAM_T(ASSIGN(U_ACT, (Real_C*)mxGetPr(plhs[0])));
		#elif(ENABLE_CUDA)
			CCODE_STREAM_T(ASSIGN(plhs[0], mxCreateNumericMatrix(NPME, 1, mxSINGLE_CLASS, mxREAL)));
			CCODE_STREAM_T(ASSIGN(U_ACT, (Real_C*)mxGetPr(plhs[0])));
		#endif			
	}
	else if (this->_nu)
	{
		// Parameterized output
		#if(!ENABLE_CUDA)
			CCODE_STREAM_T(ASSIGN(plhs[0], mxCreateDoubleMatrix(nu, 1, mxREAL)));
			CCODE_STREAM_T(ASSIGN(U_ACT, (Real_C*)mxGetPr(plhs[0])));
		#elif(ENABLE_CUDA)
			CCODE_STREAM_T(ASSIGN(plhs[0], mxCreateNumericMatrix(nu, 1, mxSINGLE_CLASS, mxREAL)));
			CCODE_STREAM_T(ASSIGN(U_ACT, (Real_C*)mxGetPr(plhs[0])));
		#endif	
	}

	// Past parameterized input 
	#if(!ENABLE_CUDA)
		CCODE_STREAM_T(ASSIGN(plhs[1], mxCreateDoubleMatrix(NMAX, 1, mxREAL)));
		CCODE_STREAM_T(ASSIGN(U_OUT, (Real_C*)mxGetPr(plhs[1])));
	#elif(ENABLE_CUDA)
		CCODE_STREAM_T(ASSIGN(plhs[1], mxCreateNumericMatrix(NMAX, 1, mxSINGLE_CLASS, mxREAL)));
		CCODE_STREAM_T(ASSIGN(U_OUT, (Real_C*)mxGetPr(plhs[1])));
	#endif

	// Computation time
	#if(!ENABLE_CUDA)
		CCODE_STREAM_T(ASSIGN(plhs[2], mxCreateDoubleMatrix(1, 1, mxREAL)));
		CCODE_STREAM_T(ASSIGN(compTime, (Real_C*)mxGetPr(plhs[2])));
	#elif(ENABLE_CUDA)
		CCODE_STREAM_T(ASSIGN(plhs[2], mxCreateNumericMatrix(1, 1, mxSINGLE_CLASS, mxREAL)));
		CCODE_STREAM_T(ASSIGN(compTime, (Real_C*)mxGetPr(plhs[2])));
	#endif	

	// Objecive value
	#if(!ENABLE_CUDA)
		CCODE_STREAM_T(ASSIGN(plhs[3], mxCreateDoubleMatrix(1, 1, mxREAL)));
		CCODE_STREAM_T(ASSIGN(fxSOL, (Real_C*)mxGetPr(plhs[3])));
	#elif(ENABLE_CUDA)
		CCODE_STREAM_T(ASSIGN(plhs[3], mxCreateNumericMatrix(1, 1, mxSINGLE_CLASS, mxREAL)));
		CCODE_STREAM_T(ASSIGN(fxSOL, (Real_C*)mxGetPr(plhs[3])));
	#endif		

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
		CCODE_STREAM_T("Real_C* P_inDevice");
		CCODE_STREAM_T("optimset_SQP* opt_device; ParaStructC* params_device");
		CCODE_STREAM_T("cudaMalloc(&P_inDevice, NMAX*sizeof(Real_C))");
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
		CCODE_STREAM_T("SQPCUDA_BBO <<< (NMAX + TPB - 1) / TPB, TPB >>> (P_inDevice, params_device, funcJHost, funcGHost, opt_device)");
		CCODE_STREAM_T("cudaMemcpy(U_OUT, P_inDevice, NMAX*sizeof(Real_C), cudaMemcpyDeviceToHost)");
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
	EXPORT_CCODE(SRC_DIR, FILE_SQP_INTERFACE_MEX_C_);
	EXPORT_HEADER(INCLUDE_DIR, FILE_SQP_INTERFACE_MEX_H_);
}
