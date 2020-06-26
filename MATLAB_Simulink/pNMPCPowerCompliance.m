function [mCompatible] = pNMPCPowerCompliance(expr)
charExpr = char(expr);
mCompatible = strrep(charExpr,'^','%%');
end

