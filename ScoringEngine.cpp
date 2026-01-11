#include "ScoringEngine.h"
#include <algorithm>
#include <cmath>

ScoringEngine::ScoringEngine(){}

void ScoringEngine::scoreApplicant(Applicant& applicant,const JobRequirement& jobReq){
    double skillScore = calculateSkillScore(applicant,jobReq);
    double eduScore = calculateEducationScore(applicant,jobReq);
    double expScore = calculateExperienceScore(applicant,jobReq);
    double extrasScore = calculateExtrasScore(applicant);

    applicant.setSkillScore(skillScore);
    applicant.setEducationScore(eduScore);
    applicant.setExperienceScore(expScore);
    applicant.setExtrasScore(extrasScore);

    applicant.calculateTotalScore(

        jobReq.getSkillWeight(),
        jobReq.getEducationWeight(),
        jobReq.getExperienceWeight(),
        jobReq.getExtrasWeight()



    );


}
void ScoringEngine::scoreAllApplicants(vector<Applicant>& applicants,const JobRequirement& jobReq){
    for(auto& applicant : applicants){
        scoreApplicant(applicant,jobReq);
    }
}

double ScoringEngine::calculateSkillScore(const Applicant& applicant,const JobRequirement& jobReq){
    vector<string> applicantSkills = applicant.getSkills();
    vector<string> requiredSkills = jobReq.getRequiredSkills();
    vector<string> preferredSkills = jobReq.getPreferredSkills();

    if(requiredSkills.empty() && preferredSkills.empty()){
        return 50.0;
    }
    double score = 0.0;
    double maxPossibleScore = 0.0;

   for(const auto& reqSkill: requiredSkills){
        double importance = jobReq.getSkillImportance(reqSkill);
        maxPossibleScore+=(10.0*importance);
        bool hasSkill = false;
        for(const auto& appSkill: applicantSkills){
            string lowerReq = reqSkill;
            string lowerApp = appSkill;
            transform(lowerReq.begin(),lowerReq.end(),lowerReq.begin(), ::tolower);
            transform(lowerApp.begin(),lowerApp.end(),lowerApp.begin(),::tolower);
            if(lowerApp.find(lowerReq)!=string::npos||lowerReq.find(lowerApp)!=string::npos){
                hasSkill = true;
                break;
            }

        }
        if(hasSkill){
            score += (10.0*importance);
        }
   }
   for(const auto& prefSkill:preferredSkills){
        maxPossibleScore+=5.0;
        bool hasSkill = false;
        for(const auto& appSkill:applicantSkills){
            string lowerPref = prefSkill;
            string lowerApp = appSkill;
            transform(lowerPref.begin(),lowerPref.end(),lowerPref.begin(), ::tolower);
            transform(lowerApp.begin(),lowerApp.end(),lowerApp.begin(),::tolower);
            if(lowerApp.find(lowerPref)!=string::npos||lowerPref.find(lowerApp)!=string::npos){
                hasSkill = true;
                break;
            }
        }
        if(hasSkill){
            score+=5.0;
        }
   }
   if(maxPossibleScore>0){
    return normalizeScore(score,maxPossibleScore);
   }
   return 50.0;

}
double ScoringEngine::calculateEducationScore(const Applicant& applicant,const JobRequirement& jobReq){
    double score =0.0;
    double cgpa = applicant.getCGPA();
    double minCGPA = jobReq.getMinCGPA();
    if(cgpa>3.75&&cgpa<=4.00){
        score+=50.0;
    }
    if(cgpa>3.5&&cgpa<=3.75){
        score+=45.0;
    }
    if(cgpa>3.25&&cgpa<=3.5){
        score+=40.0;
    }
    if(cgpa>3.0&&cgpa<=3.25){
        score+=35.0;
    }
    if(cgpa>=minCGPA){
        score+=30.0;
    }else{
        score+=15.0;
    }
    string degree = applicant.getDegree();
    vector<string> acceptableDegrees = jobReq.getAcceptableDegrees();
    bool degreeMatches = false;
    for(const auto& acceptable:acceptableDegrees){
        string lowerDegree = degree;
        string lowerAcceptable = acceptable;
        transform(lowerDegree.begin(),lowerDegree.end(),lowerDegree.begin(),::tolower);
        transform(lowerAcceptable.begin(),lowerAcceptable.end(),lowerAcceptable.begin(), ::tolower);
        if(lowerDegree.find(lowerAcceptable)!=string::npos){
            degreeMatches = true;
            break;
        }
    }
    if(degreeMatches){
        score+=50.0;
    }else if(!acceptableDegrees.empty()){
        score+=25.0;
    }else{
        score+=40.0;
    }
    return score;

}
double ScoringEngine::calculateExperienceScore(const Applicant& applicant,const JobRequirement& jobReq){
    int experience = applicant.getExperienceYears();
    int minExp = jobReq.getMinExperience();
    int maxExp = jobReq.getMaxExperience();

    if(experience>=minExp && experience<=maxExp){
        return 100.0;
    }
    if(experience<minExp){
        if(minExp==0){
            return 80.0;
        }
        double ratio = (double)experience / minExp;
        return ratio * 70.0;
    }
    int excess = experience - maxExp;
    if(excess<=2){
        return 85.0;
    }else if(excess<=5){
        return 70.0;
    }else{
        return 50.0;
    }

}
double ScoringEngine::calculateExtrasScore(const Applicant& applicant){
    double score = 0.0;
    int projectCount = applicant.getProjects().size();
    score+=(projectCount* 10.0 > 40.0) ? 40.0 : projectCount*10.0;

    int certCount = applicant.getCertifications().size();
    score+=(certCount*10.0>40.0)?40.0:certCount*10.0;

    score+=20.0;
    return score;
}

double ScoringEngine::normalizeScore(double rawScore, double maxScore) {
    if (maxScore == 0) return 50.0;
    double normalized = (rawScore / maxScore) * 100.0;
    return min(max(normalized, 0.0), 100.0);
}












