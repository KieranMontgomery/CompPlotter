#pragma once

#include <iostream>
#include <vector>
#include <filesystem>
#include <fstream>
#include <map>

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
        ParseData(const std::string& dirPath, bool hasJet);

    private:
        // Variables
        bool m_hasJet;
        std::string m_dirPath;
        std::string m_screenPath;
        std::vector<std::string> m_files;
        std::map<int, CoefficientData> m_data;

        // Functions
        void getFilesInDir();
        void fillData();
        void fillJetData();
        void writeData();
};