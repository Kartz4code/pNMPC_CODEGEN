function END_MAYER
global isObj; 
global isMay; 
MayerObjective = evalin('base','MayerObjective');
Fields = fieldnames(MayerObjective);
for i=1:size( struct2table(MayerObjective), 2)
    eval(['MayerObjective.' Fields{i} ' = ' 'vpa(eval([''MayerObjective(1).'' Fields{i}]),5);' ]);
    MayerObjectiveEqLHS = Fields{i};
    assignin('base', MayerObjectiveEqLHS, vpa(eval(['MayerObjective(1).' Fields{i}]),5));
end
if(isObj && isMay)
    save('MayerObjective', 'MayerObjective');
end
isMay = 0;
end
