#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include "RecommenderSystem.h"

void getUsers(std::string path, std::vector<std::pair<std::string, int >> &vec)
{
    std::ifstream infile(path);
    std::string line;
    std::getline(infile, line);
    while (std::getline(infile, line))
    {
        std::istringstream iss(line);
        std::string name;
        std::string currentRank;
        iss >> name;
        int countRanks = 0;
        while (iss >> currentRank)
        {
            if (currentRank != "NA")
            {
                countRanks++;
            }
        }

        vec.emplace_back(name, countRanks);
    }
}

void getMovies(std::string path, std::vector<std::string> &vec)
{
    std::ifstream infile(path);
    std::string line;
    while (std::getline(infile, line))
    {
        std::istringstream iss(line);
        std::string name;
        iss >> name;
        vec.push_back(name);
    }
}


int main(int argc, char **argv)
{
    RecommenderSystem rs;
    int result = rs.loadData(argv[1], argv[2]);
    if (result == 1)
    {
        exit(1);
    }

    std::vector<std::string> moviesNames;
    std::vector<std::pair<std::string, int >> usersNames;

    getMovies(argv[1], moviesNames);
    getUsers(argv[2], usersNames);


    for (const auto &currentName : usersNames)
    {
        std::cout << "Name: " << currentName.first << std::endl;
        std::cout << rs.recommendByContent(currentName.first) << std::endl;
        std::cout << std::endl;
    }

}