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
void PNMPCGEN::genCCodeTerminalConstraints()
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
	// Count the total number of inequalites 
	size_t counter_inner{ 0 };
	// Header files
	#if(!ENABLE_CUDA && ENABLE_CPP)
		// C++ BEGIN INTERFACE BEGIN
		__CPP_INTERFACE_BEGIN__
	#endif

	// Header guard
	HEADER_STREAM(C_HEADER_GUARD_BEGIN(FILE_CONSTRAINTS_TERM_H_));
	HEADER_STREAM(INCLUDE(FILE_PNMPC_H_));
	HEADER_STREAM(INCLUDE(FILE_PARAMETERIZATION_MAP_H_));
	// External libraries or dependent files
	std::string lhs, rhs; size_t j{ 0 }; std::vector<std::string> coll;
	for (size_t i{ 0 }; i < this->_NOps_const; i++)
	{
		if (db->accessScalarConstraintEquations()[j]->getConstraintType() == CONSTTYPE::TERMINAL)
		{
			lhs = db->accessOpSetConst().at(i).second.second.first;
			rhs = db->accessOpSetConst().at(i).second.second.second;
			if (!is_number(lhs) && !is_function(lhs))
			{
				if (db->accessAllHyperMaps()[lhs]->isExtern())
				{
					if (std::find(coll.begin(), coll.end(), lhs) == coll.end())
					{
					#if(ENABLE_CUDA)
						HEADER_STREAM("#include \"" + db->accessAllHyperMaps()[lhs]->getFunctionName() + ".cuh\"");
					#else
						HEADER_STREAM("#include \"" + db->accessAllHyperMaps()[lhs]->getFunctionName() + ".h\"");
					#endif			
					coll.emplace_back(lhs);
					}
				}
			}

			if (!is_number(rhs) && !is_function(rhs))
			{
				if (db->accessAllHyperMaps()[rhs]->isExtern())
				{
					if (std::find(coll.begin(), coll.end(), rhs) == coll.end())
					{
					#if(ENABLE_CUDA)
						HEADER_STREAM("#include \"" + db->accessAllHyperMaps()[rhs]->getFunctionName() + ".cuh\"");
					#else
						HEADER_STREAM("#include \"" + db->accessAllHyperMaps()[rhs]->getFunctionName() + ".h\"");
					#endif	
					coll.emplace_back(rhs);
					}
				}
			}
		}
		if (db->accessTokensConst()[j][0] == db->accessOpSetConst()[i].first->getName())
			j++;
	}

	HEADER_STREAM(COMMENT("Function prototype (Constraints)"));
	// Function prototype (Constraints)
	#if(ENABLE_CUDA)
		__CUDA_DEVICE_H__
	#endif
	HEADER_STREAM_T( TYPE_C_PTR << "TerminalConstraints(Real_C X[nx], Real_C U[nu], Real_C P[np])");

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
	CCODE_STREAM(INCLUDE(FILE_CONSTRAINTS_TERM_H_));
	#if(!ENABLE_CUDA)
			// Result
			CCODE_STREAM(COMMENT("Result Constraints return"));
			CCODE_STREAM_T(VAR_ARRAY(result_const_term, NC_TERM));
	#endif

	// Function prototype (Constraints)
	CCODE_STREAM(COMMENT("Function definition (Constraints)"));
	#if(ENABLE_CUDA)
		__CUDA_DEVICE_C__
	#endif
	CCODE_STREAM(TYPE_C_PTR << "TerminalConstraints(Real_C X[nx], Real_C U[nu], Real_C P[np])");

	// Begin body
	__BODY_BEGIN__

	#if(ENABLE_CUDA)
		// Result
		CCODE_STREAM(COMMENT("Result Constraints return"));
		CCODE_STREAM_T(VAR_ARRAY(result_const_term, NC_TERM));
		__BREAK_C__
	#endif

	// States, Inputs and Parameters declaration for Terminal constraints
	this->StateControlParamdecl(this->oss_c);

	__BREAK_C__

		j = 0; // j - refresh

	// Order of operations - Constraints
	if (this->_NC_term)
	{
		CCODE_STREAM(COMMENT("Order of operations - Constraints"));
		for (auto& item : coll)
			if (this->_NPME)
				CCODE_STREAM_T(TYPE_C << item << EQUALS << db->accessAllHyperMaps()[item]->getFunctionName() + "(X,ParameterInputMap(X,U,P),P)");
			else
				CCODE_STREAM_T(TYPE_C << item << EQUALS << db->accessAllHyperMaps()[item]->getFunctionName() + "(X,U,P)");
		for (size_t i{ 0 }; i < this->_NOps_const; i++)
		{
			if (db->accessScalarConstraintEquations()[j]->getConstraintType() == CONSTTYPE::TERMINAL)
			{
				lhs = db->accessOpSetConst().at(i).second.second.first;
				rhs = db->accessOpSetConst().at(i).second.second.second;
				#if(ENABLE_CUDA)
					if (is_number(lhs))
						if (!is_integer(lhs))
							lhs = lhs + "f";
					if (is_number(rhs))
						if (!is_integer(rhs))
							rhs = rhs + "f";
				#endif
				if (sOps[(db->accessOpSetConst().at(i).second.first)] == sOps[Operation::max])
					CCODE_STREAM_T(TYPE_C << db->accessOpSetConst().at(i).first->getName() << EQUALS << Functions[16] << Paren[ParenOps::open] << lhs
						<< Paren[ParenOps::comma] << rhs << Paren[ParenOps::close]);
				else if (sOps[(db->accessOpSetConst().at(i).second.first)] == sOps[Operation::min])
					CCODE_STREAM_T(TYPE_C << db->accessOpSetConst().at(i).first->getName() << EQUALS << Functions[15] << Paren[ParenOps::open] << lhs
						<< Paren[ParenOps::comma] << rhs << Paren[ParenOps::close]);
				else if (sOps[(db->accessOpSetConst().at(i).second.first)] == sOps[Operation::operate])
					CCODE_STREAM_T(TYPE_C << db->accessOpSetConst().at(i).first->getName() << EQUALS << lhs
						<< Paren[ParenOps::open] << rhs << Paren[ParenOps::close]);
				else if (sOps[(db->accessOpSetConst().at(i).second.first)] == sOps[Operation::power])
					CCODE_STREAM_T(TYPE_C << db->accessOpSetConst().at(i).first->getName() << EQUALS << Functions[17] << Paren[ParenOps::open] << lhs
						<< Paren[ParenOps::comma] << rhs << Paren[ParenOps::close]);
				else if (sOps[(db->accessOpSetConst().at(i).second.first)] == sOps[Operation::leq])
				{
					lhs = db->accessOpSetConst().at(i).second.second.first;
					rhs = db->accessOpSetConst().at(i).second.second.second;
					#if(ENABLE_CUDA)
						if (is_number(lhs))
							if (!is_integer(lhs))
								lhs = lhs + "f";
						if (is_number(rhs))
							if (!is_integer(rhs))
								rhs = rhs + "f";
					#endif
					CCODE_STREAM_T(TYPE_C << db->accessOpSetConst().at(i).first->getName() << EQUALS << Functions[16] << Paren[ParenOps::open] << lhs
						<< sOps[Operation::subract] << Paren[ParenOps::open] << rhs << Paren[ParenOps::close] << ",0" << Paren[ParenOps::close]);

					CCODE_STREAM("// Assign result const[" << counter_inner << "]");
					CCODE_STREAM_T("result_const_term[" << counter_inner << "]" << EQUALS << db->accessOpSetConst().at(i).first->getName());
					__BREAK_C__
					counter_inner++; i++;

					if (sOps[(db->accessOpSetConst().at(i).second.first)] == sOps[Operation::leq])
					{
						lhs = db->accessOpSetConst().at(i - 1).second.second.second;
						rhs = db->accessOpSetConst().at(i).second.second.second;
						#if(ENABLE_CUDA)
							if (is_number(lhs))
								if (!is_integer(lhs))
									lhs = lhs + "f";
							if (is_number(rhs))
								if (!is_integer(rhs))
									rhs = rhs + "f";
						#endif
						CCODE_STREAM_T(TYPE_C << db->accessOpSetConst().at(i).first->getName() << EQUALS << Functions[16] << Paren[ParenOps::open] << lhs
							<< sOps[Operation::subract] << Paren[ParenOps::open] << rhs << Paren[ParenOps::close] << ",0" << Paren[ParenOps::close]);
						CCODE_STREAM("// Assign result const[" << counter_inner << "]");
						CCODE_STREAM_T("result_const_term[" << counter_inner << "]" << EQUALS << db->accessOpSetConst().at(i).first->getName());
						__BREAK_C__
						counter_inner++; i++;
					}
					else if (sOps[(db->accessOpSetConst().at(i).second.first)] == sOps[Operation::geq])
					{
						lhs = db->accessOpSetConst().at(i).second.second.second;
						rhs = db->accessOpSetConst().at(i - 1).second.second.second;
						#if(ENABLE_CUDA)
							if (is_number(lhs))
								if (!is_integer(lhs))
									lhs = lhs + "f";
							if (is_number(rhs))
								if (!is_integer(rhs))
									rhs = rhs + "f";
						#endif
						CCODE_STREAM_T(TYPE_C << db->accessOpSetConst().at(i).first->getName() << EQUALS << Functions[16] << Paren[ParenOps::open] << lhs
							<< sOps[Operation::subract] << Paren[ParenOps::open] << rhs << Paren[ParenOps::close] << ",0" << Paren[ParenOps::close]);
						CCODE_STREAM("// Assign result const[" << counter_inner << "]");
						CCODE_STREAM_T("result_const_term[" << counter_inner << "]" << EQUALS << db->accessOpSetConst().at(i).first->getName());
						__BREAK_C__
						counter_inner++; i++;
					}
				}

				else if (sOps[(db->accessOpSetConst().at(i).second.first)] == sOps[Operation::geq])
				{
					lhs = db->accessOpSetConst().at(i).second.second.second;
					rhs = db->accessOpSetConst().at(i).second.second.first;
					#if(ENABLE_CUDA)
						if (is_number(lhs))
							if (!is_integer(lhs))
								lhs = lhs + "f";
						if (is_number(rhs))
							if (!is_integer(rhs))
								rhs = rhs + "f";
					#endif
					CCODE_STREAM_T(TYPE_C << db->accessOpSetConst().at(i).first->getName() << EQUALS << Functions[16] << Paren[ParenOps::open] << lhs
						<< sOps[Operation::subract] << Paren[ParenOps::open] << rhs << Paren[ParenOps::close] << ",0" << Paren[ParenOps::close]);
					CCODE_STREAM("// Assign result const[" << counter_inner << "]");
					CCODE_STREAM_T("result_const_term[" << counter_inner << "]" << EQUALS << db->accessOpSetConst().at(i).first->getName());
					__BREAK_C__
					counter_inner++; i++;

					if (sOps[(db->accessOpSetConst().at(i).second.first)] == sOps[Operation::leq])
					{
						lhs = db->accessOpSetConst().at(i - 1).second.second.second;
						rhs = db->accessOpSetConst().at(i).second.second.second;
						#if(ENABLE_CUDA)
							if (is_number(lhs))
								if (!is_integer(lhs))
									lhs = lhs + "f";
							if (is_number(rhs))
								if (!is_integer(rhs))
									rhs = rhs + "f";
						#endif
						CCODE_STREAM_T(TYPE_C << db->accessOpSetConst().at(i).first->getName() << EQUALS << Functions[16] << Paren[ParenOps::open] << lhs
							<< sOps[Operation::subract] << Paren[ParenOps::open] << rhs << Paren[ParenOps::close] << ",0" << Paren[ParenOps::close]);
						CCODE_STREAM("// Assign result const[" << counter_inner << "]");
						CCODE_STREAM_T("result_const_term[" << counter_inner << "]" << EQUALS << db->accessOpSetConst().at(i).first->getName());
						__BREAK_C__
						counter_inner++; i++;
					}
					else if (sOps[(db->accessOpSetConst().at(i).second.first)] == sOps[Operation::geq])
					{
						lhs = db->accessOpSetConst().at(i).second.second.second;
						rhs = db->accessOpSetConst().at(i - 1).second.second.second;
						#if(ENABLE_CUDA)
							if (is_number(lhs))
								if (!is_integer(lhs))
									lhs = lhs + "f";
							if (is_number(rhs))
								if (!is_integer(rhs))
									rhs = rhs + "f";
						#endif
						CCODE_STREAM_T(TYPE_C << db->accessOpSetConst().at(i).first->getName() << EQUALS << Functions[16] << Paren[ParenOps::open] << lhs
							<< sOps[Operation::subract] << Paren[ParenOps::open] << rhs << Paren[ParenOps::close] << ",0" << Paren[ParenOps::close]);
						CCODE_STREAM("// Assign result const[" << counter_inner << "]");
						CCODE_STREAM_T("result_const_term[" << counter_inner << "]" << EQUALS << db->accessOpSetConst().at(i).first->getName());
						__BREAK_C__
						counter_inner++; i++;
					}
				}
				else
					CCODE_STREAM_T(TYPE_C << db->accessOpSetConst().at(i).first->getName() << EQUALS << lhs
										  << sOps[(db->accessOpSetConst().at(i).second.first)] << rhs);
			}
			if (db->accessTokensConst()[j][0] == db->accessOpSetConst()[i].first->getName())
				j++;
		}
		CCODE_STREAM(COMMENT("Return final Constraints"));
		CCODE_STREAM_T(RETURN(result_const_term));
	}
	else
	{
		CCODE_STREAM(COMMENT("Return final Constraints"));
		CCODE_STREAM_T("result_const_term[" << 0 << "]" << EQUALS << 0);
		CCODE_STREAM_T(RETURN(result_const_term));
	}

	// End body
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
	EXPORT_CCODE(SRC_DIR, FILE_CONSTRAINTS_TERM_C_);
	EXPORT_HEADER(INCLUDE_DIR, FILE_CONSTRAINTS_TERM_H_);
}
