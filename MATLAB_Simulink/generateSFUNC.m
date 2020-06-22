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

function generateSFUNC(filename)
% Check for Code generation folder presence 
if(~exist('CCODEGEN','dir'))
    mkdir CCODEGEN 
end
if(~exist('CCODEGEN/include','dir'))
    mkdir CCODEGEN/include 
end
if(~exist('CCODEGEN/src','dir'))
    mkdir CCODEGEN/src 
end
if(~exist('CCODEGEN/interface','dir'))
    mkdir CCODEGEN/interface 
end
% Add current directory to path
addpath(genpath(pwd)); addpath(genpath('CCODEGEN')); 
% Flush all the old code-generation files
delete('CCODEGEN\include\*'); 
delete('CCODEGEN\src\*');
 delete('CCODEGEN\interface\*');
SimulinkFileName = filename;
%% Building and mexing from C++ files 
srcFile = 'Source.cpp'; 
MATLAB_INTERFACE_FLAG = '-DENABLE_MATLAB_INTERFACE=1';
OBJ = []; fileList = dir([pwd '\pNMPC_bin\*.obj']);
for i=1:size(fileList,1)
    OBJ = [OBJ ' ' pwd '\pNMPC_bin\' fileList(i).name]; 
end
OBJ = [srcFile ' ' OBJ ];
INCLUDE = ['-I' fullfile(pwd,'\pNMPC\include')];
eval(['mex ' MATLAB_INTERFACE_FLAG ' ' INCLUDE ' ' OBJ])
% Run mex generated Source mex file to generate C code 
Source
disp('============= C code generation completed successfully ! ============= ');
%% S-func CODEGEN from C files
Simulink.importExternalCTypes('PNMPC_ROOT.h','IncludeDirs',[pwd '\CCODEGEN\include']);
def = legacy_code('initialize');
def.SFunctionName = 'pNMPC_toolbox';
def.SourceFiles = {'SQPInterfaceSFUN.c',
                   'BBOSQPCONSTCall.c',
                   'BBOSQPOBJCall.c',
                   'RegularConstraints.c',
                   'TerminalConstraints.c',
                   'Dynamics.c',
                   'Lagrangian.c',
                   'Mayer.c',
                   'PNMPC_SQP.c',
                   'SQP_BBO.c',
                   'ParameterInputMap.c'};

def.HeaderFiles = {'SQPInterfaceSFUN.h',
                   'BBOSQPCONSTCall.h',
                   'BBOSQPOBJCall.h',
                   'RegularConstraints.h',
                   'TerminalConstraints.h',
                   'Dynamics.h',
                   'Lagrangian.h',
                   'Mayer.h',
                   'PNMPC_SQP.h',
                   'SQP_BBO.h',
                   'ParameterInputMap.h',
                   'PNMPC_ROOT.h',
                   'ControlClockLinear.h',
                   'ControlClockPiecewise.h',
                   'Integrator_RK45.h',
                   'Integrator_EU.h',
                   'Integrator_HEUN.h'};
         
def.SrcPaths = {[pwd '\CCODEGEN\src'] [pwd '\pNMPC\Solver']};
               
def.IncPaths = {[pwd '\CCODEGEN\include'] 
                [pwd '\pNMPC\Integrators'] 
                [pwd '\pNMPC\Parameterization'] 
                [pwd '\pNMPC\Solver']};

def.Options.language = 'C';
%% File interface and port naming
fileID = fopen('CCODEGEN\interface\MATLABInterface.txt');
interfaceText = fscanf(fileID,'%c');
newStr = splitlines(interfaceText);
Inner = []; Outer = []; Func = [];
for i=1:length(newStr)
  if(~isempty(cell2mat(strfind(newStr(i),'In: '))))
    Inner = [Inner; extractAfter(newStr(i),'In: ')];
  elseif(~isempty(cell2mat(strfind(newStr(i),'Out: '))))
    Outer = [Outer; extractAfter(newStr(i),'Out: ')];
  elseif(~isempty(cell2mat(strfind(newStr(i),'Name: '))))
    Func = [Func; extractAfter(newStr(i),'Name: ')];
  end
end
Inner = Inner(~cellfun('isempty',Inner));
Outer = Outer(~cellfun('isempty',Outer));
Func = Func(~cellfun('isempty',Func));
PORT = []; PORT_DISP = [];
for i=1:length(Inner)
      PORT = [PORT char(['port_label(''''input'''',' num2str(i) ',''''' char(Inner(i)) ''''');'])]; 
      PORT_DISP = [PORT_DISP char(['port_label(''input'',' num2str(i) ',''' char(Inner(i)) ''');'])]; 
end
for i=1:length(Outer)
     PORT = [PORT char(['port_label(''''output'''',' num2str(i) ',''''' char(Outer(i)) ''''');'])]; 
     PORT_DISP = [PORT_DISP char(['port_label(''output'',' num2str(i) ',''' char(Outer(i)) ''');'])]; 
end
fclose(fileID);
%% SFUNC Final
def.OutputFcnSpec = char(Func);
legacy_code('sfcn_cmex_generate',def);
legacy_code('generate_for_sim',def);
legacy_code('slblock_generate', def, SimulinkFileName);
save_system(SimulinkFileName);
p = Simulink.Mask.get([SimulinkFileName '/pNMPC_toolbox']);
p.Display = ['disp(''{\bf\fontsize{40}\fontname{Microsoft Himalaya}pNMPC}'', ''texmode'',''on'');' PORT_DISP];
StR = ['set_param(''' [SimulinkFileName] '/pNMPC_toolbox'', ''MaskDisplay'', ''disp(''''{\bf\fontsize{40}\fontname{Microsoft Himalaya}pNMPC}'''', ''''texmode'''',''''on''''), ' PORT '''' ')'];
p.set('Initialization',StR);
p.set('IconRotate','off');
end