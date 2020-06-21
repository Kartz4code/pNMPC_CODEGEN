#pragma once
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

