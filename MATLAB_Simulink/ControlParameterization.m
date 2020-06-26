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
function [ControlParamZ] = ControlParameterization(SymInput,Type,Points,InputMin,InputMax)
if(exist('Inputs.mat'))
    load('Inputs.mat');
    Fields = fieldnames(InputsStruct);
    isSymInput = 0;
    for i = 1:size( struct2table(InputsStruct), 2)
        InputsRHS = Fields{i};
        if(strcmp(char(SymInput),InputsRHS))
           isSymInput = 1;
           break; 
        end
    end
end

if(isSymInput)
    ControlParamZ.SymInput = SymInput;
else
    disp('Error***: ------------------- The symbolic object is not an input -------------------');
    return;
end

if(strcmp(Type,'Linear') || strcmp(Type,'Piecewise'))
    ControlParamZ.Type = Type;
else
    disp('Error***: ------------------- Enter a valid parameterization form -------------------');
    return;
end   
ControlParamZ.Points = Points;
ControlParamZ.InputMin = InputMin;
ControlParamZ.InputMax = InputMax;

save(['ControlParameterization_' char(SymInput)],'ControlParamZ');
end

