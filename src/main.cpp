#include "main.hpp"
#include <iostream>

template<size_t rows, size_t cols, typename Int = int>
class Matrix
{
	
	/* Constants */
	private: enum : size_t { CompatRows = rows };
	public: enum : size_t { NumRows = rows, NumColumns = cols, Size = rows * cols };
	
	public: template<size_t otherCols, typename otherInt>
	using MulCompatMtrx = Matrix<cols, otherCols, otherInt>;
	
	public: template<size_t otherCols>
	using MulCompatProduct = Matrix<rows, otherCols, Int>;
	
	private: Int _DATA_[Size];
	
	/* Index Calculation Functions */
	
	// Absolute Array index of specified position
	public: inline static constexpr size_t GetAbsoluteIndex(Int Row, Int Col)
	{ return Row * cols + Col; }
	
	// Column index of absolute array index
	public: inline static constexpr size_t GetColumnIndex(size_t Index)
	{ return Index % cols; }
	
	// Row index of absolute array index
	public: inline static constexpr size_t GetRowIndex(size_t Index)
	{ return (Index - GetColumnIndex(Index)) / cols; }
	/* Index = Row * cols + GetColumnIndex(Index) */
	/* Row * cols = Index - GetColumnIndex(Index) */
	/* Row = (Index - GetColumnIndex(Index)) / cols */
	
	/* Getters */
	
	public: inline constexpr Int&
	Get(size_t Index)
	{ return _DATA_[Index]; }
	
	public: inline constexpr Int
	Get(size_t Index) const
	{ return _DATA_[Index]; }
	
	public: inline constexpr Int&
	Get(size_t Row, size_t Col)
	{ return Get(GetAbsoluteIndex(Row, Col)); }
	
	public: inline constexpr Int
	Get(size_t Row, size_t Col) const
	{ return Get(GetAbsoluteIndex(Row, Col)); }
	
	/* Constructors */
	
	/* Default constructor
	*/
	public: inline constexpr
	Matrix() {}
	
	/* Initializer list constructor
	*/
	public: inline constexpr
	Matrix(const std::initializer_list<Int>& __l)
	{ std::copy(__l.begin(), __l.begin() + Size, _DATA_); }
	
	/* 2D Initializer list constructor
	*/
	public: inline constexpr
	Matrix(const std::initializer_list<std::initializer_list<Int>>& __ll)
	{ for (size_t i = 0; i < Size; i++) _DATA_[i] = *((__ll.begin() + GetRowIndex(i))->begin() + GetColumnIndex(i)); }
	
	/* Array constructor
	*/
	public: inline constexpr
	Matrix(const Int arr[Size])
	{ std::copy(arr, arr + Size, _DATA_); }
	
	/* 2D array constructor
	*/
	public: inline constexpr
	Matrix(const Int arr[rows][cols])
	{ for (size_t i = 0; i < Size; i++) _DATA_[i] = arr[GetRowIndex(i)][GetColumnIndex(i)]; }
	
	/* Operations */
	
	public: inline constexpr Matrix<rows, cols, Int>&
	operator*=(int rhs)
	{ for (size_t i = 0; i < Size; i++) Get(i) = rhs * Get(i); return *this; }
	
	public: inline constexpr Matrix
	operator*(int rhs)
	{ Matrix out = *this; return out *= rhs; }
	
	/* Internal helper function for getting the result of the multiplication between the column of the calling matrix
	   and the row of the other matrix
	*/
	private: template<size_t otherCols, typename otherInt> inline constexpr Int
	ColumnRow_Product(const MulCompatMtrx<otherCols, otherInt>& other, size_t Row, size_t Col) const
	{
		Int out = 0;
		for (size_t i = 0; i < cols; i++) {
			out += Get(Row, i) * other.Get(i, Col);
			printf("%i * %i + ", Get(Row, i), other.Get(i, Col));
		}
		printf("0 = %i\n", out);
		return out;
	}
	
	public: template<size_t otherCols, typename otherInt> inline constexpr MulCompatProduct<otherCols>
	operator*(const MulCompatMtrx<otherCols, otherInt>& other) const
	{
		MulCompatProduct<otherCols> out;
		for (size_t i = 0; i < out.Size; i++) {
			out.Get(i) = ColumnRow_Product(other, out.GetRowIndex(i), out.GetColumnIndex(i));
		}
		return out;
	}
	
	public: template<typename otherInt> inline constexpr Matrix<rows, cols, otherInt>&
	operator+=(const Matrix<rows, cols, otherInt>& other)
	{ for (size_t i = 0; i < Size; i++) Get(i) += other.Get(i); return *this; }
	
	public: template<typename otherInt> inline constexpr Matrix<rows, cols, otherInt>
	operator+(const Matrix<rows, cols, otherInt>& other) const
	{ Matrix<rows, cols, otherInt> out = *this; return out += other; }
	
	/* Debug */
	// Expensive formatted string representation of the matrix
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
	
};

int main(int argc, char* argv[])
{
	
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
	
	std::cout << m3.fmt() << std::endl;
	
	return 0;
}
