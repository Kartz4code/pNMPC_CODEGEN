# PNMPC Code Generation Tool

## MATLAB/Simulink SETUP
To create a local repo of the project and use MATLAB/Simulink interface follow the steps 
```
- Install GIT Bash for your OS and platform version 
- Clone the pNMPC CODEGEN project using git clone https://github.com/Kartz4code/pNMPC_CODEGEN.git
- Open MATLAB and add the cloned directory into MATLAB's environment path variable 
- Run "make clean" to clear any previous make files 
- To generate S-Function make files, run "make SFUNC" and generate the S-function block with "generateSFUNC("<Name of your Simulink file>")" 
- To generate MEX make files, run "make MEX" and "generateMEX" 
- To generate CUDA MEX make files, run "make MEXCUDA" and "generateMEXCUDA" 
```

The 'Source.cpp' is the main file where the pNMPC controller codes are entered and the file name must not be changed. 

The example code with this repo provides a boiler plate code for implementation of pNMPC controller for control of 2D crane. The corresponding Simlink file is 'Crane2D.slx'   
