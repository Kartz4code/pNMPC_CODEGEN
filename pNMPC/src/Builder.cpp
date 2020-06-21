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