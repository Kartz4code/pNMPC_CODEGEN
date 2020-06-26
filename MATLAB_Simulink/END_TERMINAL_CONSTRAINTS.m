function END_TERMINAL_CONSTRAINTS
global isConst; 
global isTerm; 
TerminalConstraints = evalin('base','TerminalConstraints');
Fields = fieldnames(TerminalConstraints);
for i=1:size( struct2table(TerminalConstraints), 2)
    eval(['TerminalConstraints.' Fields{i} ' = ' 'vpa(eval([''TerminalConstraints(1).'' Fields{i}]),5);' ]);
    TerminalConstraintsEqLHS = Fields{i};
    assignin('base', TerminalConstraintsEqLHS, vpa(eval(['TerminalConstraints(1).' Fields{i}]),5));
end
if(isConst && isTerm)
    save('TerminalConstraints', 'TerminalConstraints');
end
isTerm = 0;
end
