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


struct CoefficientData{
    unsigned int timestep;
    double CD;
    double CTExit;
    double CTThroat;

    CoefficientData(int step, double CD) : timestep(step), CD(CD) {};
};

void ParseDataAndOutput(const std::string& directoryPath, const bool& hasJet){

    #if debug
    std::cout << "Getting file names in directory" << std::endl;
    #endif
    // Get all files names in directory.
    std::vector<std::string> files;
    for (const auto & entry : std::filesystem::directory_iterator(directoryPath))
    {
        std::string file = entry.path().string(); // Name of file as a string
        if (file.find(".dat") != std::string::npos) files.push_back(file); // If file is a data file, add it to vector
    }

    #if debug
    std::cout << "Sorting data from .dat files" << std::endl;
    #endif
    // Sort data from .dat files
    std::vector<CoefficientData> data;
    data.reserve(files.size());
    for (size_t i = 0; i < files.size(); i++)
    {
        // Create temporary surfdata object
        Surfdata temp(files[i], hasJet);

        // Find timestep from file name
        std::string stepAsString = files[i].substr(files[i].find(".dat") - 6, 6);
        size_t stepIndex = 0;
        for ( ; stepIndex < stepAsString.length(); stepIndex++ ){ if ( isdigit(stepAsString[stepIndex]) ) break; }
        stepAsString = stepAsString.substr(stepIndex, stepAsString.length() - stepIndex );
        int step = atoi(stepAsString.c_str());

        // Add data to 
        data.push_back(CoefficientData(step, temp.CD));
    }

    if (hasJet){
        #if debug
        std::cout << "Looking in screen file for CT values" << std::endl;
        #endif
        // Find corresponding thrust values
        for (size_t i = 0; i < data.size(); i++)
        {
            CoefficientData temp = data[i];
            
            // TODO Find corresponding CT values
        }
    }


    #if debug
    std::cout << "Writing data to file" << std::endl;
    #endif
    // TODO Write data to file
}

// COMPILE SCRIPT - clang++ -std=c++17 -o a.exe -I include src/*.cpp -O3 -target x86_64-pc-windows-msvc 
int main()
{
    ParseDataAndOutput("F:/Users/kmont/Documents/LongJet/noJet/dirforces", false);

    std::cout << "Finished" << std::endl;
}
