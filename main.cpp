#include <iostream>
#include <fstream>
#include <string>
#include <math.h>
#include <sstream>
#include <mpi.h>
#include "matrix.hpp"

std::string to_string_custom(size_t num) {
    std::ostringstream oss;
    oss << num;
    return oss.str();
}

void print_info(size_t matrix_size, int num_procs, double duration_seconds, unsigned long long num_of_operations) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank != 0) return;

    std::cout << "Размер матриц: " << matrix_size << "x" << matrix_size << std::endl;
    std::cout << "Число потоков: " << num_procs << std::endl;
    std::cout << "Объём работы: " << num_of_operations << " операций" << std::endl;
    std::cout << "Время выполнения: " << static_cast<int>(duration_seconds * 1000) << " мс" << std::endl;
    std::cout << std::endl;
}

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);
    
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    
    size_t sizes[] = { 200, 400, 800, 1200, 1600, 2000 };
    int num_sizes = sizeof(sizes) / sizeof(sizes[0]);
    
    for (int idx = 0; idx < num_sizes; ++idx) {
        size_t size = sizes[idx];
        
        std::string pathA = "./matrices/matrixA" + to_string_custom(size) + "x" + to_string_custom(size) + ".txt";
        std::string pathB = "./matrices/matrixB" + to_string_custom(size) + "x" + to_string_custom(size) + ".txt";
        
        try {
            Matrix<float> matrix_a = read_from_file<float>(pathA);
            Matrix<float> matrix_b = read_from_file<float>(pathB);
            
            MPI_Barrier(MPI_COMM_WORLD);
            double start_time = MPI_Wtime();
            
            Matrix<float> result = matrix_a.mpi_mult(matrix_b);
            
            MPI_Barrier(MPI_COMM_WORLD);
            double end_time = MPI_Wtime();
            
            double duration = end_time - start_time;
            
            if (world_rank == 0) {
                unsigned long long num_of_operations = 2 * pow(size, 3) - pow(size, 2);
                print_info(size, world_size, duration, num_of_operations);
            }
        }
        catch (std::exception& ex) {
            if (world_rank == 0) {
                std::cout << "Ошибка: " << ex.what() << std::endl;
            }
        }
    }
    
    MPI_Finalize();
    return 0;
}