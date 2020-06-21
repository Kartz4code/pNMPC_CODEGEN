#include "PNMPCGEN.hpp"
void PNMPCGEN::genCCodeLagrangian()
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

	HEADER_STREAM(HEADER_GUARD);
	HEADER_STREAM(INCLUDE(FILE_PNMPC_H_));
	HEADER_STREAM(INCLUDE(FILE_PARAMETERIZATION_MAP_H_));
	// External libraries or dependent files
	std::string lhs, rhs; size_t j{ 0 };  std::vector<std::string> coll;
	for (size_t i{ 0 }; i < this->_NOps_obj; i++)
	{
		if (db->accessScalarObjectiveEquations()[j]->getObjectiveType() == OBJTYPE::LAGRANGIAN)
		{
			lhs = db->accessOpSetObj().at(i).second.second.first;
			rhs = db->accessOpSetObj().at(i).second.second.second;
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
		if (db->accessTokensObj()[j][0] == db->accessOpSetObj()[i].first->getName())
			j++;
	}

	// Function prototype (Lagrangian)
	HEADER_STREAM(COMMENT("Function prototype (Lagrangian)"));
	#if(ENABLE_CUDA)
		__CUDA_DEVICE_H__
	#endif
	HEADER_STREAM_T( TYPE_C << "Lagrangian(Real_C X[nx], Real_C U[nu], Real_C P[np])");

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
	CCODE_STREAM(INCLUDE(FILE_LAGRANGIAN_H_));

	// Function definition (Lagrangian)
	CCODE_STREAM(COMMENT("Function definition (Lagrangian)"));
	#if(ENABLE_CUDA)
		__CUDA_DEVICE_C__
	#endif
	CCODE_STREAM( TYPE_C << "Lagrangian(Real_C X[nx], Real_C U[nu], Real_C P[np])");

	// Begin body
	__BODY_BEGIN__

	// States, Inputs and Parameters declaration for Lagrangian
	this->StateControlParamdecl(this->oss_c);

	__BREAK_C__

	j = 0; // j - refresh
	// Order of operations - Objectives
	CCODE_STREAM(COMMENT("Order of operations - Lagrangian"));
	for (auto& item : coll)
		if (this->_NPME)
			CCODE_STREAM_T(TYPE_C << item << EQUALS << db->accessAllHyperMaps()[item]->getFunctionName() + "(X,ParameterInputMap(X,U,P),P)");
		else
			CCODE_STREAM_T(TYPE_C << item << EQUALS << db->accessAllHyperMaps()[item]->getFunctionName() + "(X,U,P)");
	for (size_t i{ 0 }; i < this->_NOps_obj; i++)
	{
		if (db->accessScalarObjectiveEquations()[j]->getObjectiveType() == OBJTYPE::LAGRANGIAN)
		{
			lhs = db->accessOpSetObj().at(i).second.second.first;
			rhs = db->accessOpSetObj().at(i).second.second.second;
			#if(ENABLE_CUDA)
				if (is_number(lhs))
					if (!is_integer(lhs))
						lhs = lhs + "f";
				if (is_number(rhs))
					if (!is_integer(rhs))
						rhs = rhs + "f";
			#endif
			if (sOps[(db->accessOpSetObj().at(i).second.first)] == sOps[Operation::max])
				CCODE_STREAM_T(TYPE_C << db->accessOpSetObj().at(i).first->getName() << EQUALS << Functions[16] << Paren[ParenOps::open] << lhs
					<< Paren[ParenOps::comma] << rhs << Paren[ParenOps::close]);
			else if (sOps[(db->accessOpSetObj().at(i).second.first)] == sOps[Operation::min])
				CCODE_STREAM_T(TYPE_C << db->accessOpSetObj().at(i).first->getName() << EQUALS << Functions[15] << Paren[ParenOps::open] << lhs
					<< Paren[ParenOps::comma] << rhs << Paren[ParenOps::close]);
			else if (sOps[(db->accessOpSetObj().at(i).second.first)] == sOps[Operation::operate])
				CCODE_STREAM_T(TYPE_C << db->accessOpSetObj().at(i).first->getName() << EQUALS << lhs
					<< Paren[ParenOps::open] << rhs << Paren[ParenOps::close]);
			else if (sOps[(db->accessOpSetObj().at(i).second.first)] == sOps[Operation::power])
				CCODE_STREAM_T(TYPE_C << db->accessOpSetObj().at(i).first->getName() << EQUALS << Functions[17] << Paren[ParenOps::open] << lhs
					<< Paren[ParenOps::comma] << rhs << Paren[ParenOps::close]);
			else
				CCODE_STREAM_T(TYPE_C << db->accessOpSetObj().at(i).first->getName() << EQUALS << lhs
					<< sOps[(db->accessOpSetObj().at(i).second.first)] << rhs);
		}

		if (db->accessTokensObj()[j][0] == db->accessOpSetObj()[i].first->getName())
			j++;
	}

	__BREAK_C__
	// Return final objective
	CCODE_STREAM(COMMENT("Return Lagrangian"));
	std::string str{};
	for (size_t i{ 0 }; i < this->_Ntok_obj; i++)
	{
		if (db->accessScalarObjectiveEquations()[i]->getObjectiveType() == OBJTYPE::LAGRANGIAN)
			if (i == 0)
				str = db->accessTokensObj()[i][0];
			else
				str = str + sOps[Operation::add] + db->accessTokensObj()[i][0];
	}

	if(this->_lagBool)
		CCODE_STREAM_T(TYPE_C << "result_lagrangian" << EQUALS << str);
	else
		CCODE_STREAM_T(TYPE_C << "result_lagrangian" << EQUALS << 0);

	CCODE_STREAM_T(RETURN(result_lagrangian));

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
	EXPORT_CCODE(SRC_DIR, FILE_LAGRANGIAN_C_);
	EXPORT_HEADER(INCLUDE_DIR, FILE_LAGRANGIAN_H_);
}
