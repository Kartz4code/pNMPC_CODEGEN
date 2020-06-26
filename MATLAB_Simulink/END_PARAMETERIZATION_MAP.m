function END_PARAMETERIZATION_MAP
global isParameterization;
ParameterizationMap = evalin('base','ParameterizationMap');
Fields = fieldnames(ParameterizationMap);
for i=1:size( struct2table(ParameterizationMap), 2)
    eval(['ParameterizationMap.' Fields{i} ' = ' 'vpa(eval([''ParameterizationMap(1).'' Fields{i}]),5);' ]);
    ParameterizationMapEqLHS = Fields{i};
    assignin('base', ParameterizationMapEqLHS, vpa(eval(['ParameterizationMap(1).' Fields{i}]),5));
end
if(isParameterization)
    save('ParameterizationMap', 'ParameterizationMap');
end
isParameterization = 0;
end
