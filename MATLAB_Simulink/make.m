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
ALL_FLAGS = [];
makeFile = -1;
%% Switch case 
% Make file cases
if(~isempty(varargin))
    if(strcmp(varargin{1},'clean'))
        makeFile = 0;
    elseif(strcmp(varargin{1},'SFUNC'))
        ALL_FLAGS = [SFUNC_FLAG ' ' CPP_FLAG ' ' DSPACE_FLAG];
    elseif(strcmp(varargin{1},'MEX'))
        ALL_FLAGS = [MEX_FLAG ' ' CPP_FLAG];
    elseif(strcmp(varargin{1},'MEXCUDA'))
        ALL_FLAGS = CUDA_FLAG;
    else
        Ex = MException('MyComponent:noSuchVariable', ...
        'Option %s not found',varargin{1});
        throw(Ex);
    end
else
    ALL_FLAGS = [SFUNC_FLAG ' ' CPP_FLAG];
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