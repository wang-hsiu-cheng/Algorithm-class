#include <iostream>
#include <sstream>
#include <iomanip>
#include <string>
#include <fstream>

using namespace std;

int main()
{
    unsigned char x;
    std::ifstream input("roster.dat", std::ios::binary);
    input >> std::noskipws;
    while (input >> x) {
        std::cout << std::hex << std::setw(2) << std::setfill('0')
                  << (int)x;
    }
    return 0;
}