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
// C - Code generation
void PNMPCGEN::genCCodeInterface()
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
		// C++ BEGIN INTERFACE
		__CPP_INTERFACE_BEGIN__
	#endif

	// HEADER CODE
	HEADER_STREAM(HEADER_GUARD);
	HEADER_STREAM(INCLUDE_GLOBAL(MATH));
	HEADER_STREAM(INCLUDE_GLOBAL(STDLIB));
    
    #if(!ENABLE_DSPACE)
        HEADER_STREAM(INCLUDE_GLOBAL(STDINT));
    #endif
	HEADER_STREAM(INCLUDE_GLOBAL(TIME));

	#if(ENABLE_CUDA)
	// Local headers
	//HEADER_STREAM(INCLUDE(CUDA_H));
	//HEADER_STREAM(INCLUDE(CUDA_RUNTIME_H));
	// Global headers
	//HEADER_STREAM(INCLUDE_GLOBAL(DEVICE_FUNC_H));
	HEADER_STREAM(INCLUDE_GLOBAL(DEVICE_LAUNCH_H));
	HEADER_STREAM(INCLUDE_GLOBAL(CUDA_API_H));
	#endif

	// Sign function
	HEADER_STREAM(DEFINE(sgn(x)) << Paren[ParenOps::space] << "((x < 0) ? -1 : 1)");
    
    
	#if(ENABLE_CUDA)
		HEADER_STREAM(DEFINE(min(a,b)) << Paren[ParenOps::space] << "(fminf((Real_C)(a),(Real_C)(b)))");
		HEADER_STREAM(DEFINE(max(a,b)) << Paren[ParenOps::space] << "(fmaxf((Real_C)(a),(Real_C)(b)))");
	#else
        HEADER_STREAM(DEFINE(min(a,b)) << Paren[ParenOps::space] << "(((a)<(b))?(a):(b))");
		HEADER_STREAM(DEFINE(max(a,b)) << Paren[ParenOps::space] << "(((a)>(b))?(a):(b))");
    #endif
	__BREAK_H__
	// Number of states, inputs and parameters
	HEADER_STREAM(COMMENT("Number of states, inputs and parameters"));

	if (!this->_nx)
		HEADER_STREAM(DEFINE(nx) << Paren[ParenOps::space] << this->_nx + 1);
	else
		HEADER_STREAM(DEFINE(nx) << Paren[ParenOps::space] << this->_nx);
	if (!this->_nu)
		HEADER_STREAM(DEFINE(nu) << Paren[ParenOps::space] << this->_nu + 1);
	else
		HEADER_STREAM(DEFINE(nu) << Paren[ParenOps::space] << this->_nu);
	if (!this->_np)
		HEADER_STREAM(DEFINE(np) << Paren[ParenOps::space] << this->_np + 1);
	else
		HEADER_STREAM(DEFINE(np) << Paren[ParenOps::space] << this->_np);

	__BREAK_H__
	// Look ahead time
	HEADER_STREAM(COMMENT("Look ahead time"));
	HEADER_STREAM(DEFINE(T) << Paren[ParenOps::space] << this->_final_time);

	__BREAK_H__
	// Horizon length
	HEADER_STREAM(COMMENT("Horizon length"));
	HEADER_STREAM(DEFINE(Nh) << Paren[ParenOps::space] << this->_Nh);

	__BREAK_H__
	// Step size
	HEADER_STREAM(COMMENT("Step size"));
	#if(ENABLE_CUDA)
		HEADER_STREAM(DEFINE(hstep) << Paren[ParenOps::space] << this->_step_size << "f");
	#else
		HEADER_STREAM(DEFINE(hstep) << Paren[ParenOps::space] << this->_step_size);
	#endif

	__BREAK_H__
	// Step size
	HEADER_STREAM(COMMENT("Solver tolerance"));
	HEADER_STREAM(DEFINE(TOL) << Paren[ParenOps::space] << this->getSolver()->getTolerance() << "f");


	__BREAK_H__
	// Input parameterization
	HEADER_STREAM(COMMENT("Input parameterization"));
	for (auto& item : db->accessAllInputs())
		HEADER_STREAM(DEFINE(N) << item->getName() << Paren[ParenOps::space] << item->getControlParameterization());

	__BREAK_H__
	// Dimensions of dynamics and constraints
	HEADER_STREAM(COMMENT("Dimensions of objective, constraints and dynamics"));
	if (!this->_ND)
		HEADER_STREAM(DEFINE(ND) << Paren[ParenOps::space] << this->_ND + 1);
	else
		HEADER_STREAM(DEFINE(ND) << Paren[ParenOps::space] << this->_ND);

	// Regular constraints
	if (!this->_NC_reg)
		HEADER_STREAM(DEFINE(NC_REG) << Paren[ParenOps::space] << this->_NC_reg + 1);
	else
		HEADER_STREAM(DEFINE(NC_REG) << Paren[ParenOps::space] << this->_NC_reg);

	// Terminal constraints
	if (!this->_NC_term)
		HEADER_STREAM(DEFINE(NC_TERM) << Paren[ParenOps::space] << this->_NC_term + 1);
	else
		HEADER_STREAM(DEFINE(NC_TERM) << Paren[ParenOps::space] << this->_NC_term);

	// ParameterizationMap
	if (!this->_NPME)
		HEADER_STREAM(DEFINE(NPME) << Paren[ParenOps::space] << this->_NPME + 1);
	else
		HEADER_STREAM(DEFINE(NPME) << Paren[ParenOps::space] << this->_NPME);

	// CUDA threads per block 
	#if(ENABLE_CUDA)
		HEADER_STREAM(DEFINE(TPB) << Paren[ParenOps::space] << this->_TPB);
	#endif

	__BREAK_H__
	// Max array size (Total control variables)
	HEADER_STREAM(COMMENT("Max array size (Total control variables)"));
	HEADER_STREAM(DEFINE(NMAX) << Paren[ParenOps::space] << Paren[ParenOps::open] << this->_arrSizes.back() << Paren[ParenOps::close]);

	__BREAK_H__
	#if(!ENABLE_CUDA)
		// Typedef double as Real_C for CPU
		HEADER_STREAM(COMMENT("Typedef double as Real_C"));
		HEADER_STREAM_T(TYPEDEF(double, Real_C));
	#elif(ENABLE_CUDA)
		// Typedef double as Real_C for CUDA
		HEADER_STREAM(COMMENT("Typedef float as Real_C for CUDA"));
		HEADER_STREAM_T(TYPEDEF(float, Real_C));
	#endif

    __BREAK_H__
	#if(ENABLE_DSPACE)
		// Typedef uint16_t as unsigned int for dSPACE
		HEADER_STREAM(COMMENT("Typedef unsigned int as uint16_t"));
		HEADER_STREAM_T(TYPEDEF(unsigned int, uint16_t));            
    #endif
                       
	__BREAK_H__
	// Structure for interface with BBO solvers
	HEADER_STREAM(COMMENT("Structure for interface with BBO solvers"));
	HEADER_STREAM("typedef struct {");
	HEADER_STREAM_T(VAR_ARRAY(X, nx));
	if (!this->_ParamPredict)
		HEADER_STREAM_T(VAR_ARRAY(P, np));
	else
		HEADER_STREAM_T(VAR_ARRAY2D(P_horizon, Nh, np));
	HEADER_STREAM_T("} ParaStructC");

	// Legacy codes for SQP interface
	__BREAK_H__
	// Interval structure
	HEADER_STREAM(COMMENT("Interval structure"));
	HEADER_STREAM("typedef struct { \n\
	Real_C Imin[NMAX]; \n\
	Real_C Imax[NMAX]; \n\
	} Interval;");

	__BREAK_H__
	// SQP data structure
	HEADER_STREAM(COMMENT("SQP data structure"));
	HEADER_STREAM("typedef struct { \n\
	Interval I_first; \n\
	Interval I_second; \n\
	Real_C QGmin; \n\
	Real_C QGmax; \n\
	Real_C PGmin; \n\
	Real_C PGmax; \n\
	unsigned nGz; \n\
	} SQP_data;");

	__BREAK_H__
	// Optimization dataset
	HEADER_STREAM(COMMENT("Optimization dataset"));
	HEADER_STREAM("typedef struct { \n\
	Interval I_init; \n\
	Real_C alpha[NMAX]; \n\
	unsigned Niter; \n\
	Real_C betaPlus; \n\
	Real_C betaMinus; \n\
	} optimset_SQP;");

	#if(ENABLE_CUDA)
	__BREAK_H__
	// Result vector
	HEADER_STREAM(COMMENT("Result vector"));
	HEADER_STREAM("typedef struct { \n\
	char JorG; \n\
	Real_C PCand; \n\
	} ResVec;");

	__BREAK_H__
		// Function pointer 
		HEADER_STREAM(COMMENT("Function pointer"));
		HEADER_STREAM_T("typedef Real_C(*funcPointer)(Real_C* P, const ParaStructC* params)");
	#endif

	#if(!ENABLE_CUDA && ENABLE_CPP)
		// C++ END INTERFACE
		__CPP_INTERFACE_END__
	#endif

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
	EXPORT_HEADER(INCLUDE_DIR, FILE_PNMPC_H_);
}