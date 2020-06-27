function Inputs(varargin)
for i=1:nargin
    eval(['InputsStruct.' varargin{i} ' = sym(''' varargin{i} ''');' ]);
    assignin('base', varargin{i},eval(['sym(''' varargin{i} ''')']))
end
    save('Inputs','InputsStruct');
end


