#include <iostream>
#include <vector>
#include <cstdlib> 
#include <fstream>
#include <ctime>
#include <sstream>
#include <chrono>
#include <iomanip>


std::vector<std::vector<int>> create_matrix_random(const int n)
{
    std::vector<std::vector<int>> matrix(n, std::vector<int>(n));
    for (int i = 0; i < n; ++i) 
    {
        for (int j = 0; j < n; ++j) 
        {
            matrix[i][j] = rand() % 500;
        }
    }
    return matrix;
}


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


std::vector<std::vector<int>> matrix_mult(const std::vector<std::vector<int>>& matrixA,
                         const std::vector<std::vector<int>>& matrixB, const int size)
{
    std::vector<std::vector<int>> result(size, std::vector<int>(size, 0));
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
    return result;
}


int main()
{
    srand(time(nullptr));

    std::vector<int> sizes = { 100, 250, 500, 750, 1000 };
    int x = 5;
    std::ofstream time_file("average_times.txt");

    for (int size : sizes) 
    {
        auto total_duration = std::chrono::milliseconds(0);

        for (int k = 0; k < x; ++k) 
        {
            std::vector<std::vector<int>> tmp_A_matrix = create_matrix_random(size);
            std::vector<std::vector<int>> tmp_B_matrix = create_matrix_random(size);

            auto start = std::chrono::high_resolution_clock::now();
            std::vector<std::vector<int>> result_mul = matrix_mult(tmp_A_matrix, tmp_B_matrix, size);
            auto end = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

            total_duration += duration;

            if (k == 0) 
            {
                std::stringstream filename_stream_A;
                filename_stream_A << "matrix_A_" << size << ".txt";
                std::string filename_A = filename_stream_A.str();

                std::stringstream filename_stream_B;
                filename_stream_B << "matrix_B_" << size << ".txt";
                std::string filename_B = filename_stream_B.str();

                std::stringstream filename_stream_res;
                filename_stream_res << "res_AxB_" << size << ".txt";
                std::string filename_res = filename_stream_res.str();

                write_matrix_file(result_mul, filename_res);
                write_matrix_file(tmp_A_matrix, filename_A);
                write_matrix_file(tmp_B_matrix, filename_B);
            }
        }

        auto average_duration = total_duration / x;
        time_file << size << " - " << average_duration.count() << std::endl;
    }
    time_file.close();
}