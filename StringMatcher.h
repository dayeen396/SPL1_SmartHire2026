#ifndef STRING_MATCHER_H
#define STRING_MATCHER_H
#include<string>
#include<vector>
using namespace std;
class StringMatcher{
    private:
        vector<int> sompteLPSArray(const string& pattern);
        int minimum(int a ,int b,int c);

    public:
        StringMatcher();
        vector<int> computeLPSArray(const string& pattern);
        bool kmpSearch(const string& text,const string& pattern);
        vector<int> kmpSearchAll(const string& text,const string& pattern);
        
        int editDistance(const string& s1,const string& s2);
        double fuzzySimilarity(const string& s1,const string& s2);
        bool isFuzzyMatch(const string& s1,const string& s2,double thresold = 0.7);
        bool containsIgnoreCase(const string& text,const string& substring);
        int longestCommonSubsequence(const string& s1,const string& s2);
};
#endif