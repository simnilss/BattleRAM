// SiNi Matrix class template
//
// This code has been inspired by the Matrix.hpp code in sfzCore,
// written by my friend Peter Hillerström (github.com/PetorSFZ).
// The quality of this code does not neccessarily represent the quality of
// sfzCore or any of his work.
//
// NOTE!
// Most functions are written with floating point values in mind, and some do
// therefore not always translate well to integers with undefined or undesired
// behaviour as a consquence. Until (if) this is fixed, consider casting to
// float/double when e.g. computing the inverse of a matrix (very few integer 
// matrices have integer matrix inverses!) or other more complicated operations.

#pragma once
#ifndef SINI_MATRIX_H
#define SINI_MATRIX_H

#include "sini/CudaCompat.hpp"
#include "sini/math/Vector.hpp"

using std::size_t;
using std::uint32_t;
using std::int32_t;

namespace sini {

	// General Matrix
	template<typename T, uint32_t M, uint32_t N>
	struct Matrix {
		Vector<T, N> row_vectors[M];

		Matrix() noexcept = default;
		Matrix(const Matrix<T,M,N>&) noexcept = default;
		Matrix<T,M,N>& operator= (const Matrix<T,M,N>&) noexcept = default;
		~Matrix() noexcept = default;

		//TODO	Change this to be equal to initVal * identity matrix??
		//		Does it make more sense or is more useful?
		SINI_CUDA_COMPAT Matrix(T initVal);
		SINI_CUDA_COMPAT explicit Matrix(const T* initArray) noexcept;

		template<typename T2>
		SINI_CUDA_COMPAT Matrix(const Matrix<T2,M,N>& other) noexcept;

		/// Element access with bounds checking via assert
		SINI_CUDA_COMPAT T&	at(uint32_t i, uint32_t j) noexcept;
		SINI_CUDA_COMPAT T	at(uint32_t i, uint32_t j) const noexcept;
		SINI_CUDA_COMPAT Vector<T,M> column(uint32_t j) noexcept;
		/// Element access without bounds checking
		SINI_CUDA_COMPAT T&	operator[] (uint32_t i, uint32_t j) noexcept { return row_vectors[i][j]; }
		SINI_CUDA_COMPAT T	operator[] (uint32_t i, uint32_t j) const noexcept { return row_vectors[i][j]; }

		SINI_CUDA_COMPAT void set(uint32_t i, uint32_t j, T val) noexcept;
		SINI_CUDA_COMPAT void setColumn(uint32_t i, Vector<T,N> col) noexcept;

		SINI_CUDA_COMPAT Matrix<T,M-1,N-1> submatrix(uint32_t i, uint32_t j) noexcept;

		//TODO move to square matrix specialization
		static SINI_CUDA_COMPAT Matrix<T,M,N> identity() noexcept;
	};
	//TODO add specialization for general square matrix?

	// ---------------------------------------------------
	// Common matrix forms, with a bit more functionality
	// ---------------------------------------------------
	// 2x2 Matrix
	template<typename T>
	struct Matrix<T, 2, 2> {
		// Nameless union to allow for getting rows/elements both via the
		// array or via e.g. mat.a
		union {
			struct { Vector<T,2> row_vectors[2]; };
			struct {
				T	a, b,
					c, d;
			};
			struct {
				T	e00, e01,
					e10, e11;
			};
		};

		SINI_CUDA_COMPAT T* data() noexcept { return &a; }
		SINI_CUDA_COMPAT const T* data() const noexcept { return &a; }

		Matrix() noexcept = default;
		Matrix(const Matrix<T,2,2>&) noexcept = default;
		Matrix<T, 2, 2>& operator= (const Matrix<T,2,2>&) noexcept = default;
		~Matrix() noexcept = default;

		SINI_CUDA_COMPAT Matrix(T initVal) noexcept;
		SINI_CUDA_COMPAT explicit Matrix(const T*) noexcept;
		SINI_CUDA_COMPAT Matrix(Vector<T,2> row1, Vector<T,2> row2) noexcept;

		template<typename T2>
		SINI_CUDA_COMPAT Matrix(const Matrix<T2,2,2>& other) noexcept;

		/// Element access with bounds checking via assert
		SINI_CUDA_COMPAT T&	at(uint32_t i, uint32_t j) noexcept;
		SINI_CUDA_COMPAT T	at(uint32_t i, uint32_t j) const noexcept;
		SINI_CUDA_COMPAT Vector<T,2> column(uint32_t i) noexcept;
		/// Element access without bounds checking
		SINI_CUDA_COMPAT T& operator[] (uint32_t i, uint32_t j) noexcept { return row_vectors[i][j]; }
		SINI_CUDA_COMPAT T	operator[] (uint32_t i, uint32_t j) const noexcept { return row_vectors[i][j]; }

		SINI_CUDA_COMPAT void set(uint32_t i, uint32_t j, T val) noexcept;
		SINI_CUDA_COMPAT void setColumn(uint32_t i, Vector<T, 2> col) noexcept;

		static SINI_CUDA_COMPAT Matrix<T,2,2> identity() noexcept;
	};
	// Pre-defined 2x2 matrix types
	using mat2 = Matrix<float, 2, 2>;
	using mat2d = Matrix<double, 2, 2>;
	using mat2i = Matrix<int32_t, 2, 2>;

	// 3x3 Matrix
	template<typename T>
	struct Matrix<T, 3, 3> {
		// Nameless union to allow for getting rows/elements both via the
		// array or via e.g. mat.a
		union {
			struct { Vector<T, 3> row_vectors[3]; };
			struct {
				T	a, b, c,
					d, e, f,
					g, h, i;
			};
			struct {
				T	e00, e01, e02,
					e10, e11, e12,
					e20, e21, e22;
			};
		};

		SINI_CUDA_COMPAT T* data() noexcept { return &a; }
		SINI_CUDA_COMPAT const T* data() const noexcept { return &a; }

		Matrix() noexcept = default;
		Matrix(const Matrix<T,3,3>&) noexcept = default;
		Matrix<T, 3, 3>& operator= (const Matrix<T,3,3>&) noexcept = default;
		~Matrix() noexcept = default;

		SINI_CUDA_COMPAT Matrix(T initVal) noexcept;
		SINI_CUDA_COMPAT explicit  Matrix(const T* initArray);
		SINI_CUDA_COMPAT Matrix(Vector<T, 3> row1, Vector<T,3> row2, Vector<T, 3> row3);

		template<typename T2>
		SINI_CUDA_COMPAT Matrix(const Matrix<T2,3,3>& other) noexcept;

		/// Element access with bounds checking via assert
		SINI_CUDA_COMPAT T&	at(uint32_t i, uint32_t j) noexcept;
		SINI_CUDA_COMPAT T	at(uint32_t i, uint32_t j) const noexcept;
		SINI_CUDA_COMPAT Vector<T,3> column(uint32_t i) noexcept;
		/// Element access without bounds checking
		SINI_CUDA_COMPAT T&	operator[] (uint32_t i, uint32_t j) noexcept { return row_vectors[i][j]; }
		SINI_CUDA_COMPAT T	operator[] (uint32_t i, uint32_t j) const noexcept { return row_vectors[i][j]; }

		SINI_CUDA_COMPAT void set(uint32_t i, uint32_t j, T val) noexcept;
		SINI_CUDA_COMPAT void setColumn(uint32_t j, Vector<T, 3> col) noexcept;

		SINI_CUDA_COMPAT Matrix<T,2,2> submatrix(uint32_t i, uint32_t j) noexcept;

		static SINI_CUDA_COMPAT Matrix<T,3,3> identity() noexcept;
	};
	// Pre-defined 3x3 matrices
	using mat3 = Matrix<float, 3, 3>;
	using mat3d = Matrix<double, 3, 3>;
	using mat3i = Matrix<int32_t, 3, 3>;

	template<typename T>
	struct Matrix<T, 4, 4> {
		// Nameless union to allow for getting rows/elements both via the
		// array or via e.g. mat.a
		union {
			struct { Vector<T, 4> row_vectors[4]; };
			struct {
				T	e00, e01, e02, e03,
					e10, e11, e12, e13,
					e20, e21, e22, e23,
					e30, e31, e32, e33;
			};
		};

		SINI_CUDA_COMPAT T* data() noexcept { return &e00; }
		SINI_CUDA_COMPAT const T* data() const noexcept { return &e00; }

		Matrix() noexcept = default;
		Matrix(const Matrix<T,4,4>&) noexcept = default;
		Matrix<T, 4, 4>& operator= (const Matrix<T,4,4>&) noexcept = default;
		~Matrix() noexcept = default;

		SINI_CUDA_COMPAT Matrix(T initVal) noexcept;
		SINI_CUDA_COMPAT explicit Matrix(const T* initArray);
		SINI_CUDA_COMPAT Matrix(Vector<T,4> row1, Vector<T,4> row2, Vector<T,4> row3, Vector<T,4> row4);

		template<typename T2>
		SINI_CUDA_COMPAT Matrix(const Matrix<T2, 4, 4>& other) noexcept;

		/// Element access with bounds checking via assert
		SINI_CUDA_COMPAT T&	at(uint32_t i, uint32_t j) noexcept;
		SINI_CUDA_COMPAT T	at(uint32_t i, uint32_t j) const noexcept;
		SINI_CUDA_COMPAT Vector<T,4> column(uint32_t i) noexcept;
		/// Element access without bounds checking
		SINI_CUDA_COMPAT T&	operator[] (uint32_t i, uint32_t j) noexcept { return row_vectors[i][j]; }
		SINI_CUDA_COMPAT T	operator[] (uint32_t i, uint32_t j) const noexcept { return row_vectors[i][j]; }

		SINI_CUDA_COMPAT void set(uint32_t i, uint32_t j, T val) noexcept;
		SINI_CUDA_COMPAT void setColumn(uint32_t j, Vector<T, 4> col) noexcept;

		SINI_CUDA_COMPAT Matrix<T,3,3> submatrix(uint32_t i, uint32_t j) noexcept;

		static SINI_CUDA_COMPAT Matrix<T,4,4> identity() noexcept;
	};
	// Pre-defined 4x4 matrices
	using mat4 = Matrix<float, 4, 4>;
	using mat4d = Matrix<double, 4, 4>;
	using mat4i = Matrix<int32_t, 4, 4>;


	// Math functions (and some other utilities)
	// -------------------------------------------------------------------------

	// Conversion between column vectors (sini::Vector) and row vectors
	// (sini::Matrix<T,1,N>)
	// Separated from transpose since they are separate structs
	// Might be changed in the future
	template<typename T, uint32_t N>
	SINI_CUDA_COMPAT Matrix<T,1,N> toRowVector(const Vector<T,N>& vec) noexcept;
	template<typename T, uint32_t N>
	SINI_CUDA_COMPAT Vector<T,N> toColumnVector(const Matrix<T,1,N>& mat) noexcept;

	// Element-wise matrix multiplication
	template<typename T, uint32_t M, uint32_t N>
	SINI_CUDA_COMPAT Matrix<T,M,N> eqElemMult(Matrix<T,M,N>& left, const Matrix<T,M,N>& right) noexcept;
	template<typename T, uint32_t M, uint32_t N>
	SINI_CUDA_COMPAT Matrix<T,M,N> elemMult(const Matrix<T,M,N>& left, const Matrix<T,M,N>& right) noexcept;
	// Element-wise division
	template<typename T, uint32_t M, uint32_t N>
	SINI_CUDA_COMPAT Matrix<T,M,N> eqElemDiv(Matrix<T,M,N>& left, const Matrix<T,M,N>& right) noexcept;
	template<typename T, uint32_t M, uint32_t N>
	SINI_CUDA_COMPAT Matrix<T,M,N> elemDiv(const Matrix<T,M,N>& left, const Matrix<T,M,N>& right) noexcept;

	// Transpose
	template<typename T, uint32_t M, uint32_t N>
	SINI_CUDA_COMPAT Matrix<T,N,M> transpose(const Matrix<T,M,N>& mat) noexcept;

	// Power
	template<typename T, uint32_t N>
	SINI_CUDA_COMPAT Matrix<T,N,N> eqPow(Matrix<T,N,N>& mat, uint32_t exponent) noexcept;
	template<typename T, uint32_t N>
	SINI_CUDA_COMPAT Matrix<T,N,N> pow(const Matrix<T,N,N>& mat, uint32_t exponent) noexcept;

	// Dimensions
	template<typename T, uint32_t M, uint32_t N>
	SINI_CUDA_COMPAT Vector<uint32_t,2> dimenions(const Matrix<T,M,N>&) noexcept {
		return Vector<uint32_t, 2>(M, N);
	}

	// Hasher
	template<typename T, uint32_t M, uint32_t N>
	SINI_CUDA_COMPAT size_t hash(const Matrix<T,M,N>& m) noexcept;

	// Determinant of 2x2, 3x3 and 4x4 matrices
	// For computing the determinant of an arbitrary matrix size include
	// "sini/math/MatrixMath.h", which adds more advanced matrix operations
	// like LU decomposition etc. (LU decomp. is used for computing larger
	// determinants.)
	template<typename T>
	SINI_CUDA_COMPAT T det(const Matrix<T,2,2>& mat) noexcept;
	template<typename T>
	SINI_CUDA_COMPAT T det(const Matrix<T,3,3>& mat) noexcept;
	template<typename T>
	SINI_CUDA_COMPAT T det(const Matrix<T,4,4>& mat) noexcept;
	template<typename T, uint32_t M, uint32_t N>
	SINI_CUDA_COMPAT T det(const Matrix<T,M,N>& mat) noexcept;

	// Submatrix determinant (a.k.a. minor) for 3x3, 4x4 and 5x5 matrices
	template<typename T>
	SINI_CUDA_COMPAT T minor(const Matrix<T,3,3>& mat, uint32_t i, uint32_t j) noexcept;
	template<typename T>
	SINI_CUDA_COMPAT T minor(const Matrix<T,4,4>& mat, uint32_t i, uint32_t j) noexcept;

	// Adjugate matrix
	template<typename T>
	SINI_CUDA_COMPAT Matrix<T,2,2> adj(const Matrix<T,2,2>& mat) noexcept;
	template<typename T>
	SINI_CUDA_COMPAT Matrix<T,3,3> adj(const Matrix<T,3,3>& mat) noexcept;
	template<typename T>
	SINI_CUDA_COMPAT Matrix<T,4,4> adj(const Matrix<T,4,4>& mat) noexcept;

	// Inverse of 2x2 and 3x3 matrices
	// More general inverse and pseudo-inverse in "sini/math/MatrixMath.h"
	template<typename T>
	SINI_CUDA_COMPAT T inverse(const Matrix<T,2,2>& mat) noexcept;
	template<typename T>
	SINI_CUDA_COMPAT T inverse(const Matrix<T,3,3>& mat) noexcept;

	//TODO add more functions?

	// Operators
	// -------------------------------------------------------------------------

	// Equality and inequality
	template<typename T, uint32_t M, uint32_t N>
	SINI_CUDA_COMPAT bool operator== (const Matrix<T,M,N>& left, const Matrix<T,M,N>& right) noexcept;
	template<typename T, uint32_t M, uint32_t N>
	SINI_CUDA_COMPAT bool operator!= (const Matrix<T,M,N>& left, const Matrix<T,M,N>& right) noexcept;

	// Addition
	template<typename T, uint32_t M, uint32_t N>
	SINI_CUDA_COMPAT Matrix<T,M,N> operator+= (Matrix<T,M,N>& left, const Matrix<T,M,N>& right) noexcept;
	template<typename T, uint32_t M, uint32_t N>
	SINI_CUDA_COMPAT Matrix<T,M,N> operator+ (const Matrix<T,M,N>& left, const Matrix<T,M,N>& right) noexcept;
	
	// Subtraction
	template<typename T, uint32_t M, uint32_t N>
	SINI_CUDA_COMPAT Matrix<T,M,N> operator-= (Matrix<T,M,N>& left, const Matrix<T,M,N>& right) noexcept;
	template<typename T, uint32_t M, uint32_t N>
	SINI_CUDA_COMPAT Matrix<T,M,N> operator- (constMatrix<T,M,N>& left, const Matrix<T,M,N>& right) noexcept;
	
	// Multiplication for general matrices
	template<typename T, uint32_t M, uint32_t N, uint32_t O>
	SINI_CUDA_COMPAT Matrix<T, M, O> operator* (const Matrix<T, M, N>& left, const Matrix<T, N, O>& right) noexcept;
	// Multiplication for square matrices
	template<typename T, uint32_t N>
	SINI_CUDA_COMPAT Matrix<T,N,N> operator*= (Matrix<T,N,N>& left, const Matrix<T,N,N>& right) noexcept;
	
	// Multiplication with vector (matrix*vector)
	template<typename T, uint32_t M, uint32_t N>
	SINI_CUDA_COMPAT Vector<T,M> operator* (const Matrix<T,M,N>& mat, const Vector<T,N>& vec) noexcept;
	// vector*matrix multiplication must be done by converting the vector
	// to a row vector, i.e. a 1xN matrix. The reason for this design is
	// to only allow vector*matrix multiplication when you really want it
	// (and hopefully know what you're doing)

	// Multiplication with scalar
	template<typename T, uint32_t M, uint32_t N>
	SINI_CUDA_COMPAT Matrix<T,M,N> operator*= (Matrix<T,M,N>& mat, const T scalar) noexcept;
	template<typename T, uint32_t M. uint32_t N>
	SINI_CUDA_COMPAT Matrix<T,M,N> operator* (const Matrix<T,M,N>& mat, const T scalar) noexcept;
	template<typename T, uint32_t M, uint32_t N>
	SINI_CUDA_COMPAT Matrix<T,M,N> operator* (const T scalar, const Matrix<T,M,N>& mat) noexcept;

} // namespace sini

#include "sini/math/Matrix.inl"
#endif // !SINI_MATRIX_H