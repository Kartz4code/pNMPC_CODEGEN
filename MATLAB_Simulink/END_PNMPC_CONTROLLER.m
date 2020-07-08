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

function END_PNMPC_CONTROLLER
global ispNMPC;
if(ispNMPC)
    SOURCE_BUILDER;
    if(exist('pNMPC_bin/TerminalConstraints.mat'))
        delete('pNMPC_bin/TerminalConstraints.mat');
    end
    if(exist('pNMPC_bin/RegularConstraints.mat'))
        delete('pNMPC_bin/RegularConstraints.mat');
    end
    if(exist('pNMPC_bin/LagrangianObjective.mat'))
        delete('pNMPC_bin/LagrangianObjective.mat');
    end
    if(exist('pNMPC_bin/MayerObjective.mat'))
        delete('pNMPC_bin/MayerObjective.mat');
    end
    if(exist('pNMPC_bin/ParameterizationMap.mat'))
        delete('pNMPC_bin/ParameterizationMap.mat');
    end
    if(exist('pNMPC_bin/DiffEquation.mat'))
        delete('pNMPC_bin/DiffEquation.mat');
    end
    if(exist('pNMPC_bin/Inputs.mat'))   
        load('pNMPC_bin/Inputs.mat');
        Fields = fieldnames(InputsStruct);
    for i = 1:size( struct2table(InputsStruct), 2)
        InputsRHS = Fields{i};
        eval(['delete(''' 'pNMPC_bin/ControlParameterization_' InputsRHS '.mat' ''');']);
    end
        delete('pNMPC_bin/Inputs.mat');
    end
    if(exist('pNMPC_bin/States.mat'))
        delete('pNMPC_bin/States.mat');
    end
    if(exist('pNMPC_bin/Params.mat'))
        delete('pNMPC_bin/Params.mat');
    end
end
ispNMPC = 0;
fileID = fopen('Source.cpp','r');
fclose(fileID);
end
