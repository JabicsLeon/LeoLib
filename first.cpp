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
	private:
			std::vector<std::vector<T>> MATRIX;
			size_t col, row, mat;
			std::vector<std::vector<std::string>> LABELS;
	public:
		friend matrix<T> operator+(const matrix<T>& a, const matrix<T>& b);
		friend matrix<T> operator+(const matrix<T>& a, T b);
		friend matrix<T> operator+(T b, const matrix<T>& a);
		friend matrix<T> operator-(const matrix<T>& a, const matrix<T>& b);
		friend matrix<T> operator-(const matrix<T>& a, T b);
		friend matrix<T> operator-(T b, const matrix<T>& a);
		friend matrix<T> operator*(const matrix<T>& a, T b);
		friend matrix<T> operator*(T b, const matrix<T>& a);
		friend std::ostream& operator<<(std::osream& os, const matrix<T>& m);
	public:
		friend class iterator_horizontal;
		friend class iterator_vertical;
	
		iterator_horizontal h_begin();
		iterator_horizontal h_end();

		iterator_vertical row_begin();
		iterator_vertical row_end();

		auto Column(size_t k) const {
			return ColumnRange{this, k};
		}

		auto Row(size_t k) const {
			return RowRange{this, k}
		}

	private:
		struct ColumnRange {
			const matrix<T>* M;
			size_t col_idx;

			auto begin() { return iterator_vertical(const_cast<matrix<T>*>(M), 0, col_idx); }
			auto end() { return begin() + M->size_row(); }
		};

		struct RowRange {
			const matrix<T>* M;
			size_t row_idx;

			auto begin() { return iterator_horizontal(const_cast<matrix<T>*>(M), row_idx, 0); }
			auto end() { return begin() + M->size_col(); }
		}
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

			matrix<T> Method_Gauss(const matrix<T>& A){
				if(!is_square()) throw std::invalid_argument("Method_Gauss: matrix is not square!");

				size_t n = A.size_row();

				matrix<T> B = A;
				matrix<T> I = identity(n);

				for(size_t k=0; k < n; ++k){
					auto col_begin = iterator_vertical(&B, k, k);
					auto col_end = iterator_vertical(&B, n - 1, k);
					auto it  = std::max_element(col_begin, col_end);
					auto dist_in_col = std::distance(col_begin, it);
					if(dist_in_col != k) {
						B.swap_row(k, k + dist_in_col);
						I.swap_row(k, k + dist_in_col);
					}

					if(std::abs(B[k][k]) < 1e-10) throw std::invalid_argument("Method_Gauss: matrix is singular");

					T pivot = B[k][k];
					for(size_t j=k; j < B.size_col(); ++j){
						B[k][j] /= pivot;
						I[k][j] /= pivot;
					}

					for(size_t i=k+1; i < n; ++i){
						T factor = B[i][k];
						if(std::abs(factor) > 1e-10){
							for(size_t j=k; j < n; ++j){
								B[i][j] -= factor * B[k][j];
								I[i][j] -= factor * I[k][j];
							}
						}
					}
				}

				for(int i=n-1; i >= 0; --i){
					for(int j=i-1; j >= 0; --j){
						T factor = B[j][i] / B[i][i];
						B[j][i] -= factor * B[i][i];
						for(size_t k = 0; k < n; ++k){
							I[j][k] -= factor * I[i][k];
						}
					}
				}

				return I;

			}


			T Method_Laplas(const matrix<T>& A){
				if (!is_square()) throw std::invalid_argument("Method_Laplas: matrix is not square!");	

				size_t n = A.size_row()

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
				if(size() <= 9) return (1 / det()) * attached();
				else return Method_Gauss(*this);
			}
			

	};

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


	template<class T>
        class matrix<T>::iterator_horizontal {
        private:
                matrix<T>* M;
                size_t index;
		
		size_t calculate_index(size_t r, size_t c) {
			return c + r * M->size_col();
		}

                void calculate_position(size_t idx, size_t& r, size_t& c) {
			c = idx % M -> size_col();
			r = idx / M -> size_col();
		}

		bool is_valid_position(size_t r, size_t c){
			return r < M->size_row() && c < M->size_col();
		}

        public:
                using iterator_category = std::random_access_iterator_tag;
                using value_type = T;
                using difference_type = std::ptrdiff_t;
                using pointer = T*;
                using reference = T&;

		explicit iterator_horizontal(matrix<T>* m, size_t idx = 0) : M(m), index(idx) {
                        if (index > M->size()) index = M->size();
                }

		iterator_horizontal(matrix<T>* m, size_t r = 0, size_t c = 0) : M(m) {
                        if (is_valid_position(r, c)) index = calculate_index(r, c);
                        else index = M->size();
                }
		

		reference operator*() {
			size_t r, c;
			calculate_position(index, r, c);
			if (!is_valid_position(r, c)) throw std::out_of_range("iterator_vertical: dereferencing out of range!");
			return (*M)[r][c];
                }

		pointer operator->() {
			return &(*(*this));
		}

                iterator_horizontal& operator++() { ++index; return *this; }
                iterator_horizontal operator++(int) { iterator_horizontal tmp = *this; ++index; return tmp; }
                iterator_horizontal& operator--() { --index; return *this; }
                iterator_horizontal operator--(int)  { iterator_horizontal tmp = *this; --index; return tmp; }

                iterator_horizontal& operator+=(difference_type n) { index += n; return *this; }
                iterator_horizontal& operator-=(difference_type n) { index -= n; return *this; }

                friend iterator_horizontal operator+(iterator_horizontal it, difference_type n) { return iterator_horizontal(it.M, it.index + n); }
                friend iterator_horizontal operator+(difference_type n, iterator_horizontal it) { return it + n; }
                friend iterator_horizontal operator-(iterator_horizontal it, difference_type n) { return iterator_horizontal(it.M, it.index - n); }
                friend difference_type operator-(const iterator_horizontal& a, const iterator_horizontal& b) { return a.index - b.index;  }

                reference operator[](difference_type n) const { return *(*this + n); }

                bool operator==(const iterator_horizontal& other) const { return index == other.index && M == other.M; }
                bool operator!=(const iterator_horizontal& other) const { return !(*this == other); }
                bool operator<(const iterator_horizontal& other) const { return index < other.index; }
                bool operator>(const iterator_horizontal& other) const { return index > other.index; }
                bool operator<=(const iterator_horizontal& other) const { return index <= other.index; }
                bool operator>=(const iterator_horizontal& other) const { return index >= other.index; }
        };

	template<class T>
        typename matrix<T>::iterator_horizontal matrix<T>::h_begin() { return iterator_horizontal(this, 0); }

        template<class T>
        typename matrix<T>::iterator_horizontal matrix<T>::h_end() { return iterator_horizontal(this, this -> size()); }
	


	template<class T>
	class matrix<T>::iterator_vertical {
	private:
		matrix<T>* M;
		size_t index;

		size_t calculate_index(size_t r, size_t c) { 
			return r + c * M->size_row(); 
		}

		void calculate_position(size_t idx, size_t& r, size_t& c) { 
			r = idx % M -> size_row();
			c = idx / M -> size_row();
		}

		bool is_valid_position(size_t r, size_t c){
			return r < M->size_row() && c < M->size_col();
		}

	public:
		using iterator_category = std::random_access_iterator_tag;
		using value_type = T;
		using difference_type = std::ptrdiff_t;
		using pointer = T*;
		using reference = T&;

		explicit iterator_vertical(matrix<T>* m, size_t idx = 0) : M(m), index(idx) { 
			if (index > M->size()) index = M->size();
		}

		iterator_vertical(matrix<T>* m, size_t r = 0, size_t c = 0) : M(m) { 
			if (is_valid_position(r, c)) index = calculate_index(r, c);
			else index = M->size();
		}

		reference operator*() {
			size_t r, c;
			calculate_position(index, r, c);
			if (!is_valid_position(r, c)) throw std::out_of_range("iterator_vertical: dereferencing out of range!");
			return (*M)[r][c];
		}

		pointer operator->() {
			return &(*(*this));
		}

		iterator_vertical& operator++() { ++index; return *this; }
		iterator_vertical operator++(int) { iterator tmp = *this; ++index; return tmp; } 
		iterator_vertical& operator--() { --index; return *this }
		iterator_vertical operator--(int) { iterator tmp = *this; --index; return tmp; } 

		iterator_vertical& operator+=(difference_type n) { index += n; return *this; }
		iterator_vertical& operator-=(difference_type n) { index -= n; return *this; }
		
		friend iterator_vertical operator+(iterator_vertical it, difference_type n) { return iterator_vertical(it.M, it.index + n); }
		friend iterator_vertical operator+(difference_type n, iterator_vertical it) { return it + n; }
		friend iterator_vertical operator-(iterator_vertical it, difference_type n) { return iterator_vertical(it.M, it.index - n); }
		friend difference_type operator-(const iterator_vertical& a, const iterator_vertical& b) { return a.index - b.index;  }

		reference operator[](difference_type n) const { return *(*this + n); }
		
		bool operator==(const iterator_vertical& other) const { return index == other.index && mat == other.mat; }
                bool operator!=(const iterator_vertical& other) const { return !(*this == other); }
                bool operator<(const iterator_vertical& other) const { return index < other.index; }
                bool operator>(const iterator_vertical& other) const { return index > other.index; }
                bool operator<=(const iterator_vertical& other) const { return index <= other.index; }
                bool operator>=(const iterator_vertical& other) const { return index >= other.index; }
	
	}
	
	template<class T>
        typename matrix<T>::iterator_verticlal matrix<T>::v_begin() { return iterator_verticlal(this, 0); }

	template<class T>
	typename matrix<T>::iterator_verticlal matrix<T>::v_end() { return iterator_verticlal(this, *this -> size()); }


}

namespace leo{
	const double pi = M_PI;

	template<class Iterator>
	auto MathExcept(Iterator start, Iterator end){
		using ValueType = typename std::iterator_traits<Iterator>::value_type;

		if(start == end) throw std::invalid_argument("MathEx: empty datum!");

		ValueType sume = std::accumulate(start, end, ValueType{});

		auto n = std::distance(start, end);

		return static_cast<double>(sum) / n;
	}

	template<class Iterator>
	auto RMS(Iterator start, Iterator end){
		if(start == end) throw std::invalid_argument("RMS: empty datum!");
	
		double result = std::inner_product(start, end, start, 0.0);

		result /= std::distance(start, end);
		
		return std::sqrt(result);
		
	}

	template<class Iterator>
	auto Variation(Iterator start, Iterator end){
		if(start == end) throw std::invalid_argument("Variation: empty datum!");
		
		auto n = std::distance(start, end);

		auto M = MathExcept((start, end);

		std::vector<double> res;
		res.reserve(n);
		
		std::transform(start, end, std::back_inserter(res), [M] (double x) { return x - M; });

		return RMS(res.begin(), res.end());
	}

	template<class T>
	matrix<T> Cov(matrix<T> A){
		size_t ol = A.size_col(); 
		size_t row = A.size_row();

		matrix<T> M(1, col);

		for(size_t i=0; i < col; ++i){
			auto start = matrix<T>::iterator_vertical(&A, 0, i);
			auto end = matrix<T>::iterator_vertical(&A, row, i);
			m = MathExcept(start, end);
			M[0][i] = m;
		}
		
		return 1 / (A.size() - 1) * A.transposition()(A) - n * M(M.transposition());

	}
}


