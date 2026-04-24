#ifndef MATRIX_H_
#define MATRIX_H_

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <mpi.h>

template <typename T> 
class Matrix {
private:
    T* _data;
    size_t _size;

public:
    Matrix(size_t size) {
        _size = size;
        _data = new T[size * size]();
        for (size_t i = 0; i < size * size; i++) {
            _data[i] = 0;
        }
    }

    Matrix(const Matrix<T>& other) {
        _size = other._size;
        _data = new T[_size * _size]();
        
        for (size_t i = 0; i < _size * _size; i++) {
            _data[i] = other._data[i];
        }
    }

    ~Matrix() {
        delete[] _data;
    }
    
    size_t size() const {
        return _size;
    }
    
    T& operator()(size_t row, size_t column) {
        return _data[row * _size + column];
    }
    
    const T& operator()(size_t row, size_t column) const {
        return _data[row * _size + column];
    }
    
    Matrix mpi_mult(const Matrix<T>& other) const {
		int rank, size;
		MPI_Comm_rank(MPI_COMM_WORLD, &rank);
		MPI_Comm_size(MPI_COMM_WORLD, &size);
		
		if (_size % size != 0) {
			std::stringstream ss;
			ss << "Matrix size (" << _size << ") must be divisible by number of processes (" << size << ")";
			throw ss.str();
		}
		
		Matrix<T> res(_size);
		Matrix<T> local_res(_size);
		
		size_t work = other._size / size;
		size_t local_start = rank * work;
		size_t local_end = (rank == size - 1) ? other._size : local_start + work;
		
		T* m2_block = new T[other._size * work];
		for (size_t j = local_start; j < local_end; ++j) {
			for (size_t i = 0; i < other._size; ++i) {
				m2_block[(j - local_start) * other._size + i] = other._data[i * other._size + j];
			}
		}
		
		for (size_t i = 0; i < _size; ++i) {
			for (size_t j = local_start; j < local_end; ++j) {
				T sum = 0;
				for (size_t k = 0; k < _size; ++k) {
					sum += _data[i * _size + k] * other._data[k * _size + j];
				}
				local_res(i, j) = sum;
			}
		}
		
		delete[] m2_block;
		
		MPI_Datatype mpi_type;
		if (sizeof(T) == sizeof(float)) {
			mpi_type = MPI_FLOAT;
		} else {
			mpi_type = MPI_DOUBLE;
		}
		
		MPI_Reduce(local_res._data, res._data, _size * _size, mpi_type, MPI_SUM, 0, MPI_COMM_WORLD);
		
		return res;
	}
};

template <typename T>
Matrix<T> read_from_file(std::string path) {
    std::ifstream file;
    file.open(path.c_str());
    
    if (!file.is_open()) {
        std::stringstream ss;
        ss << "Ошибка при открытии файла: " << path;
        throw ss.str();
    }
    
    size_t rows, columns;
    file >> rows >> columns;
    Matrix<T> matrix(rows);
    
    for (size_t i = 0; i < matrix.size(); i++) {
        for (size_t j = 0; j < matrix.size(); j++) {
            file >> matrix(i, j);
        }
    }
    
    file.close();
    return matrix;
}

#endif