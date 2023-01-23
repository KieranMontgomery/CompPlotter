#pragma once

#include <iostream>
#include <vector>
#include <filesystem>
#include <fstream>
#include <map>
#include <thread>
#include <string>

#include <iterator>

#include "Surfdata.h"

struct CoefficientData
{
    double CD;
    double CTExit;
    double CTThroat;

    CoefficientData() { std::cout << "Copied\n"; };
    CoefficientData(double CD) : CD(CD){};
};
class ParseData
{
public:
    ParseData(const std::string &dirPath, const std::string &outputName, bool hasJet, int numThreads = 1);

private:
    bool m_hasJet;
    int m_numThreads;
    std::string m_dirPath;
    std::string m_screenPath;
    std::string m_outputName;
    std::vector<std::string> m_files;
    std::vector<std::map<int, CoefficientData>> m_data;

    void getFilesInDir();
    void fillData();
    void threadDataWorker(size_t threadIndex);
    void fillJetData();
    void writeData();
};