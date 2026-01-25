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
#include <vector>
#include <map>
#include <string>

namespace leo{
	
	enum class SM{
		row;
		col;
		mat;
	};

	tempalte<class T, class B>
	class<T> matrix{
		friend matrix<T> operator+(const matrix<T>& a, const matrix<T>& b);
		friend matrix<T> operator+(const matrix<T>& a, T b);
		friend matrix<T> operator+(T b, const matrix<T>& a);
		friend matrix<T> operator*(const matrix<T>& a, T b);
		friend matrix<T> operator*(T b, const matrix<T>& a);
		private:
			std::vector<std::vector<T>> MATRIX;
			size_t col, row, mat;
			std::vector<std::vector<std::string>> LABELS;
			struct Headers { T ME; T VAR; T SIGMA; };
			std::vector<Headers> HEAD;
		public:
			matrix<T> (size_t r, size_t c) : row(r), col(c) {
				MATRIX.resize(row, std::vector<T>(col, 0));
				mat = row * col;
				HEAD.resize(col);
				LABELS.resize[2];
				LABELS[0].resize(row);
				LABELS[1].resize(col);
			}

			std::vector<T>& operator[](size_t r){
				if (r >= row) throw std::out_of_range("Row index out of range!");
				return MATRIX[r]
			}

			const std::vector<T>& operator[](size_t r){
				if (r >= row) throw std::out_of_range("Row index out of range!");
				return MATRIX[r]
                        }

			size_t size(SM n){
				switch(n){
					case SM::col:
						return col;
					case SM::row:
						return row;
					case SM::mat:
						return mat;
				}
			}

			size_t size_col() { return col; }
			
			size_t size_row() { return row; }

			size_t size() { return mat; }
			

			void resize(size_t nrow, size_t ncol){
				MATRIX.resize(nrow, 0);
				row = nrow;
				for (size_t i=0; i < row; ++i){
					MATRIiX[i].resize(ncol, 0);
				}
				col = ncol;
			}

			void swap_row(size_t a, size_t b){
				if ( a >= row ||  b >= row ) std::invalid_argument("Row index out of range");

				std::swap(MATRIX[a], MATRIX[b]);
			}

			void swap_col(size_t a, size_t b){
				if ( a >= col ||  b >= col ) std::invalid_argument("Column index out of range");
				
				for(size_t i=0; i < row; ++i){
					std::swap(MATRIX[i][a], MATRIX[i][a][b]);
				}
			}

			void erase_row(size_t a){
				if ( a >= row ) std::invalid_argument("Row index out of range");
				
				MATRIX.erase(MATRIX.begin() + a);
				row -= 1;
			}
			
			void erase_row(size_t a, size_t b){
				if ( a >= row ||  b >= row ) std::invalid_argument("Row index out of range");
				
				MATRIX.erase(MATRIX.begin() + a, MATRIX.begin() + b);
				row -= b - a;
			}

			void erase_col(size_t a){
				if ( a >= col ) std::invalid_argument("Column index out of range");

				for(size_t i=0; i < row; ++i){
					MATRIX[i].erase(MATRIX[i].begin() + a);
				}
				col -= 1;
			}

			void erase_col(size_t a,  size_t b){
				if ( a >= col ||  b >= col ) std::invalid_argument("Column index out of range");

				for(size_t i=0; i < row; ++i){
					MATRIX[i].erase(MATRIX[i].begin() + a, MATRIX[i].begin() + b);
				}
				col -= b - a;
			}

			void erase(size_t drow, size_t dcol){
				if ( drow >= row ) std::invalid_argument("Row index out of range");
				if ( dcol >= col ) std::invalid_argument("Column index out of range");
					
				erase_row(drow);
				erase_col(dcol);
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
				if( col != m.size_row() ) std::invalid_argument("Size of column in left matrix and size of rows in right matrix not eqvel!");
				
				matrix<T> result(row, m.size_col());
				for(size_t i=0; i < row; ++i){
					for(size_t j=0; j < m.size_col(); ++j){
						for(size_t k; k < col; ++k){
							result[i][j] += MATRIX[i][k] * m[k][j]
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
				if (A.size_col() != A.size_row()) std::invalid_argument("Size of column and row not eqvel!");	
				T DET = 0;
				if (A.size() != 4) {
					for(size_t j=0; j < A.size_col(); ++j){
						
						matrix<T> M = A;
						M.erase(0, j);

						int cof = 1;
						if ( (j + 2) % 2 != 0 ) cof = -1; 
						DET += cof * A[0][j] * ELEM;
					}
					return DET;
				}
				

				DET = A[0][0] * A[1][1] - A[0][1] * A[1][0];
				return DET;
			}


			T det(){
				return Method_Laplas(this);
			}

			
			 T algadd(size_t a, size_t b){
				if ( a >= row ) std::invalid_argument("Row index out of range")
				if ( b >= col ) std::invalid_argument("Column index out of range")

				matrix<T> A = this;
				A.erase(a, b);

				T result = A.det();
				if ( (i + j + 2) % 2 != 0 ) result *= -1;
				
				return result;
			}

			matrix<T> algadd(){
				matrix<T> A(row, col)
				for(size_t i=0; i < row; ++i){
					for(size_t j=0; j < col; ++j){
						A[i][j] = algadd(i, j);
					}
				}
				
				return A;
			}

			matrix<T> attached(){
				return this.algadd().transposition();
			}
			
			matrix<T> inverse(){
				return (1 / det()) * attached()
			}
			

	}

	template<class T>
	matrix<T> operator+(const matrix<T>& a, const matrix<T>& b){
		if ( a.size(SM::col) != b.size(SM::col) ) std::invalid_argument("Size numbers of columns in matrixs not eqvel!");
		if ( a.size(SM::row) != b.size(SM::row) ) std::invalid_argument("Size numbers of rows in matrixs not eqvel!");

		matrix<T> result = a;
		for(size_t i=0; i <  a.size(SM::row); ++i){
			for(size_t j=0; j <  a.size(SM::col); ++i){
				result[i][j] += b[i][j];
			}
		}
	
		return result;
	}

	template<class T>
	matrix<T> operator+(const matrix<T>& a, T b){
		matrix<T> result = a;
		for(size_t i=0; i <  a.size(SM::row); ++i){
			for(size_t j=0; j <  a.size(SM::col); ++i){
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
	matrix<T> operator*(T b, const matrix<T>& a){
		matrix<T> result = a;
		for(size_t i=0; i <  a.size(SM::row); ++i){
			for(size_t j=0; j <  a.size(SM::col); ++i){
				result[i][j] *= b;
			}
		}

		return result;
	}

	template<class T>
	matrix<T> operator*(const matrix<T>& a, T b);{
		return b * a;
	}

}
