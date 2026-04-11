@echo off

if exist matrices rmdir /s /q matrices
python create_matrix.py

if not exist build mkdir build
cd build
cmake ..
cmake --build .
cd ..

if exist info.txt del info.txt

"C:\Program Files\Microsoft MPI\Bin\mpiexec.exe" -n 1 .\build\Debug\lab3.exe
"C:\Program Files\Microsoft MPI\Bin\mpiexec.exe" -n 2 .\build\Debug\lab3.exe
"C:\Program Files\Microsoft MPI\Bin\mpiexec.exe" -n 4 .\build\Debug\lab3.exe
"C:\Program Files\Microsoft MPI\Bin\mpiexec.exe" -n 8 .\build\Debug\lab3.exe

python verify.py