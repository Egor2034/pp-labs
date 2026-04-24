#ifndef MATRIX_H_
#define MATRIX_H_

#include <iostream>
#include <fstream>
#include <string>
#include <random>
#include <concepts>

template <typename T> requires std::is_arithmetic_v<T>
class Matrix {
private:
	T* _data;
	size_t _size;

	friend std::ostream& operator<<(std::ostream& os, const Matrix<T>& other) {
		for (size_t i = 0; i < other.size(); i++) {
			for (size_t j = 0; j < other.size(); j++) {
				os << other(i, j) << " ";
			}
			os << "\n";
		}

		return os;
	}
public:
	Matrix(size_t size) {
		_size = size;
		_data = new T[size * size]();
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

	void print() const {
		for (size_t i = 0; i < _size; i++) {
			for (size_t j = 0; j < _size; j++) {
				std::cout << _data[i * _size + j] << " ";
			}
			std::cout << "\n";
		}
	}

	void fill() {
		for (size_t i = 0; i < _size * _size; i++) {
			std::cin >> _data[i];
		}
	}

	size_t size() { return _size; }

	T& operator()(size_t row, size_t column) {
		return _data[row * _size + column];
	}

	const T& operator()(size_t row, size_t column) const {
		return _data[row * _size + column];
	}

	Matrix operator*(const Matrix<T>& other) const {
		Matrix<T> matr(_size);

		for (int i = 0; i < _size; i++) {
			for (int j = 0; j < _size; j++) {
				for (int k = 0; k < _size; k++) { 
					matr(i, j) += _data[i * _size + k] *  other._data[k * _size + j];
				}
			}
		}

		return matr;
	}

	T* data() { return _data; }
	
    const T* data() const { return _data; }
};

template <typename T> requires std::is_arithmetic_v<T>
Matrix<T> read_from_file(std::string path) {
	std::ifstream file;
	std::string line;
 
	file.open(path);

    if (!file.is_open()) {
		throw std::exception("Ошибка при открытии файла!");
    }
	
	size_t rows, columns;
	file >> rows >> columns;
	Matrix<T> matrix(rows);

	for (size_t i = 0; i <  matrix.size(); i++) {
    	for (size_t j = 0; j <  matrix.size(); j++) {
			file >> matrix(i, j);
		}	
	}

	return matrix;
}

template <typename T> requires std::is_arithmetic_v<T>
void save_to_file(Matrix<T> matrix, std::string filename) {
	std::ofstream file;

    file.open(filename);

  	if (!file.is_open()) {
    	throw std::exception("Ошибка при открытии файла!");
	}

  	file << matrix.size() << " " << matrix.size() << "\n";

  	for (size_t i = 0; i <  matrix.size(); i++) {
    	for (size_t j = 0; j <  matrix.size(); j++) {
			file << matrix(i, j) << " ";
		}	
    	file << "\n";
	}
  
  	file.close();
}

#endif