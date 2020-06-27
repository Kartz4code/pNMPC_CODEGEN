function States(varargin)
for i=1:nargin
    eval(['StatesStruct.' varargin{i} ' = sym(''' varargin{i} ''');' ]);
    assignin('base', varargin{i},eval(['sym(''' varargin{i} ''')']))
end
    save('States','StatesStruct');
end

