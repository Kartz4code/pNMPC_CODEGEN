function Params(varargin)
for i=1:nargin
    eval(['ParamsStruct.' varargin{i} ' = sym(''' varargin{i} ''');' ]);
    assignin('base', varargin{i},eval(['sym(''' varargin{i} ''')']))
end
    save('Params','ParamsStruct');
end


