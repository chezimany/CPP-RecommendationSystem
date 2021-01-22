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


    std::cout << "Movie: Fargo Name: Christopher k: 1"<< std::endl;
    std::cout << rs.predictMovieScoreForUser("Fargo", "Christopher", 1) << std::endl;
    std::cout << std::endl;
    std::cout << "Movie: Fargo Name: Christopher k: 2"<< std::endl;
    std::cout << rs.predictMovieScoreForUser("Fargo", "Christopher", 2) << std::endl;
    std::cout << std::endl;
    std::cout << "Movie: ArabianNights Name: Christopher k: 1"<< std::endl;
    std::cout << rs.predictMovieScoreForUser("ArabianNights", "Christopher", 1) << std::endl;
    std::cout << std::endl;
    std::cout << "Movie: ArabianNights Name: Christopher k: 2"<< std::endl;
    std::cout << rs.predictMovieScoreForUser("ArabianNights", "Christopher", 2) << std::endl;
    std::cout << std::endl;
    std::cout << "Movie: ArabianNights Name: London k: 1"<< std::endl;
    std::cout << rs.predictMovieScoreForUser("ArabianNights", "London", 1) << std::endl;
    std::cout << std::endl;
    std::cout << "Movie: Shadows Name: London k: 1"<< std::endl;
    std::cout << rs.predictMovieScoreForUser("Shadows", "London", 1) << std::endl;
    std::cout << std::endl;
    std::cout << "Movie: AAmericaninParisn Name: London k: 1"<< std::endl;
    std::cout << rs.predictMovieScoreForUser("AAmericaninParisn", "London", 1) << std::endl;
    std::cout << std::endl;
    std::cout << "Movie: ArabianNights Name: Randy k: 1"<< std::endl;
    std::cout << rs.predictMovieScoreForUser("ArabianNights", "Randy", 1) << std::endl;
    std::cout << std::endl;
    std::cout << "Movie: Shadows Name: Randy k: 1"<< std::endl;
    std::cout << rs.predictMovieScoreForUser("Shadows", "Randy", 1) << std::endl;
    std::cout << std::endl;
    std::cout << "Movie: AAmericaninParisn Name: Randy k: 1"<< std::endl;
    std::cout << rs.predictMovieScoreForUser("AAmericaninParisn", "Randy", 1) << std::endl;
    std::cout << std::endl;
    std::cout << "Movie: ArabianNights Name: Brayson k: 1"<< std::endl;
    std::cout << rs.predictMovieScoreForUser("ArabianNights", "Brayson", 1) << std::endl;
    std::cout << std::endl;
    std::cout << "Movie: ArabianNights Name: Brayson k: 2"<< std::endl;
    std::cout << rs.predictMovieScoreForUser("ArabianNights", "Brayson", 2) << std::endl;
    std::cout << std::endl;
    std::cout << "Movie: Shadows Name: Brayson k: 1"<< std::endl;
    std::cout << rs.predictMovieScoreForUser("Shadows", "Brayson", 1) << std::endl;
    std::cout << std::endl;
    std::cout << "Movie: Shadows Name: Brayson k: 2"<< std::endl;
    std::cout << rs.predictMovieScoreForUser("Shadows", "Brayson", 2) << std::endl;
    std::cout << std::endl;


}