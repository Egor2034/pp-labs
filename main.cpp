#include <iostream>
#include <locale>
#include <fstream>
#include <string>
#include <format>
#include <windows.h>
#include <chrono>
#include <math.h>
#include <mpi.h>
#include "matrix.hpp"

const std::string RESULTS_FOLDER = ".\\matrices\\results";
const std::string MATRICES_FOLDER = ".\\matrices";

void save_info(std::string path, size_t matrix_size, int num_threads, std::chrono::milliseconds duration) {
	int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank != 0) return;

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

int main(int argc, char** argv) {
	MPI_Init(&argc, &argv);

	SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    setlocale(LC_ALL, "ru_RU.UTF-8");
	
	try{
		int world_size;
        MPI_Comm_size(MPI_COMM_WORLD, &world_size);
        int world_rank;
        MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
		
		size_t sizes[] = {200, 400, 800, 1200, 1600, 2000};

		for (size_t size : sizes) {
			Matrix<float> matrix_a = read_from_file<float>(std::format("{}\\matrixA{}x{}.txt", MATRICES_FOLDER, size, size));
			Matrix<float> matrix_b = read_from_file<float>(std::format("{}\\matrixB{}x{}.txt", MATRICES_FOLDER, size, size));

			MPI_Barrier(MPI_COMM_WORLD);
			auto start = std::chrono::high_resolution_clock::now();

			Matrix<float> result = matrix_a.mpi_mult(matrix_b);

			MPI_Barrier(MPI_COMM_WORLD);
			auto end = std::chrono::high_resolution_clock::now();

			auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
			
			if (world_rank == 0) {
				save_to_file<float>(result, std::format("{}\\result{}x{}_{}.txt", RESULTS_FOLDER, size, size, world_size));
				save_info("info.txt", size, world_size, duration);
			}
                
				
			}
	}
	catch (std::exception& ex){
		std::cout << ex.what();
	}

	MPI_Finalize();
	return 0;
}