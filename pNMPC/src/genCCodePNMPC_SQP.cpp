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
void PNMPCGEN::genCCodePNMPC_SQP()
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
	HEADER_STREAM(C_HEADER_GUARD_BEGIN(FILE_PNMPC_SQP_H_));
	HEADER_STREAM(INCLUDE(FILE_PNMPC_H_));

	// Function prototype - Objective
	HEADER_STREAM(COMMENT("Function prototype - Objective"));
	#if(ENABLE_CUDA)
		__CUDA_DEVICE_H__
	#endif
	HEADER_STREAM_NT(TYPE_C << "BBO_FunctionOBJ(Real_C X[nx]" << Paren[ParenOps::comma]);
	for (auto& item : db->accessAllInputs())
		HEADER_STREAM_NT(TYPE_C << "U" << item->getName() << "[N" << item->getName() << "]" << Paren[ParenOps::comma]);
	if (this->_ParamPredict)
		HEADER_STREAM_T(VAR_ARRAY2D(P_horizon, Nh, np) << Paren[ParenOps::close]);
	else
		HEADER_STREAM_T(VAR_ARRAY(P, np) << Paren[ParenOps::close]);

	// Function prototype - Constraints
	HEADER_STREAM(COMMENT("Function prototype - Constraints"));
	#if(ENABLE_CUDA)
		__CUDA_DEVICE_H__
	#endif
	HEADER_STREAM_NT(TYPE_C << "BBO_FunctionCONST(Real_C X[nx]" << Paren[ParenOps::comma]);
	for (auto& item : db->accessAllInputs())
		HEADER_STREAM_NT(TYPE_C << "U" << item->getName() << "[N" << item->getName() << "]" << Paren[ParenOps::comma]);
	if (this->_ParamPredict)
		HEADER_STREAM_T(VAR_ARRAY2D(P_horizon, Nh, np) << Paren[ParenOps::close]);
	else
		HEADER_STREAM_T(VAR_ARRAY(P, np) << Paren[ParenOps::close]);

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
	CCODE_STREAM(INCLUDE(FILE_PNMPC_SQP_H_));
	CCODE_STREAM(INCLUDE(FILE_DYNAMICS_H_));
	if(this->_NC_reg)
		CCODE_STREAM(INCLUDE(FILE_CONSTRAINTS_H_));
	if(this->_termExist)
		CCODE_STREAM(INCLUDE(FILE_CONSTRAINTS_TERM_H_));
	if(this->_lagBool)
		CCODE_STREAM(INCLUDE(FILE_LAGRANGIAN_H_));
	if(this->_mayBool)
		CCODE_STREAM(INCLUDE(FILE_MAYER_H_));

	// Integrator headers
	if (this->_itype == INTEGRATOR::RK45)
		CCODE_STREAM(INCLUDE(FILE_INTEGRATOR_RK45_H_));
	if (this->_itype == INTEGRATOR::EU)
		CCODE_STREAM(INCLUDE(FILE_INTEGRATOR_EU_H_));
	if (this->_itype == INTEGRATOR::HEUN)
		CCODE_STREAM(INCLUDE(FILE_INTEGRATOR_HEUN_H_));

	// Parameterization headers
	CCODE_STREAM(INCLUDE(FILE_CP_PIECEWISE_H_));
	CCODE_STREAM(INCLUDE(FILE_CP_LINEAR_H_));

	#if(!ENABLE_CUDA)
		// Variable control array
		CCODE_STREAM(COMMENT("Variable control array"));
		CCODE_STREAM_T(VAR_ARRAY(U, nu));

		// Variable control array Heun case 
		if (this->_itype == INTEGRATOR::HEUN)
		{
			CCODE_STREAM(COMMENT("Variable next control array"));
			CCODE_STREAM_T(VAR_ARRAY(Un, nu));
		}

		// Variable parameter array
		if (this->_ParamPredict)
		{
			CCODE_STREAM(COMMENT("Variable parameter array"));
			CCODE_STREAM_T(VAR_ARRAY(P, np));
		}
	#endif


	// Input Control parameterization time points initialization 
	CCODE_STREAM(COMMENT("Input Control parameterization time points initialization"));
	for (auto& item : db->accessAllInputs())
	{
	#if(ENABLE_CUDA)
		__CUDA_CONSTANT_C__
	#endif
		CCODE_STREAM_NT(TYPE_C << "CP_SQP" << item->getName() << "[N" << item->getName() << "]" << EQUALS << "{");
		for (auto item_in = item->getCPTime().begin(); item_in != item->getCPTime().end(); item_in++)
		{
			CCODE_STREAM_NT(*item_in);
			if (item_in != item->getCPTime().end() - 1)
				CCODE_STREAM_NT(Paren[ParenOps::comma]);
		}
		CCODE_STREAM_T("}");
	}

	// BlackBox Function BBO_Function 
	CCODE_STREAM(COMMENT("BlackBox Function BBO_Function Objective"));
	#if(ENABLE_CUDA)
		__CUDA_DEVICE_C__
	#endif
	CCODE_STREAM_NT(TYPE_C << "BBO_FunctionOBJ(Real_C X[nx]" << Paren[ParenOps::comma]);
	for (auto& item : db->accessAllInputs())
		CCODE_STREAM_NT(TYPE_C << "U" << item->getName() << "[N" << item->getName() << "]" << Paren[ParenOps::comma]);
	if (this->_ParamPredict)
		CCODE_STREAM(VAR_ARRAY2D(P_horizon, Nh, np) << Paren[ParenOps::close]);
	else
		CCODE_STREAM(VAR_ARRAY(P, np) << Paren[ParenOps::close]);

	// Outer loop - Body begin
	__BODY_BEGIN__

	#if(ENABLE_CUDA)
		// Variable control array
		CCODE_STREAM(COMMENT("Variable control array"));
		CCODE_STREAM_T(VAR_ARRAY(U, nu));

		// Variable control array Heun case 
		if (this->_itype == INTEGRATOR::HEUN)
		{
			CCODE_STREAM(COMMENT("Variable next control array"));
			CCODE_STREAM_T(VAR_ARRAY(Un, nu));
		}

		// Variable parameter array
		if (this->_ParamPredict)
		{
			CCODE_STREAM(COMMENT("Variable parameter array"));
			CCODE_STREAM_T(VAR_ARRAY(P, np));
		}

		// Clock variable
		CCODE_STREAM(COMMENT("Clock variable"));
		CCODE_STREAM_T(TYPE_C << ASSIGN(current_time, 0));

		// Objective quantification
		CCODE_STREAM(COMMENT("Objective quantification"));
		CCODE_STREAM_T(TYPE_C << ASSIGN(obj_quant, 0));

	#elif(!ENABLE_CUDA)
		// Output and clock variable
		CCODE_STREAM(COMMENT("Clock variable"));
		CCODE_STREAM_T(TYPE_C << ASSIGN(current_time, 0));

		// Objective quantification
		CCODE_STREAM(COMMENT("Objective quantification"));
		CCODE_STREAM_T(TYPE_C << ASSIGN(obj_quant, 0));
	#endif

	// Run simulation over the prediction horizon 
	CCODE_STREAM(COMMENT("Run simulation over the prediction horizon"));
	CCODE_STREAM(FOR_UNSIGNED_FORWARD(i, 0, Nh));
	// Inner loop - Body begin
	__BODY_BEGIN__

	// Control input parameterization
	CCODE_STREAM(COMMENT("Control input parameterization"));
	for (size_t k{ 0 }; k < this->_nu; k++)
	{
		if (this->_itype == INTEGRATOR::HEUN)
		{
			if (db->accessAllInputs()[k]->getCPType() == CONTROLPARAM::PIECEWISE)
			{
				CCODE_STREAM_T("U[" << k << "]" << EQUALS << "max(min(ControlClockPieceWise(current_time, T, U" << db->accessAllInputs()[k]->getName() << ", CP_SQP" << db->accessAllInputs()[k]->getName() << ", N" << db->accessAllInputs()[k]->getName() << ")," << db->accessAllInputs()[k]->getUpperBound() << ")," << db->accessAllInputs()[k]->getLowerBound() << ")");
				CCODE_STREAM_T("Un[" << k << "]" << EQUALS << "max(min(ControlClockPieceWise(current_time+h, T, U" << db->accessAllInputs()[k]->getName() << ", CP_SQP" << db->accessAllInputs()[k]->getName() << ", N" << db->accessAllInputs()[k]->getName() << ")," << db->accessAllInputs()[k]->getUpperBound() << ")," << db->accessAllInputs()[k]->getLowerBound() << ")");
			}
			else if (db->accessAllInputs()[k]->getCPType() == CONTROLPARAM::LINEAR)
			{
				CCODE_STREAM_T("U[" << k << "]" << EQUALS << "max(min(ControlClockLinear(current_time, T, U" << db->accessAllInputs()[k]->getName() << ", CP_SQP" << db->accessAllInputs()[k]->getName() << ", N" << db->accessAllInputs()[k]->getName() << ")," << db->accessAllInputs()[k]->getUpperBound() << ")," << db->accessAllInputs()[k]->getLowerBound() << ")");
				CCODE_STREAM_T("Un[" << k << "]" << EQUALS << "max(min(ControlClockLinear(current_time+h, T, U" << db->accessAllInputs()[k]->getName() << ", CP_SQP" << db->accessAllInputs()[k]->getName() << ", N" << db->accessAllInputs()[k]->getName() << ")," << db->accessAllInputs()[k]->getUpperBound() << ")," << db->accessAllInputs()[k]->getLowerBound() << ")");
			}
		}
		else
		{
			if (db->accessAllInputs()[k]->getCPType() == CONTROLPARAM::PIECEWISE)
				CCODE_STREAM_T("U[" << k << "]" << EQUALS << "max(min(ControlClockPieceWise(current_time, T, U" << db->accessAllInputs()[k]->getName() << ", CP_SQP" << db->accessAllInputs()[k]->getName() << ", N" << db->accessAllInputs()[k]->getName() << ")," << db->accessAllInputs()[k]->getUpperBound() << ")," << db->accessAllInputs()[k]->getLowerBound() << ")");
			else if (db->accessAllInputs()[k]->getCPType() == CONTROLPARAM::LINEAR)
				CCODE_STREAM_T("U[" << k << "]" << EQUALS << "max(min(ControlClockLinear(current_time, T, U" << db->accessAllInputs()[k]->getName() << ", CP_SQP" << db->accessAllInputs()[k]->getName() << ", N" << db->accessAllInputs()[k]->getName() << ")," << db->accessAllInputs()[k]->getUpperBound() << ")," << db->accessAllInputs()[k]->getLowerBound() << ")");
		}
	}
	if (this->_ParamPredict)
	{
		// Parameter values over the prediction horizon
		CCODE_STREAM(COMMENT("Parameter values over the prediction horizon"));
		CCODE_STREAM(FOR_UNSIGNED_FORWARD(j, 0, np));
		CCODE_STREAM_T(ASSIGN(P[j], P_horizon[i][j]));
	}

	// Quantify Mayer
	if (this->_mayBool)
	{
		CCODE_STREAM(COMMENT("Quantify Mayer"));
		CCODE_STREAM(IF_EQUALS(i, Nh - 1));
		CCODE_STREAM_T(ASSIGN(obj_quant, obj_quant + Mayer(X, U, P)));
	}

	// Quantify Lagrangian
	if (this->_lagBool)
	{
		CCODE_STREAM(COMMENT("Quantify Lagrangian"));
		if (this->_lagBool && this->_mayBool)
			CCODE_STREAM(ELSE);
		CCODE_STREAM_T(ASSIGN(obj_quant, obj_quant + hstep*Lagrangian(X, U, P)));
	}

	// Integrate using one step solver
	CCODE_STREAM(COMMENT("Integrate using one step solver"));
	if (this->_itype == INTEGRATOR::RK45)
		CCODE_STREAM_T(ASSIGN(X, Integrator_RK45(X, U, P, Dynamics, hstep)));
	if (this->_itype == INTEGRATOR::EU)
		CCODE_STREAM_T(ASSIGN(X, Integrator_EU(X, U, P, Dynamics, hstep)));
	if (this->_itype == INTEGRATOR::HEUN)
		CCODE_STREAM_T(ASSIGN(X, Integrator_HEUN(X, U, Un, P, Dynamics, hstep)));

	// Time counter update
	CCODE_STREAM(COMMENT("Time counter update"));
	CCODE_STREAM_T(ASSIGN(current_time, current_time + hstep));

	// Inner loop - Body end
	__BODY_END__

	// Return BBO_Function out
	CCODE_STREAM(COMMENT("Return BBO_Function Objective out"));
	CCODE_STREAM_T(TYPE_C << ASSIGN(res_out, obj_quant));
	CCODE_STREAM_T(RETURN(res_out));

	// Outer loop - Body end
	__BODY_END__

	// BlackBox Function BBO_Function - Constraints
	CCODE_STREAM(COMMENT("BlackBox Function BBO_Function - Constraints"));
	#if(ENABLE_CUDA)
		__CUDA_DEVICE_C__
	#endif
	CCODE_STREAM_NT(TYPE_C << "BBO_FunctionCONST(Real_C X[nx]" << Paren[ParenOps::comma]);
	for (auto& item : db->accessAllInputs())
		CCODE_STREAM_NT(TYPE_C << "U" << item->getName() << "[N" << item->getName() << "]" << Paren[ParenOps::comma]);
	if (this->_ParamPredict)
		CCODE_STREAM(VAR_ARRAY2D(P_horizon, Nh, np) << Paren[ParenOps::close]);
	else
		CCODE_STREAM(VAR_ARRAY(P, np) << Paren[ParenOps::close]);

	// Outer loop - Body begin
	__BODY_BEGIN__

		if (this->_NC_reg || this->_NC_term)
		{	
			#if(ENABLE_CUDA)
				// Variable control array
				CCODE_STREAM(COMMENT("Variable control array"));
				CCODE_STREAM_T(VAR_ARRAY(U, nu));
			
				// Variable control array Heun case 
				if (this->_itype == INTEGRATOR::HEUN)
				{
					CCODE_STREAM(COMMENT("Variable next control array"));
					CCODE_STREAM_T(VAR_ARRAY(Un, nu));
				}

				// Variable parameter array
				if (this->_ParamPredict)
				{
					CCODE_STREAM(COMMENT("Variable parameter array"));
					CCODE_STREAM_T(VAR_ARRAY(P, np));
				}
			#endif	

			// Constraint array
			CCODE_STREAM(COMMENT("Constraint array"));
			CCODE_STREAM_T(VAR_PTR(CollConst));

			// Clock variable
			CCODE_STREAM(COMMENT("Clock variable"));
			CCODE_STREAM_T(TYPE_C << ASSIGN(current_time, 0));

			// Constraints quantification
			CCODE_STREAM(COMMENT("Constraints initialization"));
			CCODE_STREAM_T(TYPE_C << ASSIGN(con_quant, 0));				

			// Run simulation over the prediction horizon 
			CCODE_STREAM(COMMENT("Run simulation over the prediction horizon"));
			CCODE_STREAM(FOR_UNSIGNED_FORWARD(i, 0, Nh));

			// Inner loop - Body begin
			__BODY_BEGIN__

			// Control input parameterization
			CCODE_STREAM(COMMENT("Control input parameterization"));
			for (size_t k{ 0 }; k < this->_nu; k++)
			{
				if (this->_itype == INTEGRATOR::HEUN)
				{
					if (db->accessAllInputs()[k]->getCPType() == CONTROLPARAM::PIECEWISE)
					{
						CCODE_STREAM_T("U[" << k << "]" << EQUALS << "max(min(ControlClockPieceWise(current_time, T, U" << db->accessAllInputs()[k]->getName() << ", CP_SQP" << db->accessAllInputs()[k]->getName() << ", N" << db->accessAllInputs()[k]->getName() << ")," << db->accessAllInputs()[k]->getUpperBound() << ")," << db->accessAllInputs()[k]->getLowerBound() << ")");
						CCODE_STREAM_T("Un[" << k << "]" << EQUALS << "max(min(ControlClockPieceWise(current_time+h, T, U" << db->accessAllInputs()[k]->getName() << ", CP_SQP" << db->accessAllInputs()[k]->getName() << ", N" << db->accessAllInputs()[k]->getName() << ")," << db->accessAllInputs()[k]->getUpperBound() << ")," << db->accessAllInputs()[k]->getLowerBound() << ")");
					}
					else if (db->accessAllInputs()[k]->getCPType() == CONTROLPARAM::LINEAR)
					{
						CCODE_STREAM_T("U[" << k << "]" << EQUALS << "max(min(ControlClockLinear(current_time, T, U" << db->accessAllInputs()[k]->getName() << ", CP_SQP" << db->accessAllInputs()[k]->getName() << ", N" << db->accessAllInputs()[k]->getName() << ")," << db->accessAllInputs()[k]->getUpperBound() << ")," << db->accessAllInputs()[k]->getLowerBound() << ")");
						CCODE_STREAM_T("Un[" << k << "]" << EQUALS << "max(min(ControlClockLinear(current_time+h, T, U" << db->accessAllInputs()[k]->getName() << ", CP_SQP" << db->accessAllInputs()[k]->getName() << ", N" << db->accessAllInputs()[k]->getName() << ")," << db->accessAllInputs()[k]->getUpperBound() << ")," << db->accessAllInputs()[k]->getLowerBound() << ")");
					}
				}
				else
				{
					if (db->accessAllInputs()[k]->getCPType() == CONTROLPARAM::PIECEWISE)
						CCODE_STREAM_T("U[" << k << "]" << EQUALS << "max(min(ControlClockPieceWise(current_time, T, U" << db->accessAllInputs()[k]->getName() << ", CP_SQP" << db->accessAllInputs()[k]->getName() << ", N" << db->accessAllInputs()[k]->getName() << ")," << db->accessAllInputs()[k]->getUpperBound() << ")," << db->accessAllInputs()[k]->getLowerBound() << ")");
					else if (db->accessAllInputs()[k]->getCPType() == CONTROLPARAM::LINEAR)
						CCODE_STREAM_T("U[" << k << "]" << EQUALS << "max(min(ControlClockLinear(current_time, T, U" << db->accessAllInputs()[k]->getName() << ", CP_SQP" << db->accessAllInputs()[k]->getName() << ", N" << db->accessAllInputs()[k]->getName() << ")," << db->accessAllInputs()[k]->getUpperBound() << ")," << db->accessAllInputs()[k]->getLowerBound() << ")");
				}
			}

			if (this->_ParamPredict)
			{
				// Parameter values over the prediction horizon
				CCODE_STREAM(COMMENT("Parameter values over the prediction horizon"));
				CCODE_STREAM(FOR_UNSIGNED_FORWARD(j, 0, np));
				CCODE_STREAM_T(ASSIGN(P[j], P_horizon[i][j]));
			}

			if (this->_termExist)
			{
				// Quantify terminal constraints
				CCODE_STREAM(COMMENT("Quantify terminal constraints"));
				CCODE_STREAM(IF_EQUALS(i, Nh - 1));
				__BODY_BEGIN__
					CCODE_STREAM_T((ASSIGN(CollConst, TerminalConstraints(X, U, P))));
				CCODE_STREAM(FOR_UNSIGNED_FORWARD(j, 0, NC_TERM));
				if (this->__CN == CONST_FORM::FORM_1)
					CCODE_STREAM_T(ASSIGN(con_quant, con_quant + CollConst[j]));
				else if (this->__CN == CONST_FORM::FORM_2)
					CCODE_STREAM_T(ASSIGN(con_quant, max(con_quant, CollConst[j])));
				__BODY_END__	
			}

			if(this->_termExist && this->_regularExist)
				CCODE_STREAM(ELSE);

			if (this->_regularExist)
			{
				if (this->_termExist)
				__BODY_BEGIN__
				// Quantify regular constraints 
				CCODE_STREAM(COMMENT("Quantify regular constraints"));
				CCODE_STREAM_T((ASSIGN(CollConst, RegularConstraints(X, U, P))));
				CCODE_STREAM(FOR_UNSIGNED_FORWARD(j, 0, NC_REG));
				if (this->__CN == CONST_FORM::FORM_1)
					CCODE_STREAM_T(ASSIGN(con_quant, con_quant + CollConst[j]));
				else if (this->__CN == CONST_FORM::FORM_2)
					CCODE_STREAM_T(ASSIGN(con_quant, max(con_quant, CollConst[j])));
				if (this->_termExist)
				__BODY_END__
			}

			// Integrate using one step solver
			CCODE_STREAM(COMMENT("Integrate using one step solver"));
			if (this->_itype == INTEGRATOR::RK45)
				CCODE_STREAM_T(ASSIGN(X, Integrator_RK45(X, U, P, Dynamics, hstep)));
			if (this->_itype == INTEGRATOR::EU)
				CCODE_STREAM_T(ASSIGN(X, Integrator_EU(X, U, P, Dynamics, hstep)));
			if (this->_itype == INTEGRATOR::HEUN)
				CCODE_STREAM_T(ASSIGN(X, Integrator_HEUN(X, U, Un, P, Dynamics, hstep)));

			// Time counter update
			CCODE_STREAM(COMMENT("Time counter update"));
			CCODE_STREAM_T(ASSIGN(current_time, current_time + hstep));

			// Inner loop - Body end
			__BODY_END__

			// Return BBO_Function out
			CCODE_STREAM(COMMENT("Return BBO_Function out - Constraints"));
			CCODE_STREAM_T(TYPE_C << ASSIGN(res_out, con_quant));
			CCODE_STREAM_T(RETURN(res_out));
		}
		else {
			CCODE_STREAM_T(RETURN(0));
		}


	// Outer loop - Body end
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
	EXPORT_CCODE(SRC_DIR, FILE_PNMPC_SQP_C_);
	EXPORT_HEADER(INCLUDE_DIR, FILE_PNMPC_SQP_H_);
}
