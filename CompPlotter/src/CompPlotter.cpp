#include <algorithm>
#include <cmath>
#include <iostream>
#include <vector>

#include <chrono>

#include "../include/Surfdata.h"

#define ITER_BENCH 100


struct Stats {
    double meanMs;
    double medianMs;
    double stdDev;
    double minMs;
    double maxMs;
};

double getMs(const std::chrono::nanoseconds& ns) {
    return ((double)std::chrono::duration_cast<std::chrono::microseconds>(ns).count())/1000.0;
}

Stats calculate(std::vector<std::chrono::nanoseconds>& runs) {
    Stats out;

    std::sort(runs.begin(), runs.end());

    out.minMs = getMs(runs[0]);
    out.maxMs = getMs(runs[runs.size() - 1]);

    const int ix = (runs.size() - 1) / 2;
    if (runs.size() % 2 == 0) {
        out.medianMs = (getMs(runs[ix]) + getMs(runs[ix+1]))/2;
    } else {
        out.medianMs = getMs(runs[ix]);
    }

    out.meanMs = 0;
    for (auto &v : runs) {
        out.meanMs += getMs(v);
    }
    out.meanMs /= runs.size();

    out.stdDev = 0;
    for (auto &v : runs) {
        const double ms = getMs(v);
        out.stdDev += (ms - out.meanMs) * (ms - out.meanMs);
    }
    out.stdDev /= runs.size();
    out.stdDev = std::sqrt(out.stdDev);

    return out;
}

void showStats(const Stats& s) {
    std::cout << "Mean: " << s.meanMs << "ms" << std::endl;
    std::cout << "Median (StdDev): " << s.medianMs << "ms (" << s.stdDev << "ms)" << std::endl;
    std::cout << "Minimum/Maximum: " << s.minMs << "ms" << " / " << s.maxMs << "ms" << std::endl;
} 

int main()
{
    std::string filepath = "data/forces820000.dat";

    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    std::vector<std::chrono::nanoseconds> runs;
    for (size_t i = 0; i < ITER_BENCH; i++)
    {
        std::chrono::steady_clock::time_point beginRun = std::chrono::steady_clock::now();
        Surfdata s(filepath); /* code */
        std::chrono::steady_clock::time_point endRun = std::chrono::steady_clock::now();
        runs.push_back(endRun - beginRun);
    }
    
    showStats(calculate(runs));
    //std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    // std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count()/ITER_BENCH << "[µs]" << std::endl;
/*
    filepath = "D:/Users/kmont/Documents/LongJet/forces820000.dat";
    begin = std::chrono::steady_clock::now();
    for (size_t i = 0; i < ITER_BENCH; i++)
    {
        Surfdata s(filepath);
    }
    end = std::chrono::steady_clock::now();
    std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count()/ITER_BENCH << "[µs]" << std::endl; 
*/
}
