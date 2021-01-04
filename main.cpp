#include <iostream>
#include "src/RadiPako.hpp"
#include <string>
#include <vector>

int main(int argc, char const *argv[])
{
    int skip = -1;
    int it = 0;
    std::string dest;
    std::vector<std::string> paths;
    if(argc > 2)
    {
        for(int i = 1; i < argc; i++)
        {
            if(argv[i][0] == '-')
            {
                switch (argv[i][1])
                {
                case 'o':
                    dest = argv[i+1];
                    skip = i+1;
                    break;
                //TODO: NEW CASES.
                default:
                    break;
                }
            }
            else
            {
                if(i != skip)
                {
                    paths.push_back(argv[i]);
                }
            }
            
        }
    }
    else
    {
        std::cout << "No arguments typed." << std::endl;
    }
    if(RadiPako::CreateFile(RadiPako::JointFiles(paths.size(), paths),dest.c_str()))
    {
        std::cout << dest << " has been created!" << std::endl;
    }
    else
    {
        std::cout << "Can't create this file!" << std::endl;
    }
    return 0;
}
