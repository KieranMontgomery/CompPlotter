#include <algorithm>
#include <cmath>
#include <iostream>
#include <vector>
#include <chrono>
#include <thread>
#include <utility>
#include <filesystem>

#include "ParseData.h"

#define ITER_BENCH 5

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

// COMPILE SCRIPT - clang++ -std=c++17 -o a.exe -I include src/*.cpp -O3 -target x86_64-pc-windows-msvc 

// g++-10 src/*.cpp -std=c++17 -o a.out -O3 -I include -pthread
int main()
{

    std::string path;
#ifdef __linux__

    #if test
        path = "/home/kieranunix/dirforces";
    #else
        //path = "/mnt/c/Users/kmont/Documents/Data/4LevCT0To3Jet0p05/raw/dirforces";
        path = "/media/kam2317/6TB/Results/May2021/4LevCT0To3Jet0p15/raw/dirforces";
    #endif

#else

    #if test
        path = "F:/Users/kmont/Documents/LongJet/testJet/dirforces";
    #else
        path = "C:/Users/kmont/Documents/Data/4LevCT0To3Jet0p05/raw/dirforces";
    #endif

#endif

    //ParseData("C:/Users/kmont/Documents/Data/noJet/dirforces", "noJet", false, 12);
    //ParseData(path, "Jet0p05", true, 12);
    //ParseData("C:/Users/kmont/Documents/Data/rampUpAndDown/dirforces", "JetRamp", true, 12);

    ParseData("C:/Users/kmont/Documents/Data/CT0p1/dirforces", "CT0p1", true, 12);
    ParseData("C:/Users/kmont/Documents/Data/CT0p3/dirforces", "CT0p3", true, 12);
    ParseData("C:/Users/kmont/Documents/Data/CT0p5/dirforces", "CT0p5", true, 12);
    ParseData("C:/Users/kmont/Documents/Data/CT1p5/dirforces", "CT1p5", true, 12);
    ParseData("C:/Users/kmont/Documents/Data/CT2p5/dirforces", "CT2p5", true, 12);
}
