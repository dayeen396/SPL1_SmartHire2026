#ifndef CVPARSER_H
#define CVPARSER_H
#include "Applicant.h"
#include <string>
#include <vector>
using namespace std;
class CVParser{
    private:
        string toLowerCase(string str);
        string trim(string str);
        vector<string> splitString(string str,char delimiter);
        bool containsKeyword(string line,vector<string> keywords);
        string convertPDFToText(string pdfFile);
        string getFileExtension(string filename);
        bool isPDFFile(string filename);
        vector<string> textToLines(string textContent);

    public:
        CVParser();
        Applicant parseCV(string filename);

        vector<Applicant> parseCVDirectory(string directoryPath);
        string extractName(const vector<string>& lines);
        string extractEmail(const vector<string>& lines);
        string extractPhone(const vector<string>& lines);
        double extractCGPA(const vector<string>& lines);
        int extractExperience(const vector<string>& lines);
        string extractDegree(const vector<string>& lines);
        string extractUniversity(const vector<string>& lines);
        int extractGraduationYear(const vector<string>& lines);
        vector<string> extractSkills(const vector<string>& lines); 
        vector<Project> extractProjects(const vector<string>& lines);
        vector<string> extractCertifications(const vector<string>& lines);
        vector<string> extractLanguages(const vector<string>& lines);
};
#endif


