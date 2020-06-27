function END_PNMPC_CONTROLLER
global ispNMPC;
if(ispNMPC)
    SOURCE_BUILDER;
    if(exist('TerminalConstraints.mat'))
        delete('TerminalConstraints.mat');
    end
    if(exist('RegularConstraints.mat'))
        delete('RegularConstraints.mat');
    end
    if(exist('LagrangianObjective.mat'))
        delete('LagrangianObjective.mat');
    end
    if(exist('MayerObjective.mat'))
        delete('MayerObjective.mat');
    end
    if(exist('ParameterizationMap.mat'))
        delete('ParameterizationMap.mat');
    end
    if(exist('DiffEquation.mat'))
        delete('DiffEquation.mat');
    end
    if(exist('Inputs.mat'))   
        load('Inputs.mat');
        Fields = fieldnames(InputsStruct);
    for i = 1:size( struct2table(InputsStruct), 2)
        InputsRHS = Fields{i};
        eval(['delete(''' 'ControlParameterization_' InputsRHS '.mat' ''');']);
    end
        delete('Inputs.mat');
    end
    if(exist('States.mat'))
        delete('States.mat');
    end
    if(exist('Params.mat'))
        delete('Params.mat');
    end
end
ispNMPC = 0;
fileID = fopen('Source.cpp','r');
fclose(fileID);
end
