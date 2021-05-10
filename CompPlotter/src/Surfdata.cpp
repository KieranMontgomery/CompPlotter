#include "../include/Surfdata.h"
#include "../include/DataReader.h"

std::ostream& operator<<(std::ostream& os, const SurfacePoint& sp)
{
	std::copy(sp.data.begin(), sp.data.end(), std::ostream_iterator<double>(os, "\t"));
    return os;
}

Surfdata::Surfdata(std::string filePath)
{
    auto data = extractDataFromFile(filePath);
    for (size_t i = 0; i < data.size(); i++)
    {
        points.push_back(SurfacePoint(data[i]));
    }
}
