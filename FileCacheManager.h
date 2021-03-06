
#ifndef EX2_FILECACHEMANAGER_H
#define EX2_FILECACHEMANAGER_H

#include "CacheManager.h"
#include <map>
#include <fstream>
#include <mutex>

#define DELIMITER  '$'

using  namespace std;

/*
 * thus class represent a manager that responsible on a file cache
 * its have the ability to check is some problem and her solution are found in the
 * file already and return the answer or to write a new problem and solution in the file
 */
template <class P, class S>
class FileCacheManager : public CacheManager<P,S>{
    map<string,string> pAndS;
    string fileName;
    mutable mutex m;

public:
    /*
     * constructor of fileCacheManager
     */
    FileCacheManager(string fileName){
        this->fileName = fileName;
        loadFromFile();
    }

    /*
     * this method check if some problem exist in the file
     * return true if the problem exists false otherwise
     */
    bool isProblemExist(P problem) override{
        unique_lock<mutex> ul(m);
        bool result = this->pAndS.find(problem) != pAndS.end();
        ul.unlock();
        return result;
    }

    /*
     * this method return from the file the solution as string
     */
    string getSolutionForProblem(P problem) override {
        if(isProblemExist(problem)){
            unique_lock<mutex> ul(m);
            string str = this->pAndS.at((string)problem);
            ul.unlock();
            return str;
        }
        perror("The solution of this problem not found");
        string s;
        return s;
    }

    /*
     * this save the problem and her solution to the file,if hey are ot exist
     */
    void saveProblemAndSolution(P problem, S solution) override{
        if (!isProblemExist(problem)){
            unique_lock<mutex> ul(m);
            this->pAndS.insert(make_pair(problem,solution));
            ul.unlock();
            saveToFile(problem, solution);
        }
    }


    /*
    * this method write to the file the problem and the solution with a separator $ between them
    */
    void saveToFile(P problem, S solution){
        ofstream outFile;
        outFile.open(this->fileName, ios::out | ios::app | ios::ate);
        if (!outFile.is_open()){
            perror("error opening file");
            exit(1);
        }
        unique_lock<mutex> ul(m);
        outFile << (string)problem << DELIMITER << (string)solution << endl;
        ul.unlock();
        outFile.close();
    }

    /*
     * this method load the information of the problem and the solution from the file to the map in the program
     */
    void loadFromFile(){
        ifstream inFile;
        inFile.open(this->fileName);

        //problem with opening the file
        if (inFile.bad()){
            perror("error opening file");
            exit(1);
        }

        //run over the lines in the file
        unique_lock<mutex> ul(m);
        for(string line; getline(inFile, line);){

            unsigned long splitIndex = line.find(DELIMITER);
            string problem = line.substr(0, splitIndex);
            string solution = line.substr(splitIndex+1, line.size() - problem.size() -1);
            this->pAndS.insert(make_pair(problem, solution));

        }
        ul.unlock();
        inFile.close();
    }

    ~FileCacheManager() override = default;


};


#endif //EX2_FILECACHEMANAGER_H
