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

#pragma once
#include "SQPCUDA_BBO.cuh"
#include <stdio.h>

// Objective interface code
__device__
Real_C funcJInterface(Real_C P[NMAX], const ParaStructC* params, Real_C(*funcJ)(Real_C[NMAX], const ParaStructC* params), Real_C(*funcG)(Real_C[NMAX], const ParaStructC* params), Real_C x, int index)
{
	Real_C P_temp = P[index];
	P[index] = x;
	Real_C P_out = funcJ(P, params);
	P[index] = P_temp;
	return P_out;
}

// Constraints interface code
__device__
Real_C funcGInterface(Real_C P[NMAX], const ParaStructC* params, Real_C(*funcJ)(Real_C[NMAX], const ParaStructC* params), Real_C(*funcG)(Real_C[NMAX], const ParaStructC* params), Real_C x, int index)
{
	Real_C P_temp = P[index];
	P[index] = x;
	Real_C P_out = funcG(P, params);
	P[index] = P_temp;
	return P_out;
}

__device__
bool isInSet(const Interval* I1, const Interval* I2, int index)
{
	return (I2->Imin[index] <= I1->Imin[index] && I2->Imax[index] >= I1->Imax[index]);
}

__device__
Real_C fSol(Real_C Pc, Real_C beta, Real_C bF, Real_C aF, Real_C Pmin_bar, Real_C Pmax_bar)
{
	Real_C Pf_sol;
	if (aF != 0)
		Pf_sol = Pc - (beta*bF / (2.0f * aF));
	else if (aF == 0 && bF <= 0)
		Pf_sol = INT_MAX;
	else if (aF == 0 && bF > 0)
		Pf_sol = INT_MIN;
	return min(Pmax_bar, max(Pmin_bar, Pf_sol));
}

__device__
Real_C QuadFunc(Real_C Pfstar, Real_C pc, Real_C aF, Real_C bF, Real_C cF, Real_C beta)
{
	if (aF == 0 && bF == 0)
		return cF;
	else
		return aF*SQUARE((Pfstar - pc) / beta) + bF*((Pfstar - pc) / beta) + cF;
}

__device__
Real_C SQPValue(Real_C P[NMAX], const ParaStructC* params, Real_C(*funcJ)(Real_C[NMAX], const ParaStructC* params), Real_C(*funcG)(Real_C[NMAX], const ParaStructC* params), int index, const Interval* Isearch, Real_C alpha)
{
	// Setting up variables for constraints and objectives
	Real_C Pmin_bar = max(Isearch->Imin[index], P[index] - alpha);
	Real_C Pmax_bar = min(Isearch->Imax[index], P[index] + alpha);
	Real_C Pc = AVG(Pmin_bar, Pmax_bar);
	Real_C beta = MID(Pmin_bar, Pmax_bar);

	// Objectives
	// Objective positve, 0, negative

	Real_C Jneg = funcJInterface(P, params, funcJ, funcG, Pmin_bar, index);
	Real_C J0 = funcJInterface(P, params, funcJ, funcG, Pc, index);
	Real_C Jpos = funcJInterface(P, params, funcJ, funcG, Pmax_bar, index);

	// Objective quadratic function
	Real_C aJ = AVG(Jpos, Jneg) - J0;
	Real_C bJ = MID(Jneg, Jpos);
	Real_C cJ = J0;

	// Optimal value of the approximated Objective quadratic function
	Real_C PJ_solstar = fSol(Pc, beta, bJ, aJ, Pmin_bar, Pmax_bar);
	Real_C QJ_solstar = QuadFunc(PJ_solstar, Pc, aJ, bJ, cJ, beta);


	// New min/max for objective function
	Real_C QJmin = min(Jneg, min(Jpos, QJ_solstar));
	Real_C QJmax = max(Jneg, max(Jpos, QJ_solstar));


	Real_C PJmin, PJmax;
	// PJmin for objective (Minimum)
	if (QJmin == Jneg)
		PJmin = Pmin_bar;
	else if (QJmin == Jpos)
		PJmin = Pmax_bar;
	else if (QJmin == QJ_solstar)
		PJmin = PJ_solstar;

	// PJmax for objective (Maximum)
	if (QJmax == Jneg)
		PJmax = Pmin_bar;
	else if (QJmax == Jpos)
		PJmax = Pmax_bar;
	else if (QJmax == QJ_solstar)
		PJmax = PJ_solstar;

	return PJmin;
}

__device__
SQP_data* SQPSearchSpace(Real_C P[NMAX], const ParaStructC* params, Real_C(*funcJ)(Real_C[NMAX], const ParaStructC* params), Real_C(*funcG)(Real_C[NMAX], const ParaStructC* params), int index, const Interval* Isearch, Real_C alpha)
{

	// Setting up variables for constraints and objectives
	Real_C Pmin_bar = max(Isearch->Imin[index], P[index] - alpha);
	Real_C Pmax_bar = min(Isearch->Imax[index], P[index] + alpha);
	Real_C Pc = AVG(Pmin_bar, Pmax_bar);
	Real_C beta = MID(Pmin_bar, Pmax_bar);

	// Constraints positive, 0, negative

	Real_C Gminus = funcGInterface(P, params, funcJ, funcG, Pmin_bar, index);
	Real_C G0 = funcGInterface(P, params, funcJ, funcG, Pc, index);
	Real_C Gplus = funcGInterface(P, params, funcJ, funcG, Pmax_bar, index);


	// Constraints quadratic function
	Real_C aG = AVG(Gplus, Gminus) - G0;
	Real_C bG = MID(Gminus, Gplus);
	Real_C cG = G0;

	// Optimal value of the approximated Constraints quadratic function
	Real_C PG_solstar = fSol(Pc, beta, bG, aG, Pmin_bar, Pmax_bar);
	Real_C QG_solstar = QuadFunc(PG_solstar, Pc, aG, bG, cG, beta);

	// New min/max for constraint function
	Real_C QGmin = min(Gminus, min(Gplus, QG_solstar));
	Real_C QGmax = max(Gminus, max(Gplus, QG_solstar));

	Real_C PGmin, PGmax;

	// PGmin for constraints (Minimum)
	if (QGmin == Gminus)
		PGmin = Pmin_bar;
	else if (QGmin == Gplus)
		PGmin = Pmax_bar;
	else if (QGmin == QG_solstar)
		PGmin = PG_solstar;

	// PGmax for constraints (Maximum)
	if (QGmax == Gminus)
		PGmax = Pmin_bar;
	else if (QGmax == Gplus)
		PGmax = Pmax_bar;
	else if (QGmax == QG_solstar)
		PGmax = PG_solstar;

	// Search space 
	Real_C PG0plus, PG0minus;
	if (aG == 0 && bG == 0)
	{
		PG0plus = Pc;
		PG0minus = Pc;
	}
	else
	{
		PG0plus = Pc + beta*max((-bG + sqrtf(DISC(aG, bG, cG))) / (2.0f * aG), (-bG - sqrtf(DISC(aG, bG, cG))) / (2.0f * aG));
		PG0minus = Pc + beta*min((-bG + sqrtf(DISC(aG, bG, cG))) / (2.0f * aG), (-bG - sqrtf(DISC(aG, bG, cG))) / (2.0f * aG));
	}

	// SQP data
	SQP_data UI;
	// Zero crossing interval
	Interval Izero;
	Izero.Imin[index] = PG0minus;
	Izero.Imax[index] = PG0plus;

	// Union_Interval

	if (PG0minus < Pmin_bar && Gminus <= 0)
	{
		UI.I_first.Imin[index] = Pmin_bar;
		UI.I_first.Imax[index] = PG0plus;
		UI.nGz = 1;
	}
	else if (PG0minus < Pmin_bar && Gplus <= 0)
	{
		UI.I_first.Imin[index] = PG0plus;
		UI.I_first.Imax[index] = Pmax_bar;
		UI.nGz = 1;
	}
	else if (PG0plus > Pmax_bar && Gminus <= 0)
	{
		UI.I_first.Imin[index] = Pmin_bar;
		UI.I_first.Imax[index] = PG0minus;
		UI.nGz = 1;
	}
	else if (isInSet(&Izero, Isearch, index) == 1 && aG > 0)
	{
		UI.I_first.Imin[index] = PG0minus;
		UI.I_first.Imax[index] = PG0plus;
		UI.nGz = 1;
	}
	else if (isInSet(&Izero, Isearch, index) == 1 && aG < 0)
	{
		UI.I_first.Imin[index] = Pmin_bar;
		UI.I_first.Imax[index] = PG0minus;
		UI.I_second.Imin[index] = PG0plus;
		UI.I_second.Imax[index] = Pmax_bar;
		UI.nGz = 2;
	}
	else
	{
		UI.I_first.Imin[index] = Isearch->Imin[index];
		UI.I_first.Imax[index] = Isearch->Imax[index];
		UI.nGz = 0;
	}

	// Minimum and maximum of objective and solution over interval
	UI.QGmax = QGmax;
	UI.QGmin = QGmin;
	UI.PGmax = PGmax;
	UI.PGmin = PGmin;

	// Return SQP_data object
	return &UI;
}

__device__
char checkCondition(Real_C P[NMAX], const ParaStructC* params, Real_C(*funcJ)(Real_C[NMAX], const ParaStructC* params), Real_C(*funcG)(Real_C[NMAX], const ParaStructC* params), Real_C PCand, int index, char JorG)
{
	char Condition = 0;
	switch (JorG)
	{
	case 0:
        Real_C funcJPCand = funcJInterface(P, params, funcJ, funcG, PCand, index);
        Real_C funcJPIndex = funcJInterface(P, params, funcJ, funcG, P[index], index);
        Real_C funcGPCand = funcGInterface(P, params, funcJ, funcG, PCand, index);
		if ((funcJPCand < funcJPIndex && funcGPCand <= 0) || (funcJPCand <= funcJPIndex && funcGPCand < 0))
			Condition = 1;
        return Condition;
	case 1:
		if (funcGInterface(P, params, funcJ, funcG, PCand, index) < funcGInterface(P, params, funcJ, funcG, P[index], index))
			Condition = 1;
		return Condition;
	case 2:
		if (funcJInterface(P, params, funcJ, funcG, PCand, index) < funcJInterface(P, params, funcJ, funcG, P[index], index) && funcGInterface(P, params, funcJ, funcG, PCand, index) <= 0)
			Condition = 1;
		return Condition;
	default:
		return Condition;
	}

}

__device__
ResVec runSQP(Real_C* P_in, const ParaStructC* params, Real_C(*funcJ)(Real_C* P, const ParaStructC* params), Real_C(*funcG)(Real_C* P, const ParaStructC* params), optimset_SQP* opt, int j)
{
	Real_C PCand, PCand1, PCand2; char JorG; 

	// Print for debug purposes
	SQP_data SD = *SQPSearchSpace(P_in, params, funcJ, funcG, j, &opt->I_init, opt->alpha[j]);
	if (SD.QGmax <= 0)
	{
		PCand = SQPValue(P_in, params, funcJ, funcG, j, &opt->I_init, opt->alpha[j]);
		JorG = 0;
	}
	else if (SD.QGmin > 0)
	{
		PCand = SD.PGmin;
		JorG = 1;
	}
	else if (SD.QGmax > 0 && SD.QGmin < 0)
	{
		switch (SD.nGz)
		{
		case 0:
			PCand = SQPValue(P_in, params, funcJ, funcG, j, &SD.I_first, opt->alpha[j]);
			break;
		case 1:
			PCand = SQPValue(P_in, params, funcJ, funcG, j, &SD.I_first, opt->alpha[j]);
			break;
		case 2:
			PCand1 = SQPValue(P_in, params, funcJ, funcG, j, &SD.I_first, opt->alpha[j]);
			PCand2 = SQPValue(P_in, params, funcJ, funcG, j, &SD.I_second, opt->alpha[j]);
			if ((funcJInterface(P_in, params, funcJ, funcG, PCand1, j) < funcJInterface(P_in, params, funcJ, funcG, PCand2, j)))
				PCand = PCand1;
			else
				PCand = PCand2;
			break;
		default:
			break;
		}

		if (funcGInterface(P_in, params, funcJ, funcG, PCand, j) > 0)
			JorG = 1;
		else
			JorG = 2;
	}
	ResVec result;
	result.JorG = JorG;
	result.PCand = PCand;
	return result;
}

__global__
void SQPCUDA_BBO(Real_C* P_inDevice, ParaStructC* params, const funcPointer funcJ, const funcPointer funcG, optimset_SQP* opt)
{
	const int j = blockIdx.x*blockDim.x + threadIdx.x; 
	Real_C temp[NMAX];
	if (j < NMAX)
	{
		for (uint16_t i = 0; i < opt->Niter; i++)
		{
            //temp[j] = P_inDevice[j];
			for (uint16_t l = 0; l < NMAX; l++)
				temp[l] = P_inDevice[l];

			// Distribution
			ResVec res = runSQP(temp, params, funcJ, funcG, opt, j);
			if (checkCondition(temp, params, funcJ, funcG, res.PCand, j, res.JorG))
			{
				temp[j] = res.PCand;
				opt->alpha[j] = opt->betaPlus*opt->alpha[j];
			}
			else 
				opt->alpha[j] = max(0.00f, opt->betaMinus*opt->alpha[j]);

            // Consensus
			P_inDevice[j] = temp[j];
		}
	}
}