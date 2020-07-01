#include "PNMPC_ROOT.cuh"
#include "BBOSQPCONSTCall.cuh"
#include "BBOSQPOBJCall.cuh"
#define AVG(a,b) (0.5f*(a+b))
#define MID(a,b) (0.5f*(b-a))
#define DISC(a,b,c) (b*b - 4.0f*a*c)
#define SQUARE(x) ((x)*(x))
// Solver prototype
__global__
void SQPCUDA_BBO(Real_C* P_in, Real_C* P_out, ParaStructC* params, funcPointer funcJ, funcPointer funcG, optimset_SQP* opt);
