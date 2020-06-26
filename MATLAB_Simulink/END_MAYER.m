function END_MAYER
global isObj; 
global isMay; 
Mayer = evalin('base','MayerObjective');
Fields = fieldnames(Mayer);
for i=1:size( struct2table(Mayer), 2)
    eval(['MayerObjective.' Fields{i} ' = ' 'vpa(eval([''Mayer(1).'' Fields{i}]),5);' ]);
    MayerObjectiveEqLHS = Fields{i};
    assignin('base', MayerObjectiveEqLHS, vpa(eval(['Mayer(1).' Fields{i}]),5));
end
if(isObj && isMay)
    save('MayerObjective', 'Mayer');
end
isMay = 0;
end
