#include "Screen.h"

#include <fstream>

Screen::Screen(const std::string& filePath) 
{
    std::ifstream file(filePath);
    if (!file.is_open()){
        std::cout << "Unable to open file" << std::endl; 
        throw;
    }

    
}
