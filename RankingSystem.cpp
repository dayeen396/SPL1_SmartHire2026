#include "RankingSystem.h"
#include<algorithm>
#include "Applicant.h"

RankingSystem :: RankingSystem(){}

vector<Applicant> RankingSystem::getTopKUsingHeap(vector<Applicant>& applicants,int k){
    if(k<=0||applicants.empty()){
        return vector<Applicant>();
    }
    priority_queue<Applicant,vector<Applicant>,CompareScore> maxHeap;
    for(const auto& applicant:applicants){
        maxHeap.push(applicant);
    }
    vector<Applicant> topK;
    int count = min(k,(int)maxHeap.size());
    for(int i =0;i<count;i++){
        topK.push_back(maxHeap.top());
        maxHeap.pop();
    }
    return topK;
}
void RankingSystem::sortApplicantsByScore(vector<
    Applicant>& applicants){
        sort(applicants.begin(),applicants.end(),
    [](const Applicant& a,const Applicant& b){
        return a.getTotalScore()>b.getTotalScore();
    });

}
int RankingSystem::getApplicantRank(const vector<Applicant>& sortedApplicants,const string& email){
    for(size_t i =0;i<sortedApplicants.size();i++){
        if(sortedApplicants[i].getEmail()==email){
            return i+1;
        }
    }
    return -1;

}
void RankingSystem::heapify(vector<Applicant>& arr,int n,int i){
    int largest = i;
    int left = 2*i+1;
    int right = 2*i+2;
    if(left<n && arr[left].getTotalScore()>arr[largest].getTotalScore()){
        largest = left;
    }
    if(right<n&&arr[right].getTotalScore()>arr[largest].getTotalScore()){
        largest = right;
    }
    if(largest!=i){
        swap(arr[i],arr[largest]);
        heapify(arr,n,largest);
    }
}
void RankingSystem::buildMaxHeap(vector<Applicant>& arr){
    int n = arr.size();
    for(int i =n/2;i>=0;i--){
        heapify(arr,n,i);
    }
}
    