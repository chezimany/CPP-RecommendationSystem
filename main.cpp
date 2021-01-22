#include <iostream>
#include "RecommenderSystem.h"

int main(int argc, char** argv) {
    RecommenderSystem rec;
    rec.loadData(argv[1], argv[2]);
//    double s  = rec.predictMovieScoreForUser("AnatomyofaMurder", "Princeton", 2);
//    double k = rec.predictMovieScoreForUser("EyesWideShut", "Princeton", 2);
    string s = rec.recommendByCF("Princeton", 2);
    std::cout << s << std::endl;
    return 0;
}
