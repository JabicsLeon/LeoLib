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

enum TypeMatrix{Default, Sorted, Covariation, Correlation, AutoCorrelation};

std::map<int, std::string> InKey = {{Default,"Default"},
					 {Sorted,"Sorted"},
					 {Covariation,"Covariation"},
					 {Correlation,"Correlation"},
					 {AutoCorrelation, "AutoCorrelation"}};

std::map<std::string, int> OutKey = {{"Default",Default},
					 {"Sorted",Sorted},
					 {"Covariation",Covariation},
					 {"Correlation",Correlation},
					 {"AutoCorrelation", AutoCorrelation}};


enum date_name{Signal,
		Noize,
		Fild,
		Filtering_ACF,
		Filtering_begin,
		Filtering_centr,
		Filtering_end};

//==============================Global_Struct==============================

template<class T>
struct Columns{
        std::string Name;
	T ME;
	T D;
	T Sigma;
        std::vector<T> Row;
};

template<class T>
struct Matrix{
	int Type;
	int CountRow;
	int CountColumn;
        std::vector<Columns<T>> Column;
	std::vector<std::string> Labels;
};

template<class T>
struct Measurment{
	std::map<int, Matrix<T>> matrix;
};



//==============================Read_File==============================

std::vector<std::string> split(const std::string& str, const std::string& delimiter){
	std::vector<std::string> tokens;
	size_t start = 0;
	size_t end;

	while ((end = str.find(delimiter, start)) != std::string::npos){
		std::string token = str.substr(start, end - start);
		if(!token.empty()) tokens.push_back(token);
		start = end + delimiter.length();
	}

	std::string last = str.substr(start);
	if(!last.empty()) tokens.push_back(last);

	return tokens;
}


int ReadFile(std::vector<std::string>& date){
	std::string FileName;
	std::cout << "Write file name: ";
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	std::getline(std::cin, FileName);
	
	std::ifstream file;
	file.open(FileName);
	if(!file.is_open()){
		std::cerr << "Error of reading file!\n" <<std::endl;
		return 1;
	}

	std::string line;
	while(std::getline(file, line)){
		date.push_back(line);
	}

	file.close();

	return 0;
}

std::vector<std::string> CathFile(){
	std::vector<std::string> date;
	if(ReadFile(date)) std::cout << "File has't opened\n";

	return date;
}



//==============================Write_File==============================

template<typename Func, typename... Args>
auto call_function(Func&& func, Args&&... args) -> decltype(std::forward<Func>(func)(std::forward<Args>(args)...)) {
	return std::forward<Func>(func)(std::forward<Args>(args)...);
}

template<typename Func, typename... Args>
std::stringstream CatchStream(Func&& func, Args&&... args){
	std::stringstream buf;
	auto oldBuf = std::cout.rdbuf(buf.rdbuf());
	
	try{
		call_function(std::forward<Func>(func), std::forward<Args>(args)...);
	} catch (...) {
		std::cout.rdbuf(oldBuf);
		std::cerr << "Error in outFunc()\n";
		return buf;
	}

	std::cout.rdbuf(oldBuf);
	return buf;
}

int WriteStreamFile(const std::string& FileName , const std::stringstream& buf){
	std::ofstream file(FileName);
	if (!file.is_open()) {
		std::cerr << "Error of open/creat output file!\n";
		return 1;
	}
	
	file << buf.str();
	file.close();

	std::cout << "File " << FileName << "  has been written\n";
	 return 0;
}

template<typename Func, typename... Args>
int WriteFile(const std::string& FileName, Func&& func, Args&&... args){
	std::stringstream buf = CatchStream(func, std::forward<Args>(args)...);
	if(buf.str().empty()) return 1;
	if(!WriteStreamFile(FileName, buf)) return 1;
	
	return 0;
}

template<typename Func, typename... Args>
void SendFile(Func&& func, Args&&... args){
	std::string FileName = "output.txt";
	WriteFile(FileName, func, std::forward<Args>(args)...);
}

template<typename Func, typename... Args>
std::string ToStream(Func&& func, Args&&... args){
	std::stringstream buf = CatchStream(func, std::forward<Args>(args)...);
	return buf.str();
}



//==============================Matrix_work==============================
template<class T>
T string_to(const std::string& str){
	std::istringstream iss(str);
	T value;
	if(!(iss >> value)) throw std::invalid_argument("Cannot convert to type T");

	return value;
}


template<class T>	
Matrix<T> convertMatrix(std::vector<std::string> date){
	Matrix<T> matrix;
	matrix.Type = Default;
	matrix.CountRow = date.size() - 1;

	if(date.empty()) return matrix;

	auto headers = split(date[0], "\t");
	matrix.CountColumn = headers.size();

	for(const auto& header : headers){
		matrix.Column.push_back({header, {}});
	}

	for(size_t row_index=1; row_index < date.size(); ++row_index){
		auto parts = split(date[row_index], "\t");

		if(parts.size() != matrix.CountColumn) {
			std::cerr << "String " << row_index + 1 << " has got uncorrect number of column!\n";
			continue;
		}

		for(size_t col_index=0; col_index < parts.size(); ++col_index){
			try{
				T value = string_to<T>(parts[col_index]);
				matrix.Column[col_index].Row.push_back(value);
			} catch (const std::exception& e) {
				std::cerr << "Mistake in row " << row_index + 1 << " column " << col_index + 1 << " : '" << parts[col_index] << "' ---> " << e.what() << "\n";
			}
		}
	}
	return matrix;
}

template<class T>
void MatrixShowType(Matrix<T> matrix){
	int Type = matrix.Type;
	std::cout << "Type matrix: " << InKey[Type] << "\n";
}

template<class T>
void MatrixShow(Matrix<T> matrix){
	if(!matrix.Labels.empty()) std::cout << "\t";
	for(int i=0; i!=matrix.CountColumn; i++){
		std::cout << matrix.Column[i].Name << "\t";
	}
	std::cout << "\n";

	for(int i=0; i!=matrix.CountRow; i++){
		if(!matrix.Labels.empty()) std::cout << matrix.Labels[i] << "\t";
		for(int j=0; j!=matrix.CountColumn; j++){
			if(i < matrix.Column[j].Row.size()) std::cout << matrix.Column[j].Row[i] << "\t";
			else std::cout << "N/A\t";
		}
		std::cout << "\n";
	}
}

template<typename T>
void VecShow(const std::vector<T>& vec){
        for(const auto& item : vec){
                std::cout << item << "\n";
        }
        std::cout << "\n";
}

template<class T>
std::vector<T> Sorting(std::vector<T> row){
	std::vector<T> sort;
	std::vector<T> buf = row;
	for(auto count=row.begin(); count!=row.end(); ++count){
		T Min = *buf.begin();
		auto itStat = buf.begin();
		for(auto it=buf.begin(); it!=buf.end(); ++it){
			if(*it <= Min) {
				itStat = it;
				Min = *it;
			}
		}
		sort.push_back(*itStat);
		buf.erase(itStat);
	}
	return sort;
}

template<class T>
Matrix<T> MatrixSorting(Matrix<T> matrix){
	Matrix<T> buf = matrix;
	buf.Type = Sorted;
	for(int i=0; i!=matrix.CountColumn; i++){
		buf.Column[i].Row = Sorting(buf.Column[i].Row);
	}
	return buf;
}



//==============================Math_Function==============================
//const double PI = std::acos(-1, 0);
const double pi = M_PI;

template<class T>
T MathEx(const std::vector<T>& Column){
	T Sr = 0;
	size_t n = Column.end() - Column.begin();
	for(auto it=Column.begin(); it!=Column.end(); ++it){
		Sr += *it;
	}
	Sr /= n;
	return Sr;
}


template<class T>
T Disp(const std::vector<T>& Column,T Sr){
	T disp = 0;
	for(const auto& it : Column){
		disp += (it - Sr) * (it - Sr);
	}
	disp /= Column.size();
	return disp;
}

template<class T>
T Var(const std::vector<T>& Column){
	T var = Disp(Column, MathEx(Column));
	return var;
}

template<class T>
T Var_def(const Columns<T>& Column){
	T Sr = Column.ME;
	T var = Disp(Column.Row, Sr);
	return var;

}

template<typename Func, typename... Args>
//auto vec_of(Func&& func, size_t size, Args&&... args) -> std::vector<decltype(std::forward<Func>(func)(std::forward<Args>(args)...))>{
auto vec_of(Func&& func, size_t size, Args&&... args) -> std::vector<decltype(func(0, std::forward<Args>(args)...))>{
	//using ResultType = decltype(std::forward<Func>(func)(std::forward<Args>(args)...));
	using ResultType = decltype(func(0, std::forward<Args>(args)...));
        std::vector<ResultType> buf;
	buf.reserve(size);
	
        for(int i=0; i!=size; i++){
                //buf.emplace_back(std::forward<Func>(func)(std::forward<Args>(args)...));
		buf.emplace_back(func(i, std::forward<Args>(args)...));
        }
        return buf;
}



enum FT_Front_Back{Front, Back};
template<class T>
std::vector<std::complex<T>> FT(std::vector<std::complex<T>> x, int type){
	const int N = x.size();
	std::vector<std::complex<T>> X;
	X.reserve(N);
	int sign = -1;
	if(type==Back) sign = 1;
	for(int k=0; k!=N; k++){
		std::complex<T> value(0,0);
		for(int n=0; n!=N; n++){
			value += x[n] * std::exp(std::complex<T>(0, sign * 2 * pi * k * n / N));
		}
		if(type==Back) value /= N;
		X.emplace_back(value);
	}
	return X;
}



template<class T>
void FFT(std::vector<std::complex<T>>& signal, bool inverse=false){
	const size_t N = signal.size();
	if(N<=1) return;

	//even and odd
	std::vector<std::complex<T>> even(N/2);
	std::vector<std::complex<T>> odd(N/2);

	for(size_t i=0; i<N/2; ++i){
		even[i] = signal[i*2];
		odd[i] = signal[i*2 + 1];
	}

	FFT(even, inverse);
	FFT(odd, inverse);

	//Cobine result
	T angle = 2 * pi / N * (inverse ? 1 : -1);
	std::complex<T> w(1);
	srd::complex<T> wn(std::cos(angle), std::sin(angle));

	for(size_t i=0; i<N/2; ++i){
		signal[i] = even[i] + w * odd[i];
		signal[i+N/2] = even[i] - w * odd[i];
		if(inverse){
			signal[i] /= 2;
			signal[i+N/2] /= 2;
		} 
		w *= wn;
	}
}


template<class T>
std::vector<std::complex<T>> FT_new(std::vector<T> x, int type){
	const size_t N = x.size();

	std::vector<std::complex<T>> X(N);
	for(size_t i=0; i<=N; ++i){
		X[i] = std::complex<T>(x[i], 0);
	}

	bool inverse = (type==Back);
	FFT(X, inverse);

	return X;
}


template<class T>
std::vector<T> ConvolveFFT(const std::vector<T>& f, const std::vector<T>& g){
	size_t Nf = f.size();
	size_t Ng = g.size();
	size_t N = 1;

	while(N < Nf + Ng - 1) N<<=1;

	//Add zero
	std::vector<std::complex<T>> F1(N, 0);
	std::vector<std::complex<T>> F2(N, 0);

	for(size_t i=0; i<Nf; ++i) F1[i] = f[i];
	for(size_t i=0; i<Ng; ++i) F2[i] = g[i];

	//FFT
	FFT(F1, false);
	FFT(F2, false);

	//Multiplay in spectrum 
	for(size_t i=0; i<N; ++i){
		F1[i] *= F2[i];
	}

	//Reverse FFT
	FFT(F1, true);

	//To real
	std::vector<T> result(Nf + Ng - 1);
	for(size_t i=0; i<result.size(); ++i){
		result[i] = F1[i].real();
	}

	return result;
}




template<class T>
std::vector<T> ConvolveFT(std::vector<T> f, std::vector<T> g){
	std::vector<T> convolve;
	
	const int Nf = f.size();
	const int Ng = g.size();
	convolve.reserve(Nf+Ng-1);
	
	std::vector<std::complex<T>> f_(Nf+Ng-1, 0);
	std::vector<std::complex<T>> g_(Nf+Ng-1, 0);

	for(size_t i=0; i<Nf; ++i) f_[i] = {f[i], 0};
	for(size_t i=0; i<Ng; ++i) g_[i] = {g[i], 0};	

	std::vector<std::complex<T>> F = FT(f_, Front);
	std::vector<std::complex<T>> G = FT(g_, Front);
	
	for(int i=0; i!=Nf+Ng-1; i++){
		F[i] = F[i] * G[i];
	}

	F = FT(F, Back);

	for(int i=0; i!=Nf+Ng-1; i++){
                convolve.emplace_back(F[i].real());
        }

	return convolve; 
}


template<class T>
std::vector<T> Convolve(std::vector<T> f, std::vector<T> g){
	size_t Nf = f.size();
	size_t Ng = g.size();
	size_t Nconvolve = Nf + Ng - 1;

	std::vector<T> convolve(Nconvolve, 0);
	
	for(size_t i=0; i<Nf; ++i){
		for(size_t j=0; j<Ng; ++j){
			convolve[i+j] += f[i] * g[j];
		}
	}
	return convolve;
}


//==============================Help_Function==============================

template<class T>
std::vector<T> Noizer(T min, T max, size_t size){
	std::vector<T> noize;
	noize.reserve(size);

	std::random_device rd;
	std::mt19937 gen(rd());

	std::uniform_real_distribution<T> distrib(min, max);

	for(int i=0; i<size; ++i){
		noize.emplace_back(distrib(gen));
	}

	return noize;
}

template<class T>
std::vector<T> ReverserReal(std::vector<T> vec){
	const size_t N = vec.size();
	std::vector<T> resault;
	resault.reserve(N);

	for(int i=N-1; i>=0; --i){
		if(vec[i]!=0)  resault.emplace_back(vec[i]);
	}

	return resault;
}

template<class T>
std::vector<T> Reverser(std::vector<T> vec){
	return {vec.rbegin(), vec.rend()};
}


template<class T>
std::vector<T> NormerEmpty(std::vector<T> vec){
	return vec;
}

template<class T>
std::vector<T> NormerEnergy(std::vector<T> vec){
	const size_t N = vec.size();
	T normer = 0;
	for(int i=0; i<N; ++i){
		normer += vec[i] * vec[i];
	}
	normer = std::sqrt(normer);
	std::vector<T> vec_norm;
	vec_norm.reserve(N);
	for(int i=0; i<N; ++i){
		vec_norm.emplace_back(vec[i] / normer);
	}
	
	return vec_norm;
}

template<class T>
std::vector<T> NormerMax(std::vector<T> Vec){
	const size_t N = Vec.size();
	std::vector<T> vec = Vec;
	T vec_max = *std::max_element(vec.begin(), vec.end(), [](T a, T b){return std::abs(a) < std::abs(b);});
	if (std::abs(vec_max) > 1e-10) for (auto& val : vec) val /= vec_max;
	
	return vec;
}

template<class T>
void ScalerEmpty(std::vector<T> Vec, std::vector<T> vec) {return vec;};

template<class T>
void ScalerEnergy(std::vector<T> Vec, std::vector<T> vec){
        T Vec_energy = 0;
        for(const auto& val : Vec) Vec_energy += val * val;
        Vec_energy = std::sqrt(Vec_energy);

	T vec_energy = 0;
	for(const auto& val : vec) vec_energy += val * val;
	vec_energy = std::sqrt(vec_energy);
	
	if(vec_energy > 1e-10){
		T scale_factor = Vec_energy / vec_energy;
		for(auto& val : vec) val /= scale_factor;
	}

	return vec;
}


template<class T>
std::vector<T> Scaler(std::vector<T> Vec, std::vector<T> vec){
	std::vector<T> Vec_ = Vec;
	std::vector<T> vec_ = vec;

	for(auto& it : Vec) it = std::abs(it);
	for(auto& it : vec_) it = std::abs(it);	
	
	T Vec_max = *std::max_element(Vec_.begin(), Vec_.end());
	T vec_max = *std::max_element(vec_.begin(), vec_.end());
	T scale_factor = Vec_max/vec_max;
	for(auto& val : vec) val *= scale_factor;

	std::cout << "Max(original signal): " << Vec_max << "\nMax(filtering signal): " << vec_max << "\nScale factor: " << scale_factor << "\n";
	
	return vec;
}

//==============================Sorting==============================
//Insertation Sort - smal masive [begin, end)
template<tupename Iterator>
void insertationSort(Iterator begin, Iterator end){
	if(begin==end) reurn;
	
	for(Iterator i=begin+1; i!=end; ++i){
		Iterator j = i;
		while(j!=begin && *j<*(j-1)){
			std::iter_swap(j, j - 1);
			--j;
		}
	}
}

//Heap Sort - when depth_limith ends
template<typename Iterator>
void heapSort(Iterator begin, Iterator end){
	std::make_heap(begin, end);
	for (auto i=end; i!=begin; --i){
		std::pop_heap(begin, i);
	}
}

//Pivot - mid of 3
template<typename Iterator>
Iteratot choosePivot((Iterator begin, Iterator end){
	auto size = std::distanse(begin, end);
	if (size<=1) return begin;

	Iterator mid = begin;
	std::advance(mid, size/2);

	if(*mid<*begin) std::iter_swap(mid, begin);
	if(*(end-1)<*mid) std::iter_swap(end-1, mid);
	if(*mid<*begin) std::iter_swap(mid, begin);

	return mid;
}


//Partition - Lomuto shema 
template<typename Iterator>
Iteratot partition(Iteratot begin, Iteratot end, Iteratot pivot){
	auto pivotVal  = *pivot;
	std::iter_swap(pivot, end-1);

	Iteratot i = begin;
	for(Iteratot j=begin; j!=endend-1; ++j){
		if(*j<=pivotVal){
			std::iter_swap(i,j);
			++i;
		}
	}
	std::iter_swap(i, end-1);
	return i;
}


//Main part of function
template<typename Iteratot>
void introsort(Iteratot begin, Iteratot end, int depthlimit){
	const int inserttionThreshold = 16;

	wile(std::distancce(begin, end) > insertionThreshold){
		if(dethLimit==0){
			heapSort(begin, end);
			return;
		}

		dehthLimit--;

		Iterator pivot = choosePivot(begin, end);
		Iteratot partitionPoint = partition(begin, end, pivot);
		
		introsort(partitionPoint+1, end, pivot, depthLimit);
		end = partitionPoint;
	}

	insertionSort(begin, end);
}

//To call
template<typename Iteratot>
void introSort(Iteratot begin, Iteratot end){
	if(begin==end) return;

	int size = std::distance(begin, end);
	int depthLimit = 2 * static_cast<int>(std::log2(size));

	introsort(begin, end, depthLimit);
}



//==============================Matrix_Function==============================
template<class T>
void FillStatisticHeaders(Matrix<T>& matrix){
	for(auto& col : matrix.Column){
		col.ME = MathEx(col.Row);
		col.D = Var_def<T>(col);
		col.Sigma = std::sqrt(col.D);
	}
}

template<class T>
void MatrixShowHeaders(Matrix<T>& matrix){
	MatrixShowType(matrix);
	std::cout << "Matrix size: "<< matrix.CountColumn << "x" << matrix.CountRow << "\n";
	std::cout << "\nMath exepction: \n";
	for(const auto& col : matrix.Column){
		std::cout <<  col.ME << "\t";
	}
	std::cout << "\nVariation: \n";
	for(const auto& col : matrix.Column){
		std::cout << col.D << "\t";
	}
	std::cout << "\nSigma: \n";
        for(const auto& col : matrix.Column){
                std::cout << col.Sigma << "\t";
        }

	std::cout << "\n";
}

template<class T>
Matrix<T> Cov(Matrix<T> matrix){
	Matrix<T> buf = matrix;
	Matrix<T> cov;
	
	cov.Type = Covariation;
	cov.CountRow = matrix.CountColumn;
	cov.CountColumn = matrix.CountColumn;

	for(int i=0; i < buf.CountColumn; ++i){
		cov.Column.push_back(Columns<T> {});
		cov.Labels.push_back("");
		cov.Column[i].Name = matrix.Column[i].Name;
		cov.Labels[i] = matrix.Column[i].Name;

		for(int j=0; j < buf.CountColumn; ++j){
			cov.Column[i].Row.push_back(0);
		}
	}

	for(int j = 0; j!=buf.CountColumn; j++){
		T SrRow = 0;
		for(int i=0; i!=buf.CountRow; i++){
			SrRow += buf.Column[j].Row[i];
		}
		SrRow /= buf.CountRow;

		for(int i=0; i!=buf.CountRow; i++){
			buf.Column[j].Row[i] -= SrRow;
		}
	}

	for(int j=0; j < buf.CountColumn; ++j){
		for(int k=0; k < buf.CountColumn; ++k){
			T S = 0;
			for(int i=0; i < buf.CountRow; ++i){
				S += buf.Column[j].Row[i] * buf.Column[k].Row[i];
			}
			S /= buf.CountRow;
			//std::cout << "j = " << j << ", k = " << k << "\n";
			cov.Column[j].Row[k] = S;
			/*
			if (k < cov.Column[j].Row.size()) cov.Column[j].Row[k] = S;
			else {
				cov.Column[j].Row.resize(k + 1, 0);
				cov.Column[j].Row[k] = S;
			}*/
		}
	}
	return cov;
}


template<class T>
Matrix<T> Cor_Cov(Matrix<T> matrix){
	if (matrix.CountColumn != matrix.CountRow) return matrix;
	Matrix<T> buf = matrix;
	size_t n = matrix.CountColumn;	

	buf.Type = Correlation;
        buf.CountRow = matrix.CountColumn;
        buf.CountColumn = matrix.CountColumn;

	for(int i=0; i!=n; i++){
		for(int j=0; j!=n; j++){
			buf.Column[i].Row[j] /= ( std::sqrt(matrix.Column[i].Row[i]) * std::sqrt(matrix.Column[j].Row[j]));
		}
	}
	return buf;
}

template<class T>
Matrix<T> Cor(Matrix<T> matrix){
	Matrix<T> buf = Cov(matrix);
	Matrix<T> cor = Cor_Cov(buf);
	
	return cor;
}


template<class T>
T CF(std::vector<T> a, std::vector<T> b, int t_){
	int t = std::abs(t_);
	T vkf = 0;
	size_t lenght = a.end() - a.begin();
	for(int i=0; i<=(lenght - t); ++i){
		vkf += b[i] * a[i+t];
	}
	
	return vkf;
}

template<class T>
T CF_new(const std::vector<T>& a, const std::vector<T>& b, int lag){
	size_t N = a.size();
	T result = 0;

	if (lag >= 0){
		size_t count = 0;
		for(size_t i=0; i+lag< N; ++i){
			result += a[i] * b[i+lag];
			++count;
		}
		if(count>0) result /= static_cast<T>(count);
	} else {
		size_t count = 0;
		for(size_t i=-lag; i<N; ++i){
			result += a[i] * b[i+lag];
			++count;
		}
		if(count>0) result /= static_cast<T>(count);
	}

	return result;
}


template<class T>
std::vector<T> CF_vec(std::vector<T> a, std::vector<T> b, int t_){
	std::vector<T> buf;
	for(int i=0; i<t_; ++i){
		buf.push_back(CF(a, b, i));
	}
	return buf;
}


std::string NamerACF(int t){
	return "ACF(" + std::to_string(t) + ")";
}


template<class T>
std::vector<T> ACF_vec(const std::vector<T> signal, size_t max_lag){
	size_t N = signal.size();
	max_lag = std::min(max_lag, N-1);
	std::vector<T> result(max_lag + 1);
	
	T mean = MathEx(signal);
	T variance = Disp(signal, mean);

	for(size_t lag=0; lag<=max_lag; ++lag){
		T autocorr = 0;
		for(size_t i=0; i<N-lag; ++i){
			autocorr += (signal[i] - mean) * (signal[i+lag] - mean);
		}
		result[lag] = autocorr / ((N-lag) * variance);
	}
	
	return result;
}


template<class T>
Matrix<T> ACF(std::vector<T> a, int t_){
	int t = std::abs(t_);
	Matrix <T> buf;
	buf.Type = AutoCorrelation;
	buf.CountRow = t;
	buf.CountColumn = t;

	for(int i=0; i<=t; ++i){
		buf.Column.push_back({});
		std::string name = NamerACF(i);
		buf.Column[i].Name = name;
		buf.Labels.push_back(name);
		for(int j=0; j<=t; ++j){
			int et = std::abs(i - j);
			T value = CF(a, a, et);
			buf.Column[i].Row.push_back(value);
		}
	}
	
	return buf;
}


template<class T>
Matrix<T> MatrixPluse(Matrix<T> A, Matrix<T> B){
	Matrix<T> buf;
	if (A.CountColumn != B.CountColumn || A.CountRow != B.CountRow) {
		std::cout << "Error: Size first and second matrix is't equal!\n";
		return buf; 
	}

	buf = A;
	for(int i=0; i!=buf.CountColumn; i++){
		for(int j=0; j!=buf.CountRow; j++){
			buf.Column[i].Row[j] += B.Column[i].Row[j];
		}
	}

	return buf;
}


template<class T>
void MultRow(Matrix<T> matrix, size_t row, T C){
	for(int i=0; i!=matrix.CountColumn; i++){
		matrix.Column[i].Row[row] *= C;
	}
}

template<class T>
void PluseRowsWhitConstant(Matrix<T> matrix, size_t row1, size_t row2, T C){
        for(int i=0; i!=matrix.CountColumn; i++){
                matrix.Column[i].Row[row1] += C * matrix.Column[i].Row[row2];
        }
}


template<class T>
std::vector<std::vector<T>> MatrixChangeOrientation(Matrix<T> matrix){
	int n = matrix.CountRow;
	std::vector<std::vector<T>> A(n, std::vector<T>(n));
	for(int i=0; i<n; ++i){
		for(int j=0; j<n; ++j){
			A[i][j] = matrix.Column[j].Row[i];
		}
	}
	
	return A;
}

template<class T>
std::vector<T> GaussMethod(Matrix<T> matrix, std::vector<T> par){
	int n = matrix.CountRow;
	if(matrix.CountRow != par.size() || matrix.CountRow != matrix.CountColumn){
		std::cout << " Count Row of matrix and size of vector Y is't equal/ or CountRow > CountColumn in Gauss method!\n";
		return {};
	}

	std::vector<std::vector<T>> A = MatrixChangeOrientation(matrix);
	std::vector<T> B = par;

	for(int k=0; k<n; ++k){
		//Search max element
		int maxRow = k;
		T maxVal = std::abs(A[k][k]);
		for(int i=k+1; i<n; ++i){
			if(std::abs(A[i][k]) > maxVal){
				maxVal = std::abs(A[i][k]);
				maxRow = i;
			}
		}
		//Change place row
		if(maxRow != k){
			std::swap(A[k], A[maxRow]);
			std::swap(B[k], B[maxRow]);
		}
		//Check zero
		if(std::abs(A[k][k]) < 1e-10){
			std::cout << "Matrix is singular!";
			return {};
		}
		//Normalize
		T pivot = A[k][k];
		for(int j=k; j<n; ++j){
			A[k][j] /= pivot;
		}
		B[k] /= pivot;
		//Delete element in next rows
		for(int i=k+1; i<n; ++i){
			T factor = A[i][k];
			if(std::abs(factor) > 1e-10){
				for(int j=k; j<n; ++j){
					A[i][j] -= factor * A[k][j];
				}
				B[i] -= factor * B[k];
			}
		}
	}

	std::vector<T> X(n);
	for(int i=n-1; i>=0; --i){
		X[i] =B[i];
		for(int j = i+1; j<n; ++j){
			X[i] -= A[i][j] * X[j];
		}
	}

	return X;
}



//==============================Processing==============================
template<class T>
void QCov(Measurment<T> meas){
        FillStatisticHeaders(meas.matrix[Default]);
        MatrixShowHeaders(meas.matrix[Default]);
	MatrixShow(meas.matrix[Default]);
        std::cout << "\n";

        meas.matrix[Sorted] = MatrixSorting(meas.matrix[Default]);
        MatrixShowType(meas.matrix[Sorted]);
        MatrixShow(meas.matrix[Sorted]);
        std::cout << "\n";

        meas.matrix[Covariation] = Cov(meas.matrix[Sorted]);
	//meas.matrix[Covariation] = Cov(meas.matrix[Default]);
        MatrixShowType(meas.matrix[Covariation]);
        MatrixShow(meas.matrix[Covariation]);
        std::cout << "\n";

        meas.matrix[Correlation] = Cor(meas.matrix[Sorted]);
	//meas.matrix[Correlation] = Cor(meas.matrix[Default]);
        MatrixShowType(meas.matrix[Correlation]);
        MatrixShow(meas.matrix[Correlation]);
        std::cout << "\n";
}

int CovariationProgram(){
	Measurment<double> meas;
	std::vector<std::string> ls = CathFile();
	if(ls.empty()){
		std::cout << "File not found or empty!\n";
		return 1;
	}
        meas.matrix[Default] = convertMatrix<double>(ls);
        QCov(meas);
        WriteFile("Covariation.txt", &QCov<double>, meas);  

	return 0;
}

template<class T>
void KWFilterShow(size_t N, int minf, std::map<int, std::vector<T>> signals_date, std::map<int, std::string> signals_name, Matrix<T> acf_signal, Matrix<T> acf_noize, Matrix<T> acf_fild){
	int k = 4;
        if(minf) k = 7;
        Matrix<T> matrix;
        matrix.CountRow = N;
        matrix.CountColumn = k;

        for(int i=0; i<matrix.CountColumn; ++i){
                matrix.Column.push_back({});
                matrix.Column[i].Row = signals_date[i];
                matrix.Column[i].Name = signals_name[i];

        }

	if(minf){
		std::cout << "ACF(signal, t)\n";
        	MatrixShow(acf_signal);
        	std::cout << "ACF(noize, t)\n";
        	MatrixShow(acf_noize);
		std::cout << "ACF(fild, t)\n";
        	MatrixShow(acf_fild);
	}
        MatrixShow(matrix);
}



template<class T>
void Filter_KholmogorovaWinera(std::vector<T> signal, double NoizCof, size_t lenght, int minf, int ginf){
	Matrix<T> matrix;
	const size_t N = signal.size();

	auto min_it = std::min_element(signal.begin(), signal.end());
	auto max_it = std::max_element(signal.begin(), signal.end());
	T altitude = *max_it - *min_it;
	T centr  = *min_it + altitude/2;
	std::vector<T> noize = Noizer(centr - NoizCof*altitude/2, centr + NoizCof*altitude/2, N);

	std::vector<T> fild;
	fild.reserve(N);

	for(size_t i=0; i<N; ++i){
		fild.emplace_back(signal[i] + noize[i]);
	}

	if(lenght >= N){
		std::cout << "Filter order mast mast be less than signal lenght!\n";
		lenght = std::min(N/2, static_cast<size_t>(10));
	}	

	Matrix<T> acf_signal = ACF(signal, lenght);
	Matrix<T> acf_noize = ACF(noize, lenght);
	Matrix<T> acf_fild = MatrixPluse(acf_signal, acf_noize);


	std::vector<T> acf_signal_vec = CF_vec(signal, signal, lenght);

	std::vector<T> slove = GaussMethod(acf_fild, acf_signal_vec);

	std::vector<T> convolve = Convolve(fild, slove);

	if(ginf) convolve = Scaler(signal ,convolve);

	size_t start = (convolve.size() - N)/2;
	std::vector<T> filtering_centr(convolve.begin() + start, convolve.begin() + start + N);
	std::vector<T> filtering_begin(convolve.begin(), convolve.begin() + N);	
	std::vector<T> filtering_end(convolve.begin() + convolve.size() - N, convolve.end());

	std::vector<T> CF_signal_convolve = CF_vec(convolve, signal, convolve.size());
        auto max_el = std::max_element(CF_signal_convolve.begin(), CF_signal_convolve.end());
        int dist = std::distance(CF_signal_convolve.begin(), max_el);
        std::vector<T> filtering_ACF(convolve.begin() + dist, convolve.begin() + dist + N);

	/*
	enum date_name{Signal,
                        Noize,
                        Fild,
                        Filtering_ACF,
                        Filtering_begin,
                        Filtering_centr,
                        Filtering_end};
	*/

        std::map<int, std::vector<T>> signals_date = {{Signal, signal},
                                                {Noize, noize},
                                                {Fild, fild},
                                                {Filtering_ACF, filtering_ACF},
                                                {Filtering_begin, filtering_begin},
                                                {Filtering_centr, filtering_centr},
                                                {Filtering_end, filtering_end}};

        std::map<int, std::string> signals_name = {{Signal, "signal"},
                                                {Noize, "noize"},
                                                {Fild, "fild"},
                                                {Filtering_ACF, "filtering_Hunt"},
                                                {Filtering_begin, "filtering_begin"},
                                                {Filtering_centr, "filtering_centr"},
                                                {Filtering_end, "filtering_end"}};

        auto lamda = [N, minf, signals_date, signals_name, acf_signal, acf_noize, acf_fild] {KWFilterShow(N, minf, signals_date, signals_name, acf_signal, acf_noize, acf_fild);};

        WriteFile("Kolmogorov-Winer_Filter.txt", lamda);
}




int Filter_KW(){
	std::vector<std::string> ls = CathFile();
	if (ls.empty()){
		std::cout << "File not found or empty!\n";
		return 1;
	}
	Matrix<double> matrix = convertMatrix<double>(ls);
	std::cout << "Input mult level of noize: ";
	double noizeLevel;
	std::cin >> noizeLevel;
	std::cout << "Input lenght of window: ";
	int window;
	std::cin >> window;

	std::string inf;
        int minf;
        while(inf != "all" && inf != "main"){
                std::cout << "How much inforamtion do you need? All or main?\n[all/main]: ";
                std::cin >> inf;
                if(inf == "all") minf = 1;
                if(inf == "main") minf = 0;
        }
	int ginf;
        while(inf != "on" && inf != "off"){
                std::cout << "Switche on gain control?\n[on/off]: ";
                std::cin >> inf;
                if(inf == "on") ginf = 1;
                if(inf == "off") ginf = 0;
        }

	Filter_KholmogorovaWinera(matrix.Column[0].Row, noizeLevel, window, minf, ginf);

	/*
	auto wrappedFunc = [&matrix, noizeLevel, window] {
		Filter_KholmogorovaWinera(matrix.Column[0].Row, noizeLevel, static_cast<size_t>(window));
	};
	SendFile(wrappedFunc);
	*/
	
	return 0;
}


template<class T>
void CFilterShow(size_t N, int minf, std::map<int, std::vector<T>> signals_date, std::map<int, std::string> signals_name, Matrix<T> acf_noize){
	int k = 4;
	if(minf) k = 7;
	Matrix<T> matrix;
	matrix.CountRow = N;
        matrix.CountColumn = k;

        for(int i=0; i<matrix.CountColumn; ++i){
                matrix.Column.push_back({});
                matrix.Column[i].Row = signals_date[i];
                matrix.Column[i].Name = signals_name[i];

        }

	if(minf){
        	std::cout << "ACF(noize, t)\n";
        	MatrixShow(acf_noize);
	}
	MatrixShow(matrix);
}

template<class T>
void FilterConsistent(std::vector<T> signal, double NoizCof, int minf, int ginf){
	//Matrix<T> matrix;
	const size_t N = signal.size();

	auto min_it = std::min_element(signal.begin(), signal.end());
	auto max_it = std::max_element(signal.begin(), signal.end());
	T altitude = *max_it - *min_it;
	T centr  = *min_it + altitude/2;
	std::vector<T> noize = Noizer(centr - NoizCof*altitude/2, centr + NoizCof*altitude/2, N);
	
	std::vector<T> fild;
	fild.reserve(N);

	for(size_t i=0; i<N; ++i){
		fild.emplace_back(signal[i] + noize[i]);
        }

	
	std::vector<T> signal_reverse = ReverserReal(signal);
	size_t lenght = signal_reverse.size();


	Matrix<T> acf_noize = ACF(noize, lenght);

	std::vector<T> solve_reverce = GaussMethod(acf_noize, signal_reverse);

	std::vector<T> solve_reverce_norm = NormerEnergy(solve_reverce);
	
	std::vector<T> convolve = Convolve(fild, solve_reverce_norm);

	if(ginf) convolve = Scaler(signal ,convolve);
	
	size_t start = (convolve.size() - N)/2;
        std::vector<T> filtering_centr(convolve.begin() + start, convolve.begin() + start + N);
        std::vector<T> filtering_begin(convolve.begin(), convolve.begin() + N);
        std::vector<T> filtering_end(convolve.begin() + convolve.size() - N, convolve.end());

	//std::vector<T> CF_signal_convolve = CF_vec(signal, convolve, std::abs(static_cast<int>(convolve.size())-static_cast<int>(signal.size())));
	std::vector<T> CF_signal_convolve = CF_vec(convolve, signal, convolve.size());
	auto max_el = std::max_element(CF_signal_convolve.begin(), CF_signal_convolve.end());
	int dist = std::distance(CF_signal_convolve.begin(), max_el);
	std::vector<T> filtering_ACF(convolve.begin() + dist, convolve.begin() + dist + N);

	/*
	enum date_name{Signal, 
			Noize, 
			Fild, 
			Filtering_ACF,
			Filtering_begin,
			Filtering_centr,  
			Filtering_end}; 
	*/

	std::map<int, std::vector<T>> signals_date = {{Signal, signal},
                                                {Noize, noize},
                                                {Fild, fild},
                                                {Filtering_ACF, filtering_ACF},
                                                {Filtering_begin, filtering_begin},
                                                {Filtering_centr, filtering_centr},
                                                {Filtering_end, filtering_end}};

	std::map<int, std::string> signals_name = {{Signal, "signal"}, 
						{Noize, "noize"}, 
						{Fild, "fild"}, 
						{Filtering_ACF, "filtering_Hunt"}, 
						{Filtering_begin, "filtering_begin"},
						{Filtering_centr, "filtering_centr"}, 
						{Filtering_end, "filtering_end"}};

	auto lamda = [N, minf, signals_date, signals_name, acf_noize] {CFilterShow(N, minf, signals_date, signals_name, acf_noize);};

	WriteFile("Consistent_Filter.txt", lamda);

}

int Filter_C(){
	std::vector<std::string> ls = CathFile();
	if(ls.empty()){
		std::cout << "File not found or empty!\n";
		return 1;
	}
	Matrix<double> matrix = convertMatrix<double>(ls);
        std::cout << "Input mult level of noize: ";
        double noizeLevel;
        std::cin >> noizeLevel;
	std::string inf;
	int minf;
	while(inf != "all" && inf != "main"){
		std::cout << "How much inforamtion do you need? All or main?\n[all/main]: ";
		std::cin >> inf;
		if(inf == "all") minf = 1;
		if(inf == "main") minf = 0;
	}
	int ginf;
	while(inf != "on" && inf != "off"){
                std::cout << "Switche on gain control?\n[on/off]: ";
                std::cin >> inf;
                if(inf == "on") ginf = 1;
                if(inf == "off") ginf = 0;
        }


	FilterConsistent(matrix.Column[0].Row, noizeLevel, minf, ginf);
	
	return 0;
}




int main(){
	std::string start;
	while(start != "end"){
		std::string mod;
		while(mod != "kw" && mod != "c" && mod != "cov"){
			std::cout << "Chouse module of program\nKolmogorov's-Winer's filter [kw], Consistent filter [c], Covariation [cov]\n[kw/c/cov]: ";
			std::cin >> mod;
			if(mod == "kw") Filter_KW();
			if(mod == "c") Filter_C();
			if(mod == "cov") CovariationProgram();
		}
		std::cout << "Write 'end' to close program, if do you want countinue write some one else\n[end/...]: ";
		std::cin >> start;
	}
	std::cout << "@Creat by Jabics Leon, MSU 2025\n";
	return 0;
}
