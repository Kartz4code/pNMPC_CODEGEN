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

