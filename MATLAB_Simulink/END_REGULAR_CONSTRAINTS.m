function END_REGULAR_CONSTRAINTS
global isConst; 
global isReg; 
RegularConstraints = evalin('base','RegularConstraints');
Fields = fieldnames(RegularConstraints);
for i=1:size( struct2table(RegularConstraints), 2)
    eval(['RegularConstraints.' Fields{i} ' = ' 'vpa(eval([''RegularConstraints(1).'' Fields{i}]),5);' ]);
    RegularConstraintsEqLHS = Fields{i};
    assignin('base', RegularConstraintsEqLHS, vpa(eval(['RegularConstraints(1).' Fields{i}]),5));
end
if(isConst && isReg)
    save('RegularConstraints', 'RegularConstraints');
end
isReg = 0;
end
