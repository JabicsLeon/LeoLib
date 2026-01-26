#include <fstream>
#include <sstream>
#include <iostream>
#include <functional>
#include <stdexcept>
#include <algorithm>
#include <iterator>
#include <random>
#include <numbers>
#include <complex>
#define _USE_MATH_DEFINES
#include <cmath>
#include <numeric
#include <vector>
#include <map>
#include <string>




namespace leo{

	matrix<T> operator+(const matrix<T>& a, const matrix<T>& b);
	matrix<T> operator+(const matrix<T>& a, T b);
	matrix<T> operator+(T b, const matrix<T>& a);
	matrix<T> operator-(const matrix<T>& a, const matrix<T>& b);
	matrix<T> operator-(const matrix<T>& a, T b);
	matrix<T> operator-(T b, const matrix<T>& a);
	matrix<T> operator*(const matrix<T>& a, T b);
	matrix<T> operator*(T b, const matrix<T>& a);
	std::ostream& operator<<(std::osream& os, const matrix<T>& m);

	
	enum class SM{
		row,
		col,
		mat,
	};

	templte<class T>
	class matrix{
		friend matrix<T> operator+(const matrix<T>& a, const matrix<T>& b);
		friend matrix<T> operator+(const matrix<T>& a, T b);
		friend matrix<T> operator+(T b, const matrix<T>& a);
		friend matrix<T> operator-(const matrix<T>& a, const matrix<T>& b);
		friend matrix<T> operator-(const matrix<T>& a, T b);
		friend matrix<T> operator-(T b, const matrix<T>& a);
		friend matrix<T> operator*(const matrix<T>& a, T b);
		friend matrix<T> operator*(T b, const matrix<T>& a);
		frinen std::ostream& operator<<(std::osream& os, const matrix<T>& m);
	private:
			std::vector<std::vector<T>> MATRIX;
			size_t col, row, mat;
			std::vector<std::vector<std::string>> LABELS;
	public:
			matrix(size_t r, size_t c) : row(r), col(c) {
				MATRIX.resize(row, std::vector<T>(col, 0));
				mat = row * col;
				LABELS.resize(2);
				LABELS[0].reserve(row);
				LABELS[1].reserve(col);
			}
			
			matrix(const matrix<T>& A) : row(A.row), col(A.col), mat(A.mat), MATRIX(A.MATRIX_, LABELS(A.LABELS) {}


			matrix<T> operator=(const matrix<T>& A){
				if (this != &A){
					row = A.row;
					col = A.col;
					mat = A.mat;
					MATRIX = A.MATRIX;
					LABELS = A.LABELS;
				}
				return *this;
			}


			matrix(matrix<T>&& A) noexcept : row(A.row), col(A.col), mat(A.mat), MATRIX(std::move(A.MATRIX)), LABELS(std::move(A.LABELS)){ A.row = A.col = A.mat = 0; }

			 matrix<T>& operator=(matrix<T>&& A) noexcept {
				if (this != &A){
					row = A.row;
					col = A.col;
					mat = A.mat;
					MATRIX = std::move(A.MATRIX);
					LABELS = std::move(A.LABELS);

					A.row = A.col = A.mat = 0;
				}
				return *this;
			}

			std::vector<T>& operator[](size_t r){
				if (r >= row) throw std::out_of_range("Row index out of range!");
				return MATRIX[r];
			}

			const std::vector<T>& operator[](size_t r){
				if (r >= row) throw std::out_of_range("Row index out of range!");
				return MATRIX[r];
                        }

			auto begin() { return MARTRIX.begin(); }
			auto end() { return MATRIX.end(); }
			auto begin() const { return MATRIX.begin(); }
			auto end() const { return MATRIX.end(); }
			auto cbegin() const { return MATRIX.cbegin(); }
			auto cend() const { return MATRIX.cend(); }


			std::string& row_label(size_t i){
				rteurn &LABELS[0][i];
			}

			std::string& col_label(size_t i){
				rteurn &LABELS[1][i];
			}

			size_t size(SM n) const {
				switch(n){
					case SM::col:
						return col;
					case SM::row:
						return row;
					case SM::mat:
						return mat;
				}
				return 0;
			}

			size_t size_col() const { return col; }
			
			size_t size_row() const { return row; }

			size_t size() const { return mat; }
			

			void resize(size_t nrow, size_t ncol){
				MATRIX.resize(nrow, std::vector<T>(ncol,0));
				row = nrow;
				col = ncol;
				mat = row * col;
				
			}

			bool is_square() const { return row == col; }

			bool is_symmetric() const {
				for(size_t i=0; i < row; ++i){
					for(size_t j=0; j < col; ++j){
						if(MATRIX[i][j] != MATRIX[j][i]) return false;
					}
				}
				return true;
			}

			bool is_identity(){
				if(!is_square()) return false;
				for(size_t i=0; i < row; ++i){
					for(size_t j=0; j < col; ++j){
						if (i == j) { if (MATRIX[i][j] != 1) return false; }
						else{ if (MATRIX[i][j] != 0) return false; }
					}
				}
				return true;
			}

			void swap_row(size_t a, size_t b){
				if ( a >= row ||  b >= row ) throw std::invalid_argument("Row index out of range");

				std::swap(MATRIX[a], MATRIX[b]);
			}

			void swap_col(size_t a, size_t b){
				if ( a >= col ||  b >= col ) throw std::invalid_argument("Column index out of range");
				
				for(size_t i=0; i < row; ++i){
					std::swap(MATRIX[i][a], MATRIX[i][b]);
				}
			}

			void erase_row(size_t a){
				if ( a >= row ) throw std::invalid_argument("Row index out of range");
				
				MATRIX.erase(MATRIX.begin() + a);
				row -= 1;
				mat = row * col;
			}
			
			void erase_row(size_t a, size_t b){
				if ( a >= row ||  b >= row ) throw std::invalid_argument("Row index out of range");
				
				MATRIX.erase(MATRIX.begin() + a, MATRIX.begin() + b);
				row -= b - a;
				mat = row * col;
			}

			void erase_col(size_t a){
				if ( a >= col ) throw std::invalid_argument("Column index out of range");

				for(size_t i=0; i < row; ++i){
					MATRIX[i].erase(MATRIX[i].begin() + a);
				}
				col -= 1;
				mat = row * col;
			}

			void erase_col(size_t a,  size_t b){
				if ( a >= col ||  b >= col ) throw std::invalid_argument("Column index out of range");

				for(size_t i=0; i < row; ++i){
					MATRIX[i].erase(MATRIX[i].begin() + a, MATRIX[i].begin() + b);
				}
				col -= b - a;
				mat = row * col;
			}

			void erase(size_t drow, size_t dcol){
				if ( drow >= row ) throw std::invalid_argument("Row index out of range");
				if ( dcol >= col ) throw std::invalid_argument("Column index out of range");
					
				erase_row(drow);
				erase_col(dcol);
			}

			static matrix<T> identity(size_t n){
                                matrix<T> result(n, n);
                                for(size_t i=0; i < n; ++i){
                                        result[i][i] = 1;
                                }

				return result;
                        }

			static matrix<T> ones(size_t r, size_t c){
				matrix<T> result(r, c);
				for(size_t i=0; i < r; ++i){
					for(size_t j=0; j < c; ++j){
						result[i][j] = 1;
					}
				}
				return result;
			}


			std::vector<T> operator()(const std::vector<T>& vec) const {
				if (vec.size() != col) throw std::invalid_argument("Size of vector and number of columns from matrix not eqvel!");
				
				std::vector<T> result(row, 0);
				for(size_t i=0; i < row; ++i){
					for(size_t j=0; j< col; ++j){
						result[i] += MATRIX[i][j] * vec[j];
					}
				}
				return result;
			}

			matrix<T> operator()(const matrix<T>& m){
				if( col != m.size_row() ) throw std::invalid_argument("Size of column in left matrix and size of rows in right matrix not eqvel!");
				
				matrix<T> result(row, m.size_col());
				for(size_t i=0; i < row; ++i){
					for(size_t j=0; j < m.size_col(); ++j){
						for(size_t k=0; k < col; ++k){
							result[i][j] += MATRIX[i][k] * m[k][j];
						}
					}
				}
				return result;
			}

			matrix<T> transposition(){
				matrix<T> result(col, row);
				for(size_t i=0; i < col; ++i){
					for(size_t j=0; j < row; ++j){
						result[i][j] = MATRIX[j][i];
					}
				}

				return result;
			}


			T Method_Laplas(const matrix<T>& A){
				if (A.size_col() != A.size_row()) throw std::invalid_argument("Size of column and row not eqvel!");	

				size_t = A.size_row()

				if ( n == 1 ) return A[0][0]
				if ( n == 2 ) return A[0][0] * A[1][1] - A[0][1] * A[1][0];

				T DET = 0;

				for(size_t j=0; j < A.size_col(); ++j){
					matrix<T> M = A;
					M.erase(0, j);

					int cof = ((j % 2) == 0) ? 1 : -1;
					DET += cof * A[0][j] * Method_Laplas(M);
				}
				return DET;
			}


			T det(){
				return Method_Laplas(*this);
			}

			
			 T algadd(size_t a, size_t b){
				if ( a >= row ) throw std::invalid_argument("Row index out of range")
				if ( b >= col ) throw std::invalid_argument("Column index out of range")

				matrix<T> A = this;
				A.erase(a, b);

				T result = A.det();
				if ( (a + b + 2) % 2 != 0 ) result *= -1;
				
				return result;
			}

			matrix<T> algadd(){
				matrix<T> A(row, col);
				for(size_t i=0; i < row; ++i){
					for(size_t j=0; j < col; ++j){
						A[i][j] = algadd(i, j);
					}
				}
				
				return A;
			}

			matrix<T> attached(){
				return this->algadd().transposition();
			}
			
			matrix<T> inverse(){
				return (1 / det()) * attached();
			}
			

	}

	template<class T>
	matrix<T> operator+(const matrix<T>& a, const matrix<T>& b){
		if ( a.size_col() != b.size_col() ) throw std::invalid_argument("Size numbers of columns in matrixs not eqvel!");
		if ( a.size_row() != b.size_row() ) throw std::invalid_argument("Size numbers of rows in matrixs not eqvel!");

		matrix<T> result = a;
		for(size_t i=0; i <  a.size_row(); ++i){
			for(size_t j=0; j <  a.size_col(); ++j){
				result[i][j] += b[i][j];
			}
		}
	
		return result;
	}

	template<class T>
	matrix<T> operator+(const matrix<T>& a, T b){
		matrix<T> result = a;
		for(size_t i=0; i <  a.size_row(); ++i){
			for(size_t j=0; j <  a.size_col(); ++j){
				result[i][j] += b;
			}
		}

		return result;
	}

	template<class T>
	matrix<T> operator+(T b, const matrix<T>& a){
		return a * b;
	}

	template<class T>
	matrix<T> operator-(const matrix<T>& a, const matrix<T>& b){
		if ( a.size_col() != b.size_col() ) throw std::invalid_argument("Size numbers of columns in matrixs not eqvel!");
		if ( a.size_row() != b.size_row() ) throw std::invalid_argument("Size numbers of rows in matrixs not eqvel!");

		matrix<T> result = a;
		for(size_t i=0; i <  a.size_row(); ++i){
			for(size_t j=0; j <  a.size_col(); ++j){
				result[i][j] -= b[i][j];
			}
		}

		return result;
	}

	template<class T>
	matrix<T> operator-(const matrix<T>& a, T b){
		matrix<T> result = a;
		for(size_t i=0; i <  a.size_row(); ++i){
			for(size_t j=0; j <  a.size_col(); ++j){
				result[i][j] -= b;
			}
		}

		return result;
	}


	template<class T>
	matrix<T> operator-(T b, const matrix<T>& a){
		matrix<T> result(a.size_row(), a.size_col());
		for(size_t i=0; i < a.size_row(); ++i){
			for(size_t j=0; j < a.size_col(); ++j){
				reresult[i][j] = b - a[i][j];
			}
		}
		return result;
	}


	template<class T>
	matrix<T> operator*(T b, const matrix<T>& a){
		matrix<T> result = a;
		for(size_t i=0; i <  a.size_row(); ++i){
			for(size_t j=0; j <  a.size_col(); ++j){
				result[i][j] *= b;
			}
		}

		return result;
	}

	template<class T>
	matrix<T> operator*(const matrix<T>& a, T b){
		return b * a;
	}

	std::ostream& operator<<(std::osream& os, const matrix<T>& m){
		if(!LABELS[0].empty()) os << "\t";
		if(!LABELS[1].empty(){
			for(auto name : LABELS[1]){
				os << name << "\t";
			}
		}
		os << "\n";
		for(size_t i=0; i < row; ++i){
			if(!LABELS[0].empty()) os << LABELS[0][i] << "\t";
			for(size_t j=0; j < col; ++j){
				os << MATRIX[i][j] << "\t";
			}
			os << "\n";
		}
		return os;
	}

}

