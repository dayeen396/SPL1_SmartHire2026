#include "SkillDatabase.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <cctype>

SkillDatabase::SkillDatabase(){

    //programming languages

	addSkillRelation("C++","CPP");
	addSkillRelation("C++","C Plus Plus");
	addSynonym("CPP","C++");
    addSynonym("C Plus Plus","C++");
    addSkillRelation("Python","Py");
    addSynonym("Py","Python");
    addSkillRelation("JavaScript","JS");
    addSynonym("JS","JavaScript");

    //Machine Learning

    addSkillRelation("Machine Learning","ML");
    addSkillRelation("Machine Learning","Deep Learning");
    addSkillRelation("Machine Learning","AI");
    addSkillRelation("Deep Learning","Neural Networks");
    addSkillRelation("Deep Learning","TensorFlow");
    addSkillRelation("Deep Learning","PyTorch");
    addSynonym("ML","Machine Learning");
    addSynonym("AI","Artificial Intelligence");
    addSynonym("DL","Deep Learning");

    //Web Development

    addSkillRelation("React","ReactJS");
    addSkillRelation("React","React.js");
    addSkillRelation("Node","NodeJS");
    addSkillRelation("Node","Node.js");
    addSynonym("ReactJS","React");
    addSynonym("NodeJS","Node");

    //Databases
    addSkillRelation("SQL","MySQL");
    addSkillRelation("SQL","PostgreSQL");
    addSkillRelation("NoSQL","MongoDB");
    addSynonym("DB","Database");
    addSynonym("Database","DB");
    
    //Data Structures & Algorithms

    addSkillRelation("DSA","Data Structures");
    addSkillRelation("DSA","Algorithms");
    addSkillRelation("Data Structures","Algorithms");
    addSynonym("DS","Data Structures");
    addSynonym("Algo","Algorithms");
    //Categories
    addSkillToCategory("Programming Languages","C++");
    addSkillToCategory("Programming Languages","Python");
    addSkillToCategory("Programming Languages","JavaScript");
    addSkillToCategory("Machine Learning","Machine Learning");
    addSkillToCategory("Machine Learning","Deep Learning");
    addSkillToCategory("Machine Learning","AI");
    addSkillToCategory("Web Development","React");
    addSkillToCategory("Web Development","Node");
    addSkillToCategory("Databases","SQL");
    addSkillToCategory("Databases","NoSQL");
    addSkillToCategory("Data Structures & Algorithms","Data Structures");
    addSkillToCategory("Data Structures & Algorithms","Algorithms");





}
string SkillDatabase::toLowerCase(string str){
    transform(str.begin(),str.end(),str.begin(),::tolower);
    return str;
}
void SkillDatabase::addSkillRelation(string skill1,string skill2){
    string s1 = toLowerCase(skill1);
    string s2 = toLowerCase(skill2);
    skillGraph[s1].push_back(s2);
    skillGraph[s2].push_back(s1);  //bidirectional
}

void SkillDatabase::addSynonym(string synonym,string canonical){
    synonymMap[toLowerCase(synonym)] = toLowerCase(canonical);
}
void SkillDatabase::addSkillToCategory(string
 skill,string category){
    categories[category].push_back(toLowerCase(skill));
}
bool SkillDatabase::areRelated(string skill1,string skill2){
    string s1 = toLowerCase(skill1);
    string s2 = toLowerCase(skill2);
    if(s1==s2) return true;

    //Check synonyms

    string canonical1 = getCanonicalName(s1);
    string canonical2 = getCanonicalName(s2);
    if(canonical1==canonical2) return true;

    //Check Direct Relations
    if(skillGraph.find(canonical1)!=skillGraph.end()){
        for(const auto& related:skillGraph[canonical1]){
            if(related==canonical2){
                return true;
            }
        }
    }
    //Check substring relations
    if(s1.find(s2)!=string::npos||s2.find(s1)!=string::npos){
        return true;
    }
    return false;
}
vector<string> SkillDatabase::getRelatedSkills(string skill){
    string s = toLowerCase(skill);
    string canonical = getCanonicalName(s);

    if(skillGraph.find(canonical)!=skillGraph.end()){
        return skillGraph[canonical];
    }
    return vector<string>();
}
string SkillDatabase::getCanonicalName(string skill){
    string s = toLowerCase(skill);
    if(synonymMap.find(s)!=synonymMap.end()){
        return synonymMap[s];
    }
    return s;

}
vector<string> SkillDatabase::getSkillsInCategory(string category){
    if(categories.find(category)!=categories.end()){
        return categories[category];
    }
    return vector<string>();
}
void SkillDatabase::loadFromFile(string filename){
    ifstream file(filename);
    if(!file.is_open()){
        cerr<<"[ERROR] could not open skill database file: "<<filename<<endl;
        return;
    }
    string line;
    while(getline(file,line)){
        if(line.empty()||line[0]=='#') continue; //skip empty lines and comments
        stringstream ss(line);
        string skill1,skill2,type;
        getline(ss,skill1,',');
        getline(ss,skill2,',');
        getline(ss,type,',');

        skill1 = skill1.substr(skill1.find_first_not_of(" \t"));
        skill1 = skill1.substr(0,skill1.find_last_not_of(" \t")+1);
        skill2 = skill2.substr(skill2.find_first_not_of(" \t"));;
        skill2 = skill2.substr(0,skill2.find_last_not_of(" \t")+1);
        type = type.substr(type.find_first_not_of(" \t"));;
        type = type.substr(0,type.find_last_not_of(" \t")+1);

        if(type=="SYNONYM"){
            addSynonym(skill1,skill2);
        }else if(type=="RELATION"){
            addSkillRelation(skill1,skill2);
        }else if(type=="CATEGORY"){
            addSkillToCategory(skill1,skill2);
        }

    }
    file.close();
}
void SkillDatabase::displayDatabase() const{
    cout<<"\n==== Skill Database ===="<<endl;
    cout<<"-- Skill Relations --"<<endl;
    for(const auto& [skill,related]:skillGraph){
        cout<<skill<<" -> ";
        for(const auto& rel:related){
            cout<<rel<<", ";
    }
        cout<<endl;
    }
    cout<<"\n-- Synonyms --"<<endl;
    for(const auto& [synonym,canonical]:synonymMap){
        cout<<synonym<<" => "<<canonical<<endl;

}
}  

