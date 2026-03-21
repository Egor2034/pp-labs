import argparse
import numpy as np
import os
import random


def generate_matrix(size: int, path: str, seed: int) -> None:
    """Генерация случайной матрицы и сохранение в файл"""
    if seed is not None:
        np.random.seed(seed)
    
    matrix = np.random.rand(size, size) * 10
    
    with open(path, 'w', encoding='utf-8') as file:
        file.writelines(f"{size} {size}\n")
        
        for i in range(size):
            for j in range(size):
                file.write(f"{matrix[i][j]:.4f} ")
            file.write("\n")


def main() -> None:
    os.makedirs(".\\matrices", exist_ok=True)
    os.makedirs(".\\matrices\\results", exist_ok=True)
    
    FOLDER = ".\\matrices"
    sizes = [100, 200, 400, 800, 1200, 1600, 2000]
    
    for size in sizes:
        seedA = random.randint(1, 100)
        seedB = random.randint(100, 200)

        generate_matrix(size, f".\\{FOLDER}\\matrixA{size}x{size}.txt", seedA)
        generate_matrix(size, f".\\{FOLDER}\\matrixB{size}x{size}.txt", seedB)

    
    

if __name__ == "__main__":
    main()