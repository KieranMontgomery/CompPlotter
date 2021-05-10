#include "../include/DataReader.h"

#include <fstream>
#include <string>

std::vector<std::vector<double>> extractDataFromFile(std::string& filename) 
{
    std::ifstream file(filename);
    if (!file.is_open()){
        std::cout << "Unable to open file" << std::endl; 
        throw;
    }

    std::string line;
    std::vector<std::vector<double>> output;
    while(getline(file, line))
    {
        line = line.substr(line.find_first_not_of(" "));
        size_t pos = 0;
        std::vector<double> v;
        while ((pos = line.find("  ")) != std::string::npos) {
            v.push_back(std::stod(line.substr(0, pos)));
            line.erase(0, pos + 2); // pos plus delimeter size
        }
        output.push_back(v);
    }
    file.close();
    return output;
}
