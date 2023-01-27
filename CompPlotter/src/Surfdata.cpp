#include "../include/Surfdata.h"

#include <fstream>
#include <string>

#define _USE_MATH_DEFINES
#include <math.h>

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include <algorithm>
#include <functional>
#include <cctype>
#include <locale>

#include <algorithm>
#include <cctype>
#include <locale>

// trim from start (in place)
static inline void ltrim(std::string &s)
{
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch)
                                    { return !std::isspace(ch); }));
}

// trim from end (in place)
static inline void rtrim(std::string &s)
{
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch)
                         { return !std::isspace(ch); })
                .base(),
            s.end());
}

// trim from both ends (in place)
static inline void trim(std::string &s)
{
    rtrim(s);
    ltrim(s);
}

void split(std::string to_split, std::string delimiter, std::vector<double> &output)
{
    size_t pos = 0;
    std::string token;
    while ((pos = to_split.find(delimiter)) != std::string::npos)
    {
        token = to_split.substr(0, pos);
        if (token != "")
            output.push_back(std::stod(token));
        to_split.erase(0, pos + delimiter.length());
    }
}

std::vector<SurfacePoint> extractDataFromFile(const std::string &filename)
{
#if debug == 2
    std::cout << "Extracting data from " << filename << std::endl;
#endif
    std::ifstream file(filename);
    if (!file.is_open())
    {
        std::cout << "Unable to open file" << std::endl;
        throw;
    }

    std::string line;
    std::vector<SurfacePoint> output;

    while (getline(file, line))
    {
        // Remove start and end whitespace
        trim(line);

        // Read data from line and return as vector of doubles
        std::vector<double> row;
        split(line, " ", row);
        output.push_back(SurfacePoint(row));
    }

    file.close();
    return output;
}

Surfdata::Surfdata(const std::string &filePath, const bool &hasJet) : m_hasJet(hasJet)
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

        double p = points[i].p;

        // // TODO read correct jet positions
        // if (m_hasJet && yc < 0.014 && xc < 1.5)
        // {
        //     continue;
        // }

        // CD += Cp * nx * ds * 2.0;
        CD += nx * ds * (p - 284.1922) / (0.5 * 0.0066 * 472.8044 * 472.8044);
    }

    CD /= (0.6 * 0.6 * 0.25 * 3.14159);

#if debug
    std::cout << "CD = " << CD << std::endl;
#endif
}
