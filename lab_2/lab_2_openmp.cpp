#include <iostream>
#include <vector>
#include <cstdlib> 
#include <fstream>
#include <ctime>
#include <sstream>
#include <chrono>
#include <iomanip>
#include <omp.h>


void write_matrix_file(const std::vector<std::vector<int>>& matrix, const std::string filename)
{
    std::ofstream output(filename);
    if (!output.is_open())
    {
        std::cerr << "Failed to open file\n";
        return;
    }
    for (const auto& row : matrix)
    {
        for (int value : row)
        {
            output << value << ' ';
        }
        output << '\n';
    }
    output.close();
    return;
}


std::vector<std::vector<int>> read_matrix_file(const std::string& filename)
{
    std::vector<std::vector<int>> matrix;

    std::ifstream input(filename);
    if (!input.is_open())
    {
        std::cerr << "Failed to open file\n";
        return matrix;
    }

    std::string line;
    while (std::getline(input, line))
    {
        std::vector<int> row;
        std::istringstream iss(line);
        int value;
        while (iss >> value)
        {
            row.push_back(value);
        }
        matrix.push_back(row);
    }

    input.close();
    return matrix;
}


std::vector<std::vector<int>> matrix_mult(const std::vector<std::vector<int>>& matrixA,
    const std::vector<std::vector<int>>& matrixB, const int size)
{
    std::vector<std::vector<int>> result(size, std::vector<int>(size, 0));
    int threads;
#pragma omp parallel num_threads(20) shared(threads)
    {
        threads = omp_get_num_threads();
#pragma omp for  
        for (int i = 0; i < size; ++i)
        {
            for (int j = 0; j < size; ++j)
            {
                for (int k = 0; k < size; ++k)
                {
                    result[i][j] += matrixA[i][k] * matrixB[k][j];
                }
            }
        }
    }
    return result;
}


int main()
{
    srand(time(nullptr));
    std::ofstream time_file("average_times_20_threads.txt");

    std::vector<int> sizes = { 100, 250, 500, 750, 1000 };
    int x = 5;

    for (int size : sizes)
    {
        auto total_duration = std::chrono::milliseconds(0);

        std::stringstream filename_stream_A;
        filename_stream_A << "matrix_A_" << size << ".txt";
        std::string filename_A = filename_stream_A.str();

        std::stringstream filename_stream_B;
        filename_stream_B << "matrix_B_" << size << ".txt";
        std::string filename_B = filename_stream_B.str();

        std::vector<std::vector<int>> matrix_A = read_matrix_file(filename_A);
        std::vector<std::vector<int>> matrix_B = read_matrix_file(filename_B);

        for (int k = 0; k < x; ++k)
        {
            auto start = std::chrono::high_resolution_clock::now();
            std::vector<std::vector<int>> result_mul = matrix_mult(matrix_A, matrix_B, size);
            auto end = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

            total_duration += duration;
        }

        auto average_duration = total_duration / x;
        time_file << size << " - " << average_duration.count() << std::endl;
    }
    time_file.close(); 
}

