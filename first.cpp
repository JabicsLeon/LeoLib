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


	//===========================Class_Matrix========================================
	template<class T>
	class matrix{
	private:
			std::vector<std::vector<T>> MATRIX;
			size_t col, row, mat;
			std::vector<std::vector<std::string>> LABELS;
	public:
		/*friend matrix<T> operator+<>(const matrix<T>& a, const matrix<T>& b);
		friend matrix<T> operator+<>(const matrix<T>& a, T b);
		friend matrix<T> operator+<>(T b, const matrix<T>& a);
		friend matrix<T> operator-<>(const matrix<T>& a, const matrix<T>& b);
		friend matrix<T> operator-<>(const matrix<T>& a, T b);
		friend matrix<T> operator-<>(T b, const matrix<T>& a);
		friend matrix<T> operator*<>(const matrix<T>& a, T b);
		friend matrix<T> operator*<>(T b, const matrix<T>& a);*/
		friend std::ostream& operator<< <>(std::ostream& os, const matrix<T>& m);
/*	public:
		class iterator_horizontal;
		class iterator_vertical;
	
		iterator_horizontal h_begin();
		iterator_horizontal h_end();

		iterator_vertical row_begin();
		iterator_vertical row_end();

		auto Column(size_t k) const {
			return ColumnRange{this, k};
		}

		auto Row(size_t k) const {
			return RowRange{this, k};
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
	public: */
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
				if(size() <= 9) return (1 / det()) * attached();
				else return Method_Gauss(*this);
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

	template<class Iterator>
	auto MathExcept_old(Iterator start, Iterator end) -> typename std::iterator_traits<Iterator>::value_type
        {
                using T = typename std::iterator_traits<Iterator>::value_type;	

		T sum = 0.0;

		auto n = std::distance(start, end);

		for(auto it=start; it!=end; ++it){
			sum += *it;
		}
		sum /= n;
		return sum;
	}

	template<class Iterator>
        auto MathEx(Iterator start, Iterator end, size_t len) -> typename std::iterator_traits<Iterator>::value_type
        {
                using T = typename std::iterator_traits<Iterator>::value_type;

                T sum = 0.0;
		
		size_t n_dop = end - start;

                auto n = std::distance(start, end);
		//auto it = start;
		std::cout << "\nIn math ex: 0\t";
                for(auto it=start; it!=end; ++it){
                        sum += *it;
			std::cout << " + " << *it;
                }
		std::cout << " = " << sum;
                sum /= n;
		std::cout << " | n = " << n << "; n_dop = " << n_dop << "; M = " << sum << "\n";
                return sum;
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

		auto M = MathExcept(start, end);

		std::vector<double> res;
		res.reserve(n);
		
		std::transform(start, end, std::back_inserter(res), [M] (double x) { return x - M; });

		return RMS(res.begin(), res.end());
	}


	template<class Iterator1, class Iterator2>
	auto CCF(Iterator1 fb, Iterator1 fe, Iterator2 gb, Iterator2 ge, int tau){
		if(fb == fe || gb == ge) return 0.0;
		auto sum = 0.0;
		int count = 0;
		auto Mf = MathExcept(fb, fe);
		auto Mg = MathExcept(gb, ge);
		int tau_f = 0;
		int tau_g = 0;
		if (tau >= 0) tau_g = tau;
		else tau_f = -tau;
		Iterator1 it_f = fb + tau_f;
		Iterator2 it_g = gb + tau_g;
		while( it_f != fe && it_g != ge){
			sum += (*it_f - Mf) * (*it_g - Mg);
			it_f++;
			it_g++;
			count++;
		}
		if (count != 0) return sum/count;
		return sum;
	}

	template<class Iterator1, class Iterator2>
	std::vector<double> CCF(Iterator1 fb, Iterator1 fe, Iterator2 gb, Iterator2 ge){
		auto f_dist = std::distance(fb, fe);
		auto g_dist = std::distance(gb, ge);
		std::vector<double> result;
		result.reserve(f_dist + g_dist - 1);
		for(int lag = -(g_dist-1); lag <= (f_dist - 1); ++lag){
			result.emplace_back(CCF(fb, fe, gb, ge, lag));
		}
		return result;
	}

	
	template<class Iterator>
	auto ACF(Iterator fb, Iterator fe, int tau){
		return CCF(fb, fe, fb, fe, tau);
	}

	template<class Iterator>
	auto /*std::vector<double>*/ ACF(Iterator fb, Iterator fe) -> std::vector<typename std::iterator_traits<Iterator>::value_type>
	{
		using T = typename std::iterator_traits<Iterator>::value_type;

		auto f_dist = std::distance(fb, fe);
		std::vector<T> result;
		result.reserve(f_dist);
		for(size_t lag=0; lag < f_dist; ++lag){
			result.emplace_back(ACF(fb, fe, lag));
		}
		return result;
	}

namespace Matrix{
	template<class Iterator>
	auto ACF(Iterator fb, Iterator fe) -> matrix<typename std::iterator_traits<Iterator>::value_type> 
						//decltype(matrix<typename std::iterator_traits<Iterator>::value_type>())
	{
		using T = typename std::iterator_traits<Iterator>::value_type;
		auto f_dist = std::distance(fb, fe);
		matrix<T> A(f_dist, f_dist);
		for(int i=0; i!=f_dist; ++i){
			for(int j=0; j!=f_dist; ++j){
				int lag = std::abs(j - i);
				A[i][j] = leo::ACF(fb, fe, lag);
			}
		}
	
		return A;
	}

	template<class T>
	matrix<T> Cov(matrix<T> A){
		int col = A.size_col(); 
		int row = A.size_row();

		matrix<T> M(1, col);

		for(size_t i=0; i < col; ++i){
			//auto start = typename matrix<T>::iterator_vertical(&A, 0, i);
			//auto end = typename matrix<T>::iterator_vertical(&A, row, i);
			auto start = A.Column(i).begin();
			auto end = A.Column(i).end();
			auto m = MathExcept(start, end);
			M[0][i] = m;
		}
		
		return 1.0 / (row/* - 1.0*/) * (A.transposition()(A) - row * M.transposition()(M));
	}

	template<class T>
	matrix<T> Cor(const matrix<T>& A){
		matrix<T> covariation = Matrix::Cov(A);
		matrix<T> invd = covariation.diag().inverse();
		return invd(covariation(invd));
	}
}
	
	
	template<class T>
	matrix<T> Cov(matrix<T> B){
		matrix<T> M(1,B.size_col());

		for(size_t i=0; i < B.size_row(); ++i){
			//auto start = typename matrix<T>::iterator_vertical(&B, 0, i);
			//auto end = typename matrix<T>::iterator_vertical(&B, B.size_row(), i);
			auto start = B.Column(i).begin();
			auto end = B.Column(i).end();
			auto m = MathExcept(start, end);
			M[0][i] = m;
			//for(auto it : B.Row(i)) it -= m;
		}
		for(size_t i=0; i < B.size_row(); ++i){
                        for(size_t j=0; j < B.size_col(); ++j){
				B[i][j] -= M[0][j];
			}
		}

		return 1.0 / (B.size_row() /*- 1.0*/) * B.transposition()(B);
	}


	template<class T>
	matrix<T> Cov_old(matrix<T> A){
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


	template<class T>
	std::vector<T> solve(matrix<T> A, std::vector<T> d){
		return A.inverse()(d);
	}
	
}

//===============================================================================Test_modul=========================================================================================


int main(){
	leo::matrix<double> A(4, 4);

	A[0][0] = 1; A[0][1] = 2; A[0][2] = 3; A[0][3] = 3;

	A[1][0] = 4; A[1][1] = 5; A[1][2] = 10; A[1][3] = 3;

	A[2][0] = 7; A[2][1] = 8; A[2][2] = 9; A[2][3] = 10;

	A[3][0] = 20; A[3][1] = 8; A[3][2] = 5; A[3][3] = 3;

	std:: cout << A;/* << "\nTransposition:"; << A.transposition();

	std:: cout << A.det();

	std:: cout << A.inverse();

	std::vector<double> d = { 1, 2, 3, 4 };

	std::vector<double> sl = leo::solve(A, d);
	
	std::cout << "\n";

	for(auto el : sl){
		std::cout << el << "\t";
	}	

	std::cout << "\n";

	std::cout << leo::Matrix::ACF(d.begin(), d.end());

	std::vector<double> tl = leo::ACF(d.begin(), d.end());

	std::cout << "\n";

	for(auto el : tl){
                std::cout << el << "\t";
        }
	
	std::cout << "\n";

	std::cout << 2 * leo::matrix<double>::identity(4);

	std::cout << "\n";

	double k = 1.0 / (A.size_row() - 1.0);

	std::cout << k << "\n" << 1.0 / (A.size_row() - 1.0) * A.transposition()(A);

*/
	std::cout << "\n";
	int row = A.size_row();
	int col = A.size_col();
	leo::matrix<double> M(1, col);
	leo::matrix<double> M_(1, col);
        for(size_t i=0; i < col; ++i){
                        auto start = A.Column(i).begin();
                        auto end = A.Column(i).end();
			//auto start = typename leo::matrix<double>::iterator_vertical(&A, 0, i);
                        //auto end = typename leo::matrix<double>::iterator_vertical(&A, A.size_row(), i);
                        auto m = leo::MathExcept(start, end);
			auto m_ = leo::MathEx(start, end, A.size_row());
                        M[0][i] = m;
			M_[0][i] = m_;
			for(auto it : A.Column(i)) std::cout << it << "\t";
			std::cout << "\n";
			
         }
	std::cout << "MathExcept:\t";
	std::cout << M;

	std::cout << "MathEx:\t";
	std::cout << M_;

	//std::cout << leo::Variation
	
	std::cout << leo::Matrix::Cov(A);

	//std::cout << leo::Matrix::Cov(A).diag().inverse();

	//std::cout << leo::Matrix::Cor(A);

	std::cout << leo::Cov(A);

	std::cout << leo::Cov_old(A);

/*	leo::matrix<double> test(2, 2);
	test[0][0] = 1; test[0][1] = 2;
	test[1][0] = 3; test[1][1] = 4;

	leo::matrix<double> test_T = test.transposition();
	std::cout << "test:\n" << test << std::endl;
	std::cout << "test transposed:\n" << test_T << std::endl;

	// Умножение test_T на test
	leo::matrix<double> test_ATA = test_T(test);
	std::cout << "testᵀ * test:\n" << test_ATA << std::endl;
*/

	

	return 0;
}




