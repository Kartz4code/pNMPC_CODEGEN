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

#ifndef BUILDER_H
#define BUILDER_H
class Builder
{
private:
	// Differential build
	static bool _diffBuild;
	// Constraints build
	static bool _constBuild;
	// Regular Constraints build
	static bool _regconstBuild;
	// Terminal Constraints build
	static bool _termconstBuild;
	// Objective build
	static bool _objBuild;
	// Objective Lagrangian build
	static bool _lagobjBuild;
	// Objective Mayer build
	static bool _mayobjBuild;
	// ParameterizationMap build
	static bool _pmBuild;
	Builder() = default;
public:
	// Differential build static functions
	static void boolDiffFlip(const bool& val);
	static const bool getDiffBuild();

	// Constraints build static functions
	static void boolConstFlip(const bool& val);
	static const bool getConstBuild();

	// Regular constraints build static functions
	static void boolRegConstFlip(const bool& val);
	static const bool getRegConstBuild();

	// Terminal constraints build static functions
	static void boolTermConstFlip(const bool& val);
	static const bool getTermConstBuild();

	// Objective build static functions
	static void boolObjFlip(const bool& val);
	static const bool getObjBuild();

	// Lagrangian objective build static functions
	static void boolLagObjFlip(const bool& val);
	static const bool getLagObjBuild();

	// Mayer objective build static functions
	static void boolMayObjFlip(const bool& val);
	static const bool getMayObjBuild();

	// ParameterizationMap build static functions
	static void boolPMFlip(const bool& val);
	static const bool getPMBuild();
	~Builder() = default;
};
#endif
