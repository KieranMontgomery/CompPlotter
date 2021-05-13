#include <algorithm>
#include <cmath>
#include <iostream>
#include <vector>
#include <chrono>
#include <thread>
#include <utility>
#include <filesystem>

#include "Surfdata.h"

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

int main()
{

    std::vector<std::string> files;

    std::string path = "F:/Users/kmont/Documents/LongJet/noJet/dirforces"; // Not NVME drive, will move data when we do performance analysis
    for (const auto & entry : std::filesystem::directory_iterator(path)) // Get all files names in directory.
        files.push_back(entry.path().string());

    std::vector<Surfdata> data;

    double average = 0;
    for (size_t i = 0; i < files.size(); i++)
    {
        data.push_back(files[i]);
        average += data[i].CD;
    }
    average /= files.size();

    std::cout << "Average CD is " << average << std::endl;
    
}
