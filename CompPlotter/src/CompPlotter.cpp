#include "ParseData.h"

// COMPILE SCRIPT - clang++ -std=c++17 -o a.exe -I include src/*.cpp -O3 -target x86_64-pc-windows-msvc
// g++-10 src/*.cpp -std=c++17 -o a.out -O3 -I include -pthread

int main()
{
    ParseData("/media/Uni/workstation/dirforces", "LongTest", false, 4);
}