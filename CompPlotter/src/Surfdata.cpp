#include "../include/Surfdata.h"

#include <fstream>
#include <string>

#define _USE_MATH_DEFINES
#include <math.h>

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
        int ix = 0;
        // ignore whitespace
        while (ix < line.size() && line[ix] == ' ') { ix++; }
        int stIx = ix;
        while (ix < line.size()) {
            if (line[ix] == ' ') {
                // TODO: test this code lol
                double result = 0;
                if (v_len == 1 || v_len == 2 || v_len == 3 || v_len == 6 || v_len == 8) {
                    result = std::stod(line.substr(stIx, ix-stIx));
                } // 1 2 3 6 8
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
            if (v_len == 1 || v_len == 2 || v_len == 3 || v_len == 6 || v_len == 8) {
                v[v_len] = std::stod(line.substr(stIx, ix-stIx));
            }
            v_len++;
        }
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

        // TODO read correct jet positions
        if (m_hasJet && yc < 0.014 && xc < 1.5){
            continue;
        }

        CD += Cp * nx * ds * yc * 2.0;
    }

    CD /= (0.509537 * 0.509537 * 0.25);

    #if debug==2
    std::cout << "CD = " << CD << std::endl;
    #endif
}
