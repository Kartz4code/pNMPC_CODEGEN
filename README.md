# PNMPC Code Generation Tool

## MATLAB/Simulink SETUP
To create a local repo of the project and use MATLAB/Simulink interface follow the steps 

- Install GIT for your OS and platform version.
  - You can download GIT from the link [GIT](https://git-scm.com/downloads).
- Clone the pNMPC CODEGEN project using the command `git clone https://github.com/Kartz4code/pNMPC_CODEGEN.git`.
- Open MATLAB and add the cloned directory into MATLAB's environment path variable.
- Run "make clean" to clear any previous make files. This will also generate a folder named `pNMPC_bin` where the compiled binaries are stored.

## MATLAB/Simulink interface (S-function) with 2D Crane example
### S-function generation 
- To generate S-Function make files, run "make SFUNC". This will populate the pNMPC_bin directory with the object files required during the S-function compilation process.
- Open `Main.m` file, which is an example for design of pNMPC controller for 2D-Crane control. 
- The syntax corresponds to MATLAB interface codes for the toolbox. Run the file to generate the S-function block in the `Crane.slx` Simulink file.
- Place the generated S-function block at appropriate place and connect the links. 
- General Tip: To generate the S-function, call `generateSFUNC(<Name of your Simulink file>)`. This is shown in the last line of `Main.m` file.
