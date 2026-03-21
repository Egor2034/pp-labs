#include <iostream>
#include <locale>
#include <fstream>
#include <string>
#include <format>
#include <windows.h>
#include <chrono>
#include "matrix.hpp"

const std::string RESULTS_FOLDER = ".\\matrices\\results";
const std::string MATRICES_FOLDER = ".\\matrices\\";

void save_info(std::string path, size_t matrix_size, std::chrono::milliseconds duration) {
	std::ofstream file;

	file.open(path, std::ios::app);
	
  	if (!file.is_open()) {
    	throw std::exception("Ошибка при открытии файла!");
	}

	file << "Размер матриц: " << matrix_size << "x" << matrix_size << "\n";
	file << "Объём работы: " << Matrix<float>::total_operations() << " операций\n";
	file << "Время выполнения: " << duration.count() << " мс\n\n";
	
	file.close();
	 
}

int main() {
	SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    setlocale(LC_ALL, "ru_RU.UTF-8");

	try{
		int sizes[] = {100, 200, 400, 800, 1200, 1600, 2000};

		for (int size : sizes) {
			Matrix<float> matrix_a = read_from_file<float>(std::format("{}\\matrixA{}x{}.txt", MATRICES_FOLDER, size, size));
			Matrix<float> matrix_b = read_from_file<float>(std::format("{}\\matrixB{}x{}.txt", MATRICES_FOLDER, size, size));

			auto start = std::chrono::high_resolution_clock::now();

			Matrix<float> result = matrix_a * matrix_b;

			auto end = std::chrono::high_resolution_clock::now();
			auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
			
			save_to_file(result, std::format("{}\\result{}x{}.txt", RESULTS_FOLDER,size, size));
			save_info("info.txt", matrix_a.rows(), duration);
		}
	}
	catch (std::exception& ex){
		std::cout << ex.what();
	}
	return 0;
}