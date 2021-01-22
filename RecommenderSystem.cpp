//
// Created by ciman on 6/19/2020.
//


#include "RecommenderSystem.h"
#include <sstream>
#include <cmath>

#define OPEN_ERR_MSG "Unable to open file "
#define NO_USR_MSG "USER NOT FOUND"

/**
 * a function used to split a string by spaces.
 * @param line - line to split.
 * @return - a vector containing the splitted line.
 */
vector<string> RecommenderSystem::_splitLineBySpace(const string& line)
{
    vector<string> words;
    std::istringstream strStream(line);
    for (string s; strStream >> s; )
    {
        words.push_back(s);
    }
    return words;
}


/**
 * recieves paths to input files and loads them into the system.
 * @param moviesAttributesFilePath - path to the attributes file.
 * @param userRanksFilePath - path to the user ranks file.
 * @return -0 upon success, 1 upon failure.
 */
int RecommenderSystem:: loadData(const string &moviesAttributesFilePath, const string &userRanksFilePath)
{

    std::fstream moviesStream;
    std::fstream usersStream;
    string moviesLine;
    string usersLine;
    moviesStream.open(moviesAttributesFilePath);

    if (!moviesStream.is_open())
    {
        std::cout << OPEN_ERR_MSG << moviesAttributesFilePath << std:: endl;
        return -1;
    }

    usersStream.open(userRanksFilePath);
    if (!usersStream.is_open())
    {
        moviesStream.close();
        std::cout << OPEN_ERR_MSG << userRanksFilePath << std:: endl;
        return -1;
    }

    while (std::getline(moviesStream, moviesLine))
    {
        vector<string> splitted = _splitLineBySpace(moviesLine);
        string movieName = splitted[0];
        vector<int> properties = _convertToInts(vector<string>(splitted.begin() + 1, splitted.end()));
        movies[movieName] = properties;
        norms[movieName] = _getNorm(properties);
    }

    std::getline(usersStream, usersLine);
    vector<string> fileMovies = _splitLineBySpace(usersLine);
    moviesBy2ndFile = fileMovies;
    while (std::getline(usersStream, usersLine))
    {
        vector<string> splitted = _splitLineBySpace(usersLine);
        string userName = splitted[0];
        vector<int> scoresVector = _convertToInts(vector<string>(splitted.begin() + 1, splitted.end()));
        unordered_map<string, int> scores;
        for (long unsigned int i = 0; i < fileMovies.size(); ++i)
        {
            scores[fileMovies.at(i)] = scoresVector.at(i);
        }
        users[userName] = scores;
    }
    moviesStream.close();
    usersStream.close();
    return 0;
}

/**
 * recommend a movie by the content recommendation algorithm.
 * @param userName - name of the user to recommend a movie to.
 * @return - name of the movie recommended.
 */
string RecommenderSystem::recommendByContent(const string &userName) const
{
    const bool isIn = users.find(userName) != users.end();
    if (!isIn)
    {
        return NO_USR_MSG;
    }
    std::priority_queue<std::pair<long double, string>> unseen;
    double average = _getAverage(users.at(userName));
    map<string, double> normalized = _getNormalizedVector(users.at(userName), average);
    vector<double> preferences = _getPreferenceVector(normalized, movies);
    for (auto & i : movies)
    {
        if (users.at(userName).at(i.first) == 0.0)
        {
            double angle = _getAngle(preferences, i.first);
            unseen.push(std::make_pair(angle, i.first));
        }
    }
    return unseen.top().second;
}

/**
 * predicts a given movie's score by a given user.
 * @param movieName - movie to be scored.
 * @param userName - user which score is to be predicted.
 * @param k - number of the most similar movies for the prediction algorithm.
 * @return - the predicted score, -1 if the user name or movie name doesn't exist.
 */
double RecommenderSystem::predictMovieScoreForUser(const string &movieName, const string &userName, int k) const
{
    bool containMovie = movies.find(movieName) != movies.end();
    bool containUser = users.find(userName) != users.end();
    if (!containMovie || !containUser)
    {
        return -1;
    }
    unordered_map<string, int> userRanks = users.at(userName);
    unordered_set<string> seenMovies(userRanks.size());
    for (auto &i : userRanks)
    {
        if (i.second == 0)
        {
            continue;
        }
        else
        {
            seenMovies.insert(i.first);
        }
    }
    map<string, double> mostSimilar = _getMostSimilar(movieName, seenMovies, k);
    double top = 0.0;
    double bottom = 0.0;
    for (auto &i : mostSimilar)
    {
        top += (i.second * (double) userRanks[i.first]);
        bottom += i.second;
    }

    return top / bottom;
}

/**
 * recommend a movie by the content recommendation algorithm.
 * @param userName - name of the user to recommend a movie to.
 * @return - name of the movie recommended.
 */
string RecommenderSystem::recommendByCF(const string &userName, int k) const
{
    std::priority_queue<std::pair<double, string>> predictedRankings;
    unordered_map<string, int> rankings = users.at(userName);
    for (const string &i : moviesBy2ndFile)
    {
        if (rankings[i] == 0)
        {
            double score = predictMovieScoreForUser(i, userName, k);
            if (score == -1)
            {
                return NO_USR_MSG;
            }
            if (!predictedRankings.empty() && score == predictedRankings.top().first)
            {
                continue;
            }
            predictedRankings.push(std::make_pair(score, i));
        }
    }
    return predictedRankings.top().second;
}

/**
 * convert a given vector to an integer vector.
 * @param vector - vector to convert.
 * @return - converted vector.
 */
vector<int> RecommenderSystem::_convertToInts(const vector<string> &vector)
{
    std::vector<int> toReturn(vector.size());
    for (long unsigned int i = 0; i < vector.size(); i++)
    {
        string str = vector[i];

        if (str == "NA")
        {
            toReturn[i] = 0;
        }
        else
        {
            toReturn[i] = stoi(str);
        }

    }
    return toReturn;
}

/**
 * get the average of a users ratings map.
 * @param map - map of the users ratings.
 * @return - average of all ratings.
 */
double RecommenderSystem::_getAverage(const unordered_map<string, int> &map)
{
    if (map.empty())
    {
        return 0;
    }
    double sum = 0.0;
    double counter = 0.0;
    for (const auto &i : map)
    {

        if (i.second != 0)
        {
            sum += (double) i.second;
            counter += 1.0;
        }
    }
    return sum / counter;
}

/**
 * creates the normalized vector for the recommending algorithms.
 * @param average - average of all movie scores;
 * @return - normalized vector.
 */
map<string, double> RecommenderSystem::_getNormalizedVector(const unordered_map<string, int> &map, double average)
{
    std::map<string, double> toReturn;
    for (const auto &i : map)
    {
        if (i.second > 0)
        {
            toReturn[i.first] = (double) i.second - average;
        }
        else
        {
            toReturn[i.first] = (double) i.second;
        }
    }
    return toReturn;
}

/**
 * multiply a vector by a scalar.
 * @param scalar - scalar to multiply.
 * @param vector - vector to multiply.
 * @return - result of multiplication.
 */
vector<double> RecommenderSystem::_multiplyByScalar(double scalar, const vector<int> &vector)
{
    std::vector<double> toReturn(vector.size());
    for (long unsigned int i = 0; i < vector.size(); i++)
    {
        toReturn[i] = (scalar * (double) vector.at(i));

    }
    return toReturn;
}
/**
 * multiply 2 vectors.
 * @param vector1 - 1st to multiply.
 * @param vector2 - 2nd to multiply.
 * @return - result of the multiplication.
 */
double RecommenderSystem::_multiplyVectors(const vector<int> &vector1, const vector<int> &vector2)
{
    double toReturn = 0.0;
    if (vector1.size() != vector2.size())
    {
        return toReturn;
    }
    for (long unsigned int i = 0; i < vector1.size(); ++i)
    {
        toReturn += ((double) vector1.at(i) * (double) vector2.at(i));

    }
    return toReturn;
}

/**
 * multiply 2 vectors.
 * @param vector1 - 1st to multiply.
 * @param vector2 - 2nd to multiply.
 * @return - result of the multiplication.
 */
double RecommenderSystem::_multiplyVectors(const vector<double> &vector1, const vector<double> &vector2)
{
    double toReturn = 0.0;
    if (vector1.size() != vector2.size())
    {
        return toReturn;
    }
    for (long unsigned int i = 0; i < vector1.size(); ++i)
    {
        toReturn += (vector1.at(i) * vector2.at(i));

    }
    return toReturn;
}

/**
 * multiply 2 vectors.
 * @param vector1 - 1st to multiply.
 * @param vector2 - 2nd to multiply.
 * @return - result of the multiplication.
 */
double RecommenderSystem::_multiplyVectors(const vector<double> &vector1, const vector<int> &vector2)
{
    double toReturn = 0.0;
    if (vector1.size() != vector2.size())
    {
        return toReturn;
    }
    for (long unsigned int i = 0; i < vector1.size(); ++i)
    {
        toReturn += (vector1.at(i) * (double) vector2.at(i));

    }
    return toReturn;
}

/**
 * get the preference vector.
 * @param normalizedVector - scores vector after normalization.
 * @param movies - map of all the movies.
 * @return - preference vector.
 */
vector<double> RecommenderSystem::_getPreferenceVector(const map<string, double> &normalizedVector, const unordered_map<string, vector<int>> &givenMovies)
{
    vector<double> toReturn(givenMovies.begin()->second.size(), 0.0);
    for (const auto & i : normalizedVector)
    {
        if (i.second != 0.0)
        {
            const vector<int>& toMultiply = givenMovies.at(i.first);
            vector<double> movieFeaturesByUser = _multiplyByScalar(i.second, toMultiply);
            _addToVector(toReturn, movieFeaturesByUser);
        }
    }
    return toReturn;
}

/**
* get the standard norm of a vector.
* @param vector - vector to calculate it's norm.
* @return - result of the calculation.
*/
double RecommenderSystem::_getNorm(const vector<double> &vector)
{
    return std::sqrt(_multiplyVectors(vector, vector));
}

/**
 * get the angle between 2 vectors
 * @param vector1 - 1st vector.
 * @param vector2 - 2nd vector.
 * @return
 */
double RecommenderSystem::_getAngle(const vector<double> &vector1, const string &vector2) const
{
    double top = _multiplyVectors(vector1, movies.at(vector2));
    double bottom = _getNorm(vector1) * _getNorm(vector2);
    if (bottom == 0)
    {
        return 0;
    }
    return top / bottom;
}

/**
 * get the standard norm of a vector.
 * @param vector - vector to calculate it's norm.
 * @return - result of the calculation.
 */
double RecommenderSystem::_getNorm(const vector<int> &vector)
{
    return std::sqrt(_multiplyVectors(vector, vector));
}

/**
 * get the k most similar movies to a given movie.
 * @param movie - movie to find most similar movies to.
 * @param unseenMovies - set of movies to find which of them are the most similar.
 * @param k - number of the most similar movies to look for.
 * @return - a map between the movies names, and their similarity.
 */
map<string, double> RecommenderSystem::_getMostSimilar(const string &movie, const unordered_set<string> &seenMovies, int k) const
{
    std::priority_queue<std::pair<double, string>> temp;
    map<string, double> toReturn;
    for (const string &i : seenMovies)
    {
        temp.push(std::make_pair(_getAngle(i, movie), i));
    }
    int i = 0;
    while (i < k)
    {
        toReturn[temp.top().second] = temp.top().first;
        temp.pop();
        i++;
    }
    return toReturn;
}

/**
 * get the angle between 2 vectors
 * @param vector1 - 1st vector.
 * @param vector2 - 2nd vector.
 * @return - the angle
 */
double RecommenderSystem::_getAngle(const string &vector1, const string &vector2) const
{
    double top = _multiplyVectors(movies.at(vector1), movies.at(vector2));
    double bottom = _getNorm(vector1) * _getNorm(vector2);
    if (bottom == 0)
    {
        return 0;
    }
    return top / bottom;
}

/**
 * get the norm of a given movie's vector.
 * @param movie - na,e of the movie to get it's norm.
 * @return - the norm.
 */
double RecommenderSystem::_getNorm(const string &movie) const
{
    return norms.at(movie);
}

/**
 * make sum 2 vectors;
 * @param toReturn - reference to vector to add to.
 * @param toAdd - reference to vector to be added.
 * @return - reference to the 1st vector.
 */
vector<double> &RecommenderSystem::_addToVector(vector<double> &toReturn, const vector<double> &toAdd)
{
    if (toReturn.size() != toAdd.size())
    {
        return toReturn;
    }
    for (long unsigned int i = 0; i < toReturn.size(); ++i)
    {
        toReturn[i] += toAdd[i];
    }
    return toReturn;

}











