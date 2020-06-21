#ifdef __cplusplus
extern "C" {
#endif
#include "PNMPC_ROOT.h"
#define MAX(a,b) max(a,b)
#define MIN(a,b) min(a,b)
#define AVG(a,b) (0.5*(a+b))
#define MID(a,b) (0.5*(b-a))
#define DISC(a,b,c) ((b*b) - (4*a*c))
#define SQUARE(x) ((x)*(x))
#define INT_MAX (9999999)
#define INT_MIN (-INT_MAX)
// Main function
void BBO_SQPFunction(Real_C P_in[NMAX], ParaStructC* params, Real_C(*funcJ)(Real_C[NMAX], const void*), Real_C(*funcG)(Real_C[NMAX], const void*), optimset_SQP* opt);
#ifdef __cplusplus
}
#endif