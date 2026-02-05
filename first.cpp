#include <fstream>
#include <sstream>
#include <iostream>
#include <functional>
#include <stdexcept>
#include <tuple>
#include <type_traits>
#include <utility>
#include <algorithm>
#include <iterator>
#include <random>
#include <numbers>
#include <complex>
#define _USE_MATH_DEFINES
#include <cmath>
#include <numeric>
#include <vector>
#include <map>
#include <string>


//===============================================================================Matrix_modul=========================================================================================

namespace leo{

	template<class T> class matrix;

	template<class T1,class T2> 
	auto operator+(const matrix<T1>& a, const matrix<T2>& b) -> matrix< decltype(std::declval<T1>() + std::declval<T2>()) >;

	template<class T1, class T2>
	auto operator-(const matrix<T1>& a, const matrix<T2>& b) -> matrix< decltype(std::declval<T1>() - std::declval<T2>()) >;

	template<class T, class Scalar> 
	auto operator+(const matrix<T>& a, Scalar b) -> matrix< decltype(std::declval<T>() + std::declval<Scalar>()) >;

	template<class T, class Scalar>
	auto operator+(Scalar b ,const matrix<T>& a) -> matrix< decltype(std::declval<T>() + std::declval<Scalar>()) >;

	template<class T, class Scalar>
	auto operator-(const matrix<T>& a, Scalar b) -> matrix< decltype(std::declval<T>() - std::declval<Scalar>()) >;

	template<class T, class Scalar>
	auto  operator-(Scalar b, const matrix<T>& a) -> matrix< decltype(std::declval<Scalar>() - std::declval<T>()) >;

	template<class T, class Scalar> 
	auto  operator*(const matrix<T>& a, T b) -> matrix< decltype(std::declval<T>() * std::declval<Scalar>()) >;

	template<class T, class Scalar> 
	auto operator*(T b, const matrix<T>& a) -> matrix< decltype(std::declval<Scalar>() * std::declval<T>()) >;

	template<class T>  std::ostream& operator<<(std::ostream& os, const matrix<T>& m);

	
	enum class SM{
		row,
		col,
		mat,
	};

	struct Rows_tag {};
	struct Cols_tag {};

	inline constexpr Rows_tag Rows {};
	inline constexpr Cols_tag Cols {};




	//===========================Class_Matrix========================================
	template<class T>
	class matrix{
	private:
			std::vector<std::vector<T>> MATRIX;
			size_t col, row, mat;
			std::vector<std::vector<std::string>> LABELS;
	public:
		friend std::ostream& operator<< <>(std::ostream& os, const matrix<T>& m);

			matrix(size_t r, size_t c) : row(r), col(c) {
				MATRIX.resize(row, std::vector<T>(col, 0));
				mat = row * col;
				LABELS.resize(2);
				LABELS[0].reserve(row);
				LABELS[1].reserve(col);
			}
			
			matrix(const matrix<T>& A) : row(A.row), col(A.col), mat(A.mat), MATRIX(A.MATRIX), LABELS(A.LABELS) {}


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

			const std::vector<T>& operator[](size_t r) const {
				if (r >= row) throw std::out_of_range("Row index out of range!");
				return MATRIX[r];
                        }

			std::string& row_label(size_t i){
				return &LABELS[0][i];
			}

			std::string& col_label(size_t i){
				return &LABELS[1][i];
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

			size_t size(Cols_tag) { return col; }

			size_t size(Rows_tag) { return row; }
			
	public:
                class iterator_horizontal;
                class iterator_vertical;
    
                iterator_horizontal h_begin();
                iterator_horizontal h_end();

                iterator_vertical v_begin();
                iterator_vertical v_end();

                auto Column(size_t k) const {
                        return ColumnRange{this, k}; 
                }   

                auto Row(size_t k) const {
                        return RowRange{this, k}; 
                }   


		auto AllColumn(){
			return AllColumnRange{this};
		}

		auto AllRow(){
			return AllRowRange{this};
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
                };  

		struct AllColumnRange{
			const matrix<T>* M;
			
			auto begin() { return iterator_vertical(const_cast<matrix<T>*>(M), 0); }
			auto end() { return begin() + M->size(); }
		};

		struct AllRowRange{
			const matrix<T>* M;

			auto begin() { return iterator_horizontal(const_cast<matrix<T>*>(M), 0); }
			auto end() { return begin() + M->size(); }
		};
		

        public:



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

			matrix<T> diag(){
				if(!is_square()) throw std::invalid_argument("Method_Gauss: matrix is not square!");

				size_t n = size_row();
				matrix<T> M(n, n);
				for(size_t i=0; i < n; ++i){
					M[i][i] = MATRIX[i][i];
				}

				return M;
			}

			matrix<T> Method_Gauss(const matrix<T>& A){
				if(!A.is_square()) throw std::invalid_argument("Method_Gauss: matrix is not square!");

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

				size_t n = A.size_row();

				if ( n == 1 ) return A[0][0];
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
				if ( a >= row ) throw std::invalid_argument("Row index out of range");
				if ( b >= col ) throw std::invalid_argument("Column index out of range");

				matrix<T> A = *this;
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
				if(!is_square()) throw std::logic_error("Inverse: matrix is not square!");
				if(size() <= 9) return (1 / det()) * attached();
				else return Method_Gauss(*this);
			}


			T get_value(){
				if(mat != 1) throw std::logic_error("Class matrix, get_value - matrix not scalar!");
				return MATRIX[0][0];
			}

			
			static matrix<T> Cov(matrix<T> A){
			       matrix<T> cov(A.size_col(), A.size_col());

				for(size_t j=0; j < A.size_col(); ++j){
					T mean = 0;
					for(size_t i=0; i < A.size_row(); ++i){
						mean += A[i][j];
					}
					mean /= A.size_row();
					for(size_t i=0; i < A.size_row(); ++i){
						A[i][j] -= mean;
					}
				}

				for(size_t j=0; j < A.size_col(); ++j){
					for(size_t k=0; k < A.size_col(); ++k){
						T mean = 0;
						for(size_t i=0; i < A.size_row(); ++i){
							mean += A[i][j] * A[i][k];
						}
						mean /=  A.size_row();
						cov[k][j] = mean;
					}
				}
				return cov;
			}


			static matrix<T> Cor(matrix<T> A){
				size_t n = A.size_col();
				matrix<T> cov = leo::matrix<T>::Cov(A);
				matrix<T> cor = cov;

				for(size_t i=0; i<n; ++i){
					for(size_t j=0; j<n; ++j){
						cor[j][i] /= std::sqrt(cov[i][i]) * std::sqrt(cov[j][j]); 
					}
				}
				return cor;
			}

	};


	//===========================Operators_to_Matrix========================================

	template<class T1,class T2>
        auto operator+(const matrix<T1>& a, const matrix<T2>& b) -> matrix< decltype(std::declval<T1>() + std::declval<T2>()) >{
		using ResultType = decltype(std::declval<T1>() + std::declval<T2>());

		if ( a.size_col() != b.size_col() ) throw std::invalid_argument("Size numbers of columns in matrixs not eqvel!");
		if ( a.size_row() != b.size_row() ) throw std::invalid_argument("Size numbers of rows in matrixs not eqvel!");

		matrix<ResultType> result = a;
		for(size_t i=0; i <  a.size_row(); ++i){
			for(size_t j=0; j <  a.size_col(); ++j){
				result[i][j] += b[i][j];
			}
		}
	
		return result;
	}


	template<class T, class Scalar>
        auto operator+(const matrix<T>& a, Scalar b) -> matrix< decltype(std::declval<T>() + std::declval<Scalar>()) >{
		using ResultType = decltype(std::declval<T>() + std::declval<Scalar>());

		matrix<ResultType> result = a;
		for(size_t i=0; i <  a.size_row(); ++i){
			for(size_t j=0; j <  a.size_col(); ++j){
				result[i][j] += b;
			}
		}

		return result;
	}

	template<class T, class Scalar>
        auto operator+(Scalar b ,const matrix<T>& a) -> matrix< decltype(std::declval<T>() + std::declval<Scalar>()) >{
		return a + b;
	}


	template<class T1,class T2>
        auto operator-(const matrix<T1>& a, const matrix<T2>& b) -> matrix< decltype(std::declval<T1>() - std::declval<T2>()) >{
		using ResultType = decltype(std::declval<T1>() - std::declval<T2>());

		if ( a.size_col() != b.size_col() ) throw std::invalid_argument("Size numbers of columns in matrixs not eqvel!");
		if ( a.size_row() != b.size_row() ) throw std::invalid_argument("Size numbers of rows in matrixs not eqvel!");

		matrix<ResultType> result = a;
		for(size_t i=0; i <  a.size_row(); ++i){
			for(size_t j=0; j <  a.size_col(); ++j){
				result[i][j] -= b[i][j];
			}
		}

		return result;
	}


	template<class T, class Scalar>
        auto operator-(const matrix<T>& a, Scalar b) -> matrix< decltype(std::declval<T>() - std::declval<Scalar>()) >{
		using ResultType = decltype(std::declval<T>() - std::declval<Scalar>());

		matrix<ResultType> result = a;
		for(size_t i=0; i <  a.size_row(); ++i){
			for(size_t j=0; j <  a.size_col(); ++j){
				result[i][j] -= b;
			}
		}

		return result;
	}


	template<class T, class Scalar>
        auto  operator-(Scalar b, const matrix<T>& a) -> matrix< decltype(std::declval<Scalar>() - std::declval<T>()) >{
		using ResultType = decltype(std::declval<Scalar>() - std::declval<T>());

		matrix<ResultType> result(a.size_row(), a.size_col());
		for(size_t i=0; i < a.size_row(); ++i){
			for(size_t j=0; j < a.size_col(); ++j){
				result[i][j] = b - a[i][j];
			}
		}
		return result;
	}


	template<class T, class Scalar>
        auto  operator*(Scalar b, const matrix<T>& a) -> matrix< decltype(std::declval<Scalar>() * std::declval<T>()) >{
		using ResultType = decltype(std::declval<Scalar>() * std::declval<T>());

		matrix<T> result = a;
		for(size_t i=0; i <  a.size_row(); ++i){
			for(size_t j=0; j <  a.size_col(); ++j){
				result[i][j] *= b;
			}
		}

		return result;
	}

	template<class T, class Scalar> 
        auto operator*(const matrix<T>& a, Scalar b) -> matrix< decltype(std::declval<T>() * std::declval<Scalar>()) >{
		return b * a;
	}

	template<class T>
	std::ostream& operator<<(std::ostream& os, const matrix<T>& m){
		if(!m.LABELS[0].empty()) os << "\t";
		if(!m.LABELS[1].empty()){
			for(auto name : m.LABELS[1]){
				os << name << "\t";
			}
		}
		os << "\n";
		for(size_t i=0; i < m.size_row(); ++i){
			if(!m.LABELS[0].empty()) os << m.LABELS[0][i] << "\t";
			for(size_t j=0; j < m.size_col(); ++j){
				os << m.MATRIX[i][j] << "\t";
			}
			os << "\n";
		}
		return os;
	}

	//===========================Iterators_to_Matrix========================================
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

		iterator_horizontal(matrix<T>* m, size_t r, size_t c) : M(m) {
                        if (is_valid_position(r, c)) index = calculate_index(r, c);
                        else {
				index = M->size();
				throw std::invalid_argument("iterator_vertical - index out of range");
			}
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

		iterator_vertical(matrix<T>* m, size_t r, size_t c) : M(m) { 
			if (is_valid_position(r, c)) index = calculate_index(r, c);
			else {
				index = M->size();
				throw std::invalid_argument("iterator_vertical - index out of range");
			}
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
		iterator_vertical operator++(int) { iterator_vertical tmp = *this; ++index; return tmp; } 
		iterator_vertical& operator--() { --index; return *this; }
		iterator_vertical operator--(int) { iterator_vertical tmp = *this; --index; return tmp; } 

		iterator_vertical& operator+=(difference_type n) { index += n; return *this; }
		iterator_vertical& operator-=(difference_type n) { index -= n; return *this; }
		
		friend iterator_vertical operator+(iterator_vertical it, difference_type n) { return iterator_vertical(it.M, it.index + n); }
		friend iterator_vertical operator+(difference_type n, iterator_vertical it) { return it + n; }
		friend iterator_vertical operator-(iterator_vertical it, difference_type n) { return iterator_vertical(it.M, it.index - n); }
		friend difference_type operator-(const iterator_vertical& a, const iterator_vertical& b) { return a.index - b.index;  }

		reference operator[](difference_type n) const { return *(*this + n); }
		
		bool operator==(const iterator_vertical& other) const { return index == other.index && M == other.M; }
                bool operator!=(const iterator_vertical& other) const { return !(*this == other); }
                bool operator<(const iterator_vertical& other) const { return index < other.index; }
                bool operator>(const iterator_vertical& other) const { return index > other.index; }
                bool operator<=(const iterator_vertical& other) const { return index <= other.index; }
                bool operator>=(const iterator_vertical& other) const { return index >= other.index; }
	
	};
	
	template<class T>
        typename matrix<T>::iterator_vertical matrix<T>::v_begin() { return iterator_vertical(this, 0); }

	template<class T>
	typename matrix<T>::iterator_vertical matrix<T>::v_end() { return iterator_vertical(this, *this -> size()); }


}

//===============================================================================Applay_function_modul=========================================================================================


//===============================================================================Math_modul=========================================================================================
namespace leo{
	const double pi = M_PI;

	template<class Iterator>
	auto MathExcept(Iterator start, Iterator end){

		using ValueType = typename std::iterator_traits<Iterator>::value_type;

		if(start == end) throw std::invalid_argument("MathEx: empty datum!");

		ValueType sum = std::accumulate(start, end, ValueType{});

		auto n = std::distance(start, end);

		return static_cast<double>(sum) / n;
	}


	template<typename T>
	auto MathExcept(std::vector<T> X){	return MathExcept(X.begin(), X.end());	}


	template<class Iterator>
	auto RMS(Iterator start, Iterator end){
		if(start == end) throw std::invalid_argument("RMS: empty datum!");
	
		double result = std::inner_product(start, end, start, 0.0);

		result /= std::distance(start, end);
		
		return std::sqrt(result);
		
	}


	template<typename T>
	auto RMS(std::vector<T> X){	return RMS(X.begin(), X.end());	}



	template<class Iterator>
	auto Variation(Iterator start, Iterator end){
		if(start == end) throw std::invalid_argument("Variation: empty datum!");
		
		auto n = std::distance(start, end);

		auto M = MathExcept(start, end);

		std::vector<double> res;
		res.reserve(n);
		
		std::transform(start, end, std::back_inserter(res), [M] (double x) { return x - M; });

		return RMS(res.begin(), res.end());
	}


	template<typename T>
	auto Variation(std::vector<T> X){	return Variation(X.begin(), X.end());	}


	template<class Iterator1, class Iterator2>
	auto CCF(Iterator1 fb, Iterator1 fe, Iterator2 gb, Iterator2 ge, int tau, bool norm=true){
		if(fb == fe || gb == ge) return 0.0;
		auto sum = 0.0;
		auto Mf = MathExcept(fb, fe);
		auto Mg = MathExcept(gb, ge);
		auto f_dist = std::distance(fb, fe);
		auto g_dist = std::distance(gb, ge);
		int tau_f = 0;
		int tau_g = 0;
		int top;
		if(tau >= 0) {
			tau_f = tau;
			top = std::min(f_dist - 1 - tau_f, g_dist - 1);
		} else {
			tau_g = -tau;
			top = std::min(g_dist - 1 - tau_g, f_dist - 1);
		}
		if(top < 0) return 0.0;
		for(size_t i=0; i <= top; ++i){
			auto f_el = *(fb + i + tau_f);
			auto g_el = *(gb  + i + tau_g);
			if(norm) { f_el -= Mf; g_el -= Mg; }
			sum += f_el * g_el;
		}
		if(norm && top != 0) return sum/top;
		return sum;
	}




	template<typename T1, typename T2>
	auto CCF(std::vector<T1> f, std::vector<T2> g, int tau,  bool norm=true){	return  CCF(f.begin(), f.end(), g.begin(), g.end(), tau, norm);	}


	template<class Iterator1, class Iterator2>
	std::vector<double> CCF(Iterator1 fb, Iterator1 fe, Iterator2 gb, Iterator2 ge, bool norm=true, bool natural=false){
		auto f_dist = std::distance(fb, fe);
		auto g_dist = std::distance(gb, ge);
		std::vector<double> result;
		if(natural) result.reserve(f_dist);
		else result.reserve(f_dist + g_dist - 1);
		int start_lag = -(g_dist-1);
		int end_lag = (f_dist - 1);
		if(natural) start_lag = 0;
		for(int lag = start_lag; lag <= end_lag; ++lag){
			result.emplace_back(CCF(fb, fe, gb, ge, lag, norm));
		}
		return result;
	}



	template<typename T1, typename T2>
	std::vector<double> CCF(std::vector<T1> f, std::vector<T2> g, bool norm=false, bool natural=false){	return  CCF(f.begin(), f.end(), g.begin(), g.end(), norm, natural);	}

	
	template<class Iterator>
	auto ACF(Iterator fb, Iterator fe, int tau,  bool norm=true){
		return CCF(fb, fe, fb, fe, tau, norm);
	}


	template<typename T1>
	auto ACF(std::vector<T1> f, int tau,  bool norm=true) {	return CCF(f.begin(), f.end(), f.begin(), f.end(), tau, norm);	}



	template<class Iterator>
	auto ACF(Iterator fb, Iterator fe,  bool norm=true) -> std::vector<typename std::iterator_traits<Iterator>::value_type>
	{
		using T = typename std::iterator_traits<Iterator>::value_type;

		auto f_dist = std::distance(fb, fe);
		std::vector<T> result;
		result.reserve(f_dist);
		for(size_t lag=0; lag < f_dist; ++lag){
			result.emplace_back(ACF(fb, fe, lag, norm));
		}
		return result;
	}


	template<typename T1>
	auto ACF(std::vector<T1> f,  bool norm=true) {	return ACF(f.begin(), f.end(), norm);	}



namespace Matrix{
	template<class Iterator>
	auto ACF(Iterator fb, Iterator fe,  bool norm=true) -> matrix<typename std::iterator_traits<Iterator>::value_type> 
	{
		using T = typename std::iterator_traits<Iterator>::value_type;
		auto f_dist = std::distance(fb, fe);
		matrix<T> A(f_dist, f_dist);
		for(int i=0; i!=f_dist; ++i){
			for(int j=0; j!=f_dist; ++j){
				int lag = std::abs(j - i);
				A[i][j] = leo::ACF(fb, fe, lag, norm);
			}
		}
	
		return A;
	}

	template<typename T1>
	auto ACF(std::vector<T1> f,  bool norm=true) {   return Matrix::ACF(f.begin(), f.end(), norm);	}

	template<class T>
	matrix<T> Cov(matrix<T> A){
		int col = A.size_col(); 
		int row = A.size_row();

		matrix<T> M(1, col);

		for(size_t i=0; i < col; ++i){
			auto start = A.Column(i).begin();
			auto end = A.Column(i).end();
			auto m = MathExcept(start, end);
			M[0][i] = m;
		}
		
		return 1.0 / (row - 1.0) * (A.transposition()(A) - row * M.transposition()(M));
	}

	template<class T>
	matrix<T> Cor(const matrix<T>& A){
		matrix<T> covariation = Matrix::Cov(A);
		matrix<T> disp = covariation.diag();
		for(size_t i=0; i < disp.size_col(); ++i) disp[i][i] = std::sqrt(disp[i][i]);
		matrix<T> invd = disp.inverse();
		return invd(covariation(invd));
	}
}
	
	template<class T>
	std::vector<T> solve(matrix<T> A, std::vector<T> d){
		return A.inverse()(d);
	}




namespace BackFFT{

	template<class T>
	void FFT(std::vector<std::complex<T>>& x, bool inverse=false){
		const size_t N = x.size();
		if(N<=1) return;

		std::vector<std::complex<T>> even(N/2);
		std::vector<std::complex<T>> odd(N/2);

		for(size_t i=0; i < N/2; ++i){
			even[i] = x[i * 2];
			odd[i] = x[i * 2 + 1];
		}

		FFT(even, inverse);
		FFT(odd, inverse);

		T angle = 2 * pi / N * (inverse ? 1 : -1);
		std::complex<T> w(1);
		std::complex<T> wn(std::cos(angle), std::sin(angle));

		for(size_t i=0; i<N/2; ++i){
			x[i] = even[i] + w * odd[i];
			x[i + N/2] = even[i] - w * odd[i];
			if(inverse){
				x[i] /= 2;
				x[i + N/2] /= 2;
			}
			w *= wn;
		} 

	}

	size_t next_power_of_two(size_t n){
		if(n == 0) return 1;
		size_t power = 1;
		while (power < n) power <<= 1;
		return power;
	}

	template<typename T>
	struct is_complex : std::false_type {};

	template<typename T>
	struct is_complex<std::complex<T>> : std::true_type {};

	template<typename T>
	inline constexpr bool is_complex_v = is_complex<T>::value;

	template<typename Iterator>
	bool is_complex_iterator(Iterator it) {
		using ValueType = std::decay_t<decltype(*it)>;
		return is_complex_v<ValueType>;
	}


	//////
	template<typename T>
        struct scalar_type {
                using type = T;
        };

        template<typename T>
        struct scalar_type<std::complex<T>> {
                using type = T;
        };

        template<typename T>
        using scalar_type_t = typename scalar_type<std::decay_t<T>>::type;
	///////



	template<typename Iterator>
	auto FFT_complex(Iterator xb, Iterator xe, bool inverse=false, bool full=0)// -> std::vector<decltype(*xb)>
	{
		using elem_type = std::decay_t<decltype(*xb)>;
		using value_type = scalar_type_t<elem_type>;

		size_t n = std::distance(xb, xe);

		std::vector<std::complex<value_type>> res;
		res.reserve(n);

		for(auto it = xb; it != xe; ++it){ 
			res.emplace_back(*it);
		}

		size_t N = next_power_of_two(n);
		res.resize(N, {});

		BackFFT::FFT(res, inverse);
		if(!full) res.resize(n);
		
		return res;
	}

	template<typename Iterator>
	auto FFT_real_back(Iterator xb, Iterator xe, bool inverse=false, bool full=0)// -> std::vector<decltype(*xb)>
	{
		using elem_type = std::decay_t<decltype(*xb)>;
		using value_type = scalar_type_t<elem_type>;

		size_t n = std::distance(xb, xe);
			
		std::vector<std::complex<value_type>> res;
		res.reserve(n);

		for(auto it = xb; it != xe; ++it){
			res.emplace_back(std::complex<value_type>(*it, 0));
		}

		size_t N = next_power_of_two(n);
		res.resize(N, {});

		BackFFT::FFT(res, inverse);
		
	
		std::vector<value_type> result(N);
		for(size_t i=0; i < N; ++i){
			 result[i] = res[i].real();
		}
		if(!full) result.resize(n);

		return result;
	}


	template<typename Iterator>
	auto FFT_real_front(Iterator xb, Iterator xe, bool inverse=false, bool full=0)
	{
		using elem_type = std::decay_t<decltype(*xb)>;
		using value_type = scalar_type_t<elem_type>;

		size_t n = std::distance(xb, xe);

		std::vector<std::complex<value_type>> res;
		res.reserve(n);

		for(auto it = xb; it != xe; ++it){
			res.emplace_back(std::complex<value_type>(*it, 0));
		}

		size_t N = next_power_of_two(n);
		res.resize(N, {});

		BackFFT::FFT(res, inverse);
		if(!full) res.resize(n);
		
		return res;
	}


}

	template<typename Iterator>
	auto FFT(Iterator xb, Iterator xe, bool inverse=false, bool full=0)// -> std::vector<decltype(*xb)>
	{
		using value_type = std::decay_t<decltype(*xb)>;
		if constexpr (!BackFFT::is_complex_v<value_type>) return BackFFT::FFT_real_back(full, xb, xe, inverse);
		else {
			if(BackFFT::is_complex_v<value_type>) return BackFFT::FFT_complex(full, xb, xe, inverse);
		}
	}



	template<typename T>
	struct scalar_type {
		using type = T;
	};

	template<typename T>
	struct scalar_type<std::complex<T>> {
		using type = T;
	};

	template<typename T>
	using scalar_type_t = typename scalar_type<std::decay_t<T>>::type; 

	struct complex_tag {};
	struct real_tag {};
	struct image_tag {};


	inline constexpr complex_tag complex{};
	inline constexpr real_tag real{};
	inline constexpr image_tag image {};

	template<typename Iterator>
	auto FFT(real_tag, Iterator xb, Iterator xe, bool inverse=false, bool full=0)
	{
		using elem_type = std::decay_t<decltype(*xb)>;
		using value_type = scalar_type_t<elem_type>;
		
		std::vector<std::complex<value_type>> result;
		
		if constexpr (BackFFT::is_complex_v<elem_type>) result = BackFFT::FFT_complex(xb, xe, inverse, full);
		else result = BackFFT::FFT_real_front(xb, xe, inverse, full);

		std::vector<value_type> real_part;
		real_part.reserve(result.size());
		for(const auto& x : result){
			real_part.push_back(x.real());
		}
		return real_part;
	}


	template<typename Iterator>
	auto FFT(image_tag, Iterator xb, Iterator xe, bool inverse=false, bool full=0)
	{
		using elem_type = std::decay_t<decltype(*xb)>;
		using value_type = scalar_type_t<elem_type>;

		std::vector<std::complex<value_type>> result;

		if constexpr (BackFFT::is_complex_v<elem_type>) result = BackFFT::FFT_complex(xb, xe, inverse, full);
		else result = BackFFT::FFT_real_front(xb, xe, inverse, full);

		std::vector<value_type> image_part;
		image_part.reserve(result.size());
		for(const auto& x : result){
			image_part.push_back(x.imag());
		}
		return image_part;
	}


	template<typename Iterator>
	auto FFT(complex_tag, Iterator xb, Iterator xe, bool inverse=false, bool full=0)
	{
		using elem_type = std::decay_t<decltype(*xb)>;
		using value_type = scalar_type_t<elem_type>;

		std::vector<std::complex<value_type>> result;

		if constexpr (BackFFT::is_complex_v<elem_type>) result = BackFFT::FFT_complex(xb, xe, inverse, full);
		else result = BackFFT::FFT_real_front(xb, xe, inverse, full);

		return result;
	}
	

	template<typename Tag, typename T>
	auto FFT(Tag tag, std::vector<T> X, bool inverse=false, bool full=0){	return FFT(tag, X.begin(), X.end(), inverse, full);	}

	template<typename T>
	auto FFT(std::vector<T> X, bool inverse=false, bool full=0){	return FFT(X.begin(), X.end(), inverse, full);	}



/*	template<typename T>
	std::vector<std::complex<T>> DFT(std::vector<std::complex<T>>& x, bool inverse=false){
			int N = x.size();
			std::complex<double> X(N, {});
			for(size_t i = 0; i < N; ++i){
				for(size_t j = 0; j < N; ++j){
					X[i] += x[j] * std::exp( std::complex<T>(0, 2 * pi * i * j / N * (inverse ? -1 : 1))  );
				}
				if(inverse) X[i] /= N;
				
			}
			return X;
	}
*/

	template<typename Iterator>
	auto DFT(Iterator xb, Iterator xe, bool inverse=false);





	template<typename Iterator1, typename Iterator2>
	auto ConvolveFFT(Iterator1 Ab, Iterator1 Ae, Iterator2 Bb, Iterator2 Be)
	{
		using A_elem_type = std::decay_t<decltype(*Ab)>;
		using A_scalar_type = scalar_type_t<A_elem_type>;
		using B_elem_type = std::decay_t<decltype(*Bb)>;
		using B_scalar_type = scalar_type_t<B_elem_type>;
		using result_elem_type = decltype( std::declval<A_elem_type>() * std::declval<B_elem_type>() );
		using result_scalar_type = decltype( std::declval<A_scalar_type>() * std::declval<B_scalar_type>() );

		auto NA = std::distance(Ab, Ae);
		auto NB = std::distance(Bb, Be);
		size_t N = 1;
		size_t N_cor = NA + NB - 1;

		while(N < NA + NB - 1) N<<=1;

		std::vector<std::complex<A_scalar_type>> F1(N, 0);
		std::vector<std::complex<B_scalar_type>> F2(N, 0);

		for(size_t i=0; i < NA; ++i) F1[i] = *(Ab + i);
		for(size_t i=0; i < NB; ++i) F2[i] = *(Bb + i);

		BackFFT::FFT(F1);
		BackFFT::FFT(F2);

		for(size_t i=0; i < N; ++i) F1[i] *= F2[i];

		BackFFT::FFT(F1, true);

		F1.resize(N_cor);

		if constexpr (BackFFT::is_complex_v<result_elem_type>) return F1;
		else {
			std::vector<result_scalar_type> real_part(N_cor, 0);
			for (size_t i=0; i < N_cor; ++i) real_part[i] = F1[i].real();
			return real_part;
		}
	}



	template<typename Iterator1, typename Iterator2>
	auto Convolve(Iterator1 Ab, Iterator1 Ae, Iterator2 Bb, Iterator2 Be)
	{	
		using A_elem_type = std::decay_t<decltype(*Ab)>;
		using B_elem_type = std::decay_t<decltype(*Bb)>;
		using result_elem_type = decltype( std::declval<A_elem_type>() * std::declval<B_elem_type>() );

		size_t NA = std::distance(Ab, Ae);
		size_t NB = std::distance(Bb, Be);
		size_t N = NA + NB - 1;

		std::vector<result_elem_type> Res(N, {});

		for(auto it_a = Ab; it_a != Ae; ++it_a){
			for(auto it_b = Bb; it_b != Be; ++it_b){
				size_t i = std::distance(Ab, it_a) + std::distance(Bb, it_b);// - 2;
				Res[i] += *it_a * *it_b;
			}
		}

		return Res;
	}


	template<typename T1, typename T2>
	auto Convolve(std::vector<T1> F, std::vector<T2> G){
		return Convolve(F.begin(), F.end(), G.begin(), G.end());
	}

	template<typename T1, typename T2>
	auto ConvolveFFT(std::vector<T1> F, std::vector<T2> G){
		return ConvolveFFT(F.begin(), F.end(), G.begin(), G.end());
	}


	template<typename Iterator1, typename Iterator2>	
	auto CCFbyFFT(Iterator1 Ab, Iterator1 Ae, Iterator2 Bb, Iterator2 Be){
		using A_elem_type = std::decay_t<decltype(*Ab)>;
		using A_scalar_type = scalar_type_t<A_elem_type>;
		using B_elem_type = std::decay_t<decltype(*Bb)>;
		using B_scalar_type = scalar_type_t<B_elem_type>;
		using result_elem_type = decltype( std::declval<A_elem_type>() * std::declval<B_elem_type>() );
		using result_scalar_type = decltype( std::declval<A_scalar_type>() * std::declval<B_scalar_type>() );

		auto NA = std::distance(Ab, Ae);
		auto NB = std::distance(Bb, Be);
		size_t N = 1;
		size_t N_cor = NA + NB - 1;

		while(N < NA + NB - 1) N<<=1;

		std::vector<std::complex<A_scalar_type>> F1(N, 0);
		std::vector<std::complex<B_scalar_type>> F2(N, 0);

		for(size_t i=0; i < NA; ++i) F1[i] = *(Ab + i);
		for( int i=(NB - 1); i >= 0; --i) F2[NB - 1 - i] = *(Bb + i);

		BackFFT::FFT(F1);
		BackFFT::FFT(F2);

		for(size_t i=0; i < N; ++i) F1[i] *= F2[i];
	
		BackFFT::FFT(F1, true);

		F1.resize(N_cor);

		if constexpr (BackFFT::is_complex_v<result_elem_type>) return F1;
		else {
			std::vector<result_scalar_type> real_part(N_cor, 0);
			for (size_t i=0; i < N_cor; ++i) real_part[i] = F1[i].real();
			return real_part;
		}
		
	}


}

//===============================================================================Filters_modul=========================================================================================

namespace leo{
	

	template<typename Iterator1, typename Iterator2>
	auto Filter_Kolmogorow_Winer(Iterator1 Sb, Iterator1 Se, Iterator2 ESb, Iterator2 ESe, bool norm=false, bool filtiring=false){
		auto ccf = CCF(Sb, Se, ESb, ESe, norm, true);
		auto M = Matrix::ACF(Sb, Se, norm).inverse();
		ccf.resize(M.size_col());
		auto h_filter = M( ccf );
		if (filtiring) return ConvolveFFT(Sb, Se, h_filter.begin(), h_filter.end());
		else return h_filter;
	}


	


	template<typename T1, typename T2>
	auto Filter_Kolmogorow_Winer(std::vector<T1> signal, std::vector<T2> excepted_signal, bool norm=false, bool filtiring=false){
		return Filter_Kolmogorow_Winer(signal.begin(), signal.end(), excepted_signal.begin(), excepted_signal.end(), norm, filtiring); 
	}

	template<typename Iterator1, typename Iterator2>
	auto Filter_Consistent(Iterator1 Noise_b, Iterator1 Noise_e, Iterator2 ESb, Iterator2 ESe, bool norm=false){
		using signal_type = std::decay_t<decltype(*Noise_b)>;

		auto ccf = CCF(Noise_b, Noise_e, ESb, ESe, norm, true);
		auto M = Matrix::ACF(Noise_b, Noise_e, norm).inverse();
		M.resize(M.size_col(), ccf.size);
		auto h_filter = M( ccf );
		matrix<signal_type> S(1, h_filter.size());
		for(size_t i=0; i < h_filter.size(); ++i) S[0][i] =  h_filter[i];
		auto h_norm = S(h_filter).get_value();
		for(size_t i=0; i < h_filter.size(); ++i) h_filter[i] /= h_norm;
		return h_filter;
	}

	template<typename Iterator1, typename Iterator2, typename Iterator3>
	auto Filter_Consistent(Iterator1 Noise_b, Iterator1 Noise_e, Iterator2 ESb, Iterator2 ESe, Iterator3 Sb, Iterator3 Se, bool norm=false){
		auto h_filter = Filter_Consistent(Noise_b, Noise_e, ESb, ESe, norm);
		return ConvolveFFT(Sb, Se, h_filter.begin(), h_filter.end());
	}

	template<typename T1, typename T2>
        auto Filter_Consistent(std::vector<T1> noise, std::vector<T2> excepted_signal, bool norm=false){
		return Filter_Consistent(noise.begin(), noise.end(), excepted_signal.begin(), excepted_signal.end(), norm);
	}

	template<typename T1, typename T2, typename T3>
	auto Filter_Consistent(std::vector<T1> noise, std::vector<T2> excepted_signal, std::vector<T3> signal, bool norm=false){
		return Filter_Consistent(signal.begin(), signal.end(), excepted_signal.begin(), excepted_signal.end(), signal.begin(), signal.end(), norm);
	}

	/*template<typename Iterator1, typename Iterator2>
	auto Filter_Energy(Iterator1 Noise_b, Iterator1 Noise_e, Iterator2 ESb, Iterator2 ESe){
		using signal_type = std::decay_t<decltype(*Sb)>;

		
	}*/


}


//===============================================================================Solve_linal_modul========================================================================================= 

template<typename Iterator>
auto Norm_FuncSpace_C(Iterator fb, Iterator fe) -> std::decay_t<decltype(*fb)>
{
	if( fb == fe) return 0.0;
	else return std::max(fb, fe);
}

template<typename Iterator1, typename Iterator2>
auto Norm_FuncSpace_C(Iterator1 fb, Iterator1 fe, Iterator2 gb, Iterator2 ge)
{
	if( fb == fe || gb == ge ) return 0.0;
	
	auto f_dist = std::distance(fb, fe);
	auto g_dist = std::distance(gb, ge);

	size_t dist = std::min(f_dist, g_dist);

	auto rVal = std::abs( *fb - *gb );

	for(size_t i = 1; i < dist; ++i){
		auto nVal = std::abs( *(fb + i) - *(gb + i));
		if( rVal < nVal ) rVal = nVal;
	}

	return rVal;
}



template<typename Iterator>
auto Norm_FuncSpace_L1(Iterator fb, Iterator fe)
{
	if( fb == fe) return 0.0;

	auto f_dist = std::distance(fb, fe);

	auto rVal = std::abs(*fb);

	for(size_t i = 1; i < f_dist; ++i){
		rVal += std::abs(*(fb + i));
	}

	return rVal;
}



template<typename Iterator1, typename Iterator2>
auto Norm_FuncSpace_L1(Iterator1 fb, Iterator1 fe, Iterator2 gb, Iterator2 ge)
{
	if( fb == fe || gb == ge ) return 0.0;

	auto f_dist = std::distance(fb, fe);
	auto g_dist = std::distance(gb, ge);

	size_t dist = std::min(f_dist, g_dist);

	auto rVal = std::abs( *fb - *gb );

	for(size_t i = 1; i < dist; ++i){
		rVal += std::abs(*(fb + i) - *(gb + i));
	}

	return rVal;
}


template<typename Iterator>
auto Norm_FuncSpace_L2(Iterator fb, Iterator fe)
{
	if( fb == fe) return 0.0;

	auto f_dist = std::distance(fb, fe);

	auto rVal = *fb * *fb;

	for(size_t i = 1; i < f_dist; ++i){
		rVal += *(fb + i) * *(fb + i);
	}

	rVal = std::sqrt(rVal);

	return rVal;
}


template<typename Iterator1, typename Iterator2>
auto Norm_FuncSpace_L2(Iterator1 fb, Iterator1 fe, Iterator2 gb, Iterator2 ge)
{
	if( fb == fe || gb == ge ) return 0.0;

	auto f_dist = std::distance(fb, fe);
	auto g_dist = std::distance(gb, ge);

	size_t dist = std::min(f_dist, g_dist);

	auto rVal = (*fb - *gb) * (*fb - *gb);

	for(size_t i = 1; i < dist; ++i){
		rVal += (*(fb + i) - *(gb + i)) * (*(fb + i) - *(gb + i));
	}

	rVal = std::sqrt(rVal);

	return rVal;
}


template<typename Iterator>
auto Norm_FuncSpace_W(Iterator fb, Iterator fe, double g=1.0, double w_d=1.0)
{
	if( fb == fe) return 0.0;

	auto f_dist = std::distance(fb, fe);

	auto f_d = (*(fb + 2)  - *fb)/(3 * w_d);

	auto rVal = *(fb + 1) * *(fb + 1) + g * g * f_d * f_d;

	for(size_t i = 2; i < (f_dist - 1); ++i){
		f_d = (*(fb + i + 1)  - *(fb + i - 1))/(3 * w_d);
		rVal += *(fb + i) * *(fb + i) + g * g * f_d * f_d;
	}

	rVal = std::sqrt(rVal);

	return rVal;
}


template<typename Iterator1, typename Iterator2>
auto Norm_FuncSpace_W(Iterator1 fb, Iterator1 fe, Iterator2 gb, Iterator2 ge, double g=1.0, double wf_d=1.0, double wg_d=1.0)
{
	if( fb == fe || gb == ge ) return 0.0;

	auto f_dist = std::distance(fb, fe);
	auto g_dist = std::distance(gb, ge);

	size_t dist = std::min(f_dist, g_dist);

	auto f_d = (*(fb + 2) - *fb)/(3 * wf_d);
	auto g_d = (*(gb + 2) - *gb)/(3 * wg_d);

	auto fg_d = *(fb + 1) - *(gb + 1);

	auto rVal = fg_d * fg_d + g * g * (f_d - g_d) * (f_d - g_d);

	for(size_t i = 2; i < (dist - 1); ++i){
		f_d = (*(fb + i + 1) - *(fb + i - 1))/(3 * wf_d);
		g_d = (*(gb + i + 1) - *(gb + i - 1))/(3 * wg_d);
		fg_d = *(fb + i) - *(gb + i);

		rVal += fg_d * fg_d + g * g * (f_d - g_d) * (f_d - g_d);
	}

	rVal = std::sqrt(rVal);

	return rVal;
}


//===============================================================================Test_modul=========================================================================================


int main(){
	leo::matrix<double> A(4, 4);

	A[0][0] = 1; A[0][1] = 2; A[0][2] = 3; A[0][3] = 3;

	A[1][0] = 4; A[1][1] = 5; A[1][2] = 10; A[1][3] = 3;

	A[2][0] = 7; A[2][1] = 8; A[2][2] = 9; A[2][3] = 10;

	A[3][0] = 20; A[3][1] = 8; A[3][2] = 5; A[3][3] = 3;

	std::cout << A;

	std::cout << leo::Matrix::Cor(A);
	
	std::cout << leo::matrix<double>::Cor(A);

	std::vector<std::complex<double>> Front = leo::FFT(leo::complex, A.Column(0).begin(), A.Column(0).end());

	std::vector<double> Back_real = leo::FFT(leo::real,  Front.begin(), Front.end(), true);

	std::vector<double> Back_image = leo::FFT(leo::image, Front.begin(), Front.end(), true);

	std::cout << "\nColumn\tFront\tBack_real\tBack_image\n";
	auto it = A.Column(0).begin();
	for(size_t i=0; i < A.size_row(); ++i) {
		std::cout << *it << "\t" << Front[i] << "\t" << Back_real[i] << "\t" << Back_image[i] << "\n";
		++it;
	}
	

	std::vector<double> conv = leo::ConvolveFFT(A.Column(0).begin(), A.Column(0).end(), A.Row(0).begin(), A.Row(0).end());
	
	std::cout << "\n";
	for(auto it : conv) std::cout << it << " ";
	std::cout << "\n" << "size conv: " << conv.size() << "\n";
	

	std::vector<double> conv_ = leo::Convolve(A.Column(0).begin(), A.Column(0).end(), A.Row(0).begin(), A.Row(0).end());

	std::cout << "\n";
	for(auto it : conv_) std::cout << it << " ";
	std::cout << "\n" << "size conv_: " << conv_.size() << "\n";

	std::vector<double> beta(A.size_row(), 0);

	beta[0] = 1;

	leo::matrix<double> M = leo::Matrix::ACF(A.AllRow().begin(), A.AllRow().end());

	std::cout << "\n";
        for(auto it : A.AllRow()) std::cout << it << " ";
	std::cout << "\n" << "size signal: " << A.size() << "\n";


	std::cout << "Size of past matrix: " << A.size() << "\nSize of matrix col: " << M.size_col() << "\n" << M;

	
	std::cout << "\n";
	for(auto it : A.AllRow()) std::cout << it << " ";
	std::cout << "\n" << "size signal: " << A.size() << "\n";

	std::cout << "\n";
	for(auto it : beta) std::cout << it << " ";
	std::cout << "\n" << "size beta: " << beta.size() << "\n";

	std::vector<double> FKW = Filter_Kolmogorow_Winer(A.AllRow().begin(), A.AllRow().end(), beta.begin(),beta.end(), false, true);

	std::cout << "\n";
	for(auto it : FKW) std::cout << it << " ";
	std::cout << "\n" << "size FKW: " << FKW.size() << "\n";


	leo::matrix<double> K = A + 3;

	std::cout << K;

	std::vector<double> ccf = leo::CCF(A.AllRow().begin(), A.AllRow().end(), beta.begin(),beta.end(), false);

	std::vector<double> ccf_fft = leo::CCFbyFFT(A.AllRow().begin(), A.AllRow().end(), beta.begin(),beta.end());

	std::cout << "ccf\tccf_fft\n";
	for(size_t i=0; i < ccf.size(); ++i) std::cout << ccf[i] << "\t" << ccf_fft[i] << "\n";
	std::cout << "size ccf = " << ccf.size() << "\tsize ccf_fft = " << ccf_fft.size() << "\n";

	return 0;
}




