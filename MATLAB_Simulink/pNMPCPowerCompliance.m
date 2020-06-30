%
%    This file is part of pNMPC software.
%    Copyright (c) 2020 GIPSA lab [https://github.com/Kartz4code/pNMPC_CODEGEN]
%
%    Main developer - Karthik Murali Madhavan Rathai
%
%    pNMPC - A Code Generation Software Tool For Implementation of Derivative Free
%    Parameterized NMPC Scheme for Embedded Control Systems
%    The software was developed by Karthik Murali Madhavan Rathai under the supervision of
%    Mazen Alamir and Olivier Sename affiliated to Univ. Grenoble Alpes, CNRS, Grenoble INP,
%    GIPSA lab, 38000 Grenoble, France.
%
%    pNMPC software is free software: you can redistribute it and/or modify
%    it under the terms of the GNU General Public License as published by
%    the Free Software Foundation, either version 3 of the License, or
%    (at your option) any later version.
%
%    pNMPC software is distributed in the hope that it will be useful,
%    but WITHOUT ANY WARRANTY; without even the implied warranty of
%    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
%    GNU General Public License for more details.
%
%    You should have received a copy of the GNU General Public License
%    along with pNMPC software.  If not, see <http://www.gnu.org/licenses/>.
%
%

function [charExpr] = pNMPCPowerCompliance(expr)
% Operation array 
arrayOps = ['+','-','*','/'];
% Convert to character array
charExpr = char(expr); 
% Count the number of power symbols
countArray = count(charExpr,'^');
for i=1:countArray
    l = 0;
    powerArray = strfind(charExpr,'^');
    m = powerArray(i);
    charExpr = insertBefore(charExpr,m,')');
    charExpr = insertBefore(charExpr,m+3,')');
    while(~ismember(charExpr(m),arrayOps) || ~(l == 1))
        if(strcmp(charExpr(m),')'))
            l = l+1;
        elseif(strcmp(charExpr(m),'('))
            l = l-1;
        end
        m = m - 1;
        if(m == 0 || l <= 0)
            break;
        end
    end
    charExpr = insertAfter(charExpr,m,'(');
    charExpr = insertAfter(charExpr,m,'(');
end
end

