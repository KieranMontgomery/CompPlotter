#pragma once

#include <iostream>
#include <vector>
#include <filesystem>
#include <fstream>
#include <map>
#include <thread>

#include "Surfdata.h"

struct CoefficientData{
    double CD;
    double CTExit;
    double CTThroat;

    CoefficientData(){std::cout << "Copied\n"; }; // TODO Why do we need a default constructor? Can we remove? Does not compile without, probably to do with map.
    CoefficientData(double CD) : CD(CD) {};
};

class ParseData{
    public:
        // Variables

        // Functions
        ParseData(const std::string& dirPath, bool hasJet, int numThreads = 1);

    private:
        // Variables
        bool m_hasJet;
        int m_numThreads;
        std::string m_dirPath;
        std::string m_screenPath;
        std::vector<std::string> m_files;
        std::vector<std::map<int, CoefficientData>> m_data;

        // Functions
        void getFilesInDir();
        void fillData();
        void threadDataWorker(size_t threadIndex); // worker for thread

        void fillJetData();
        void writeData();
};