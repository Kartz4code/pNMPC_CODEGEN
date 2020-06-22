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


/* 2D Crane control example */
/* Boiler plate code */

#if(ENABLE_MATLAB_INTERFACE)
	#include "mex.h"
#endif
	#include "pNMPC_headers.hpp"
#define PI 3.14159
using namespace pNMPC;

#if(ENABLE_MATLAB_INTERFACE)
	void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
#else
	void main()
#endif
{
	States r, rd, theta, thetad;
	Inputs p1; Params kappa1, kappa2;
	Real m = 200, M = 1500, l = 100, fr = pow(10, 5), umax = 80;
	Real thetamax = PI / 6, thetadmax = (2 * PI) / 9, g = 9.81;

	// State and input vectors
	MATHyperStates Xs(4, 1), Xd(4, 1);

	// Set point
	Xd[0][0] = kappa1 + kappa2; // Rest of the vector is zero

								// States
	Xs[0][0] = r;
	Xs[1][0] = rd;
	Xs[2][0] = theta;
	Xs[3][0] = thetad;

	// Input parameterization 
	BEGIN_PARAMETERIZATION_MAP
		ParameterizationMap u = p1;
	END_PARAMETERIZATION_MAP

	// Differential equations
	BEGIN_DIFFERENTIAL
		DiffEquation d1 = rd;
	DiffEquation d2 = (u + m*g*cos(theta)*sin(theta) + m*l*thetad*thetad*sin(theta)) / (M + m*(1 - (cos(theta)*cos(theta))));
	DiffEquation d3 = thetad;
	DiffEquation d4 = (-u*cos(theta) - m*l*thetad*thetad*cos(theta)*sin(theta) - (M - m)*g*sin(theta) - fr*thetad) / ((M + m*sin(theta)*sin(theta))*l);
	END_DIFFERENTIAL

		// Custom Constraint equations (State/State-input constraints) : gmin(x,u,p) <= g(x,u,p) <= gmax(x,u,p)
		BEGIN_CONSTRAINTS
		BEGIN_REGULAR_CONSTRAINTS
		ScalarConstraint G1 = { -umax <= u <= umax };
	ScalarConstraint G2 = { -thetamax <= theta <= thetamax };
	ScalarConstraint G3 = { -thetadmax <= thetad <= thetadmax };
	END_REGULAR_CONSTRAINTS
		END_CONSTRAINTS

		// Cost matrices
		MATReal Q = diag({ 100,1,1,1 });
	MATReal Qf = 100 * eye(4);
	Real R = 0.01;
	// Lagrangian and Mayer Cost
	BEGIN_OBJECTIVES
		// Lagrangian Cost 
		BEGIN_LAGRANGIAN
		ScalarObjective LC1 = 0.5*transpose(Xs - Xd)*Q*(Xs - Xd);
	ScalarObjective LC2 = 0.5*R*u*u;
	END_LAGRANGIAN
		//  Mayer Cost		
		BEGIN_MAYER
		ScalarObjective MC = 0.5*transpose(Xs - Xd)*Qf*(Xs - Xd);
	END_MAYER
		END_OBJECTIVES

		// Input parameterization
		ControlParamZ<Linear>{128, p1, -umax, umax };

	// pNMPC codegen object
	PNMPCGEN* pNMPC = PNMPCGEN::getSton();
	pNMPC->setInitialTime(0);
	pNMPC->setFinalTime(10);
	pNMPC->setStepSize(0.2);
	pNMPC->getSolver()->setNiter(5);
	pNMPC->setConstForm(CONST_FORM::FORM_1);
	pNMPC->setIntegrator(INTEGRATOR::RK45);
	pNMPC->setParameterPredict(0);

	// Generate C code
	pNMPC->genCCode();

	// Free memory
	pNMPC::pNMPC_free();
}