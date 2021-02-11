#ifndef __INK__MATRIX__HEADER_FILE__
#define __INK__MATRIX__HEADER_FILE__

#include <stdlib.h>
#include <math.h>
#include <concepts>
#include <span>

#ifdef _DEBUG
#include <string>
#endif

namespace ink {
	
	// Template structs for parameter pack evaluation
	template<bool...> struct bool_pack;
	template<bool... v> using all_true = std::is_same< bool_pack<true, v...>, bool_pack<v..., true> >;
	
	class BasicMatrix {};
	
	template<size_t rows, size_t cols = rows, typename Int = int>
	requires( rows * cols > 0 && std::is_arithmetic<Int>::value )
	class StaticMatrix: BasicMatrix
	{
		/* Constants */
		public: enum : size_t
		{ NumRows = rows, NumColumns = cols, Size = rows * cols };
		
		/* Contiguous Matrixes */
		public: template<size_t _Cols, typename _Int>
		using MulCompatMtrx = StaticMatrix<cols, _Cols, _Int>;
		
		public: template<size_t _Cols>
		using MulCompatProduct = StaticMatrix<rows, _Cols, Int>;
		
		/* Constructors */
		
		public: constexpr
		StaticMatrix()
		{  }
		
		public: constexpr
		StaticMatrix(const std::initializer_list<Int>& __l)
		{ *this = __l.begin(); }
		
		public: constexpr
		StaticMatrix(const std::initializer_list< std::initializer_list<Int> >& __ll)
		{ for (size_t i = 0; i < Size; i++) M_data[i] = *((__ll.begin() + RowIndex(i))->begin() + ColIndex(i)); }
		
		public: constexpr
		StaticMatrix(const Int arr[Size])
		{ std::copy(arr, arr + Size, M_data); }
		
		public: constexpr
		StaticMatrix(const Int arr[rows][cols])
		{ for (size_t i = 0; i < Size; i++) M_data[i] = arr[RowIndex(i)][ColIndex(i)]; }
		
		public: constexpr
		StaticMatrix(const StaticMatrix& other)
		{ std::copy( other.M_data, other.M_data + Size, M_data ); }
		
		public: template<typename... args>  constexpr
		StaticMatrix(args... elems) requires( sizeof...(args) == Size && all_true<std::is_convertible<args&, Int&>{}...>::value )
		{ *this = std::move({ elems... }); }
		
		/* Getters */
		
		public: constexpr Int&
		Get(size_t Index)
		{ return M_data[Index]; }
		
		public: constexpr Int
		Get(size_t Index) const
		{ return M_data[Index]; }
		
		public: constexpr Int&
		Get(size_t Row, size_t Col)
		{ return Get(AbsIndex(Row, Col)); }
		
		public: constexpr Int
		Get(size_t Row, size_t Col) const
		{ return Get(AbsIndex(Row, Col)); }
		
		/* Index transformations */
		
		public: static constexpr size_t
		AbsIndex(Int Row, Int Col)
		{ return Row * cols + Col; }
		
		public: static constexpr size_t
		ColIndex(size_t Index)
		{ return Index % cols; }
		
		public: static constexpr size_t
		RowIndex(size_t Index)
		{ return (Index - ColIndex(Index)) / cols; }
		
		/* External Operations */
		
		public: template<typename _Int> constexpr StaticMatrix&
		operator+=(const StaticMatrix& other)
		{
			for (size_t i = 0; i < Size; i++)
			Get(i) += other.Get(i);
			return *this;
		}
		
		public: template<typename _Int> constexpr StaticMatrix
		operator+(const StaticMatrix& other) const
		{ StaticMatrix out = *this; out += other; return out; }
		
		public: template<typename _Int> constexpr StaticMatrix&
		operator*=(const _Int& rhs) requires( std::is_arithmetic<_Int>::value )
		{ for (size_t i = 0; i < Size; i++) Get(i) *= rhs; return *this; }
		
		public: template<typename _Int> constexpr StaticMatrix
		operator*(const _Int& rhs) const requires( std::is_arithmetic<_Int>::value )
		{ return StaticMatrix(*this) *= rhs; }
		
		public: template<size_t _Cols, typename _Int> constexpr MulCompatProduct<_Cols>
		operator*(const MulCompatMtrx<_Cols, _Int>& rhs) const
		{
			MulCompatProduct<_Cols> out;
			for (size_t i = 0; i < out.Size; i++)
				out.Get(i) = ColumnRowProduct(rhs, out.RowIndex(i), out.ColIndex(i));
			return out;
		}
		
		/* Internal Operations */
		
		public: static constexpr StaticMatrix
		UnitMatrix() requires ( rows == cols )
		{ StaticMatrix out; for (size_t i = 0; i < Size; i++) out.Get(i) = out.ColIndex(i) == out.RowIndex(i); return out; }
		
		public: constexpr Int
		Determinant() const requires ( rows == cols && rows == 2 )
		{ return Get(0,0) * Get(1,1) - Get(0,1) * Get(1,0); }
		
		public: constexpr Int
		Determinant() const requires ( rows == cols && rows == 3 )
		{
			return
				( Get(0,0) * Get(1,1) * Get(2,2) + Get(1,0) * Get(2,1) * Get(0,2) + Get(2,0) * Get(0,1) * Get(1,2) )
			-	( Get(0,2) * Get(1,1) * Get(2,0) + Get(0,0) * Get(2,1) * Get(1,2) + Get(1,0) * Get(0,1) * Get(2,2) )
			;
		}
		
		public: constexpr StaticMatrix
		Inverse() const requires ( rows == cols && rows == 2 )
		{
			StaticMatrix out;
			if ( Determinant() == 0 ) return out;
			out = Determinant() * (*this);
			return out;
		}
		
		public: constexpr StaticMatrix
		Inverse() const requires ( rows == cols && rows == 3 )
		{
			StaticMatrix out;
			
			if (Determinant() == 0) return out;
			
			out.Get(0) = Get(1, 1) * Get(2, 2) - Get(1, 2) * Get(2, 1);
			out.Get(1) = Get(0, 2) * Get(2, 1) - Get(0, 1) * Get(2, 2);
			out.Get(2) = Get(0, 1) * Get(1, 2) - Get(0, 2) * Get(1, 1);
			
			out.Get(3) = Get(1, 2) * Get(2, 0) - Get(1, 0) * Get(2, 2);
			out.Get(4) = Get(0, 0) * Get(2, 2) - Get(0, 2) * Get(2, 0);
			out.Get(5) = Get(0, 2) * Get(1, 0) - Get(0, 0) * Get(1, 2);
			
			out.Get(6) = Get(1, 0) * Get(2, 1) - Get(1, 1) * Get(2, 0);
			out.Get(7) = Get(0, 1) * Get(2, 0) - Get(0, 0) * Get(2, 1);
			out.Get(8) = Get(0, 0) * Get(1, 1) - Get(0, 1) * Get(1, 0);
			
			return out *= (1 / Determinant());
		}
		
		/* Internal functions */
		private: template<size_t _Cols, typename _Int> constexpr Int
		ColumnRowProduct(const MulCompatMtrx<_Cols, _Int>& other, size_t Row, size_t Col) const
		{
			Int out = 0;
			for (size_t i = 0; i < cols; i++)
				out += Get(Row, i) * other.Get(i, Col);
			return out;
		}
		
		/* Representation */
		public:  constexpr std::string
		fmt() const
		{
			std::string out = "{ ";
			std::string buff("  ");
			bool is_end;
			for (size_t i = 0; i < Size; i++)
			{
				is_end = (i + 1) % cols == 0;
				buff = std::to_string(M_data[i]);
				out = out + ' ' + ( (M_data[i] < 10 && M_data[i] >= 0) ? '0' + buff : buff) + ' ' + ( is_end ? " }\n" : "") + ( is_end && i + 1 != Size ? "{ " : "");
			}
			return out;
		} 
		
		/* Internal data */
		private: Int M_data[Size] = { 0 };
		
	};
	
	typedef int T;
	class Matrix
	{
		public: static constexpr size_t
		AbsIndex(size_t max_cols, size_t row, size_t col)
		{ return row * max_cols + col; }
		
		public: static constexpr size_t
		ColIndex(size_t max_cols, size_t index)
		{ return index % max_cols; }
		
		public: static constexpr size_t
		RowIndex(size_t max_cols, size_t index)
		{ return (index - ColIndex(max_cols, index)) / max_cols; }
		
		// Default Constructs Unit Matrix of 3x3.
		public: constexpr
		Matrix(): rows(3), cols(3), M_data(new T[9])
		{
			T res[9] = {
				(T)1, (T)0, (T)0,
				(T)0, (T)1, (T)0,
				(T)0, (T)0, (T)1,
			};
			std::move(res, res + 9, M_data);
		}
		
		// Constructs Unit Matrix of n x n dimensions.
		public: constexpr
		Matrix(size_t n): rows(n), cols(n), M_data(new T[n*n])
		{ for(size_t i = 0; i < n; i++) *(M_data + AbsIndex(cols, i, i) ) = 1; }
		
		// Constructs Matrix of rows x cols dimensions, with elements all = 0.
		public: constexpr
		Matrix(size_t rows, size_t cols):
		rows(rows), cols(cols), M_data(new T[rows * cols]) {}
		
		public: constexpr
		~Matrix() { delete M_data; }
		
		private: size_t rows, cols;
		private: T* M_data;
	};
	
}

template<size_t rows, size_t cols, typename Int, typename _Int>  constexpr
ink::StaticMatrix<rows, cols, Int> operator*(_Int lhs, const ink::StaticMatrix<rows, cols, Int>& rhs) requires( std::is_arithmetic<_Int>::value )
{ return ink::StaticMatrix<rows, cols, Int>(rhs) *= lhs; }

#endif
