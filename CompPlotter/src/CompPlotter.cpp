#include <iostream>
#include <vector>

#include "../include/Surfdata.h"

int main()
{
    std::string filepath = "data/forces820000.dat";
    Surfdata s(filepath);


    for (size_t i = 0; i < 10; i++)
    {
        std::cout << s.points[i] << std::endl;
    }
    


}
