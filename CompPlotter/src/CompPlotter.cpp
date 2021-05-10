#include <iostream>
#include <vector>

#include <chrono>

#include "../include/Surfdata.h"

#define ITER_BENCH 100

int main()
{
    std::string filepath = "data/forces820000.dat";

    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    for (size_t i = 0; i < ITER_BENCH; i++)
    {
        Surfdata s(filepath); /* code */
    }
    
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count()/ITER_BENCH << "[µs]" << std::endl;

    filepath = "D:/Users/kmont/Documents/LongJet/forces820000.dat";
    begin = std::chrono::steady_clock::now();
    for (size_t i = 0; i < ITER_BENCH; i++)
    {
        Surfdata s(filepath); /* code */
    }
    end = std::chrono::steady_clock::now();
    std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count()/ITER_BENCH << "[µs]" << std::endl; 
}
