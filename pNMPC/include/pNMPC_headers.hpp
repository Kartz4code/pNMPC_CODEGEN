/*
*    This file is part of pNMPC software.
*    Copyright (c) 2020 GIPSA lab [https://github.com/Kartz4code/pNMPC_CODEGEN]
*
*    Main developer - Karthik Murali Madhavan Rathai
*
*    pNMPC - A Code Generation Software Tool For Implementation of Derivative Free
*    Parameterized NMPC Scheme for Embedded Control Systems
*    The software was developed by Karthik Murali Madhavan Rathai under the supervision of
*    Mazen Alamir and Olivier Sename affiliated to Univ. Grenoble Alpes, CNRS, Grenoble INP,
*    GIPSA lab, 38000 Grenoble, France.
*
* 	 pNMPC software is free software: you can redistribute it and/or modify
*    it under the terms of the GNU General Public License as published by
*    the Free Software Foundation, either version 3 of the License, or
*    (at your option) any later version.
*
*	 pNMPC software is distributed in the hope that it will be useful,
*	 but WITHOUT ANY WARRANTY; without even the implied warranty of
*	 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*	 GNU General Public License for more details.
*
*	 You should have received a copy of the GNU General Public License
* 	 along with pNMPC software.  If not, see <http://www.gnu.org/licenses/>.
*
*/

#ifndef PNMPC_HEADERS_H
#define PNMPC_HEADERS_H
// All flags
#define CODEGEN
#include <iostream>
#include <chrono>
#include <cmath>
#include <vector>
#include <string>
#include <stdlib.h>
#include <algorithm>
#include <functional>
#include <sstream>
#include <map>
#include <memory>
#include <type_traits>
#include <fstream>
#include <stdio.h>
// All custom headers
#include "pNMPC.hpp"
#include "Database.hpp"
#include "Element.hpp"
#include "HyperStates.hpp"
#include "States.hpp"
#include "Inputs.hpp"
#include "Params.hpp"
#include "External.hpp"
#include "EquationProcessor.hpp"
#include "Builder.hpp"
#include "DiffEquation.hpp"
#include "ScalarConstraint.hpp"
#include "ScalarObjective.hpp"
#include "ParameterizationMap.hpp"
#include "BlockConstraint.hpp"
#include "ControlParamZ.hpp"
#include "Linear.hpp"
#include "Piecewise.hpp"
#include "SQPSolverCodeGen.hpp"
#include "PNMPCGEN.hpp"
#endif