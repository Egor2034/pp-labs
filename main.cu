#include <iostream>
#include <locale>
#include <fstream>
#include <string>
#include <format>
#include <windows.h>
#include <chrono>
#include <vector>
#include <cuda_runtime.h>
#include "matrix.hpp"

__global__ void mat_mul_kernel(const float* a, const float* b, float* c, size_t n) {
    size_t row = blockIdx.y * blockDim.y + threadIdx.y;
    size_t col = blockIdx.x * blockDim.x + threadIdx.x;

    if (row < n && col < n) {
        float sum = 0.0f;
        for (size_t k = 0; k < n; ++k) {
            sum += a[row * n + k] * b[k * n + col];
        }
        c[row * n + col] = sum;
    }
}

void multiply_matrices_cuda(const float* h_a, const float* h_b, float* h_c, size_t n, dim3 block_size) {
    size_t size = n * n * sizeof(float);
    float *d_a, *d_b, *d_c;

    cudaMalloc((void**)&d_a, size);
    cudaMalloc((void**)&d_b, size);
    cudaMalloc((void**)&d_c, size);

    cudaMemcpy(d_a, h_a, size, cudaMemcpyHostToDevice);
    cudaMemcpy(d_b, h_b, size, cudaMemcpyHostToDevice);

    dim3 grid_size((n + block_size.x - 1) / block_size.x, 
                   (n + block_size.y - 1) / block_size.y);

    mat_mul_kernel<<<grid_size, block_size>>>(d_a, d_b, d_c, n);

    cudaDeviceSynchronize();
    
    cudaError_t err = cudaGetLastError();
    if (err != cudaSuccess) {
        throw std::runtime_error(cudaGetErrorString(err));
    }

    cudaMemcpy(h_c, d_c, size, cudaMemcpyDeviceToHost);

    cudaFree(d_a);
    cudaFree(d_b);
    cudaFree(d_c);
}

void save_log(std::ofstream& log, size_t size, auto block_size, auto operations, auto duration) {
    log << "Размер матриц: " << size << "x" << size << "\n";
    log << "Число потоков: " << (block_size.x * block_size.y) << "\n";
    log << "Конфигурация блока: " << block_size.x << "x" << block_size.y << "\n";
    log << "Объём работы: " << operations << " операций\n";
    log << "Время выполнения: " << duration.count() << " мс\n\n";
}

const std::string results_folder = ".\\matrices\\results";
const std::string matrices_folder = ".\\matrices\\";

int main() {
    SetConsoleOutputCP(CP_UTF8);
    setlocale(LC_ALL, ".utf8");

    std::vector<size_t> sizes = {100, 200, 400, 800, 1200, 1600, 2000};
    
    std::vector<dim3> block_configs = {
        dim3(8, 8),
        dim3(16, 16),
        dim3(32, 32)
    };

    std::ofstream log1("info8x8.txt");
    std::ofstream log2("info16x16.txt");
    std::ofstream log3("info32x32.txt");

    try {
        for (size_t size : sizes) {
            std::cout << size << "x" << size << std::endl;

            Matrix<float> matrix_a = read_from_file<float>(std::format("{}matrixA{}x{}.txt", matrices_folder, size, size));
            Matrix<float> matrix_b = read_from_file<float>(std::format("{}matrixB{}x{}.txt", matrices_folder, size, size));
            Matrix<float> result_cuda(size);

            size_t operations = 2LL * size * size * size;

            for (const auto& block_size : block_configs) {
                auto start = std::chrono::high_resolution_clock::now();
                
                multiply_matrices_cuda(matrix_a.data(), matrix_b.data(), result_cuda.data(), size, block_size);
                
                auto end = std::chrono::high_resolution_clock::now();
                auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
                
                if (block_size.x * block_size.y == 64) {
                    save_log(log1, size, block_size, operations, duration);
                } 
                else if (block_size.x * block_size.y == 256) {
                    save_log(log2, size, block_size, operations, duration);
                } 
                else {
                    save_log(log3, size, block_size, operations, duration);
                }
            }

            save_to_file(result_cuda, std::format("{}\\result{}x{}.txt", results_folder, size, size));
        }
        
        log1.close();
        log2.close();
        log3.close();
    }
    catch (std::exception& ex) {
        std::cout << ex.what();
    }
    return 0;
}