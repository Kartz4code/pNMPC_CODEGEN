function END_LAGRANGIAN
global isObj; 
global isLag; 
Lagrangian = evalin('base','LagrangianObjective');
Fields = fieldnames(Lagrangian);
for i=1:size( struct2table(Lagrangian), 2)
    eval(['LagrangianObjective.' Fields{i} ' = ' 'vpa(eval([''Lagrangian(1).'' Fields{i}]),5);' ]);
    LagrangianObjectiveEqLHS = Fields{i};
    assignin('base', LagrangianObjectiveEqLHS, vpa(eval(['Lagrangian(1).' Fields{i}]),5));
end
if(isObj && isLag)
    save('LagrangianObjective', 'Lagrangian');
end
isLag = 0;
end
