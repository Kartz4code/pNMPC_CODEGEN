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

#include "Builder.hpp"

// Differential build
bool Builder::_diffBuild{false};
// Constraints build
bool Builder::_constBuild{false};
// Objective build
bool Builder::_objBuild{false};
// Objective Lagrangian build
bool Builder::_lagobjBuild{false};
// Objective Mayer build
bool Builder::_mayobjBuild{false};
// ParameterizationMap build
bool Builder::_pmBuild{false};
// Regular Constraints build
bool Builder::_regconstBuild{false};
// Terminal Constraints build
bool Builder::_termconstBuild{false};

// Differential build static functions
void Builder::boolDiffFlip(const bool& val)
{
	_diffBuild = val;
}
const bool Builder::getDiffBuild()
{ 
	return _diffBuild; 
}

// Constraints build static functions
void Builder::boolConstFlip(const bool& val)
{
	_constBuild = val;
}
const bool Builder::getConstBuild()
{
	return _constBuild;
}

// Regular constraints build static functions
void  Builder::boolRegConstFlip(const bool& val)
{
	_regconstBuild = val;
}
const bool Builder::getRegConstBuild()
{
	return _regconstBuild;
}

// Terminal constraints build static functions
void  Builder::boolTermConstFlip(const bool& val)
{
	_termconstBuild = val;
}
const bool Builder::getTermConstBuild()
{
	return _termconstBuild;
}

// Objective build static functions
void Builder::boolObjFlip(const bool& val)
{
	_objBuild = val;
}
const bool Builder::getObjBuild()
{
	return _objBuild;
}

// Lagrangian objective build static functions
void Builder::boolLagObjFlip(const bool& val)
{
	_lagobjBuild = val;
}
const bool Builder::getLagObjBuild()
{
	return _lagobjBuild;
}
// Mayer objective build static functions
void Builder::boolMayObjFlip(const bool& val)
{
	_mayobjBuild = val;
}
const bool Builder::getMayObjBuild()
{
	return _mayobjBuild;
}

// ParameterizationMap build static functions
void Builder::boolPMFlip(const bool& val)
{
	_pmBuild = val;
}
const bool Builder::getPMBuild()
{
	return _pmBuild;
}