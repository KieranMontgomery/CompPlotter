#include "../include/Surfdata.h"
#include "../include/DataReader.h"

Surfdata::Surfdata(std::string filePath)
{
    auto data = extractDataFromFile(filePath);
    for (size_t i = 0; i < data.size(); i++)
    {
        points.push_back(SurfacePoint(data[i]));
    }
}
