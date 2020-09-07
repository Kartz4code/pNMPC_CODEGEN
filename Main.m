%
%    This file is part of pNMPC software.
%    Copyright (c) 2020 GIPSA lab [https://github.com/Kartz4code/pNMPC_CODEGEN]
%
%    Main developer - Karthik Murali Madhavan Rathai
%
%    pNMPC - A Code Generation Software Tool For Implementation of Derivative Free
%    Parameterized NMPC Scheme for Embedded Control Systems
%    The software was developed by Karthik Murali Madhavan Rathai under the supervision of
%    Mazen Alamir and Olivier Sename affiliated to Univ. Grenoble Alpes, CNRS, Grenoble INP,
%    GIPSA lab, 38000 Grenoble, France.
%
%    pNMPC software is free software: you can redistribute it and/or modify
%    it under the terms of the GNU General Public License as published by
%    the Free Software Foundation, either version 3 of the License, or
%    (at your option) any later version.
%
%    pNMPC software is distributed in the hope that it will be useful,
%    but WITHOUT ANY WARRANTY; without even the implied warranty of
%    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
%    GNU General Public License for more details.
%
%    You should have received a copy of the GNU General Public License
%    along with pNMPC software.  If not, see <http://www.gnu.org/licenses/>.
%
%

clc; clear all; close all; 
make clean;
%% pNMPC MATLAB interface 
BEGIN_PNMPC_CONTROLLER
% States, inputs and parameters
States r rd theta thetad;
Inputs p1; 
Params kappa;
% Constant parameters
m = 200; M = 1500; l = 100; fr = 10^5; umax = 80;
thetamax = pi/6; thetadmax = (2*pi)/9; g = 9.81; 
% Cost weight matrix
Q = diag([100,1,1,1]); Qf = 100*eye(4); R = 0.01;
% State vector and parameter vector
Xvec = [r;rd;theta;thetad];
Xset = [kappa;0;0;0];
%% OCP code begins from here
% Parameterization map
BEGIN_PARAMETERIZATION_MAP
    ParameterizationMap.u = p1;
END_PARAMETERIZATION_MAP

% Differential equations
BEGIN_DIFFERENTIAL 
        DiffEquation.d1 = rd;
        DiffEquation.d2 = (u + m*g*cos(theta)*sin(theta) + m*l*thetad*thetad*sin(theta))/(M + m*(1-(cos(theta)*cos(theta))));
        DiffEquation.d3 = thetad;
        DiffEquation.d4 = (-u*cos(theta) - m*l*thetad*thetad*cos(theta)*sin(theta) - (M-m)*g*sin(theta) - fr*thetad)/((M + m*sin(theta)*sin(theta))*l);
END_DIFFERENTIAL

% Constraints 
BEGIN_CONSTRAINTS
    BEGIN_REGULAR_CONSTRAINTS
        RegularConstraints.R1 = (-umax <= u <= umax);
        RegularConstraints.R2 = (-thetamax <= theta <= thetamax);
        RegularConstraints.R3 = (-thetadmax <= thetad <= thetadmax);
    END_REGULAR_CONSTRAINTS
END_CONSTRAINTS    

% Objectives
BEGIN_OBJECTIVES
    BEGIN_LAGRANGIAN
		LagrangianObjective.LC1 = (Xvec - Xset).'*Q*(Xvec - Xset);
		LagrangianObjective.LC2 = R*u*u;
    END_LAGRANGIAN    
    %BEGIN_MAYER
        %MayerObjective.MC = 0.5*(Xvec - Xset).'*Qf*(Xvec - Xset);
    %END_MAYER
END_OBJECTIVES

% Control parameterization
ControlParameterization(p1, 'Linear', 5, -umax, umax);

% OCP setup
pNMPCCODEGEN.setInitialTime = 0;
pNMPCCODEGEN.setFinalTime = 10;
pNMPCCODEGEN.setStepSize = 0.2;
pNMPCCODEGEN.setIterations = 5;
pNMPCCODEGEN.setConstForm = 'FORM_1';
pNMPCCODEGEN.setIntegrator = 'RK45';

END_PNMPC_CONTROLLER
%% Generate SFUNCTION
make SFUNC;
generateSFUNC('CRANE2D');
