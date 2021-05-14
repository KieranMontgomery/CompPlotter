#include "ParseData.h"

ParseData::ParseData(const std::string& dirPath, bool hasJet)  : m_dirPath(dirPath), m_hasJet(hasJet)
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
    //m_data.reserve(m_files.size()); 
    // TODO Can we reserve space for map to improve?
    for (size_t i = 0; i < m_files.size(); i++)
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
        CoefficientData s(temp.CD);
        m_data[step] = s;
    }

    #if debug
    std::cout << "[fillData] Sorted " << m_data.size() << " files." << std::endl;
    #endif
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

    std::vector<int> timeSteps;
    timeSteps.reserve(m_data.size());
    // Fill vector with found time-steps
    for (const auto& [key, value] : m_data){
        timeSteps.push_back(key);
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
        }
        else if (line.find("CT") != std::string::npos){
            size_t stepIndex = 0;
            for ( ; stepIndex < line.length(); stepIndex++ ){ if ( isdigit(line[stepIndex]) ) break; }
            line = line.substr(stepIndex, line.length() - stepIndex );
            CT = std::atof(line.c_str());
        }

        std::vector<int>::iterator index = std::find(timeSteps.begin(), timeSteps.end(), step);
        if (index != timeSteps.end()){
            timeSteps.erase(index); // Remove from timesteps
            m_data[step].CTExit = CT;
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

    for (const auto& [key, value] : m_data){
        myfile << key << "\t" << value.CD;
        if (m_hasJet) myfile << "\t" << value.CTExit << "\t" << value.CTThroat;
        myfile << "\n";
    }
    myfile.close();
}
