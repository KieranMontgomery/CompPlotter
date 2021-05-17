#include "ParseData.h"

ParseData::ParseData(const std::string& dirPath, bool hasJet, int numThreads)  : m_dirPath(dirPath), m_hasJet(hasJet), m_numThreads(numThreads)
{
    getFilesInDir();
    fillData();
    if (m_hasJet) fillJetData();
    writeData();
}

void ParseData::getFilesInDir() 
{
    #if debug
    std::cout << "Getting file names in directory" << std::endl;
    #endif
    // Get all files names in directory.
    for (const auto & entry : std::filesystem::directory_iterator(m_dirPath))
    {
        std::string file = entry.path().string(); // Name of file as a string
        if (file.find(".dat") != std::string::npos) m_files.push_back(file); // If file is a data file, add it to vector
        else if (file.find("screen") != std::string::npos) m_screenPath = file;
    }
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
        th[thIx] = std::thread([this, thIx] { this->threadDataWorker(thIx); });
    }

    for (int thIx = 0; thIx < m_numThreads; thIx++) {
            th[thIx].join();
        }
    #if debug
    std::cout << "[fillData] Sorted " << m_data.size() << " files." << std::endl;
    #endif
}

void ParseData::threadDataWorker(size_t threadIndex) 
{
    std::map<int, CoefficientData> tempMap;
    for (size_t i = threadIndex; i < m_files.size(); i+=m_numThreads)
    {
        // Create temporary surfdata object
        Surfdata temp(m_files[i], m_hasJet);

        // Find timestep from file name
        std::string stepAsString = m_files[i].substr(m_files[i].find(".dat") - 6, 6);
        size_t stepIndex = 0;
        for ( ; stepIndex < stepAsString.length(); stepIndex++ ){ if ( isdigit(stepAsString[stepIndex]) ) break; }
        stepAsString = stepAsString.substr(stepIndex, stepAsString.length() - stepIndex );
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
    if (!file.is_open()){
        std::cout << "Unable to open file" << std::endl; throw;
    }


    std::string line;
    double CT;
    int step;
    while(getline(file, line))
    {
        if (line.find("step") != std::string::npos){
            size_t stepIndex = 0;
            for ( ; stepIndex < line.length(); stepIndex++ ){ if ( isdigit(line[stepIndex]) ) break; }
            line = line.substr(stepIndex, line.length() - stepIndex );
            step = std::atoi(line.c_str());
           
            //m_data[step].CTExit = CT;

            for (auto& data: m_data) {
                auto search = data.find(step);
                if (search != data.end()) {
                    search->second.CTExit = CT;
                    break;
                }
            }                  
        }
        else if (line.find("CT") != std::string::npos){
            size_t stepIndex = 0;
            for ( ; stepIndex < line.length(); stepIndex++ ){ if ( isdigit(line[stepIndex]) ) break; }
            line = line.substr(stepIndex, line.length() - stepIndex );
            CT = std::atof(line.c_str());
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
    myfile.open ("test.txt");

    // TODO sort data before writing. May be able to write data in order without sorting.
    for (auto& data: m_data) {
        for (const auto& [key, value] : data){
            myfile << key << "\t" << value.CD;
            if (m_hasJet) myfile << "\t" << value.CTExit << "\t" << value.CTThroat;
            myfile << "\n";
        }
    }
    myfile.close();
}
