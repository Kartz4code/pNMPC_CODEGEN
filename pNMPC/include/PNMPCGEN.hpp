#pragma once
#include "pNMPC_headers.hpp"
// CODEGEN preprocessors
#define HEADER_GUARD "#pragma once"

#define COMMENT(x) "// " ## #x

#define DEFINE(x) "#define " ## #x
#define _DEFINE_(x,y) "#define " ## #x ## #y ## " "

#define INCLUDE_GLOBAL(x) "#include <" ## x ## ">"
#define INCLUDE(x) "#include \"" ## x ## "\""

#define TYPEDEF(x,y) "typedef " ## #x ## " " ## #y
#define TYPE_C "Real_C "
#define TYPE_C_PTR "Real_C* "
#define TYPE_C_PTR_PTR "Real_C** "

#define ASSIGN(x,y) #x ## " = " ## #y

#define RETURN(x) "return " ## #x

#define HEADER_STREAM(x) oss_h << x << std::endl
#define HEADER_STREAM_T(x) oss_h << x << ";" << std::endl 
#define HEADER_STREAM_NT(x) oss_h << x 

#define INTERFACE_STREAM(x) oss_in << x << std::endl;
#define INTERFACE_STREAM_T(x) oss_in << x << ";" << std::endl;
#define INTERFACE_STREAM_NT(x) oss_in << x 

#define CCODE_STREAM(x) oss_c << x << std::endl
#define CCODE_STREAM_T(x) oss_c << x << ";" << std::endl 
#define CCODE_STREAM_NT(x) oss_c << x

#define __BREAK_H__ oss_h << std::endl; 
#define __BREAK_C__ oss_c << std::endl; 

#define __CUDA_DEVICE_H__ oss_h << "__device__" << std::endl;
#define __CUDA_DEVICE_C__ oss_c << "__device__" << std::endl;

#define __CUDA_HOST_H__ oss_h << "__host__" << std::endl;
#define __CUDA_HOST_C__ oss_c << "__host__" << std::endl;

#define __CUDA_GLOBAL_H__ oss_h << "__global__" << std::endl;
#define __CUDA_GLOBAL_C__ oss_c << "__global__" << std::endl;

#define __CUDA_CONSTANT_H__ oss_h << "__constant__" << std::endl;
#define __CUDA_CONSTANT_C__ oss_c << "__constant__" << std::endl;

#define __BODY_BEGIN__ oss_c << "{" << std::endl;
#define __BODY_END__ oss_c << "}" << std::endl;

#define EQUALS Paren[ParenOps::space] + sOps[Operation::eq] + Paren[ParenOps::space]

#define ARRAY(x) "[" ## #x ## "]"
#define VAR(x) TYPE_C #x
#define VAR_PTR(x) TYPE_C_PTR #x
#define VAR_PTR_PTR(x) TYPE_C_PTR_PTR #x
#define VAR_ARRAY(x,y) TYPE_C #x ## "[" ## #y ## "]"
#define VAR_ARRAY2D(x,y,z) TYPE_C #x ## "[" ## #y ## "]" ## "[" ## #z ## "]" 

#define FOR_UNSIGNED_FORWARD(index,begin,end) "for(uint16_t " ## #index "=" ## #begin ## "; " ## #index ## "<" ## #end ## "; " ## #index ## "++)"
#define IF_EQUALS(x,y) "if(" ## "(" ## #x ## ")" ## " == " #y ## ")"
#define IF_NOT_EQUALS(x,y) "if(!(" ## "(" ## #x ## ")" ## " == " #y ## "))"
#define ELSE "else"


#define MEX_EXCEPTION(y,x) "mexErrMsgIdAndTxt(" ## y ## "," ## #x ")"
#define MEX_NOTDOUBLE "\"pNMPC:notDouble\""
#define MEX_NOTRIGHTDIMENSION "\"pNMPC:notRightDimension\""
#define MEX_NRHS "\"pNMPC:nrhs\""
#define MEX_NLHS "\"pNMPC:nlhs\""

// CPP INTERFACE CODE BEGIN
#define __CPP_INTERFACE_BEGIN__ std::ifstream cpp_begin("pNMPC/CPPInterface/CPP_INTERFACE_BEGIN.dat");\
								if (cpp_begin)\
								{\
									oss_h << cpp_begin.rdbuf();\
									cpp_begin.close();\
								}\

// CPP INTERFACE CODE END
#define __CPP_INTERFACE_END__ 	std::ifstream cpp_end("pNMPC/CPPInterface/CPP_INTERFACE_END.dat");\
								if (cpp_end)\
								{\
									oss_h << cpp_end.rdbuf();\
									cpp_end.close();\
								}\
// EXPORT HEADER
#define EXPORT_HEADER(y, x) std::ofstream outfile_h{ std::string(y) + x };\
						    outfile_h << oss_h.str() << std::endl;\
						    outfile_h.close();\
						    oss_h.str(std::string())\

#define EXPORT_CCODE(y,x) std::ofstream outfile_c{ std::string(y) + x };\
						  outfile_c << oss_c.str() << std::endl;\
						  outfile_c.close();\
						  oss_c.str(std::string())\

#define EXPORT_INTERFACE(y,x) std::ofstream outfile_in{ std::string(y) + x };\
						      outfile_in << oss_in.str();\
						      outfile_in.close();\
						      oss_in.str(std::string())\

// CODEGEN FILES
#define MATH "math.h"
#define STDLIB "stdlib.h"
#define STDINT "stdint.h"
#define TIME "time.h"
#define INCLUDE_DIR "CCODEGEN\\include\\"
#define SRC_DIR "CCODEGEN\\src\\"
#define INTERFACE_DIR "CCODEGEN\\interface\\"

#define FILE_SQP_INTERFACE_SFUN_H_ "SQPInterfaceSFUN.h"
#define FILE_SQP_INTERFACE_SFUN_C_ "SQPInterfaceSFUN.c"
#define FILE_MEX "mex.h"
#define FILE_SQP_H_ "SQP_BBO.h"
#define FILE_SQPCUDA_H_ "SQPCUDA_BBO.cuh"

#if(!ENABLE_CUDA)
	#define FILE_SQP_INTERFACE_MEX_H_ "pNMPC_MEX.h"
	#define FILE_SQP_INTERFACE_MEX_C_ "pNMPC_MEX.c"
	#define FILE_PNMPC_H_ "PNMPC_ROOT.h"
	#define FILE_PNMPC_SQP_C_  "PNMPC_SQP.c"
	#define FILE_PNMPC_SQP_H_ "PNMPC_SQP.h"
	#define FILE_DYNAMICS_C_ "Dynamics.c"
	#define FILE_DYNAMICS_H_ "Dynamics.h"
	#define FILE_PARAMETERIZATION_MAP_C_ "ParameterInputMap.c"
	#define FILE_PARAMETERIZATION_MAP_H_ "ParameterInputMap.h"
	#define FILE_CONSTRAINTS_C_ "RegularConstraints.c"
	#define FILE_CONSTRAINTS_H_ "RegularConstraints.h"
	#define FILE_CONSTRAINTS_TERM_C_ "TerminalConstraints.c"
	#define FILE_CONSTRAINTS_TERM_H_ "TerminalConstraints.h"
	#define FILE_LAGRANGIAN_H_ "Lagrangian.h"
	#define FILE_LAGRANGIAN_C_ "Lagrangian.c"
	#define FILE_MAYER_H_ "Mayer.h"
	#define FILE_MAYER_C_ "Mayer.c"
	#define FILE_SQP_BBOSQPOBJCall_H_ "BBOSQPOBJCall.h"
	#define FILE_SQP_BBOSQPOBJCall_C_ "BBOSQPOBJCall.c"
	#define FILE_SQP_BBOSQPCONSTCall_H_ "BBOSQPCONSTCall.h"
	#define FILE_SQP_BBOSQPCONSTCall_C_ "BBOSQPCONSTCall.c"
	#define FILE_SQP_INTERFACE_H_ "SQPInterface.h"
	#define FILE_SQP_INTERFACE_C_ "SQPInterface.c"
	#define FILE_INTEGRATOR_RK45_H_ "Integrator_RK45.h"
	#define FILE_INTEGRATOR_EU_H_ "Integrator_EU.h"
	#define FILE_INTEGRATOR_HEUN_H_ "Integrator_HEUN.h"
	#define FILE_CP_PIECEWISE_H_ "ControlClockPieceWise.h"
	#define FILE_CP_LINEAR_H_ "ControlClockLinear.h"
#elif(ENABLE_CUDA)
	#define FILE_SQP_INTERFACE_MEX_H_ "pNMPC_MEXCUDA.cuh"
	#define FILE_SQP_INTERFACE_MEX_C_ "pNMPC_MEXCUDA.cu"
	#define FILE_PNMPC_H_ "PNMPC_ROOT.cuh"
	#define FILE_PNMPC_SQP_C_  "PNMPC_SQP.cu"
	#define FILE_PNMPC_SQP_H_ "PNMPC_SQP.cuh"
	#define FILE_DYNAMICS_C_ "Dynamics.cu"
	#define FILE_DYNAMICS_H_ "Dynamics.cuh"
	#define FILE_PARAMETERIZATION_MAP_C_ "ParameterInputMap.cu"
	#define FILE_PARAMETERIZATION_MAP_H_ "ParameterInputMap.cuh"
	#define FILE_CONSTRAINTS_C_ "RegularConstraints.cu"
	#define FILE_CONSTRAINTS_H_ "RegularConstraints.cuh"
	#define FILE_CONSTRAINTS_TERM_C_ "TerminalConstraints.cu"
	#define FILE_CONSTRAINTS_TERM_H_ "TerminalConstraints.cuh"
	#define FILE_LAGRANGIAN_C_ "Lagrangian.cu"
	#define FILE_LAGRANGIAN_H_ "Lagrangian.cuh"
	#define FILE_MAYER_C_ "Mayer.cu"
	#define FILE_MAYER_H_ "Mayer.cuh"
	#define FILE_SQP_BBOSQPOBJCall_C_ "BBOSQPOBJCall.cu"
	#define FILE_SQP_BBOSQPOBJCall_H_ "BBOSQPOBJCall.cuh"
	#define FILE_SQP_BBOSQPCONSTCall_C_ "BBOSQPCONSTCall.cu"
	#define FILE_SQP_BBOSQPCONSTCall_H_ "BBOSQPCONSTCall.cuh"
	#define FILE_SQP_INTERFACE_C_ "SQPInterface.cu"
	#define FILE_SQP_INTERFACE_H_ "SQPInterface.cuh"
	#define FILE_INTEGRATOR_RK45_H_ "Integrator_RK45.cuh"
	#define FILE_INTEGRATOR_EU_H_ "Integrator_EU.cuh"
	#define FILE_INTEGRATOR_HEUN_H_ "Integrator_HEUN.cuh"
	#define FILE_CP_PIECEWISE_H_ "ControlClockPieceWise.cuh"
	#define FILE_CP_LINEAR_H_ "ControlClockLinear.cuh"
#endif

// CUDA headers
// Local headers
#define CUDA_H "cuda.h"
#define CUDA_RUNTIME_H "cuda_runtime.h"
#define DEVICE_LAUNCH_H "device_launch_parameters.h"
// Global headers
#define DEVICE_FUNC_H "device_functions.h"
#define CUDA_API_H "cuda_runtime_api.h"
// MATLAB interface
#define MATLAB_INTERFACE_ "MATLABInterface.txt"

class CODEGEN PNMPCGEN
{
private:
	// Header and source code string streams
	std::stringstream oss_h, oss_c, oss_in;

	// Switch off objective calculation
	bool _needObjective{ true };

	// Input array sizes vector for loops
	std::vector<std::string> _arrSizes;
	void setArrayVector();

	// Does Lagrangian and Mayer exist?
	bool _lagBool{ false }, _mayBool{ false };
	void lagMayBool();

	// Does Terminal constraint exist? 
	bool _termExist{ false }, _regularExist{false};
	void termConstExist();

	// C - code generation helper functions
	void StateControlParamdecl(const std::stringstream& oss);
	void InterfaceVariables(const std::stringstream& oss);
	void OptimizationParameters(const std::stringstream& oss);

	// C - code generation functions
	void genCCodeInterface();
	void genCCodeDynamics();
	void genCCodeParameterizationMap();
	void genCCodeRegularConstraints();
	void genCCodeTerminalConstraints();
	void genCCodeLagrangian();
	void genCCodeMayer();
	void genCCodePNMPC_SQP();
	void genCCodeBBONMCall();
	void genCCodeBBOSQPOBJCall();
	void genCCodeBBOSQPCONSTCall();
	void genCCodeSQPInterface();
	void genCCodeSQPInterface_SFUN();
	void genCCodeSQPInterface_MEX(); 

	// Integrator variables
	Real _init_time{ 0 };
	Real _final_time{ 1 };
	Real _step_size{ 0.1 };
	// SolverCodeGen nullptr
	SQPSolverCodeGen _SCG;
	// Integrator type
	INTEGRATOR _itype{INTEGRATOR::EU};
	// Parameter prediction
	bool _ParamPredict{false};
	// CUDA threads perblock
	size_t _TPB{ 64 };
	// Constraint norm type
	CONST_FORM __CN{CONST_FORM::FORM_1};
	size_t _Nh{ static_cast<size_t>((this->_final_time - this->_init_time) / this->_step_size) };

	// Size of States, Inputs and Parameters 
	size_t _nx{ db->accessAllStates().size() };
	size_t _nu{ db->accessAllInputs().size() };
	size_t _np{ db->accessAllParams().size() };

	// Operation set for dynamics, constraints and objectives
	size_t _NOps_diff{ db->accessOpSetDiff().size() }; size_t _Ntok_diff{ db->accessTokensDiff().size() };
	size_t _NOps_const{ db->accessOpSetConst().size() }; 
	size_t _NOps_obj{ db->accessOpSetObj().size() }; size_t _Ntok_obj{ db->accessTokensObj().size() };
	size_t _NOps_pme{ db->accessOpSetPME().size() }; size_t _Ntok_pme{ db->accessTokensPME().size() };

	// Size of differential, constraints and objective equations
	size_t _ND{ db->accessTokensDiff().size() };
	size_t _NC_reg{ this->getRegularConstraintSize() };
	size_t _NC_term{ this->getTerminalConstraintSize() };
	size_t _NO{ db->accessTokensObj().size() };
	size_t _NPME{ db->accessTokensPME().size() };
	
	// Privatize constructor
	PNMPCGEN();
	static PNMPCGEN* _instance;

public:
	static PNMPCGEN* getSton();
	void setInitialTime(const Real& init_time);
	void setFinalTime(const Real& final_time);
	void setStepSize(const Real& step_size);
	SQPSolverCodeGen* getSolver();
	void setConstForm(const CONST_FORM& CN);
	void setIntegrator(const INTEGRATOR& itype);
	void setParameterPredict(const bool& ParamPredict);
	void setThreadsPerBlock(const size_t& TPB);
	void setNeedObjective(const bool& sno);

	void genCCode();
	const size_t getRegularConstraintSize() const;
	const size_t getTerminalConstraintSize() const;
	~PNMPCGEN();
};

