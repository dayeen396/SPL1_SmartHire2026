#ifndef SKILL_DATABASE_H
#define SKILL_DATABASE_H
#include <string>
#include <vector>
#include <unordered_map>
#include <map>
#include <set>
using namespace std;

class SkillDatabase{
    private:
        unordered_map<string, vector<string>> skillGraph;
        unordered_map<string, string> synonymMap;
        std::map<string, vector<string>> categories;
        string toLowerCase(string str);
public:
    SkillDatabase();
    void addSkillRelation(string skill1, string skill2);
    void addSynonym(string synonym,string canonical);
    void addSkillToCategory(string skill, string category);
    bool areRelated(string skill1,string skill2);
    vector<string> getRelatedSkills(string skill);
    string getCanonicalName(string skill);
    vector<string> getSkillsInCategory(string category);
    void loadFromFile(string filename);

    void displayDatabase() const ;


};
#endif
