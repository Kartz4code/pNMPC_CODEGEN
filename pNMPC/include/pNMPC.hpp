#pragma once
#include "pNMPC_headers.hpp"

#define sgn(x) ((x<0)?-1:1)

#define BEGIN_DIFFERENTIAL Builder::boolDiffFlip(true); 
#define END_DIFFERENTIAL Builder::boolDiffFlip(false);

#define BEGIN_CONSTRAINTS Builder::boolConstFlip(true); 
#define END_CONSTRAINTS Builder::boolConstFlip(false);

#define BEGIN_REGULAR_CONSTRAINTS Builder::boolRegConstFlip(true); 
#define END_REGULAR_CONSTRAINTS Builder::boolRegConstFlip(false);

#define BEGIN_TERMINAL_CONSTRAINTS Builder::boolTermConstFlip(true); 
#define END_TERMINAL_CONSTRAINTS Builder::boolTermConstFlip(false);

#define BEGIN_OBJECTIVES Builder::boolObjFlip(true); 
#define END_OBJECTIVES Builder::boolObjFlip(false);

#define BEGIN_LAGRANGIAN Builder::boolLagObjFlip(true); 
#define END_LAGRANGIAN Builder::boolLagObjFlip(false);

#define BEGIN_MAYER Builder::boolMayObjFlip(true); 
#define END_MAYER Builder::boolMayObjFlip(false);

#define BEGIN_PARAMETERIZATION_MAP Builder::boolPMFlip(true); 
#define END_PARAMETERIZATION_MAP Builder::boolPMFlip(false);

class Database;
class HyperStates;
class EquationProcessor;
namespace pNMPC {
	// Double type as Reals
	typedef double Real;

	// Operations
	const std::string sOps[] = { "/","*","-","+","^", "o", "<=", ">=", "=", ">>","<<","<",">" };
	enum Operation { divide, multiply, subract, add, power, operate, leq, geq, eq, max, min, less, greater };

	// List of supported functions
	const std::string Functions[] = { "sin", "cos", "tan", "sinh", "cosh", "tanh", "exp", "log", "abs",
		"asin", "acos", "atan", "asinh", "acosh", "atanh", "min", "max", "pow","sgn" };

	// Paren operations
	const std::string Paren[] = { "(",")", " ", ";", "," };
	enum ParenOps { open, close, space, semi, comma };
	const std::string Var[] = { "x","u","p" };
	enum TYPE { STATE, INPUT, PARAMETER, EXTERNAL };
	enum class OBJTYPE { LAGRANGIAN, MAYER };
	enum class CONSTTYPE { REGULAR, TERMINAL };
	enum class INTEGRATOR { RK45, EU, HEUN };
	enum class CONTROLPARAM { PIECEWISE, LINEAR };
	enum class CONST_FORM { FORM_1, FORM_2 };

	// Create a static database distributed across all the classes and functions
	static Database* db;
	// Is a string a number? 
	bool is_number(const std::string& token);
	// Is integer?
	bool is_integer(const std::string& val);
	bool is_function(const std::string& token);
	size_t countStringSpaces(const std::string& str);
	bool isProperFunctionName(const std::string& str);

	// Matrix class
	template<typename T>
	class MAT {
	private:
		// Matrix and it's dimensions
		std::vector<std::vector<T>> _Matrix;
		size_t _rows{ 0 }, _cols{ 0 };
	public:
		MAT() :_rows{ 0 }, _cols{ 0 } {};
		// Initalize the matrix
		MAT(size_t rows, size_t cols) :_rows{ rows }, _cols{ cols }
		{
			for (size_t i{ 0 }; i < this->_rows; i++)
			{
				std::vector<T> myvector;
				for (size_t j{ 0 }; j < this->_cols; j++)
					myvector.push_back(T{});
				this->_Matrix.push_back(myvector);
			}
		}
		// Initialize array
		void operator=(std::vector<T>&& data_vec)
		{
			size_t sizer{ data_vec.size() };
			if (sizer < this->_rows*this->_cols)
				for (size_t k{ 0 }; k < this->_rows*this->_cols - sizer; k++)
					data_vec.push_back(0);

			for (size_t i{ 0 }; i < this->_rows; i++)
				for (size_t j{ 0 }; j < this->_cols; j++)
					this->_Matrix[i][j] = data_vec[i*this->_cols + j];
		}
		// Resizing matrix 
		void resize(size_t numRows, size_t numColums)
		{
			this->_rows = numRows; this->_cols = numColums;
			this->_Matrix.resize(numRows, std::vector<T>(numColums));
		}
		// Returns the full matrix
		std::vector<std::vector<T>> getFul_Matrix() const { return this->_Matrix; }
		// Returns the matrix element
		T getMatrixElement(const size_t& i, const size_t& j) const { return this->_Matrix[i][j]; }
		// Sets the matrix element
		void setMatrixElement(const size_t& i, const size_t& j, const T& val) { this->_Matrix[i][j] = val; }

		// Get number of _rows
		size_t getNumRows() const { return this->_rows; }
		// Get number of columns
		size_t getNumColumns() const { return this->_cols; }
		// Set number of _rows
		void setNumRows(const size_t& nR) { this->_rows = nR; }
		// Set number of columns
		void setNumColumns(const size_t& nC) { this->_cols = nC; }
		// Get a column
		MAT<T> getColumn(const size_t& col)
		{
			if (col <= this->getNumColumns())
			{
				MAT<T> ResultColumn{ this->getNumRows(),1 };
				for (size_t i{ 0 }; i < this->getNumRows(); i++)
					ResultColumn.setMatrixElement(i, 0, this->getMatrixElement(i, col));
				return ResultColumn;
			}
			else {
				std::cerr << "The number of columns are greater than the matrix dimensions" << std::endl;
			}
		}
		// Get a row
		MAT<T> getRow(const size_t& row)
		{
			if (row <= this->getNumRows())
			{
				MAT<T> ResultRow{ 1,this->getNumColumns() };
				for (size_t i{ 0 }; i < this->getNumColumns(); i++)
					ResultRow.setMatrixElement(0, i, this->getMatrixElement(i, row));
				return ResultRow;
			}
			else {
				std::cerr << "The number of _rows are greater than the matrix dimensions" << std::endl;
			}
		}

		// Operator overloading []
		T* operator[](const size_t& n) { return &this->_Matrix[n][0]; }
		// Matrix access via A(x,y)
		T& operator()(const size_t& n, const size_t& m)
		{
			return this->_Matrix[n][m];
		}
		// Assignment operator
		void operator=(const T& data) { this->setMatrixElement(this->lookR, this->lookC, std::move(data)); }

		// L-value functions
		// Templatized <= operator (Matrix version)
		template<typename U>
		MAT<T> operator<=(const MAT<U>& rhs_MAT)
		{
			MAT<T> ResultLeq(this->getNumRows(), rhs_MAT.getNumColumns());
			if (this->getNumColumns() == rhs_MAT.getNumColumns() && this->getNumRows() == rhs_MAT.getNumRows())
			{
				for (size_t i{ 0 }; i < this->getNumRows(); i++)
					for (size_t j{ 0 }; j < rhs_MAT.getNumColumns(); j++)
						ResultLeq.setMatrixElement(i, j, this->getMatrixElement(i, j) <= rhs_MAT.getMatrixElement(i, j));
				return ResultLeq;
			}
			else
				std::cerr << "The matrix multiplication dimensions are inconsistent" << std::endl;
		}

		// Friend function <= operator (Matrix version)
		template<typename U>
		friend MAT<T> operator<=(const MAT<U>& lhs_MAT, const MAT<T>& rhs_MAT)
		{
			MAT<T> ResultLeq(lhs_MAT.getNumRows(), rhs_MAT.getNumColumns());
			if (lhs_MAT.getNumColumns() == rhs_MAT.getNumColumns() && lhs_MAT.getNumRows() == rhs_MAT.getNumRows())
			{
				for (size_t i{ 0 }; i <lhs_MAT.getNumRows(); i++)
					for (size_t j{ 0 }; j < rhs_MAT.getNumColumns(); j++)
						ResultLeq.setMatrixElement(i, j, lhs_MAT.getMatrixElement(i, j) <= rhs_MAT.getMatrixElement(i, j));
				return ResultLeq;
			}
			else
				std::cerr << "The matrix multiplication dimensions are inconsistent" << std::endl;
		}

		// Specialized <= operator (Matrix version)
		MAT<HyperStates> operator<=(const MAT<HyperStates>& rhs_MAT)
		{
			MAT<HyperStates> ResultLeq(this->getNumRows(), rhs_MAT.getNumColumns());
			if (this->getNumColumns() == rhs_MAT.getNumColumns() && this->getNumRows() == rhs_MAT.getNumRows())
			{
				for (size_t i{ 0 }; i < this->getNumRows(); i++)
					for (size_t j{ 0 }; j < rhs_MAT.getNumColumns(); j++)
						ResultLeq.setMatrixElement(i, j, this->getMatrixElement(i, j) <= rhs_MAT.getMatrixElement(i, j));
				return ResultLeq;
			}
			else
				std::cerr << "The matrix multiplication dimensions are inconsistent" << std::endl;
		}

		// <= operator (Scalar version)
		template<typename U>
		MAT<T> operator<=(const U& rhs)
		{
			MAT<T> ResultLeq(this->getNumRows(), 1);
			if (this->getNumColumns() == 1 && this->getNumRows() == 1)
			{
				for (size_t i{ 0 }; i < this->getNumRows(); i++)
					ResultLeq.setMatrixElement(i, 0, this->getMatrixElement(i, 0) <= rhs);
				return ResultLeq;
			}
			else
				std::cerr << "The matrix multiplication dimensions are inconsistent" << std::endl;
		}

		// templatized >= operator
		template<typename U>
		MAT<T> operator>=(const MAT<U>& rhs_MAT)
		{
			MAT<T> ResultGeq(this->getNumRows(), rhs_MAT.getNumColumns());
			if (this->getNumColumns() == rhs_MAT.getNumColumns() && this->getNumRows() == rhs_MAT.getNumRows())
			{
				for (size_t i{ 0 }; i < this->getNumRows(); i++)
					for (size_t j{ 0 }; j < rhs_MAT.getNumColumns(); j++)
						ResultGeq.setMatrixElement(i, j, this->getMatrixElement(i, j) >= rhs_MAT.getMatrixElement(i, j));
				return ResultGeq;
			}
			else
				std::cerr << "The matrix multiplication dimensions are inconsistent" << std::endl;
		}

		// Friend specialized function >= operator (Matrix version)
		template<typename U>
		friend MAT<HyperStates> operator>=(const MAT<U>& lhs_MAT, const MAT<T>& rhs_MAT)
		{
			MAT<T> ResultLeq(lhs_MAT.getNumRows(), rhs_MAT.getNumColumns());
			if (lhs_MAT.getNumColumns() == rhs_MAT.getNumColumns() && lhs_MAT.getNumRows() == rhs_MAT.getNumRows())
			{
				for (size_t i{ 0 }; i <lhs_MAT.getNumRows(); i++)
					for (size_t j{ 0 }; j < rhs_MAT.getNumColumns(); j++)
						ResultLeq.setMatrixElement(i, j, lhs_MAT.getMatrixElement(i, j) >= rhs_MAT.getMatrixElement(i, j));
				return ResultLeq;
			}
			else
				std::cerr << "The matrix multiplication dimensions are inconsistent" << std::endl;
		}

		// Specialized >= operator
		MAT<HyperStates> operator>=(const MAT<HyperStates>& rhs_MAT)
		{
			MAT<HyperStates> ResultGeq(this->getNumRows(), 1);
			if (this->getNumColumns() == rhs_MAT.getNumColumns() && this->getNumRows() == rhs_MAT.getNumRows())
			{
				for (size_t i{ 0 }; i < this->getNumRows(); i++)
					for (size_t j{ 0 }; j < rhs_MAT.getNumColumns(); j++)
						ResultGeq.setMatrixElement(i, j, this->getMatrixElement(i, j) >= rhs_MAT.getMatrixElement(i, j));
				return ResultGeq;
			}
			else
				std::cerr << "The matrix multiplication dimensions are inconsistent" << std::endl;
		}

		// >= operator (Scalar version)
		template<typename U>
		MAT<T> operator>=(const U& rhs)
		{
			MAT<T> ResultGeq(this->getNumRows(), 1);
			if (this->getNumColumns() == 1 && this->getNumRows() == 1)
			{
				for (size_t i{ 0 }; i < this->getNumRows(); i++)
					ResultGeq.setMatrixElement(i, 0, this->getMatrixElement(i, 0) >= rhs);
				return ResultGeq;
			}
			else
				std::cerr << "The matrix multiplication dimensions are inconsistent" << std::endl;
		}

		// templatized Matrix/Vector - Matrix/Vector Multiplication
		template<typename U>
		MAT<T> operator*(const MAT<U>& rhs_MAT)
		{
			T temp{}; MAT<T> ResultMul(this->getNumRows(), rhs_MAT.getNumColumns());
			if (this->getNumColumns() == rhs_MAT.getNumRows())
			{
				for (size_t i{ 0 }; i < this->getNumRows(); i++)
				{
					for (size_t j{ 0 }; j < rhs_MAT.getNumColumns(); j++)
					{
						for (size_t k{ 0 }; k < rhs_MAT.getNumRows(); k++)
							temp = temp + this->getMatrixElement(i, k)*rhs_MAT.getMatrixElement(k, j);
						ResultMul.setMatrixElement(i, j, temp);
						temp = 0;
					}
				}
				return ResultMul;
			}
			else
				std::cerr << "The matrix multiplication dimensions are inconsistent" << std::endl;
		}

		// Specified HyperStates class operator* overloading
		MAT<HyperStates> operator*(const MAT<HyperStates>& rhs_MAT)
		{
			HyperStates temp{}; MAT<HyperStates> ResultMul(this->getNumRows(), rhs_MAT.getNumColumns());
			if (this->getNumColumns() == rhs_MAT.getNumRows())
			{
				for (size_t i{ 0 }; i < this->getNumRows(); i++)
				{
					for (size_t j{ 0 }; j < rhs_MAT.getNumColumns(); j++)
					{
						for (size_t k{ 0 }; k < rhs_MAT.getNumRows(); k++)
							temp = temp + this->getMatrixElement(i, k)*rhs_MAT.getMatrixElement(k, j);
						ResultMul.setMatrixElement(i, j, temp);
						temp = 0;
					}
				}
				return ResultMul;
			}
			else
				std::cerr << "The matrix multiplication dimensions are inconsistent" << std::endl;
		}

		// templatized Matrix - Matrix addition
		template<typename U>
		MAT<T> operator+(const MAT<U>& rhs_MAT)
		{
			MAT<T> ResultAdd(this->getNumRows(), rhs_MAT.getNumColumns());
			if (this->getNumColumns() == rhs_MAT.getNumColumns() && this->getNumRows() == rhs_MAT.getNumRows())
			{
				for (size_t i{ 0 }; i < this->getNumRows(); i++)
					for (size_t j{ 0 }; j < rhs_MAT.getNumColumns(); j++)
						ResultAdd.setMatrixElement(i, j, this->getMatrixElement(i, j) + rhs_MAT.getMatrixElement(i, j));
				return ResultAdd;
			}
			else
				std::cerr << "The matrix multiplication dimensions are inconsistent" << std::endl;
		}

		// Specified HyperStates class operator+ overloading
		MAT<HyperStates> operator+(const MAT<HyperStates>& rhs_MAT)
		{
			MAT<T> ResultAdd(this->getNumRows(), rhs_MAT.getNumColumns());
			if (this->getNumColumns() == rhs_MAT.getNumColumns() && this->getNumRows() == rhs_MAT.getNumRows())
			{
				for (size_t i{ 0 }; i < this->getNumRows(); i++)
					for (size_t j{ 0 }; j < rhs_MAT.getNumColumns(); j++)
						ResultAdd.setMatrixElement(i, j, this->getMatrixElement(i, j) + rhs_MAT.getMatrixElement(i, j));
				return ResultAdd;
			}
			else
				std::cerr << "The matrix multiplication dimensions are inconsistent" << std::endl;
		}

		// templatized Matrix - Matrix subraction
		template<typename U>
		MAT<T> operator-(const MAT<U>& rhs_MAT)
		{
			MAT<T> ResultSubract(this->getNumRows(), rhs_MAT.getNumColumns());
			if (this->getNumColumns() == rhs_MAT.getNumColumns() && this->getNumRows() == rhs_MAT.getNumRows())
			{
				for (size_t i{ 0 }; i < this->getNumRows(); i++)
					for (size_t j{ 0 }; j < rhs_MAT.getNumColumns(); j++)
						ResultSubract.setMatrixElement(i, j, this->getMatrixElement(i, j) - rhs_MAT.getMatrixElement(i, j));
				return ResultSubract;
			}
			else
				std::cerr << "The matrix multiplication dimensions are inconsistent" << std::endl;
		}

		// Specified HyperStates class operator- overloading
		MAT<HyperStates> operator-(const MAT<HyperStates>& rhs_MAT)
		{
			MAT<T> ResultSubract(this->getNumRows(), rhs_MAT.getNumColumns());
			if (this->getNumColumns() == rhs_MAT.getNumColumns() && this->getNumRows() == rhs_MAT.getNumRows())
			{
				for (size_t i{ 0 }; i < this->getNumRows(); i++)
					for (size_t j{ 0 }; j < rhs_MAT.getNumColumns(); j++)
						ResultSubract.setMatrixElement(i, j, this->getMatrixElement(i, j) - rhs_MAT.getMatrixElement(i, j));
				return ResultSubract;
			}
			else
				std::cerr << "The matrix multiplication dimensions are inconsistent" << std::endl;
		}


		// templatized Scalar multiplication
		MAT<T> operator*(const T& rhs)
		{
			MAT<T> ResultMul(this->getNumRows(), this->getNumColumns());
			for (size_t i{ 0 }; i < this->getNumRows(); i++)
				for (size_t j{ 0 }; j < this->getNumColumns(); j++)
					ResultMul.setMatrixElement(i, j, this->getMatrixElement(i, j) * rhs);
			return ResultMul;
		}

		// templatized Scalar addition
		MAT<T> operator+(const T& rhs)
		{
			MAT<T> ResultAdd(this->getNumRows(), this->getNumColumns());
			for (size_t i{ 0 }; i < this->getNumRows(); i++)
				for (size_t j{ 0 }; j < this->getNumColumns(); j++)
					ResultAdd.setMatrixElement(i, j, this->getMatrixElement(i, j) + rhs);
			return ResultAdd;
		}

		// templatized Scalar subraction
		MAT<T> operator-(const T& rhs)
		{
			MAT<T> ResultSubract(this->getNumRows(), this->getNumColumns());
			for (size_t i{ 0 }; i < this->getNumRows(); i++)
				for (size_t j{ 0 }; j < this->getNumColumns(); j++)
					ResultSubract.setMatrixElement(i, j, this->getMatrixElement(i, j) - rhs);
			return ResultSubract;
		}

		// templatized Scalar division
		MAT<T> operator/(const T& rhs)
		{
			MAT<T> ResultDiv(this->getNumRows(), this->getNumColumns());
			for (size_t i{ 0 }; i < this->getNumRows(); i++)
				for (size_t j{ 0 }; j < this->getNumColumns(); j++)
					ResultDiv.setMatrixElement(i, j, this->getMatrixElement(i, j) / rhs);
			return ResultDiv;
		}

		// templatized Scalar exponent
		MAT<T> operator^(const T& rhs)
		{
			MAT<T> ResultDiv(this->getNumRows(), this->getNumColumns());
			for (size_t i{ 0 }; i < this->getNumRows(); i++)
				for (size_t j{ 0 }; j < this->getNumColumns(); j++)
					ResultDiv.setMatrixElement(i, j, ((this->getMatrixElement(i, j)) ^ rhs));
			return ResultDiv;
		}

		// Matrix negation
		MAT<T> operator-()
		{
			MAT<T> ResultNegate(this->getNumRows(), this->getNumColumns());
			for (size_t i{ 0 }; i < this->getNumRows(); i++)
				for (size_t j{ 0 }; j < this->getNumColumns(); j++)
					ResultNegate.setMatrixElement(i, j, -this->getMatrixElement(i, j));
			return ResultNegate;
		}
		// Trace of matrix
		T trace()
		{
			T ResultTrace;
			if (this->getNumColumns() == this->getNumRows())
			{
				for (size_t i{ 0 }; i < this->getNumRows(); i++)
					ResultTrace = ResultTrace + this->getMatrixElement(i, i);
				return ResultTrace;
			}
			else
				std::cerr << "The matrix multiplication dimensions are inconsistent" << std::endl;
		}
		// Trace of matrix
		friend T trace(const MAT<T>& rhs)
		{
			T ResultTrace;
			if (rhs.getNumColumns() == rhs.getNumRows())
			{
				for (size_t i{ 0 }; i < rhs.getNumRows(); i++)
					ResultTrace = ResultTrace + rhs.getMatrixElement(i, i);
				return ResultTrace;
			}
			else
				std::cerr << "The matrix multiplication dimensions are inconsistent" << std::endl;
		}
		// Transpose of matrix
		MAT<T> transpose()
		{
			MAT<T> ResultTranspose(this->getNumColumns(), this->getNumRows());
			for (size_t i{ 0 }; i < this->getNumColumns();i++)
				for (size_t j{ 0 }; j < this->getNumRows();j++)
					ResultTranspose.setMatrixElement(i, j, this->getMatrixElement(j, i));
			return ResultTranspose;
		}
		// Transpose of matrix
		friend MAT<T> transpose(const MAT<T>& rhs)
		{
			MAT<T> ResultTranspose(rhs.getNumColumns(), rhs.getNumRows());
			for (size_t i{ 0 }; i < rhs.getNumColumns();i++)
				for (size_t j{ 0 }; j < rhs.getNumRows();j++)
					ResultTranspose.setMatrixElement(i, j, rhs.getMatrixElement(j, i));
			return ResultTranspose;
		}

		// To output stream
		friend std::ostream& operator<<(std::ostream& os, const MAT& m)
		{
			for (size_t i{ 0 };i < m._rows;i++)
			{
				for (size_t j{ 0 }; j < m._cols;j++)
					os << m._Matrix[i][j] << " ";
				os << std::endl;
			}
			return os;
		}
		~MAT() = default;
	};

	typedef MAT<Real> MATReal;
	typedef MAT<HyperStates> MATHyperStates;

	MATHyperStates operator*(const Real& x, MATHyperStates& X);
	MATHyperStates operator*(MATHyperStates& X, const Real& x);

	// Identity matrix
	MATReal eye(const size_t& n);
	// Diagonal matrix
	MATReal diag(const std::initializer_list<Real>& X);
	// Ones matrix
	MATReal ones(const size_t& n, const size_t& m);
	// Zeros matrix
	MATReal zeros(const size_t& n, const size_t& m);

	// Matrix functions (No templates due to no implicit conversion from int to double)
	// Multiplication operator
	MATReal operator*(const Real& x, MATReal X);
	// Addition operator
	MATReal operator+(const Real& x, MATReal X);
	// Subraction operator
	MATReal operator-(const Real& x, MATReal X);

	// Multiplication operator
	MATHyperStates operator*(const Real& x, MATHyperStates X);
	// Addition operator
	MATHyperStates operator+(const Real& x, MATHyperStates X);
	// Subraction operator
	MATHyperStates operator-(const Real& x, MATHyperStates X);
	// Less than or equal to operator
	MATHyperStates operator<=(const Real& x, MATHyperStates X);
	// Greater than or equal to operator
	MATHyperStates operator>=(const Real& x, MATHyperStates X);

	// Multiplication operator
	MATHyperStates operator*(HyperStates& x, MATHyperStates X);
	// Addition operator
	MATHyperStates operator+(HyperStates& x, MATHyperStates X);
	// Subraction operator
	MATHyperStates operator-(HyperStates& x, MATHyperStates X);
	// Less than or equal to operator
	MATHyperStates operator<=(HyperStates& x, MATHyperStates X);
	// Greater than or equal to operator
	MATHyperStates operator>=(HyperStates& x, MATHyperStates X);


	// Otherfunctions
	MATHyperStates sin(MATHyperStates X);
	MATHyperStates cos(MATHyperStates X);
	MATHyperStates tan(MATHyperStates X);
	MATHyperStates sinh(MATHyperStates X);
	MATHyperStates cosh(MATHyperStates X);
	MATHyperStates tanh(MATHyperStates X);
	MATHyperStates exp(MATHyperStates X);
	MATHyperStates log(MATHyperStates X);
	MATHyperStates abs(MATHyperStates X);
	MATHyperStates asin(MATHyperStates X);
	MATHyperStates acos(MATHyperStates X);
	MATHyperStates atan(MATHyperStates X);
	MATHyperStates asinh(MATHyperStates X);
	MATHyperStates acosh(MATHyperStates X);
	MATHyperStates atanh(MATHyperStates X);
	MATHyperStates sign(MATHyperStates X);

	MATHyperStates maximum(MATHyperStates X, HyperStates& x);
	MATHyperStates maximum(HyperStates& x, MATHyperStates X);
	MATHyperStates maximum(MATHyperStates X, const Real& x);
	MATHyperStates maximum(const Real& x, MATHyperStates X);

	MATHyperStates minimum(MATHyperStates X, HyperStates& x);
	MATHyperStates minimum(HyperStates& x, MATHyperStates X);
	MATHyperStates minimum(MATHyperStates X, const Real& x);
	MATHyperStates minimum(const Real& x, MATHyperStates X);


	MATHyperStates transpose(EquationProcessor& EQ);
	void pNMPC_free();
}