#include "../include/Surfdata.h"
#include "../include/DataReader.h"

#include <fstream>
#include <string>

// #define OLD_PATH

std::vector<SurfacePoint> extractDataFromFile(std::string& filename) 
{
    std::ifstream file(filename);
    if (!file.is_open()){
        std::cout << "Unable to open file" << std::endl; 
        throw;
    }

    std::string line;
    std::vector<SurfacePoint> output;
    double v[30];
    int v_len = 0;
    while(getline(file, line))
    {
        v_len = 0;
        #ifdef OLD_PATH
        line = line.substr(line.find_first_not_of(" "));
        size_t pos = 0;
        while ((pos = line.find("  ")) != std::string::npos) {
            v[v_len] = std::stod(line.substr(0, pos));
            v_len++;
            line.erase(0, pos + 2); // pos plus delimeter size
        }
        #else
        int ix = 0;
        // ignore whitespace
        while (ix < line.size() && line[ix] == ' ') { ix++; }
        int stIx = ix;
        while (ix < line.size()) {
            if (line[ix] == ' ') {
                double result = std::stod(line.substr(stIx, ix-stIx));
                v[v_len] = result;
                v_len++;
                while (ix < line.size() && line[ix] == ' ') {
                    ix++;
                }          
                stIx = ix;
            } else {
                ix++;
            }
        }
        /* parse the last number if there's any left */
        if (stIx < line.size()) {
            v[v_len] = std::stod(line.substr(stIx, ix-stIx));
            v_len++;
        }
        #endif
        output.push_back(SurfacePoint(v));
    }
    file.close();
    return output;
}


std::ostream& operator<<(std::ostream& os, const SurfacePoint& sp)
{
	// std::copy(sp.data.begin(), sp.data.end(), std::ostream_iterator<double>(os, "\t"));
    return os;
}

Surfdata::Surfdata(std::string filePath)
{
    // Collect and sort data from file
    points = std::move(extractDataFromFile(filePath));
}
