#include <iostream>
#include <locale>
#include <fstream>
#include <string>
#include <format>
#include <windows.h>
#include <chrono>
#include <math.h>
#include "matrix.hpp"

const std::string RESULTS_FOLDER = ".\\matrices\\results";
const std::string MATRICES_FOLDER = ".\\matrices";

void save_info(std::string path, size_t matrix_size, size_t num_threads, std::chrono::milliseconds duration) {
	std::ofstream file;

	file.open(path, std::ios::app);
	
  	if (!file.is_open()) {
    	throw std::exception("Ошибка при открытии файла!");
	}

	unsigned long long num_of_operations = 2 * pow(matrix_size, 3) - pow(matrix_size, 2); 
	
	file << "Размер матриц: " << matrix_size << "x" << matrix_size << "\n";
	file << "Число потоков: " << num_threads << "\n";
	file << "Объём работы: " << num_of_operations << " операций\n";
	file << "Время выполнения: " << duration.count() << " мс\n\n";
	
	file.close();
	 
}

int main() {
	SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    setlocale(LC_ALL, "ru_RU.UTF-8");

	try{
		int sizes[] = {100, 200, 400, 800, 1200, 1600, 2000};
		int num_of_threads[] = {1, 2, 4, 8};
		
		for (int el : num_of_threads) {
			for (int size : sizes) {
				omp_set_num_threads(el);
				Matrix<float> matrix_a = read_from_file<float>(std::format("{}\\matrixA{}x{}.txt", MATRICES_FOLDER, size, size));
				Matrix<float> matrix_b = read_from_file<float>(std::format("{}\\matrixB{}x{}.txt", MATRICES_FOLDER, size, size));

				auto start = std::chrono::high_resolution_clock::now();

				Matrix<float> result = matrix_a * matrix_b;

				auto end = std::chrono::high_resolution_clock::now();
				auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
				
				save_to_file(result, std::format("{}\\result{}x{}_{}.txt", RESULTS_FOLDER, size, size, el));
				save_info("info.txt", matrix_a.rows(), el, duration);
			}
		}
	}
	catch (std::exception& ex){
		std::cout << ex.what();
	}
	return 0;
}