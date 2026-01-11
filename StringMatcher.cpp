#include "StringMatcher.h"
#include <algorithm>
#include <cctype>

StringMatcher::StringMatcher(){}
//-------kmp Algorithm-------
vector<int> StringMatcher::computeLPSArray(const string& pattern){
    int m = pattern.length();
    vector<int> lps(m,0);
    int len = 0;
    int i =1;

    while(i<m){
        if(pattern[i]==pattern[len]){
            len++;
            lps[i]=len;
            i++;

        }else{
            if(len!=0){
                len = lps[len-1];
            }else{
                lps[i]=0;
                i++;
            }
        }
    }
    return lps;

}
bool StringMatcher::kmpSearch(const string& text,const string& pattern){
    int n = text.length();
    int m = pattern.length();
    if(m==0) return true;
    if(n<m) return false;

    vector<int> lps = computeLPSArray(pattern);
    int i =0;
    int j =0;
    while(i<n){
        if(pattern[j]==text[i]){
            i++;
            j++;
        }
        if(j==m){
            return true;

        }else if(i<n && pattern[j]!=text[i]){
            if(j!=0){
                j = lps[j-1];
            }else{
                i++;
            }
        }
    }
    return false;

}
vector<int> StringMatcher::kmpSearchAll(const string& text,const string& pattern){
    vector<int> positions;
    int n = text.length();
    int m = pattern.length();

    if(m==0||n<m) return positions;
    vector<int> lps = computeLPSArray(pattern);
    int i =0;
    int j =0;

    while(i<n){
        if(pattern[j]==text[i]){
            i++;
            j++;
        }
        if(j==m){
            positions.push_back(i-j);
            j = lps[j-1];
        }else if(i<n&& pattern[j]!=text[i]){
            if(j!=0){
                j =lps[j-1];
            }else{
                i++;
            }
        }
    }
    return positions;

}
//-------Edit Distance-------
int StringMatcher::minimum(int a ,int b,int c){
    return min(min(a,b),c);
}
    int StringMatcher::editDistance(const string& s1,const string& s2){
        int m = s1.length();
        int n = s2.length();

        vector<vector<int>> dp(m+1,vector<int>(n+1));
        for(int i =0;i<=m;i++){
            dp[i][0]=i;
        }
        for(int j =0;j<=n;j++){
            dp[0][j]=j;
        }
        for(int i =1;i<=m;i++){
            for(int j =1;j<=n;j++){
                if(s1[i-1]==s2[j-1]){
                    dp[i][j]=dp[i-1][j-1];
                }else{
                    dp[i][j]=minimum(dp[i-1][j],dp[i][j-1],dp[i-1][j-1])+1;
                }
                
            }
        }
        return dp[m][n];
    }

 //-------FUZZY MATCHING-------
 double StringMatcher::fuzzySimilarity(const string& s1,const string& s2){
    if(s1.empty()&&s2.empty()) return 100.0;
    if(s1.empty()||s2.empty()) return 0.0;
    int maxLen = max(s1.length(),s2.length());
    int distance = editDistance(s1,s2);
    double similarity = (1.0 - (double)distance / maxLen) * 100.0;
    return similarity;
 }
 bool StringMatcher::isFuzzyMatch(const string& s1,const string& s2,double thresold)
{
    double similarity = fuzzySimilarity(s1,s2);
    return similarity >= thresold;
}
//-------CASE INSENSITIVE CONTAINS-------
bool StringMatcher::containsIgnoreCase(const string& text,const string& substring){
    string lowerText = text;
    string lowerSubstring = substring;
    transform(lowerText.begin(),lowerText.end(),lowerText.begin(),::tolower);
    transform(lowerSubstring.begin(),lowerSubstring.end(),lowerSubstring.begin(),::tolower);
    return lowerText.find(lowerSubstring)!=string::npos;
}
//-------LONGEST COMMON SUBSEQUENCE-------
int StringMatcher::longestCommonSubsequence(const string& s1,const string& s2){
    int m = s1.length();
    int n  = s2.length();

    vector<vector<int>> dp(m+1,vector<int>(n+1,0));
    for(int i =1;i<=m;i++){
        for(int j =1;j<=n;j++){
            if(s1[i-1]==s2[j-1]){
                dp[i][j]=dp[i-1][j-1]+1;
            }else{
                dp[i][j]=max(dp[i-1][j],dp[i][j-1]);
            }
        }
    }
    return dp[m][n];
}

