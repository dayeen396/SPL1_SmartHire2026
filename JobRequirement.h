#ifndef JOBREQUIREMENT_H
#define JOBREQUIREMENT_H
#include <string>
#include <vector>
#include <map>
using namespace std;
class Applicant; // forward declaration

class JobRequirement{
    private:
        vector<string> requiredSkills;
        vector<string> preferredSkills;
        double minCGPA;
        int minExperience;
        int maxExperience;
        string degreeRequired;
        vector<string> acceptableDegrees;

        double skillWeight;
        double educationWeight;
        double experienceWeight;
        double extrasWeight;

        map<string, double> skillImportance;

    public:
        JobRequirement();
        void addRequiredSkill(string skill);
        void addPreferredSkill(string skill);
        void setMinCGPA(double cgpa);
        void setMinExperience(int years);
        void setMaxExperience(int years);
        void setDegreeRequired(string degree);
        void addAcceptableDegree(string degree);
        void setWeights(double skill, double edu, double exp, double extra);
        void setSkillImportance(string skill,double importance);
        
        vector<string> getRequiredSkills() const;
        vector<string> getPreferredSkills() const;
        double getMinCGPA() const;
        int getMinExperience() const;
        int getMaxExperience() const;
        string getDegreeRequired() const;
        vector<string> getAcceptableDegrees() const;
        double getSkillWeight() const;
        double getEducationWeight() const;
        double getExperienceWeight() const;
        double getExtrasWeight() const;
        double getSkillImportance(string skill) const;
        bool meetsMinimumRequirements(const class Applicant& applicant) const;
        void display() const;

};
#endif