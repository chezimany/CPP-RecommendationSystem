//
// Created by ciman on 6/19/2020.
//

#ifndef EX5_RECOMMENDERSYSTEM_H
#define EX5_RECOMMENDERSYSTEM_H
#include <string>
#include <iostream>
#include <fstream>
#include <iterator>
#include <map>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <set>
#include <bits/stdc++.h>

using std::string;
using std::map;
using std::set;
using std::vector;
using std::unordered_map;
using std::unordered_set;
using std::priority_queue;
/**
 * a system that recommends movies to users using 2 main algorithms.
 */
class RecommenderSystem
{

private:

    /**
     * a map to contain the movies.
     */
    unordered_map<string, vector<int>> movies;

    /**
     * a map to contain the users;
     */
    unordered_map<string , unordered_map<string, int>> users;

    /**
     * a list of the movies by the order of the ranks file;
     */
    vector<string> moviesBy2ndFile;

    /**
     * a map of all the norms.
     */
    unordered_map<string, double> norms;

     /**
      * a function used to split a string by spaces.
      * @param line - line to split.
      * @return - a vector containing the splitted line.
      */
    static vector<string> _splitLineBySpace(const string& line);

    /**
     * convert a given vector to an integer vector.
     * @param vector - vector to convert.
     * @return - converted vector.
     */
    static vector<int> _convertToInts(const vector<string> &vector);

    /**
     * get the average of a users ratings map.
     * @param map - map of the users ratings.
     * @return - average of all ratings.
     */
    static double _getAverage(const unordered_map<string, int> &map);

    /**
     * creates the normalized vector for the recommending algorithms.
     * @param average - average of all movie scores;
     * @return - normalized vector.
     */
    static map<string, double> _getNormalizedVector(const unordered_map<string, int> &map, double average) ;

    /**
     * multiply a vector by a scalar.
     * @param scalar - scalar to multiply.
     * @param vector - vector to multiply.
     * @return - result of multiplication.
     */
    static vector<double> _multiplyByScalar(double scalar, const vector<int> &vector) ;

    /**
     * multiply 2 vectors.
     * @param vector1 - 1st to multiply.
     * @param vector2 - 2nd to multiply.
     * @return - result of the multiplication.
     */
    static double _multiplyVectors(const vector<int> &vector1, const vector<int> &vector2) ;

    /**
     * multiply 2 vectors.
     * @param vector1 - 1st to multiply.
     * @param vector2 - 2nd to multiply.
     * @return - result of the multiplication.
     */
    static double _multiplyVectors(const vector<double> &vector1, const vector<double> &vector2) ;

    /**
     * multiply 2 vectors.
     * @param vector1 - 1st to multiply.
     * @param vector2 - 2nd to multiply.
     * @return - result of the multiplication.
     */
    static double _multiplyVectors(const vector<double> &vector1, const vector<int> &vector2) ;

    /**
     * get the preference vector.
     * @param normalizedVector - scores vector after normalization.
     * @param movies - map of all the movies.
     * @return - preference vector.
     */
    static vector<double> _getPreferenceVector(const map<string, double> &normalizedVector, const unordered_map<string, vector<int>> &givenMovies) ;

    /**
     * get the standard norm of a vector.
     * @param vector - vector to calculate it's norm.
     * @return - result of the calculation.
     */
    static double _getNorm(const vector<double> &vector) ;

    /**
     * get the standard norm of a vector.
     * @param vector - vector to calculate it's norm.
     * @return - result of the calculation.
     */
    static double _getNorm(const vector<int> &vector) ;

    /**
     * get the norm of a given movie's vector.
     * @param movie - na,e of the movie to get it's norm.
     * @return - the norm.
     */
    double _getNorm(const string &movie) const;

    /**
     * get the angle between 2 vectors
     * @param vector1 - 1st vector.
     * @param vector2 - 2nd vector.
     * @return
     */
    double _getAngle(const vector<double> &vector1, const string &vector2) const ;

    /**
     * get the angle between 2 vectors
     * @param vector1 - 1st vector.
     * @param vector2 - 2nd vector.
     * @return - the angle
     */
    double _getAngle(const string &vector1, const string &vector2) const ;

    /**
     * get the k most similar movies to a given movie.
     * @param movie - movie to find most similar movies to.
     * @param unseenMovies - set of movies to find which of them are the most similar.
     * @param k - number of the most similar movies to look for.
     * @return - a map between the movies names, and their similarity.
     */
     map<string, double> _getMostSimilar(const string &movie, const unordered_set<string> &seenMovies, int k) const;

     /**
      * make sum 2 vectors;
      * @param toReturn - reference to vector to add to.
      * @param toAdd - reference to vector to be added.
      * @return - reference to the 1st vector.
      */
     static vector<double> &_addToVector(vector<double> &toReturn, const vector<double> &toAdd);

public:

    /**
     * recieves paths to input files and loads them into the system.
     * @param moviesAttributesFilePath - path to the attributes file.
     * @param userRanksFilePath - path to the user ranks file.
     * @return -0 upon success, 1 upon failure.
     */
    int loadData(const string &moviesAttributesFilePath, const string &userRanksFilePath);

    /**
     * recommend a movie by the content recommendation algorithm.
     * @param userName - name of the user to recommend a movie to.
     * @return - name of the movie recommended.
     */
    string recommendByContent(const string &userName) const;

    /**
     * predicts a given movie's score by a given user.
     * @param movieName - movie to be scored.
     * @param userName - user which score is to be predicted.
     * @param k - number of the most similar movies for the prediction algorithm.
     * @return - the predicted score, -1 if the user name or movie name doesn't exist.
     */
    double predictMovieScoreForUser(const string &movieName, const string &userName, int k) const ;

    /**
     *recommend a movie by the shared filter algorithm.
     * @param userName - a user to recommend a movie to.
     * @param k - the parameter for the algorithm.
     * @return - the name of the recommended movie.
     */
    string recommendByCF(const string &userName, int k) const;

};
#endif //EX5_RECOMMENDERSYSTEM_H
