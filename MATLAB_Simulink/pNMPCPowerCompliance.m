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
        if(m == 0)
            break;
        end
    end
    charExpr = insertAfter(charExpr,m,'(');
    charExpr = insertAfter(charExpr,m,'(');
end
end

