#include "PNMPCGEN.hpp"
void PNMPCGEN::genCCodeParameterizationMap()
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

	// BODY
	HEADER_STREAM(HEADER_GUARD);
	HEADER_STREAM(INCLUDE(FILE_PNMPC_H_));

	// External libraries or dependent files
	std::string lhs, rhs; std::vector<std::string> coll;
	for (size_t i{ 0 }; i < this->_NOps_pme; i++)
	{
		lhs = db->accessOpSetPME().at(i).second.second.first;
		rhs = db->accessOpSetPME().at(i).second.second.second;
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

	// Function prototype (ParameterizationMap device)
	HEADER_STREAM(COMMENT("Function prototype (ParameterizationMap device)"));
	#if(ENABLE_CUDA)
		__CUDA_DEVICE_H__
	#endif
	HEADER_STREAM_T(TYPE_C_PTR << "ParameterInputMap(Real_C X[nx], Real_C U[nu], Real_C P[np])");

	#if(ENABLE_CUDA)
		// Function prototype (ParameterizationMap host)
		HEADER_STREAM(COMMENT("Function prototype (ParameterizationMap host)"));
		#if(ENABLE_CUDA)
			__CUDA_HOST_H__
		#endif
		HEADER_STREAM_T(TYPE_C_PTR << "ParameterInputMapHost(Real_C X[nx], Real_C U[nu], Real_C P[np])");
	#endif

	#if(!ENABLE_CUDA && ENABLE_CPP)
		// C++ END INTERFACE
		__CPP_INTERFACE_END__
	#endif

/*
==========================================================================================================
			  ______      ______   ______    _______   _______      _______ .______    __    __  
			 /      |    /      | /  __  \  |       \ |   ____|    /  _____||   _  \  |  |  |  | 
			|  ,----'   |  ,----'|  |  |  | |  .--.  ||  |__      |  |  __  |  |_)  | |  |  |  | 
			|  |        |  |     |  |  |  | |  |  |  ||   __|     |  | |_ | |   ___/  |  |  |  | 
			|  `----.   |  `----.|  `--'  | |  '--'  ||  |____    |  |__| | |  |      |  `--'  | 
			 \______|    \______| \______/  |_______/ |_______|    \______| | _|       \______/  
                                                                                     
 ==========================================================================================================
*/

	// C code
	// BODY
	CCODE_STREAM(INCLUDE(FILE_PARAMETERIZATION_MAP_H_));
	
	#if(1)
		// Result ParameterizationMap evaluation
		CCODE_STREAM(COMMENT("Result ParameterizationMap evaluation for host"));
		// Variable declaration for return 
		CCODE_STREAM_T(VAR_ARRAY(result_pme, NPME));
	#endif

	// Function definition (ParameterizationMap)
	CCODE_STREAM(COMMENT("Function definition (ParameterizationMap)"));
	#if(ENABLE_CUDA)
		__CUDA_DEVICE_C__
	#endif
	CCODE_STREAM(TYPE_C_PTR << "ParameterInputMap(Real_C X[nx], Real_C U[nu], Real_C P[np])");

	// Begin of function body
	__BODY_BEGIN__

	#if(ENABLE_CUDA)
		// Result ParameterizationMap evaluation
		CCODE_STREAM(COMMENT("Result ParameterizationMap evaluation"));
		// Variable declaration for return 
		CCODE_STREAM_T(VAR_ARRAY(result_pme, NPME));
	#endif

	// States, Inputs and Parameters declaration for Parameterization map
	this->StateControlParamdecl(this->oss_c);

	__BREAK_C__
		// Order of operations - ParameterizationMap
		if (this->_NOps_pme)
		{
			CCODE_STREAM(COMMENT("Order of operations - ParameterizationMap"));
			for (auto& item : coll)
				CCODE_STREAM_T(TYPE_C << item << EQUALS << db->accessAllHyperMaps()[item]->getFunctionName() + "(X,U,P)");
			for (size_t i{ 0 }; i < this->_NOps_pme; i++)
			{
				lhs = db->accessOpSetPME().at(i).second.second.first;
				rhs = db->accessOpSetPME().at(i).second.second.second;
				#if(ENABLE_CUDA)
					if (is_number(lhs))
						if (!is_integer(lhs))
							lhs = lhs + "f";
					if (is_number(rhs))
						if (!is_integer(rhs))
							rhs = rhs + "f";
				#endif
				if (sOps[(db->accessOpSetPME().at(i).second.first)] == sOps[Operation::max])
					CCODE_STREAM_T(TYPE_C << db->accessOpSetPME().at(i).first->getName() << EQUALS << Functions[16] << Paren[ParenOps::open] << lhs
						<< Paren[ParenOps::comma] << rhs << Paren[ParenOps::close]);
				else if (sOps[(db->accessOpSetPME().at(i).second.first)] == sOps[Operation::min])
					CCODE_STREAM_T(TYPE_C << db->accessOpSetPME().at(i).first->getName() << EQUALS << Functions[15] << Paren[ParenOps::open] << lhs
						<< Paren[ParenOps::comma] << rhs << Paren[ParenOps::close]);
				else if (sOps[(db->accessOpSetPME().at(i).second.first)] == sOps[Operation::operate])
					CCODE_STREAM_T(TYPE_C << db->accessOpSetPME().at(i).first->getName() << EQUALS << lhs
						<< Paren[ParenOps::open] << rhs << Paren[ParenOps::close]);
				else if (sOps[(db->accessOpSetPME().at(i).second.first)] == sOps[Operation::power])
					CCODE_STREAM_T(TYPE_C << db->accessOpSetPME().at(i).first->getName() << EQUALS << Functions[17] << Paren[ParenOps::open] << lhs
						<< Paren[ParenOps::comma] << rhs << Paren[ParenOps::close]);
				else
					CCODE_STREAM_T(TYPE_C << db->accessOpSetPME().at(i).first->getName() << EQUALS << lhs
						<< sOps[(db->accessOpSetPME().at(i).second.first)] << rhs);
			}
		}

	__BREAK_C__
		// Return ParameterizationMap result
		if (this->_Ntok_pme)
		{
			CCODE_STREAM(COMMENT("Return ParameterizationMap result"));
			for (size_t i{ 0 }; i < this->_Ntok_pme; i++)
				CCODE_STREAM_T("result_pme[" << i << "]" << EQUALS << db->accessTokensPME()[i][0]);
			CCODE_STREAM_T(RETURN(result_pme));
		}
		else
		{
			CCODE_STREAM(COMMENT("Return ParameterizationMap result"));
			CCODE_STREAM_T("result_pme[" << 0 << "]" << EQUALS << 0);
			CCODE_STREAM_T(RETURN(result_pme));
		}

	// End body
	__BODY_END__

	__BREAK_C__

/*
==========================================================================================================
			  ______      ______   ______    _______   _______      ______ .______    __    __  
			 /      |    /      | /  __  \  |       \ |   ____|    /      ||   _  \  |  |  |  | 
			|  ,----'   |  ,----'|  |  |  | |  .--.  ||  |__      |  ,----'|  |_)  | |  |  |  | 
			|  |        |  |     |  |  |  | |  |  |  ||   __|     |  |     |   ___/  |  |  |  | 
			|  `----.   |  `----.|  `--'  | |  '--'  ||  |____    |  `----.|  |      |  `--'  | 
			 \______|    \______| \______/  |_______/ |_______|    \______|| _|       \______/  
                                                                                                                                                                 
 ==========================================================================================================
*/
	// C code
	#if(ENABLE_CUDA)
		// Function definition (ParameterizationMap)
		CCODE_STREAM(COMMENT("Function definition (ParameterizationMapHost)"));
		#if(ENABLE_CUDA)
			__CUDA_HOST_C__
		#endif
			CCODE_STREAM(TYPE_C_PTR << "ParameterInputMapHost(Real_C X[nx], Real_C U[nu], Real_C P[np])");

		// Begin of function body
		__BODY_BEGIN__

		// States, Inputs and Parameters declaration for Parameterization map
		this->StateControlParamdecl(this->oss_c);

		__BREAK_C__
		// Order of operations - ParameterizationMap
		if (this->_NOps_pme)
		{
			CCODE_STREAM(COMMENT("Order of operations - ParameterizationMap"));
			for (auto& item : coll)
				CCODE_STREAM_T(TYPE_C << item << EQUALS << db->accessAllHyperMaps()[item]->getFunctionName() + "(X,U,P)");
			for (size_t i{ 0 }; i < this->_NOps_pme; i++)
			{
				lhs = db->accessOpSetPME().at(i).second.second.first;
				rhs = db->accessOpSetPME().at(i).second.second.second;
				if (sOps[(db->accessOpSetPME().at(i).second.first)] == sOps[Operation::max])
					CCODE_STREAM_T(TYPE_C << db->accessOpSetPME().at(i).first->getName() << EQUALS << Functions[16] << Paren[ParenOps::open] << lhs
						<< Paren[ParenOps::comma] << rhs << Paren[ParenOps::close]);
				else if (sOps[(db->accessOpSetPME().at(i).second.first)] == sOps[Operation::min])
					CCODE_STREAM_T(TYPE_C << db->accessOpSetPME().at(i).first->getName() << EQUALS << Functions[15] << Paren[ParenOps::open] << lhs
						<< Paren[ParenOps::comma] << rhs << Paren[ParenOps::close]);
				else if (sOps[(db->accessOpSetPME().at(i).second.first)] == sOps[Operation::operate])
					CCODE_STREAM_T(TYPE_C << db->accessOpSetPME().at(i).first->getName() << EQUALS << lhs
						<< Paren[ParenOps::open] << rhs << Paren[ParenOps::close]);
				else if (sOps[(db->accessOpSetPME().at(i).second.first)] == sOps[Operation::power])
					CCODE_STREAM_T(TYPE_C << db->accessOpSetPME().at(i).first->getName() << EQUALS << Functions[17] << Paren[ParenOps::open] << lhs
						<< Paren[ParenOps::comma] << rhs << Paren[ParenOps::close]);
				else
					CCODE_STREAM_T(TYPE_C << db->accessOpSetPME().at(i).first->getName() << EQUALS << lhs
						<< sOps[(db->accessOpSetPME().at(i).second.first)] << rhs);
			}
		}

		__BREAK_C__
		// Return ParameterizationMap result
		if (this->_Ntok_pme)
		{
			CCODE_STREAM(COMMENT("Return ParameterizationMap result"));
			for (size_t i{ 0 }; i < this->_Ntok_pme; i++)
				CCODE_STREAM_T("result_pme[" << i << "]" << EQUALS << db->accessTokensPME()[i][0]);
			CCODE_STREAM_T(RETURN(result_pme));
		}
		else
		{
			CCODE_STREAM(COMMENT("Return ParameterizationMap result"));
			CCODE_STREAM_T("result_pme[" << 0 << "]" << EQUALS << 0);
			CCODE_STREAM_T(RETURN(result_pme));
		}

		// End body
		__BODY_END__
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
	EXPORT_CCODE(SRC_DIR, FILE_PARAMETERIZATION_MAP_C_);
	EXPORT_HEADER(INCLUDE_DIR, FILE_PARAMETERIZATION_MAP_H_);
}
