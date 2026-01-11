#ifndef APPLICANT_H
#define APPLICANT_H
#include <string>
#include <vector>
using namespace std;

struct Project {
    string title;
    vector <string> technologies;
    string description;
};
class Applicant {
    private:
        string name;
        string email;
        string phone;

        string degree;
        string university;
        double cgpa;
        int graduationYear;

        vector<string> skills;
        int experienceYears;
        vector<string> companies;
        vector<string> positions;

        vector<Project> projects;
        vector<string> certifications;
        vector<string> languages;
        double skillScore;
        double educationScore;
        double experienceScore;
        double extrasScore;
        double totalScore;

    public:
        Applicant();
        Applicant(string name,string email);
        string getName() const;
        string getEmail() const;
        string getPhone() const;
        double getCGPA() const;
        int getExperienceYears() const;
        vector<string> getSkills() const;
        double getTotalScore() const;
        string getDegree() const;
        vector<Project> getProjects() const;
        vector<string> getCertifications() const;

        void setName(string n);
        void setEmail(string e);
        void setPhone(string p);
        void setDegree(string d);
        void setUniversity(string u);
        void setCGPA(double c);
        void setGraduationYear(int y);
        void addSkill(string skill);
        void setExperienceYears(int years);
        void addCompany(string company);
        void addPosition(string position);
        void addProject(Project proj);
        void addCertification(string cert);
        void addLanguage(string lang);

        void setSkillScore(double score);
        void setEducationScore(double score);
        void setExperienceScore(double score);
        void setExtrasScore(double score);
        void calculateTotalScore(double skillWeight,double eduWeitght,double expWeight,double extraWeight);

        void displayInfo() const;
        string toDetailedString() const;

        bool operator<(const Applicant &other) const;



};
#endif // APPLICANT_H