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

#include "SQP_BBO.h"
#include <stdio.h>
// All variables
SQP_data UI, SD;
Interval Izero;
Real_C P_temp, P_out, Pf_sol, Pmin_bar, Pmax_bar, Pc, beta, Jneg, J0, Jpos, aJ, bJ, cJ, PJ_solstar, QJ_solstar;
Real_C QJmin, QJmax, PJmin, PJmax, PCand, PCand1, PCand2, Gminus, G0, Gplus; Real_C P_inOld[NMAX];
Real_C aG, bG, cG, PG_solstar, QG_solstar, QGmin, QGmax, PGmin, PGmax, PG0plus, PG0minus, tempS;
Real_C funcJPCand, funcJPIndex, funcGPCand;
char JorG, Condition, has_changed;

inline Real_C norm2(Real_C* X, Real_C* Y, uint16_t size)
{
	tempS = 0;
	for (uint16_t i = 0; i < size; i++)
		tempS = tempS + SQUARE(X[i] - Y[i]);
	return sqrt(tempS);
}

inline Real_C norm2Vec(Real_C* X, uint16_t size)
{
    tempS = 0;
    for (uint16_t i = 0; i < size; i++)
		tempS = tempS + SQUARE(X[i]);
	return sqrt(tempS);
}

// Objective interface code
inline Real_C funcJInterface(Real_C P[NMAX], ParaStructC* params, Real_C(*funcJ)(Real_C[NMAX], const void*), Real_C(*funcG)(Real_C[NMAX], const void*), Real_C x, int index)
{
	P_temp = P[index];
	P[index] = x;
	P_out = funcJ(P, params);
	P[index] = P_temp;
	return P_out;
}

// Constraints interface code
inline Real_C funcGInterface(Real_C P[NMAX], ParaStructC* params, Real_C(*funcJ)(Real_C[NMAX], const void*), Real_C(*funcG)(Real_C[NMAX], const void*), Real_C x, int index)
{
	P_temp = P[index];
	P[index] = x;
	P_out = funcG(P, params);
	P[index] = P_temp;
	return P_out;
}

inline char isInSet(Interval* I1, Interval* I2, int index)
{
	return (I2->Imin[index] <= I1->Imin[index] && I2->Imax[index] >= I1->Imax[index]);
}

inline Real_C fSol(Real_C Pc, Real_C beta, Real_C bF, Real_C aF, Real_C Pmin_bar, Real_C Pmax_bar)
{
	if (aF != 0)
		Pf_sol = Pc - (beta*bF / (2*aF));
	else if (aF == 0 && bF <= 0)
		Pf_sol = INT_MAX;
	else if (aF == 0 && bF > 0)
		Pf_sol = INT_MIN;
	return MIN(Pmax_bar, MAX(Pmin_bar, Pf_sol));
}

inline Real_C QuadFunc(Real_C Pfstar, Real_C pc, Real_C aF, Real_C bF, Real_C cF, Real_C beta)
{
	if (aF == 0 && bF == 0)
		return cF;
	else
		return aF*SQUARE((Pfstar - pc) / beta) + bF*((Pfstar - pc) / beta) + cF;
}

inline Real_C SQPValue(Real_C P[NMAX], ParaStructC* params, Real_C(*funcJ)(Real_C[NMAX], const void*), Real_C(*funcG)(Real_C[NMAX], const void*), int index, Interval* Isearch, Real_C alpha)
{
	// Setting up variables for constraints and objectives
	Pmin_bar = MAX(Isearch->Imin[index], P[index] - alpha);
	Pmax_bar = MIN(Isearch->Imax[index], P[index] + alpha);
	Pc = AVG(Pmin_bar, Pmax_bar);
	beta = MID(Pmin_bar, Pmax_bar);

	// Objectives
	// Objective positve, 0, negative
	Jneg = funcJInterface(P, params, funcJ, funcG, Pmin_bar, index);
	J0 = funcJInterface(P, params, funcJ, funcG, Pc, index);
	Jpos = funcJInterface(P, params, funcJ, funcG, Pmax_bar, index);

	// Objective quadratic function
	aJ = AVG(Jpos, Jneg) - J0;
	bJ = MID(Jneg, Jpos);
	cJ = J0;

	// Optimal value of the approximated Objective quadratic function
	PJ_solstar = fSol(Pc, beta, bJ, aJ, Pmin_bar, Pmax_bar);
	QJ_solstar = QuadFunc(PJ_solstar, Pc, aJ, bJ, cJ, beta);

	// New min/max for objective function
	QJmin = MIN(Jneg, MIN(Jpos, QJ_solstar));
	QJmax = MAX(Jneg, MAX(Jpos, QJ_solstar));

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

inline SQP_data* SQPSearchSpace(Real_C P[NMAX], ParaStructC* params, Real_C(*funcJ)(Real_C[NMAX], const void*), Real_C(*funcG)(Real_C[NMAX], const void*), int index, Interval* Isearch, Real_C alpha)
{

	// Setting up variables for constraints and objectives
	Pmin_bar = MAX(Isearch->Imin[index], P[index] - alpha);
	Pmax_bar = MIN(Isearch->Imax[index], P[index] + alpha);
	Pc = AVG(Pmin_bar, Pmax_bar);
	beta = MID(Pmin_bar, Pmax_bar);

	// Constraints positive, 0, negative
	Gminus = funcGInterface(P, params, funcJ, funcG, Pmin_bar, index);
	G0 = funcGInterface(P, params, funcJ, funcG, Pc, index);
	Gplus = funcGInterface(P, params, funcJ, funcG, Pmax_bar, index);

	// Constraints quadratic function
	aG = AVG(Gplus, Gminus) - G0;
	bG = MID(Gminus, Gplus);
	cG = G0;

	// Optimal value of the approximated Constraints quadratic function
	PG_solstar = fSol(Pc, beta, bG, aG, Pmin_bar, Pmax_bar);
	QG_solstar = QuadFunc(PG_solstar, Pc, aG, bG, cG, beta);

	// New min/max for constraint function
	QGmin = MIN(Gminus, MIN(Gplus, QG_solstar));
	QGmax = MAX(Gminus, MAX(Gplus, QG_solstar));

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
	if (aG == 0 && bG == 0)
	{
		PG0plus = Pc;
		PG0minus = Pc;
	}
	else
	{
		PG0plus = Pc + beta*MAX((-bG + sqrt(DISC(aG, bG, cG))) / (2 * aG), (-bG - sqrt(DISC(aG, bG, cG))) / (2 * aG));
		PG0minus = Pc + beta*MIN((-bG + sqrt(DISC(aG, bG, cG))) / (2 * aG), (-bG - sqrt(DISC(aG, bG, cG))) / (2 * aG));
	}

	// Zero crossing interval
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

inline char checkCondition(Real_C P[NMAX], ParaStructC* params, Real_C(*funcJ)(Real_C[NMAX], const void*), Real_C(*funcG)(Real_C[NMAX], const void*), Real_C PCand, int index, char JorG)
{
	Condition = 0;
	switch (JorG)
	{
	case 0:
        funcJPCand = funcJInterface(P, params, funcJ, funcG, PCand, index);
        funcJPIndex = funcJInterface(P, params, funcJ, funcG, P[index], index);
        funcGPCand = funcGInterface(P, params, funcJ, funcG, PCand, index);
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

void BBO_SQPFunction(Real_C P_in[NMAX], ParaStructC* params, Real_C(*funcJ)(Real_C[NMAX], const void*), Real_C(*funcG)(Real_C[NMAX], const void*), optimset_SQP* opt)
{
	for (uint16_t i = 0; i < opt->Niter; i++)
	{
        has_changed = 0;
		for (uint16_t m = 0; m < NMAX; m++)
			P_inOld[m] = P_in[m];
		for (uint16_t j = 0; j < NMAX; j++)
		{
			SD = *SQPSearchSpace(P_in, params, funcJ, funcG, j, &opt->I_init, opt->alpha[j]);
			
            if (SD.QGmax > 0 && SD.QGmin < 0)
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
					PCand = ((funcJInterface(P_in, params, funcJ, funcG, PCand1, j) < funcJInterface(P_in, params, funcJ, funcG, PCand2, j)) ? PCand1 : PCand2);
					break;
				default:
					break;
				}

				if (funcGInterface(P_in, params, funcJ, funcG, PCand, j) > 0)
					JorG = 1;
				else
					JorG = 2;
			}        
            else if (SD.QGmax <= 0)
			{
				PCand = SQPValue(P_in, params, funcJ, funcG, j, &opt->I_init, opt->alpha[j]);
				JorG = 0;
			}
			else if (SD.QGmin > 0)
			{
				PCand = SD.PGmin;
				JorG = 1;
			}

			if (checkCondition(P_in, params, funcJ, funcG, PCand, j, JorG) == 1)
			{
				P_in[j] = PCand; has_changed = 1;
				opt->alpha[j] = opt->betaPlus*opt->alpha[j];
			}
			else
				opt->alpha[j] = MAX(0, opt->betaMinus*opt->alpha[j]);
		}
		if ((norm2(P_inOld, P_in, NMAX)/norm2Vec(P_inOld, NMAX)) <= TOL && has_changed == 1)
			break;
	}
}
