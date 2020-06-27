function END_LAGRANGIAN
global isObj; 
global isLag; 
LagrangianObjective = evalin('base','LagrangianObjective');
Fields = fieldnames(LagrangianObjective);
for i=1:size( struct2table(LagrangianObjective), 2)
    eval(['LagrangianObjective.' Fields{i} ' = ' 'vpa(eval([''LagrangianObjective(1).'' Fields{i}]),5);' ]);
    LagrangianObjectiveEqLHS = Fields{i};
    assignin('base', LagrangianObjectiveEqLHS, vpa(eval(['LagrangianObjective(1).' Fields{i}]),5));
end
if(isObj && isLag)
    save('LagrangianObjective', 'LagrangianObjective');
end
isLag = 0;
end
