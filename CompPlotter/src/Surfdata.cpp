#include "../include/Surfdata.h"
#include "../include/DataReader.h"

#include <fstream>
#include <string>

#define _USE_MATH_DEFINES
#include <math.h>

// #define OLD_PATH

std::vector<SurfacePoint> extractDataFromFile(const std::string& filename) 
{
    #if debug==2 
    std::cout << "Extracting data from " << filename << std::endl;
    #endif
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

Surfdata::Surfdata(const std::string& filePath, const bool& hasJet) : m_hasJet(hasJet)
{
    // Collect and sort data from file
    points = std::move(extractDataFromFile(filePath));

    // Calculate values
    CalculateCD();
}

void Surfdata::CalculateCD() 
{
    for (size_t i = 0; i < points.size(); i++)
    {
        double nx = points[i].nx;
        double xc = points[i].xc;
        double yc = points[i].yc;
        double ds = points[i].ds;
        double Cp = points[i].Cp;

        if (m_hasJet && yc < 0.02 && xc < 1.4){
            continue;
        }

        CD += Cp * nx * ds * yc * 2.0;
    }

    CD /= (0.509537 * 0.509537 * 0.25);

    #if debug==2
    std::cout << "CD = " << CD << std::endl;
    #endif
}
