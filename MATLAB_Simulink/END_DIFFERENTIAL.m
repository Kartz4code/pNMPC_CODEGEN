function END_DIFFERENTIAL
% End differential guard
global isDiff;
DiffEquation = evalin('base','DiffEquation');
Fields = fieldnames(DiffEquation);
for i=1:size( struct2table(DiffEquation), 2)
    eval(['DiffEquation.' Fields{i} ' = ' 'vpa(eval([''DiffEquation(1).'' Fields{i}]),5);' ]);
    DiffEquationEqLHS = Fields{i};
    assignin('base', DiffEquationEqLHS, vpa(eval(['DiffEquation(1).' Fields{i}]),5));
end
if(isDiff)
    save('DiffEquation', 'DiffEquation');
end
isDiff = 0;
end
