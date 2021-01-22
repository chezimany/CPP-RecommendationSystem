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


    std::cout << "Movie: Twilight Name: Sofia k: 1"<< std::endl;
    std::cout << rs.predictMovieScoreForUser("Twilight", "Sofia", 1) << std::endl;
    std::cout << std::endl;
    std::cout << "Movie: Batman Name: Sofia k: 1"<< std::endl;
    std::cout << rs.predictMovieScoreForUser("Batman", "Sofia", 1) << std::endl;
    std::cout << std::endl;
    std::cout << "Movie: StarWars Name: Sofia k: 1"<< std::endl;
    std::cout << rs.predictMovieScoreForUser("StarWars", "Sofia", 1) << std::endl;
    std::cout << std::endl;
    std::cout << "Movie: Titanic Name: Michael k: 1"<< std::endl;
    std::cout << rs.predictMovieScoreForUser("Titanic", "Michael", 1) << std::endl;
    std::cout << std::endl;
    std::cout << "Movie: Titanic Name: Michael k: 2"<< std::endl;
    std::cout << rs.predictMovieScoreForUser("Titanic", "Michael", 2) << std::endl;
    std::cout << std::endl;
    std::cout << "Movie: Batman Name: Michael k: 1"<< std::endl;
    std::cout << rs.predictMovieScoreForUser("Batman", "Michael", 1) << std::endl;
    std::cout << std::endl;
    std::cout << "Movie: Batman Name: Michael k: 2"<< std::endl;
    std::cout << rs.predictMovieScoreForUser("Batman", "Michael", 2) << std::endl;
    std::cout << std::endl;
    std::cout << "Movie: Titanic Name: Nicole k: 1"<< std::endl;
    std::cout << rs.predictMovieScoreForUser("Titanic", "Nicole", 1) << std::endl;
    std::cout << std::endl;
    std::cout << "Movie: Titanic Name: Nicole k: 2"<< std::endl;
    std::cout << rs.predictMovieScoreForUser("Titanic", "Nicole", 2) << std::endl;
    std::cout << std::endl;
    std::cout << "Movie: Twilight Name: Nicole k: 1"<< std::endl;
    std::cout << rs.predictMovieScoreForUser("Twilight", "Nicole", 1) << std::endl;
    std::cout << std::endl;
    std::cout << "Movie: Twilight Name: Nicole k: 2"<< std::endl;
    std::cout << rs.predictMovieScoreForUser("Twilight", "Nicole", 2) << std::endl;
    std::cout << std::endl;
    std::cout << "Movie: Titanic Name: Arik k: 1"<< std::endl;
    std::cout << rs.predictMovieScoreForUser("Titanic", "Arik", 1) << std::endl;
    std::cout << std::endl;
    std::cout << "Movie: ForestGump Name: Arik k: 1"<< std::endl;
    std::cout << rs.predictMovieScoreForUser("ForestGump", "Arik", 1) << std::endl;
    std::cout << std::endl;
    std::cout << "Movie: StarWars Name: Arik k: 1"<< std::endl;
    std::cout << rs.predictMovieScoreForUser("StarWars", "Arik", 1) << std::endl;
    std::cout << std::endl;


}