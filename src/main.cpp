#include "main.hpp"
#include <iostream>

template<size_t rows, size_t cols, typename Int = int>
class Matrix
{
	/* Constants */
	public: enum : size_t { NumRows = rows, NumColumns = cols, Size = rows * cols };
	
	/* Contiguous Matrixes */
	public: template<size_t _Cols, typename _Int> using MulCompatMtrx = Matrix<cols, _Cols, _Int>;
	public: template<size_t _Cols> using MulCompatProduct = Matrix<rows, _Cols, Int>;
	
	/* Constructors */

	public: inline constexpr Matrix()
	{}
	
	public: inline constexpr Matrix(const std::initializer_list<Int>& __l)
	{ std::copy(__l.begin(), __l.begin() + Size, _DATA_); }
	
	public: inline constexpr Matrix(const std::initializer_list<std::initializer_list<Int>>& __ll)
	{ for (size_t i = 0; i < Size; i++) _DATA_[i] = *((__ll.begin() + RowIndex(i))->begin() + ColIndex(i)); }
	
	public: inline constexpr Matrix(const Int arr[Size])
	{ std::copy(arr, arr + Size, _DATA_); }
	
	public: inline constexpr Matrix(const Int arr[rows][cols])
	{ for (size_t i = 0; i < Size; i++) _DATA_[i] = arr[RowIndex(i)][ColIndex(i)]; }
	
	public: inline constexpr Matrix(const Matrix& other)
	{ std::copy( other._DATA_, other._DATA_ + Size, _DATA_ ); }
	
	/* Getters */
	
	public: inline constexpr Int& Get(size_t Index)
	{ return _DATA_[Index]; }
	
	public: inline constexpr Int Get(size_t Index) const
	{ return _DATA_[Index]; }
	
	public: inline constexpr Int& Get(size_t Row, size_t Col)
	{ return Get(AbsIndex(Row, Col)); }
	
	public: inline constexpr Int Get(size_t Row, size_t Col) const
	{ return Get(AbsIndex(Row, Col)); }
	
	/* Index transformations */
	
	public: static inline constexpr size_t AbsIndex(Int Row, Int Col)
	{ return Row * cols + Col; }
	
	public: static inline constexpr size_t ColIndex(size_t Index)
	{ return Index % cols; }
	
	public: static inline constexpr size_t RowIndex(size_t Index)
	{ return (Index - ColIndex(Index)) / cols; }
	
	/* Operations */
	
	public: template<typename _Int> inline constexpr Matrix& operator+=(const Matrix& other)
	{
		for (size_t i = 0; i < Size; i++)
		Get(i) += other.Get(i);
		return *this;
	}
	
	public: template<typename _Int> inline constexpr Matrix operator+(const Matrix& other) const
	{ Matrix out = *this; out += other; return out; }
	
	public: template<typename _Int> inline constexpr Matrix& operator*=(const _Int& rhs)
	{ for (size_t i = 0; i < Size; i++) Get(i) *= rhs; return *this; }
	
	public: template<typename _Int> inline constexpr Matrix operator*(const _Int& rhs) const
	{ Matrix out(*this); return out *= rhs; }
	
	public: template<size_t _Cols, typename _Int> inline constexpr
	MulCompatProduct<_Cols> operator*(const MulCompatMtrx<_Cols, _Int>& rhs) const
	{
		MulCompatProduct<_Cols> out;
		for (size_t i = 0; i < out.Size; i++)
			out.Get(i) = ColumnRowProduct(rhs, out.RowIndex(i), out.ColIndex(i));
		return out;
	}
	
	/* Representation */
	public: inline constexpr std::string fmt() const
	{
		std::string out = "{ ";
		std::string buff("  ");
		bool is_end;
		for (size_t i = 0; i < Size; i++)
		{
			is_end = (i + 1) % cols == 0;
			buff = std::to_string(_DATA_[i]);
			out = out + ' ' + ( (_DATA_[i] < 10 && _DATA_[i] >= 0) ? '0' + buff : buff) + ' ' + ( is_end ? " }\n" : "") + ( is_end && i + 1 != Size ? "{ " : "");
		}
		return out;
	} 
	
	/* Internal functions */
	private: template<size_t _Cols, typename _Int> inline constexpr Int
	ColumnRowProduct(const MulCompatMtrx<_Cols, _Int>& other, size_t Row, size_t Col) const
	{
		Int out = 0;
		for (size_t i = 0; i < cols; i++)
			out += Get(Row, i) * other.Get(i, Col);
		return out;
	}
	
	/* Internal data */
	private: Int _DATA_[Size];
	
};

template<size_t rows, size_t cols, typename Int, typename _Int>
Matrix<rows, cols, Int> operator*(_Int lhs, const Matrix<rows, cols, Int>& rhs)
{ auto out(rhs); return out *= lhs; }

int main(int argc, char* argv[])
{
	
	/*
	{
		MulCompatProduct<_Cols> out;
		for (size_t i = 0; i < out.Size; i++)
			out.Get(i) = ColumnRowProduct(other, out.RowIndex(i), out.ColIndex(i));
		return out;
	}
	*/
	
	Matrix<2,3> A = {
		2, 3, 5,
		7, 2, 4
	};
	
	Matrix<3,2> C = {
		 1, 6,
		 7, 2,
		 0, -5
	};
	
	auto m3 = A * C;
	
	std::cout << (2 * A).fmt() << std::endl;
	
	return 0;
}
