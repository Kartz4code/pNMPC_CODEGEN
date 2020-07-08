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

function SOURCE_BUILDER

% Beginning
fileID = fopen('Source.cpp','w');
fprintf(fileID, '#include "mex.h"\n');
fprintf(fileID, '#include "pNMPC_headers.hpp"\n');
fprintf(fileID, 'using namespace pNMPC;\n\n');
fprintf(fileID, 'void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])\n');
fprintf(fileID, '{\n');

%%%%%%%%%%%%% States declaration %%%%%%%%%%%%%
if(exist('States.mat'))
    load('States.mat');
    Fields = fieldnames(StatesStruct);
    fprintf(fileID, '// States declaration\n');
    for i=1:size( struct2table(StatesStruct), 2)
        StatesRHS = Fields{i};
        fprintf(fileID, ['States ' StatesRHS ';\n']);
    end
end
fprintf(fileID, '\n');

%%%%%%%%%%%%% Inputs declaration %%%%%%%%%%%%%
if(exist('Inputs.mat'))
    load('Inputs.mat');
    Fields = fieldnames(InputsStruct);
    fprintf(fileID, '// Inputs declaration\n');
    for i=1:size( struct2table(InputsStruct), 2)
        InputsRHS = Fields{i};
        fprintf(fileID, ['Inputs ' InputsRHS ';\n']);
    end
end
fprintf(fileID, '\n');

%%%%%%%%%%%%% Parameters declaration %%%%%%%%%%%%%
if(exist('Params.mat'))
    load('Params.mat');
        Fields = fieldnames(ParamsStruct);
        fprintf(fileID, '// Parameter declaration\n');
        for i=1:size( struct2table(ParamsStruct), 2)
            ParamsRHS = Fields{i};
            fprintf(fileID, ['Params ' ParamsRHS ';\n']);
        end
end
fprintf(fileID, '\n');

%%%%%%%%%%%%% ParameterizationMap equations %%%%%%%%%%%%%
if(exist('ParameterizationMap.mat'))
    load('ParameterizationMap.mat');
    Fields = fieldnames(ParameterizationMap);
    fprintf(fileID, '// Parameterization map equations declaration\n');
    fprintf(fileID, ['BEGIN_PARAMETERIZATION_MAP\n']);
    for i=1:size( struct2table(ParameterizationMap), 2)
        ParameterizationMapEqLHS = Fields{i};
        ParameterizationMapEqRHS = pNMPCPowerCompliance(char(eval(['ParameterizationMap(1).' Fields{i}])));
        fprintf(fileID, ['	ParameterizationMap ' ParameterizationMapEqLHS ' = ' ParameterizationMapEqRHS ';\n']);
    end
    fprintf(fileID, ['END_PARAMETERIZATION_MAP\n']);
end
fprintf(fileID, '\n');

%%%%%%%%%%%%% Differential equations %%%%%%%%%%%%%
if(exist('DiffEquation.mat'))
    load('DiffEquation.mat');
    Fields = fieldnames(DiffEquation);
    fprintf(fileID, '// Differential equations declaration\n');
    fprintf(fileID, ['BEGIN_DIFFERENTIAL\n']);
    for i=1:size( struct2table(DiffEquation), 2)
        DiffEqLHS = Fields{i};
        DiffEqRHS = pNMPCPowerCompliance(char(eval(['DiffEquation(1).' Fields{i}])));
        fprintf(fileID, ['	DiffEquation ' DiffEqLHS ' = ' DiffEqRHS ';\n']);
    end
    fprintf(fileID, ['END_DIFFERENTIAL\n']);
end
fprintf(fileID, '\n');

%%%%%%%%%%%%% Constraints %%%%%%%%%%%%%
if(exist('RegularConstraints.mat') || exist('TerminalConstraints.mat'))
fprintf(fileID, '// Constraints declaration\n');    
fprintf(fileID, ['BEGIN_CONSTRAINTS\n']);
%%%%%%%%%%%%% Regular constraints %%%%%%%%%%%%%
    if(exist('RegularConstraints.mat'))
        load('RegularConstraints.mat');
        Fields = fieldnames(RegularConstraints);
        fprintf(fileID, ['  BEGIN_REGULAR_CONSTRAINTS\n']);
        for i=1:size( struct2table(RegularConstraints), 2)
            RegEqLHS = Fields{i};
            RegEqRHS = pNMPCPowerCompliance(char(eval(['RegularConstraints(1).' Fields{i}])));
            fprintf(fileID, ['	ScalarConstraint ' RegEqLHS ' = ' RegEqRHS ';\n']);
        end    
        fprintf(fileID, ['  END_REGULAR_CONSTRAINTS\n']);
    end
%%%%%%%%%%%%% Terminal constraints %%%%%%%%%%%%%
    if(exist('TerminalConstraints.mat'))
        load('TerminalConstraints.mat');
        Fields = fieldnames(TerminalConstraints);
        fprintf(fileID, ['  BEGIN_TERMINAL_CONSTRAINTS\n']);
        for i=1:size( struct2table(TerminalConstraints), 2)
            TermEqLHS = Fields{i};
            TermEqRHS = pNMPCPowerCompliance(char(eval(['TerminalConstraints(1).' Fields{i}])));
            fprintf(fileID, ['	ScalarConstraint ' TermEqLHS ' = ' TermEqRHS ';\n']);
        end    
        fprintf(fileID, ['  END_TERMINAL_CONSTRAINTS\n']);
    end
fprintf(fileID, ['BEGIN_CONSTRAINTS\n']);
end
fprintf(fileID, '\n');

%%%%%%%%%%%%% Objective %%%%%%%%%%%%%
if(exist('LagrangianObjective.mat') || exist('MayerObjective.mat'))
fprintf(fileID, '// Objectives declaration\n');    
fprintf(fileID, ['BEGIN_OBJECTIVES\n']);
%%%%%%%%%%%%% Lagrangian objectives %%%%%%%%%%%%%
    if(exist('LagrangianObjective.mat'))
        load('LagrangianObjective.mat');
        Fields = fieldnames(LagrangianObjective);
        fprintf(fileID, ['  BEGIN_LAGRANGIAN\n']);
        for i=1:size( struct2table(LagrangianObjective), 2)
            LagEqLHS = Fields{i};
            LagEqRHS = pNMPCPowerCompliance(char(eval(['LagrangianObjective(1).' Fields{i}])));
            fprintf(fileID, ['	ScalarObjective ' LagEqLHS ' = ' LagEqRHS ';\n']);
        end    
        fprintf(fileID, ['  END_LAGRANGIAN\n']);
    end
%%%%%%%%%%%%% Mayer objectives %%%%%%%%%%%%%
    if(exist('MayerObjective.mat'))
        load('MayerObjective.mat');
        Fields = fieldnames(MayerObjective);
        fprintf(fileID, ['  BEGIN_MAYER\n']);
        for i=1:size( struct2table(MayerObjective), 2)
            MayEqLHS = Fields{i};
            MayEqRHS = pNMPCPowerCompliance(char(eval(['MayerObjective(1).' Fields{i}])));
            fprintf(fileID, ['	ScalarObjective ' MayEqLHS ' = ' MayEqRHS ';\n']);
        end    
        fprintf(fileID, ['  END_MAYER\n']);
    end
fprintf(fileID, ['END_OBJECTIVES\n']);
end
fprintf(fileID, '\n');

%%%%%%%%%%%%% Control Parameterization %%%%%%%%%%%%%
if(exist('Inputs.mat'))
    load('Inputs.mat');
    Fields = fieldnames(InputsStruct);
    fprintf(fileID, '// Control parameterization declaration\n');      
    for i=1:size( struct2table(InputsStruct), 2)
        InputsRHS = Fields{i};
       if(exist(['ControlParameterization_' InputsRHS '.mat']))
           load(['ControlParameterization_' InputsRHS '.mat']);
           fprintf(fileID, ['ControlParamZ<' ControlParamZ.Type '>{' num2str(ControlParamZ.Points) ',' ...
           char(ControlParamZ.SymInput) ',' num2str(ControlParamZ.InputMin) ',' num2str(ControlParamZ.InputMax) '};\n']);
       end
    end
end
fprintf(fileID, '\n');

%%%%%%%%%%%%% CodeGeneration %%%%%%%%%%%%%
fprintf(fileID, '// C code generation\n');  
fprintf(fileID,'PNMPCGEN* pNMPC = PNMPCGEN::getSton();\n');
pNMPCCODEGEN = evalin('base','pNMPCCODEGEN');
% Set initial time
if(isfield(pNMPCCODEGEN,'setInitialTime'))
    if(isnumeric(pNMPCCODEGEN.setInitialTime)  && pNMPCCODEGEN.setStepSize > 0)
        fprintf(fileID,['pNMPC->setInitialTime(' num2str(pNMPCCODEGEN.setInitialTime) ');\n']);
    else
        disp('Error***: ------------------- Enter a valid number -------------------');
        return;
    end
end
% Set final time
if(isfield(pNMPCCODEGEN,'setFinalTime'))
    if(isnumeric(pNMPCCODEGEN.setFinalTime) && pNMPCCODEGEN.setStepSize > 0)
        fprintf(fileID,['pNMPC->setFinalTime(' num2str(pNMPCCODEGEN.setFinalTime) ');\n']);
    else
        disp('Error***: ------------------- Enter a valid number -------------------');
        return;
    end
end
% Set stepsize
if(isfield(pNMPCCODEGEN,'setStepSize'))
    if(isnumeric(pNMPCCODEGEN.setStepSize) && pNMPCCODEGEN.setStepSize > 0)
        fprintf(fileID,['pNMPC->setStepSize(' num2str(pNMPCCODEGEN.setStepSize) ');\n']);
    else
        disp('Error***: ------------------- Enter a valid number -------------------');
        return;
    end
end
% Set solver iteration
if(isfield(pNMPCCODEGEN,'setIterations'))
    if(mod(pNMPCCODEGEN.setIterations,1) == 0 && pNMPCCODEGEN.setIterations > 0)
        fprintf(fileID,['pNMPC->getSolver()->setNiter(' num2str(pNMPCCODEGEN.setIterations) ');\n']);
    else
        disp('Error***: ------------------- Enter a valid postive and integral number -------------------');
        return;
    end
end
% Set solver tolerance
if(isfield(pNMPCCODEGEN,'setTolerance'))
    if(isnumeric(pNMPCCODEGEN.setIterations) && pNMPCCODEGEN.setIterations > 0)
        fprintf(fileID,['pNMPC->getSolver()->setTolerance(' num2str(pNMPCCODEGEN.setTolerance) ');\n']);
    else
        disp('Error***: ------------------- Enter a valid postive and integral number -------------------');
        return;
    end
end
% Set solver alpha value
if(isfield(pNMPCCODEGEN,'setAlpha'))
    if(isnumeric(pNMPCCODEGEN.setAlpha) && pNMPCCODEGEN.setAlpha > 0)
        fprintf(fileID,['pNMPC->getSolver()->setAlpha(' num2str(pNMPCCODEGEN.setAlpha) ');\n']);
    else
        disp('Error***: ------------------- Enter a valid postive and integral number -------------------');
        return;
    end
end
% Set solver betaminus value
if(isfield(pNMPCCODEGEN,'setBetaMinus'))
    if(isnumeric(pNMPCCODEGEN.setBetaMinus) && pNMPCCODEGEN.setBetaMinus > 0)
        fprintf(fileID,['pNMPC->getSolver()->setBetaMinus(' num2str(pNMPCCODEGEN.setBetaMinus) ');\n']);
    else
        disp('Error***: ------------------- Enter a valid postive and integral number -------------------');
        return;
    end
end
% Set solver betaplus value
if(isfield(pNMPCCODEGEN,'setBetaPlus'))
    if(isnumeric(pNMPCCODEGEN.setBetaPlus) && pNMPCCODEGEN.setBetaPlus > 0)
        fprintf(fileID,['pNMPC->getSolver()->setBetaPlus(' num2str(pNMPCCODEGEN.setBetaPlus) ');\n']);
    else
        disp('Error***: ------------------- Enter a valid postive and integral number -------------------');
        return;
    end
end
% Set constraint form
if(isfield(pNMPCCODEGEN,'setConstForm'))
    if(strcmp(pNMPCCODEGEN.setConstForm,'FORM_1') || strcmp(pNMPCCODEGEN.setConstForm,'FORM_2'))
        fprintf(fileID,['pNMPC->setConstForm(CONST_FORM::' pNMPCCODEGEN.setConstForm ');\n']);
    else
        disp('Error***: ------------------- Enter a valid constraint form -------------------');
        return;
    end
end
% Set integrator
if(isfield(pNMPCCODEGEN,'setIntegrator'))
    if(strcmp(pNMPCCODEGEN.setIntegrator,'RK45') || strcmp(pNMPCCODEGEN.setIntegrator,'EU') || strcmp(pNMPCCODEGEN.setIntegrator,'HEUN'))
        fprintf(fileID,['pNMPC->setIntegrator(INTEGRATOR::' pNMPCCODEGEN.setIntegrator ');\n']);
    else
        disp('Error***: ------------------- Enter a valid integrator -------------------');
        return;
    end
end
% Set parameter prediction
if(isfield(pNMPCCODEGEN,'setParameterPredict'))
    if(pNMPCCODEGEN.setParameterPredict == 0 || pNMPCCODEGEN.setParameterPredict == 1)
        fprintf(fileID,['pNMPC->setParameterPredict(' num2str(pNMPCCODEGEN.setParameterPredict) ');\n']);
    else
        disp('Error***: ------------------- Enter a boolean value -------------------');
        return;
    end
end
% Set need objective
if(isfield(pNMPCCODEGEN,'setNeedObjective'))
    if(pNMPCCODEGEN.setNeedObjective == 0 || pNMPCCODEGEN.setNeedObjective == 1)
        fprintf(fileID,['pNMPC->setNeedObjective(' num2str(pNMPCCODEGEN.setNeedObjective) ');\n']);
    else
        disp('Error***: ------------------- Enter a boolean value -------------------');
        return;
    end
end
% Set threads per block
if(isfield(pNMPCCODEGEN,'setThreadsPerBlock'))
    if(mod(pNMPCCODEGEN.setThreadsPerBlock,1) == 0 && pNMPCCODEGEN.setThreadsPerBlock > 0)
        fprintf(fileID,['pNMPC->setThreadsPerBlock(' num2str(pNMPCCODEGEN.setThreadsPerBlock) ');\n']);
    else
        disp('Error***: ------------------- Enter a valid postive and integral number -------------------');
        return;
    end
end


% Ending
fprintf(fileID, '// Generate C code\n');  
fprintf(fileID,['pNMPC->genCCode();\n']);
fprintf(fileID, '\n');
%%%%%%%%%%%%% Free memory %%%%%%%%%%%%%
fprintf(fileID, '// Free pNMPC memory\n'); 
fprintf(fileID, 'pNMPC::pNMPC_free();\n');
fprintf(fileID, '}\n');
fclose(fileID);
disp('------------------------- Source file generated successfully -------------------------')
end

