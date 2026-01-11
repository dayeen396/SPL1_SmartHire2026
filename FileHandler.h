#ifndef FILE_HANDLER_H
#define FILE_HANDLER_H
#include "Applicant.h"
#include "JobRequirement.h"
#include <string>
#include <vector>
using namespace std;

class FileHandler{
    private:
    string trim(string str);
    public:
    FileHandler();
    //read cv files from the directory

    vector<string> getCVFilesFromDirectory(string directoryPath);
    bool isValidCVFile(string filename);
    string getFileExtension(string filename);

    JobRequirement loadJobRequirements(string filename);

    void saveTopCandidates(const vector<Applicant>& candidates,string filename);
    void exportToCSV(const vector<Applicant>& applicants,string filename);

    bool fileExists(string filename);
    bool createDirectory(string path);



};
#endif