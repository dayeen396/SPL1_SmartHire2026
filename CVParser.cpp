#include "CVParser.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <regex>
#include <cctype>
#include <cstdlib>
#include <ctime>
#include <unistd.h>

CVParser::CVParser() {}

string CVParser::toLowerCase(string str){
    transform(str.begin(),str.end(),str.begin(), ::tolower);
    return str;
}
string CVParser::trim(string str){
    size_t first = str.find_first_not_of(" \t\n\r");
    if(first == string::npos)
        return "";
    size_t last = str.find_last_not_of(" \t\n\r");
    return str.substr(first, (last - first + 1));
}
vector<string> CVParser::splitString(string str,char delimiter){
    vector<string> tokens;
    stringstream ss(str);
    string token;
    while (getline(ss,token,delimiter)){
        tokens.push_back(trim(token));
    }
    return tokens;

}
bool CVParser::containsKeyword(string line,vector<string> keywords){
    string lowerLine = toLowerCase(line);
    for(const auto& keyword : keywords){
        if(lowerLine.find(toLowerCase(keyword)) !=string::npos){
            return true;
        }
    }
    return false;
}
string CVParser::getFileExtension(string filename){
    size_t pos = filename.find_last_of(".");
    if(pos !=string::npos){
        string ext = filename.substr(pos+1);
        transform(ext.begin(),ext.end(),ext.begin(),::tolower);
        return ext;

    }
    return "";
}
bool CVParser::isPDFFile(string filename){
    return getFileExtension(filename) == "pdf";

}
string CVParser::convertPDFToText(string pdfFile){
    cout<< "\n[PDF CONVERTER] Starting conveersion......"<<endl;
    cout<<"[PDF CONVERTER] Input file: "<<pdfFile<<endl;
    time_t now = time(0);
    string tempFile = "temp_cv_" +to_string(now)+" .txt";

    string command = "pdftotext -layout -nopgbrk \""+pdfFile+"\" \""+tempFile+"\" 2>/dev/null";
    cout<<"[PDF CONVERTER] Executing command: "<<command<<endl;
    int result = system(command.c_str());
    if(result !=0){
        cerr <<"\n================================================"<<endl;
        cerr<<"ERROR: PDF conversion failed!"<<endl;
        cerr<<"================================================="<<endl;
        cerr<<"\nPossible reasons:"<<endl;
        cerr<<"1, pdftotext not found in PATH"<<endl;
        cerr<<"2. PDF file is corrupted or password-protected"<<endl;
        cerr<<"3. Poppler not installed"<<endl;

        cerr<<"================================================="<<endl;
        cerr<<"\n Solution for macOS: "<<endl;
        cerr<<"1.Install Poppler: brew install poppler"<<endl;
        cerr<<"2.Verify: pdftotext -v"<<endl;
        cerr<<"3. Check Path: which pdftotext"<<endl;
        cerr<<"================================================="<<endl;

        if(system("which pdftotext >/dev/null 2>&1") !=0){
            cerr<<"pdftotext not found in PATH. Please install Poppler and ensure pdftotext is accessible from command line."<<endl;
        }else{
            cout<<"[CHECK] pdftotext found in PATH"<<endl;
            cerr<<"[INFO] Please check if the PDF file is corrupted or password-protected."<<endl;
        }

        return "";
}
usleep(300000);

ifstream file(tempFile);
if(!file.is_open()){
    cerr<<"[ERROR] could not read temp file: "<<tempFile<<endl;
    cerr<<"[INFO] The conversion may have failed silently."<<endl;
    return "";
}

string content((istreambuf_iterator<char>(file)),istreambuf_iterator<char>());
file.close();
remove(tempFile.c_str());
cout<<"[PDF CONVERTER] Conversion successful. Temp file removed."<<endl;
if(content.empty()){
    cerr<<"[WARNING] Extracted text is EMPTY!"<<endl;
    cerr<<"[INFO] The PDF might be :"<<endl;
    cerr<<" -Image based (scanned document without OCR)"<<endl;
    cerr<<" -Encrypted or password-protected"<<endl;
    cerr<<" -Corrupted or malformed"<<endl;
    return "";
}
cout<<"[SUCCESS] PDF conversion completed!" <<endl;
cout<<"[INFO] Extracted text length: "<<content.length()<<" characters"<<endl;
return content;
}

vector<string> CVParser::textToLines(string textContent){
    vector<string> lines;
    stringstream ss(textContent);
    string line;
    while(getline(ss,line)){
        line = trim(line);
        if(!line.empty()){
            lines.push_back(line);
        
    }
    
}
    return lines;
}
Applicant CVParser::parseCV(string filename){
    Applicant applicant;

    cout <<"\n" << string(60,'=') <<endl;
    cout << "PROCESSING CV FILE:"<<endl;
    cout<< string(60,'=') <<endl;
    cout<<"[INFO] Filename: "<<filename<<endl;

    string ext = getFileExtension(filename);
    cout<<"Format: "<<ext<<" (";
    vector<string> lines;
    if(ext == "pdf"){
        cout<<"PDF - requires conversion)"<<endl;
        string textContent = convertPDFToText(filename);
        if(textContent.empty()){
            cerr<<"[ERROR] No text extracted from PDF. Skipping file."<<endl;
            cerr<< string(60,'=') <<endl;
            return applicant;
    }
        lines = textToLines(textContent);
}else if(ext=="txt"){
    cout<<"TXT-direct read)"<<endl;
    ifstream file(filename);
    if(!file.is_open()){
        cerr<<"[ERROR] could not open TXT file: "<<filename<<endl;
        return applicant;

    }
    string line;
    while(getline(file,line)){
        line = trim(line);
        if(!line.empty()){
            lines.push_back(line);
        }
    }
    file.close();
}else{
    cerr<<"UNSUPPORTED FILE FORMAT: "<<ext<<". Only PDF and TXT are supported."<<endl;
    cerr<<string(60,'=') <<endl;
    return applicant;  
}
if(lines.empty()){
    cerr<<"[WARNING] No text lines extracted from file. Skipping."<<endl;
    cerr<<string(60,'=') <<endl;
    return applicant;
}
cout<<"\n[EXTRACTIION] found "<<lines.size()<<" lines of text."<<endl;
cout<<"[EXTRACTION] Extracting applicant information..."<<endl;

applicant.setName(extractName(lines));
applicant.setEmail(extractEmail(lines));
applicant.setPhone(extractPhone(lines));
applicant.setCGPA(extractCGPA(lines));
applicant.setExperienceYears(extractExperience(lines));
applicant.setDegree(extractDegree(lines));
applicant.setUniversity(extractUniversity(lines));
applicant.setGraduationYear(extractGraduationYear(lines));
vector<string> skills = extractSkills(lines);
for(const auto& skill : skills){
    applicant.addSkill(skill);
}
vector<Project> projects = extractProjects(lines);
for(const auto& project : projects){
    applicant.addProject(project);
}
vector<string> certs = extractCertifications(lines);
for(const auto& cert : certs){
    applicant.addCertification(cert);
}

cout<<"\n[SUMMARY] Extracted Applicant Information:"<<endl;
cout<<" Name: "<<applicant.getName()<<endl;
cout<<" Email: "<<applicant.getEmail()<<endl;
cout<<" CGPA: "<<applicant.getCGPA()<<endl;
cout<<" Skils: "<<skills.size()<<" found"<<endl;
cout<< string(60,'=') <<endl;
    return applicant;
}

string CVParser::extractName(const vector<string>& lines){
    for(const auto& line : lines){
        if(line.find("Name:") !=string::npos || line.find("NAME:") !=string::npos){
            size_t pos = line.find(":");
            if(pos != string::npos){
                return trim(line.substr(pos+1));
            }
        }
    }
    if(!lines.empty() && !lines[0].empty()){
        return lines[0];
    }
    return "Unknown";
}

string CVParser::extractEmail(const vector<string>& lines){
    regex emailPattern(R"([a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,})");
    smatch match;
    for(const auto& line : lines){
        if(regex_search(line,match,emailPattern)){
            return match.str();

}
    }
    return "Not Found";
}   

string CVParser::extractPhone(const vector<string>& lines){
    regex phonePattern(R"((\+?\d{1,3}[-.\s]?)?(\(?\d{3}\)?[-.\s]?)?\d{3}[-.\s]?\d{4})");
    smatch match;
    for(const auto& line : lines){
        if(regex_search(line,match,phonePattern)){
            return match.str();
        }
    }
    return "Not Found";

}
double CVParser::extractCGPA(const vector<string>& lines){
    regex cgpaPattern(R"((CGPA|GPA)[:\s]*([0-4]\.\d{1,2}|[0-9]\.\d{1,2}))",regex::icase);
    smatch match;
    for(const auto& line : lines){
       if(regex_search(line,match,cgpaPattern)){
            string cgpaStr = match[1].str().empty()? match[2].str() : match[1].str();
            try{
                return stod(cgpaStr);
            
        }catch(const invalid_argument&){
            continue;
        }
       } 
    }
    return 0.0;
}
string CVParser::extractDegree(const vector<string>& lines){
    vector<string> degreeKeywords = {"BSC","B.SC","Bachelor","MSc","M.SC","Master","PhD","Ph.D","Doctorate"};
    for(const auto& line:lines){
        for(const auto& keyword : degreeKeywords){
            if(line.find(keyword) !=string::npos){
                return trim(line);
            }
        }
    }
    return "Not specified";
}
string CVParser::extractUniversity(vector<string>const& lines){
    vector<string> uniKeywords = {"University","Institute","College","School of","Faculty of"};
    for(const auto& line : lines){
        for(const auto& keyword:uniKeywords){
            if(line.find(keyword) !=string::npos){
                return trim(line);
            }
        }
    }
    return "Not specified";
}
int CVParser::extractGraduationYear(const vector<string>& lines){
    regex yearPattern(R"((19|20)\d{2}\b)");
    smatch match;
    for(const auto& line : lines){
        if(regex_search(line,match,yearPattern)){
            try{
                return stoi(match.str());
            }catch(const invalid_argument&){
                continue;
            }
            
        }
    }
    return 0;
}

vector<string> CVParser::extractSkills(const vector<string>& lines){
    vector<string> skills;
    bool inSkillsSection = false;
    for(const auto& line:lines){
        string lowerLine = toLowerCase(line);
        if(lowerLine.find("skills") !=string::npos|| lowerLine.find("technical skills") !=string::npos){
            inSkillsSection = true;
            continue;

        }
        if(inSkillsSection){
            if(lowerLine.find("experience") !=string::npos || lowerLine.find("education") !=string::npos || lowerLine.find("projects") !=string::npos){
                break;
            }
            if(!line.empty()&& line.find(":")==string::npos){
                vector<string> tokens = splitString(line, ',');
                for(const auto& token:tokens){
                    if(!token.empty()){
                        skills.push_back(token);
                    }
                }

            }

        }
    }
    return skills;
} int CVParser::extractExperience(const vector<string>& lines){
    regex expPattern (R"((\d+)\s*(year|yr|month|mo))");
    smatch match;
    int totalYears = 0;
    for(const auto& line:lines){
        if(regex_search(line,match,expPattern)){
            try{int value = stoi(match[1].str());
            string unit = toLowerCase(match[2].str());
            if(unit.find("year") !=string::npos || unit.find("yr") !=string::npos){
                totalYears += value;
            }else if(unit.find("month") !=string::npos || unit.find("mo") !=string::npos){
                totalYears += value /12;
            }
        }catch(const invalid_argument&){
            continue;
        }
    }
    
}
    return totalYears;
}

vector<Project> CVParser::extractProjects(const vector<string>& lines){
    vector<Project> projects;
    bool inProjectsSection = false;
    Project currentProject;
    for(size_t i =0;i<lines.size();i++){
        string lowerLine = toLowerCase(lines[i]);
        if(lowerLine.find("projects")!=string::npos|| lowerLine.find("project") !=string::npos){
            inProjectsSection = true;
            continue;
        }

        if(inProjectsSection){
            if(lowerLine.find("certification") !=string::npos || lowerLine.find("education") !=string::npos){
                if(!currentProject.title.empty()){
                    projects.push_back(currentProject);
                    currentProject = Project();
                } break;

            }
            if(lines[i].find(".")!=string::npos||lines[i].find("-")!=string::npos||lines[i].find("*")!=string::npos){
                if(!currentProject.title.empty()){
                    projects.push_back(currentProject);
                    currentProject = Project();
                }
                currentProject.title = trim(lines[i].substr(1));
            }
        }


    }
    if(!currentProject.title.empty()){
        projects.push_back(currentProject);
    }
    return projects;
}
vector<string> CVParser::extractCertifications(const vector<string>& lines){
    vector<string> certifications;
    bool inCertSection = false;
    for(const auto& line:lines){
        string lowerLine = toLowerCase(line);
        if(lowerLine.find("certification") !=string::npos||lowerLine.find("certificate")!=string::npos){
            inCertSection = true;
            continue;
        }
        if(inCertSection){
            if(lowerLine.find("language") != string :: npos || lowerLine.find("reference")!=string::npos){
                break;
            }
        
            if(!line.empty() && line.find(":")==string::npos){
            certifications.push_back(trim(line));
            }
        }
    }

    return certifications;
}

