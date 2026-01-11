#ifndef SCORING_ENGINE_H
#define SCORING_ENGINE_H
#include "Applicant.h"
#include "JobRequirement.h"
#include<vector>
using namespace std;

class ScoringEngine{
    private:
    double calculateSkillScore(const Applicant& applicant,const JobRequirement& jobReq);
    double calculateEducationScore(const Applicant& applicant,const JobRequirement& jobReq);
    double calculateExperienceScore(const Applicant& applicant,const JobRequirement& jobReq);
    double calculateExtrasScore(const Applicant& applicant);

    double normalizeScore(double rawScore,double maxScore);
    int countMatchingSkills(const vector<string>& applicantSkills,const vector<string>& requiredSkills);
    public:
        ScoringEngine();

        void scoreApplicant(Applicant& applicant ,const JobRequirement& jobReq);

        void scoreAllApplicants(vector<Applicant>& applicants,const JobRequirement& jobReq);


};

#endif