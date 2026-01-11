#include "Applicant.h"
#include <iostream>
#include <iomanip>
#include <sstream>

Applicant::Applicant():cgpa(0.0),experienceYears(0),graduationYear(0),
    skillScore(0.0),educationScore(0.0),experienceScore(0.0),extrasScore(0.0),totalScore(0.0) {}

Applicant::Applicant(string name,string email):name(name),email(email),
    cgpa(0.0),experienceYears(0),totalScore(0.0) {}


string Applicant::getName() const {
    return name;
}
string Applicant::getEmail() const {
    return email;
}
string Applicant::getPhone() const{
    return phone;
}    
double Applicant::getCGPA() const {
    return cgpa;
}
int Applicant::getExperienceYears() const {
    return experienceYears;
}
vector<string> Applicant::getSkills() const {
    return skills;
}
double Applicant::getTotalScore() const {
    return totalScore;
}
string Applicant::getDegree() const {
    return degree;
}
vector<Project> Applicant::getProjects() const{
    return projects;
}
vector<string> Applicant::getCertifications() const{
    return certifications;
}

void Applicant::setName(string n){
    name = n;
}
void Applicant::setEmail(string e){
    email = e;
}
void Applicant::setPhone(string p){
    phone = p;
}
void Applicant::setDegree(string d){
    degree = d;
}
void Applicant::setUniversity(string u){
    university = u;
}
void Applicant::setCGPA(double c){
    cgpa = c;
}
void Applicant::setGraduationYear(int y){
    graduationYear = y;
}
void Applicant::addSkill(string skill){
    skills.push_back(skill);
}
void Applicant::setExperienceYears(int years){
    experienceYears = years;
}
void Applicant::addCompany(string company){
    companies.push_back(company);
}
void Applicant::addPosition(string position){
    positions.push_back(position);
}
void Applicant::addProject(Project proj){
    projects.push_back(proj);

}
void Applicant::addCertification(string cert){
    certifications.push_back(cert);
}
void Applicant::addLanguage(string lang){
    languages.push_back(lang);
}
void Applicant::setSkillScore(double score){
    skillScore = score;
}
void Applicant::setEducationScore(double score){
    educationScore = score;
}
void Applicant::setExperienceScore(double score){
    experienceScore = score;
}
void Applicant::setExtrasScore(double score){
    extrasScore = score;
}
void Applicant::calculateTotalScore(double skillWeight,double eduWeitght,double expWeight,double extraWeight){
    totalScore = (skillScore * skillWeight) + (educationScore * eduWeitght) +
                 (experienceScore * expWeight) + (extrasScore * extraWeight);
}
void Applicant::displayInfo() const{
    cout<< "Name : "<<name<<endl;
    cout<< "Email : "<<email<<endl;
    cout<<"CGPA : "<<fixed<<setprecision(2)<<cgpa<<endl;
    cout<<"Experience Years : "<<experienceYears<<endl;
    cout<<"Total Score : "<< totalScore<<endl;

}
string Applicant::toDetailedString() const{
    stringstream ss;
    ss <<"===================== Applicant Details ===================="<<endl;
    ss << "Name: " << name << endl;
    ss << "Email: " << email << endl;
    ss << "Phone: " << phone << endl;
    ss << "Degree: " << degree << endl;
    ss << "University: " << university << endl;
    ss << "CGPA: " << fixed << setprecision(2) << cgpa << endl;
    ss << "Experience Years: " << experienceYears << endl;
    ss << "Skills: ";
    for (const auto &skill : skills) {
        ss << skill << ", ";
    }
    ss << endl;
    ss << "Total Score: " << totalScore << endl;

    return ss.str();
}
bool Applicant::operator<(const Applicant &other) const{
    return totalScore < other.totalScore;
}

