import numpy as np


RESULTS_FOLDER = ".\\matrices\\results\\"
MATRICES_FOLDER = ".\\matrices\\"


def main() -> None:
    sizes = [100, 200, 400, 800, 1200, 1600, 2000]

    for size in sizes:
        matrix_a = np.loadtxt(f"{MATRICES_FOLDER}\\matrixA{size}x{size}.txt", skiprows=1)
        matrix_b = np.loadtxt(f"{MATRICES_FOLDER}\\matrixB{size}x{size}.txt", skiprows=1)
        result1 = np.loadtxt(f"{RESULTS_FOLDER}\\result{size}x{size}.txt", skiprows=1)

        result2 = np.dot(matrix_a, matrix_b)

        if (np.allclose(result1, result2)):
            print("Умножение матриц выполнено правильно")
        else:
            print("Допущены ошибки при вычислении")


if __name__ == "__main__":
    main()