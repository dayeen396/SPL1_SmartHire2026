#ifndef RANKING_SYSTEM_H
#define RANKING_SYSTEM_H
#include "Applicant.h"
#include <vector>
#include <queue>

using namespace std;

struct CompareScore{
    bool operator()(const Applicant& a,const Applicant& b) const{
        return a.getTotalScore() < b.getTotalScore();
    }
};
class RankingSystem{
    private:
        void heapify(vector<Applicant>& arr,int n , int i);
        void buildMaxHeap(vector<Applicant>& arr);

    public :
        RankingSystem();
        vector<Applicant> getTopKUsingHeap(vector<Applicant>& applicants,int k);
        void sortApplicantsByScore(vector<Applicant>& applicants);
        int getApplicantRank(const vector<Applicant>& sortedApplicants,const string& email);    
};
#endif
