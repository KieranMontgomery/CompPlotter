#include "ParseData.h"

#include <iterator>
ParseData::ParseData(const std::string &dirPath, const std::string &outputName, bool hasJet, int numThreads) : m_dirPath(dirPath), m_hasJet(hasJet), m_numThreads(numThreads), m_outputName(outputName)
{
#if debug
    std::cout << "dirPath = " << m_dirPath << std::endl;
    std::cout << "outputName = " << m_outputName << std::endl;
    std::cout << "hasJet = " << m_hasJet << std::endl;
    std::cout << "numOfThreads = " << m_numThreads << std::endl;
#endif

    getFilesInDir();
    fillData();
    if (m_hasJet)
        fillJetData();
    writeData();

    std::cout << "Finished " << outputName << "." << std::endl;
}

void ParseData::getFilesInDir()
{
#if debug
    std::cout << "Getting file names in directory" << std::endl;
#endif
    bool foundScreen = false;
// Get all files names in directory.
#if debug
    std::cout << "Path:\t" << m_dirPath << std::endl;
#endif
    for (const auto &entry : std::filesystem::directory_iterator(m_dirPath))
    {
#if debug
        std::cout << "\t\tFile:\t" << entry.path().string() << std::endl;
#endif
        std::string file = entry.path().string(); // Name of file as a string
        if (file.find(".dat") != std::string::npos)
            m_files.push_back(file); // If file is a data file, add it to vector
        else if (file.find("screen") != std::string::npos)
        {
            m_screenPath = file;
            foundScreen = true;
        }
    }
    if (!foundScreen)
    {
        size_t posOfSlash = m_dirPath.find_last_of('/');
        m_screenPath = m_dirPath.substr(0, posOfSlash + 1) + "screen";
    }

    std::cout << m_screenPath << std::endl;
}

void ParseData::fillData()
{
#if debug
    std::cout << "[fillData] Sorting data from .dat files" << std::endl;
#endif

    // Sort data from .dat files
    std::thread th[12];
    for (size_t thIx = 0; thIx < m_numThreads; thIx++)
    {
        th[thIx] = std::thread([this, thIx]
                               { this->threadDataWorker(thIx); });
    }

    for (int thIx = 0; thIx < m_numThreads; thIx++)
    {
        th[thIx].join();
    }
#if debug
    std::cout << "[fillData] Sorted " << m_data.size() << " files." << std::endl;
#endif
}

void ParseData::threadDataWorker(size_t threadIndex)
{
    std::map<int, CoefficientData> tempMap;
    int filesChunkSize = m_files.size() / m_numThreads;
    int start = threadIndex * filesChunkSize;
    int end = threadIndex == m_numThreads - 1 ? m_files.size() : (threadIndex + 1) * filesChunkSize;

#if debug
    std::cout << "Thread " << threadIndex << " doing files from " << start << " to " << end << ".   " << std::endl;
#endif

    for (size_t i = start; i < end; i++)
    {
#if debug
        int jobs = end - start;
        double percentage = (double)(i - start) / (double)jobs * 100.0;
        std::cout << "Thread " << threadIndex << ": " << percentage << "% Index: " << i << std::endl;
#endif
        // Create temporary surfdata object
        Surfdata temp(m_files[i], m_hasJet);

        // Find timestep from file name
        auto start = m_files[i].find("forces") + 13; // TODO: Make better please
        auto end = m_files[i].find(".dat");
        std::string stepAsString = m_files[i].substr(start, end - start);
        size_t stepIndex = 0;
        for (; stepIndex < stepAsString.length(); stepIndex++)
        {
            if (isdigit(stepAsString[stepIndex]))
                break;
        }
        stepAsString = stepAsString.substr(stepIndex, stepAsString.length() - stepIndex);
        int step = atoi(stepAsString.c_str());

        // Add data to
        tempMap.emplace(step, CoefficientData(temp.CD));
    }
    m_data.push_back(tempMap);
}

void ParseData::fillJetData()
{
#if debug
    std::cout << "Looking in screen file for CT values" << std::endl;
#endif

    std::ifstream file(m_screenPath);
    if (!file.is_open())
    {
        std::cout << "Unable to open screen file at " + m_screenPath << std::endl;
        throw;
    }

    std::string line;
    double CT;
    double CT_Throat;
    int step;
    while (getline(file, line))
    {
        if (line.find("step") != std::string::npos)
        {
            size_t stepIndex = 0;
            for (; stepIndex < line.length(); stepIndex++)
            {
                if (isdigit(line[stepIndex]))
                    break;
            }
            line = line.substr(stepIndex, line.length() - stepIndex);
            step = std::atoi(line.c_str());

            // m_data[step].CTExit = CT;

            for (auto &data : m_data)
            {
                auto search = data.find(step);
                if (search != data.end())
                {
                    search->second.CTExit = CT;
                    search->second.CTThroat = CT_Throat;
                    break;
                }
            }
        }
        else if (line.find("CT ") != std::string::npos)
        {
            size_t stepIndex = 0;
            for (; stepIndex < line.length(); stepIndex++)
            {
                if (isdigit(line[stepIndex]))
                    break;
            }
            line = line.substr(stepIndex, line.length() - stepIndex);
            CT = std::atof(line.c_str());
        }
        else if (line.find("CT_Throat") != std::string::npos)
        {
            size_t stepIndex = 0;
            for (; stepIndex < line.length(); stepIndex++)
            {
                if (isdigit(line[stepIndex]))
                    break;
            }
            line = line.substr(stepIndex, line.length() - stepIndex);
            CT_Throat = std::atof(line.c_str());
        }
    }

    file.close();
#if debug
    std::cout << "[fillJetData] file closed" << std::endl;
#endif
}

void ParseData::writeData()
{
#if debug
    std::cout << "Writing data to file" << std::endl;
#endif
    std::ofstream myfile;
    myfile.open(m_outputName + ".txt");

    // std::sort(m_data.begin(), m_data.end(), [](const std::map<int, CoefficientData>& a, const std::map<int, CoefficientData>& b){
    //     return a.begin()->first < b.begin()->first;
    // });

    std::map<int, CoefficientData> allData;

    for (auto &data : m_data)
    {
        allData.merge(data);
    }

    for (const auto &[key, value] : allData)
    {
        myfile << key << "\t" << value.CD;
        if (m_hasJet)
            myfile << "\t" << value.CTExit << "\t" << value.CTThroat;
        myfile << "\n";
    }
    myfile.close();
}
