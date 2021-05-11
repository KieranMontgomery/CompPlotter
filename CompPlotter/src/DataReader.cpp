#include "../include/DataReader.h"

#include <fstream>
#include <string>

#define OLD_PATH

std::vector<std::vector<double>> extractDataFromFile(std::string& filename) 
{
    std::ifstream file(filename);
    if (!file.is_open()){
        std::cout << "Unable to open file" << std::endl; 
        throw;
    }

    std::string line;
    std::vector<std::vector<double>> output;
    std::vector<double> v;
    v.reserve(25);
    while(getline(file, line))
    {
        v.clear();

        #ifdef OLD_PATH
        line = line.substr(line.find_first_not_of(" "));
        size_t pos = 0;
        while ((pos = line.find("  ")) != std::string::npos) {
            v.push_back(std::stod(line.substr(0, pos)));
            line.erase(0, pos + 2); // pos plus delimeter size
        }
        #else
        int ix = 0;
        // ignore whitespace
        while (ix < line.size() && line[ix] == ' ') { ix++; }
        int stIx = ix;
        while (ix < line.size()) {
            if (line[ix] == ' ') {
                v.push_back(std::stod(line.substr(stIx, ix-stIx)));
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
            v.push_back(std::stod(line.substr(stIx, ix-stIx)));
        }
        #endif
        output.push_back(v);
    }
    file.close();
    return output;
}
