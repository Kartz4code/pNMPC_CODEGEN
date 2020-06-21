#include "PNMPCGEN.hpp"
void PNMPCGEN::genCCodeRegularConstraints()
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
	// Count the total number of inequalites 
	size_t counter_inner{ 0 };

	// Header files
	#if(!ENABLE_CUDA && ENABLE_CPP)
		// C++ BEGIN INTERFACE
		__CPP_INTERFACE_BEGIN__
	#endif

	HEADER_STREAM(HEADER_GUARD);
	HEADER_STREAM(INCLUDE(FILE_PNMPC_H_));
	HEADER_STREAM(INCLUDE(FILE_PARAMETERIZATION_MAP_H_));
	// External libraries or dependent files
	std::string lhs, rhs; size_t j{ 0 }; std::vector<std::string> coll;
	for (size_t i{ 0 }; i < this->_NOps_const; i++)
	{
		if (db->accessScalarConstraintEquations()[j]->getConstraintType() == CONSTTYPE::REGULAR)
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

	// Function prototype (Constraints)
	HEADER_STREAM(COMMENT("Function prototype (Constraints)"));
	#if(ENABLE_CUDA)
		__CUDA_DEVICE_H__
	#endif
	HEADER_STREAM_T(TYPE_C_PTR << "RegularConstraints(Real_C X[nx], Real_C U[nu], Real_C P[np])");

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
	CCODE_STREAM(INCLUDE(FILE_CONSTRAINTS_H_));

	#if(!ENABLE_CUDA)
			// Result
			CCODE_STREAM(COMMENT("Result Constraints return"));
			CCODE_STREAM_T(VAR_ARRAY(result_const_reg, NC_REG));
	#endif

	// Function prototype (Constraints)
	CCODE_STREAM(COMMENT("Function definition (Constraints)"));
	#if(ENABLE_CUDA)
		__CUDA_DEVICE_C__
	#endif
	CCODE_STREAM(TYPE_C_PTR << "RegularConstraints(Real_C X[nx], Real_C U[nu], Real_C P[np])");

	// Begin body
	__BODY_BEGIN__

	#if(ENABLE_CUDA)
		// Result
		CCODE_STREAM(COMMENT("Result Constraints return"));
		CCODE_STREAM_T(VAR_ARRAY(result_const_reg, NC_REG));
		__BREAK_C__
	#endif

	// States, Inputs and Parameters declaration for Regular constraints
	this->StateControlParamdecl(this->oss_c);

	__BREAK_C__

	j = 0; // j - refresh
	// Order of operations - Constraints
	if (this->_NC_reg)
	{
			CCODE_STREAM(COMMENT("Order of operations - Constraints"));
			for (auto& item : coll)
				if (this->_NPME)
					CCODE_STREAM_T(TYPE_C << item << EQUALS << db->accessAllHyperMaps()[item]->getFunctionName() + "(X,ParameterInputMap(X,U,P),P)");
				else
					CCODE_STREAM_T(TYPE_C << item << EQUALS << db->accessAllHyperMaps()[item]->getFunctionName() + "(X,U,P)");
			for (size_t i{ 0 }; i < this->_NOps_const; i++)
			{
				if (db->accessScalarConstraintEquations()[j]->getConstraintType() == CONSTTYPE::REGULAR)
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
						CCODE_STREAM_T("result_const_reg[" << counter_inner << "]" << EQUALS << db->accessOpSetConst().at(i).first->getName());
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
							CCODE_STREAM_T("result_const_reg[" << counter_inner << "]" << EQUALS << db->accessOpSetConst().at(i).first->getName());
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
							CCODE_STREAM_T("result_const_reg[" << counter_inner << "]" << EQUALS << db->accessOpSetConst().at(i).first->getName());
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
						CCODE_STREAM_T("result_const_reg[" << counter_inner << "]" << EQUALS << db->accessOpSetConst().at(i).first->getName());
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
							CCODE_STREAM_T("result_const_reg[" << counter_inner << "]" << EQUALS << db->accessOpSetConst().at(i).first->getName());
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
							CCODE_STREAM_T("result_const_reg[" << counter_inner << "]" << EQUALS << db->accessOpSetConst().at(i).first->getName());
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
			CCODE_STREAM_T(RETURN(result_const_reg));
	}
	else
	{
		CCODE_STREAM(COMMENT("Return final Constraints"));
		CCODE_STREAM_T("result_const_reg[" << 0 << "]" << EQUALS << 0);
		CCODE_STREAM_T(RETURN(result_const_reg));
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
	EXPORT_CCODE(SRC_DIR, FILE_CONSTRAINTS_C_);
	EXPORT_HEADER(INCLUDE_DIR, FILE_CONSTRAINTS_H_);
}
