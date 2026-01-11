#include "JobRequirement.h"
#include "Applicant.h"
#include <iostream>
#include <algorithm>

JobRequirement::JobRequirement() : minCGPA(0.0),minExperience(0),maxExperience(100),
    skillWeight(0.4),educationWeight(0.3),experienceWeight(0.2),extrasWeight(0.1){}



void JobRequirement::addRequiredSkill(string skill){
    requiredSkills.push_back(skill);

}   
void JobRequirement::addPreferredSkill(string skill){
    preferredSkills.push_back(skill);
}
void JobRequirement::setMinCGPA(double cgpa){
    minCGPA = cgpa;
} 
void JobRequirement::setMinExperience(int years){
    minExperience = years;
}
void JobRequirement::setMaxExperience(int years){
    maxExperience = years;
}
void JobRequirement::setDegreeRequired(string degree){
    degreeRequired = degree;
}
void JobRequirement::addAcceptableDegree(string degree){
    acceptableDegrees.push_back(degree);
}
void JobRequirement::setWeights(double skill,double edu,double exp,double extra){
    skillWeight = skill;
    educationWeight = edu;
    experienceWeight = exp;
    extrasWeight = extra;
}
void JobRequirement::setSkillImportance(string skill,double importance){
    skillImportance[skill] = importance;
}

vector<string> JobRequirement::getRequiredSkills() const{
    return requiredSkills;
}
vector<string> JobRequirement::getPreferredSkills() const{
    return preferredSkills;
}
double JobRequirement::getMinCGPA() const{
    return minCGPA;

}
int JobRequirement::getMinExperience() const{
    return minExperience;
}
int JobRequirement::getMaxExperience() const{
    return maxExperience;
}
string JobRequirement::getDegreeRequired() const{
    return degreeRequired;
}
vector<string> JobRequirement:: getAcceptableDegrees() const{
    return acceptableDegrees;
}
double JobRequirement::getSkillWeight() const{
    return skillWeight;
}
double JobRequirement::getEducationWeight() const{
    return educationWeight;
}
double JobRequirement::getExperienceWeight() const{
    return experienceWeight;
}
double JobRequirement::getExtrasWeight() const{
    return extrasWeight;
}
double JobRequirement::getSkillImportance(string skill) const{
    auto it = skillImportance.find(skill);
    if(it != skillImportance.end()){
        return it->second;
    }
    return 1.0;//default importance
}
bool JobRequirement::meetsMinimumRequirements(const Applicant& applicant) const{
    if(applicant.getCGPA() < minCGPA){
        return false;
    }
    if(applicant.getExperienceYears() < minExperience){
        return false;
    }
    if(applicant.getExperienceYears() > maxExperience){
        return false;
    }

    string appDegree = applicant.getDegree();
    bool degreeMatch = false;
    if(!acceptableDegrees.empty()){
        for(const auto& degree : acceptableDegrees){
            if(appDegree.find(degree) != string::npos){
                degreeMatch = true;
                break;
            }
        }
        if(!degreeMatch){
            return false;
        }
    } else if(!degreeRequired.empty()){
        if(appDegree.find(degreeRequired) == string::npos){
            return false;
        }
    }

    vector<string> appSkills = applicant.getSkills();
    int matchCount = 0;
    for(const auto& reqSkill : requiredSkills){
        for(const auto& appSkill : appSkills){
            string lowerReq = reqSkill;
            string lowerApp = appSkill;
            transform(lowerReq.begin(), lowerReq.end(), lowerReq.begin(), ::tolower);
            transform(lowerApp.begin(), lowerApp.end(), lowerApp.begin(), ::tolower);

            if(lowerApp.find(lowerReq) != string::npos || lowerReq.find(lowerApp) != string::npos){
                matchCount++;
                break;
            }
        }
    }

    if(requiredSkills.size() > 0){
        int needed = (requiredSkills.size() + 1) / 2; // at least 50%
        if(matchCount < needed){
            return false;
        }
    }

    return true;
}

    void JobRequirement::display() const{
        cout<<"Job Requirements : "<<endl;
        cout<<"Required Skills: ";
        for(const auto& skill:requiredSkills){
            cout<<skill<<" , ";
        }
        cout<<"\nMin CGPA: "<<minCGPA<<endl;
        cout<<"Experience: "<<minExperience<<"-"<<maxExperience<<" years"<<endl;
        if (!acceptableDegrees.empty()) {
        cout << "Acceptable Degrees: ";
        for (const auto& degree : acceptableDegrees) {
            cout << degree << ", ";
        }
        cout << endl;
    }
}
