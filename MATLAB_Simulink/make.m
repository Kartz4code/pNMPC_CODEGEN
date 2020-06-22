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

function make(varargin)
% Check for bin folder presence 
if(~exist('pNMPC_bin','dir'))
    mkdir pNMPC_bin;
end
%% File directories
% Add current path
addpath(genpath(pwd));
% Compiled binaries path
BIN = [fullfile(pwd,'pNMPC_bin')];
% Includes path
INCLUDE = ['-I' fullfile(pwd,'\pNMPC\include')];

%% All flags for pre-compiled libraries
SFUNC_FLAG = '-DENABLE_SFUNCTION=1';
CPP_FLAG = '-DENABLE_CPP=1';
CUDA_FLAG = '-DENABLE_CUDA=1';
MEX_FLAG = '-DENABLE_MEX=1';
DSPACE_FLAG = '-DENABLE_DSPACE=1';
MATLAB_INTERFACE_FLAG = '-DENABLE_MATLAB_INTERFACE=1';
ALL_FLAGS = [];
makeFile = -1;
%% Switch case 
% Make file cases
if(~isempty(varargin))
    if(strcmp(varargin{1},'clean'))
        makeFile = 0;
    elseif(strcmp(varargin{1},'SFUNC'))
        ALL_FLAGS = [SFUNC_FLAG ' ' CPP_FLAG ' ' DSPACE_FLAG ' ' MATLAB_INTERFACE_FLAG];
    elseif(strcmp(varargin{1},'MEX'))
        ALL_FLAGS = [MEX_FLAG ' ' CPP_FLAG ' ' MATLAB_INTERFACE_FLAG];
    elseif(strcmp(varargin{1},'MEXCUDA'))
        ALL_FLAGS = [CUDA_FLAG ' ' MATLAB_INTERFACE_FLAG];
    else
        Ex = MException('MyComponent:noSuchVariable', ...
        'Option %s not found',varargin{1});
        throw(Ex);
    end
else
    ALL_FLAGS = [SFUNC_FLAG ' ' CPP_FLAG ' ' MATLAB_INTERFACE_FLAG];
end
%% Building and mexing
switch(makeFile)
    case 0
        delete('pNMPC_bin\*');
    otherwise
        SRC = []; fileList = dir([pwd '\pNMPC\src\*.cpp']);
        for i=1:size(fileList,1)
            SRC = [SRC ' ' pwd '\pNMPC\src\' fileList(i).name]; 
        end
        eval(['mex -c -outdir ' BIN ' ' ALL_FLAGS ' ' INCLUDE ' ' SRC])
end