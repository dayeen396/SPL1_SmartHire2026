#include "FileHandler.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <dirent.h>      // macOS directory reading
#include <sys/stat.h>    // macOS file stats

FileHandler::FileHandler() {}

string FileHandler::trim(string str) {
    size_t first = str.find_first_not_of(" \t\n\r");
    if (first == string::npos) return "";
    size_t last = str.find_last_not_of(" \t\n\r");
    return str.substr(first, last - first + 1);
}

string FileHandler::getFileExtension(string filename) {
    size_t pos = filename.find_last_of(".");
    if (pos != string::npos) {
        string ext = filename.substr(pos + 1);
        transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
        return ext;
    }
    return "";
}

bool FileHandler::isValidCVFile(string filename) {
    string ext = getFileExtension(filename);
    return (ext == "txt" || ext == "pdf");
}


vector<string> FileHandler::getCVFilesFromDirectory(string directoryPath) {
    vector<string> cvFiles;
    
    DIR* dir = opendir(directoryPath.c_str());
    if (dir == nullptr) {
        cerr << "Error: Cannot open directory: " << directoryPath << endl;
        return cvFiles;
    }
    
    struct dirent* entry;
    while ((entry = readdir(dir)) != nullptr) {
        string filename = entry->d_name;
        
        // Skip . and ..
        if (filename == "." || filename == "..") continue;
        
        // Skip hidden files (starting with .)
        if (filename[0] == '.') continue;
        
        string fullPath = directoryPath + "/" + filename;
        
        // Check if it's a file (not directory)
        struct stat statbuf;
        if (stat(fullPath.c_str(), &statbuf) == 0) {
            if (S_ISREG(statbuf.st_mode)) {  // Regular file
                if (isValidCVFile(filename)) {
                    cvFiles.push_back(fullPath);
                }
            }
        }
    }
    
    closedir(dir);
    
    cout << "\n[FILE SCANNER] Found " << cvFiles.size() 
         << " CV files (TXT/PDF) in " << directoryPath << endl;
    
    return cvFiles;
}

JobRequirement FileHandler::loadJobRequirements(string filename) {
    JobRequirement jobReq;
    
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Could not open job requirements file: " 
             << filename << endl;
        return jobReq;
    }
    
    string line;
    while (getline(file, line)) {
        if (line.empty() || line[0] == '#') continue;
        
        size_t colonPos = line.find(":");
        if (colonPos == string::npos) continue;
        
        string key = trim(line.substr(0, colonPos));
        string value = trim(line.substr(colonPos + 1));
        
        transform(key.begin(), key.end(), key.begin(), ::tolower);
        
        if (key == "required_skills" || key == "skills") {
            stringstream ss(value);
            string skill;
            while (getline(ss, skill, ',')) {
                jobReq.addRequiredSkill(trim(skill));
            }
        } else if (key == "min_cgpa") {
            try {
                jobReq.setMinCGPA(stod(value));
            } catch (...) {}
        } else if (key == "min_experience") {
            try {
                jobReq.setMinExperience(stoi(value));
            } catch (...) {}
        } else if (key == "max_experience") {
            try {
                jobReq.setMaxExperience(stoi(value));
            } catch (...) {}
        } else if (key == "degree") {
            jobReq.addAcceptableDegree(value);
        }
    }
    
    file.close();
    cout << "[CONFIG] Job requirements loaded from: " << filename << endl;
    
    return jobReq;
}

void FileHandler::saveTopCandidates(const vector<Applicant>& candidates,
                                    string filename) {
    ofstream file(filename);
    
    if (!file.is_open()) {
        cerr << "Error: Could not create file: " << filename << endl;
        return;
    }
    
    file << "======================================\n";
    file << "   TOP CANDIDATES REPORT\n";
    file << "======================================\n\n";
    
    for (size_t i = 0; i < candidates.size(); i++) {
        file << "Rank #" << (i + 1) << "\n";
        file << "Name: " << candidates[i].getName() << "\n";
        file << "Email: " << candidates[i].getEmail() << "\n";
        file << "Score: " << fixed << setprecision(2) 
             << candidates[i].getTotalScore() << "/100\n";
        file << "CGPA: " << candidates[i].getCGPA() << "\n";
        file << "Experience: " << candidates[i].getExperienceYears() << " years\n";
        file << string(40, '-') << "\n\n";
    }
    
    file.close();
    cout << "[REPORT] Top candidates saved to: " << filename << endl;
}

void FileHandler::exportToCSV(const vector<Applicant>& applicants,
                              string filename) {
    ofstream file(filename);
    
    if (!file.is_open()) {
        cerr << "Error: Could not create CSV file: " << filename << endl;
        return;
    }
    
    file << "Rank,Name,Email,Total Score,CGPA,Experience (Years)\n";
    
    for (size_t i = 0; i < applicants.size(); i++) {
        file << (i + 1) << ",";
        file << applicants[i].getName() << ",";
        file << applicants[i].getEmail() << ",";
        file << fixed << setprecision(2) << applicants[i].getTotalScore() << ",";
        file << applicants[i].getCGPA() << ",";
        file << applicants[i].getExperienceYears() << "\n";
    }
    
    file.close();
    cout << "[EXPORT] Data exported to CSV: " << filename << endl;
}

bool FileHandler::fileExists(string filename) {
    ifstream file(filename);
    return file.good();
}

bool FileHandler::createDirectory(string path) {
    return mkdir(path.c_str(), 0777) == 0 || errno == EEXIST;
}