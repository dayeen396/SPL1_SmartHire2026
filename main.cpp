#include <iostream>
#include <vector>
#include <string>
#include <iomanip>

#include "Applicant.h"
#include "JobRequirement.h"
#include "CVParser.h"
#include "ScoringEngine.h"
#include "RankingSystem.h"
#include "FileHandler.h"

using namespace std;

void printBanner(string text) {
    int width = 70;
    cout << "\n" << string(width, '=') << "\n";
    cout << "   " << text << "\n";
    cout << string(width, '=') << "\n\n";
}

void displayMenu() {
    cout << "\n" << string(60, '=') << "\n";
    cout << "                    MAIN MENU\n";
    cout << string(60, '=') << "\n";
    cout << "1. Run Automated CV Screening (PDF/TXT Support)\n";
    cout << "2. Display System Information\n";
    cout << "3. Exit\n";
    cout << string(60, '=') << "\n";
}

void displaySystemInfo() {
    printBanner("SYSTEM INFORMATION");
    
    cout << "System Name: SmartHire CV Screening System\n";
    cout << "Version: 2.0.0 (MACOS + POPPLER PDF SUPPORT)\n";
    cout << "Platform: macOS\n\n";
    
    cout << "Features:\n";
    cout << string(50, '-') << "\n";
    cout << "+ PDF file support (via Poppler pdftotext)\n";
    cout << "+ TXT file support (direct parsing)\n";
    cout << "+ Automated CV parsing and data extraction\n";
    cout << "+ Multi-criteria scoring algorithm\n";
    cout << "+ Heap-based ranking (O(n log k))\n";
    cout << "+ CSV export for Excel\n";
    cout << "+ Comprehensive reports\n\n";
    
    cout << "Supported CV Formats:\n";
    cout << string(50, '-') << "\n";
    cout << "* PDF (.pdf) - Converted using pdftotext\n";
    cout << "* Plain Text (.txt) - Direct parsing\n\n";
    
    cout << "Algorithms Implemented:\n";
    cout << string(50, '-') << "\n";
    cout << "1. Weighted Scoring Algorithm\n";
    cout << "2. Max Heap / Priority Queue Ranking\n";
    cout << "3. String Matching for Skill Detection\n";
    cout << "4. Regex-based Information Extraction\n\n";
    
    cout << "PDF Support (Poppler):\n";
    cout << string(50, '-') << "\n";
    cout << "Tool: pdftotext (from Poppler)\n";
    cout << "Installation: brew install poppler\n";
    cout << "Verify: pdftotext -v\n\n";
}

void runScreening(int topK) {
    printBanner("AUTOMATED CV SCREENING PROCESS");
    
    cout << "[INIT] Initializing system components..." << endl;
    
    CVParser parser;
    ScoringEngine scoringEngine;
    RankingSystem rankingSystem;
    FileHandler fileHandler;
    
    cout << "[SUCCESS] Components initialized!\n" << endl;
    
    // Load job requirements
    cout << "[CONFIG] Loading job requirements..." << endl;
    JobRequirement jobReq = fileHandler.loadJobRequirements("data/job_req.txt");
    
    cout << "\n[JOB REQUIREMENTS]" << endl;
    cout << string(50, '-') << endl;
    jobReq.display();
    cout << endl;
    
    // Load CVs
    cout << "[SCANNER] Scanning for CV files..." << endl;
    vector<string> cvFiles = fileHandler.getCVFilesFromDirectory("data/cvs");
    
    if (cvFiles.empty()) {
        cerr << "\n[ERROR] No CV files found in data/cvs/ directory!" << endl;
        cerr << "[INFO] Please add PDF or TXT CV files to data/cvs/" << endl;
        return;
    }
    
    cout << "[SUCCESS] Found " << cvFiles.size() << " CV files!\n" << endl;
    
    // Parse all CVs
    cout << "[PARSING] Processing CV files...\n" << endl;
    
    vector<Applicant> allApplicants;
    
    for (size_t i = 0; i < cvFiles.size(); i++) {
        cout << "\n[" << (i+1) << "/" << cvFiles.size() << "] ";
        
        Applicant applicant = parser.parseCV(cvFiles[i]);
        if (!applicant.getName().empty() && applicant.getName() != "Unknown") {
            allApplicants.push_back(applicant);
        } else {
            cerr << "[WARNING] Failed to parse: " << cvFiles[i] << endl;
        }
    }
    
    cout << "\n[SUMMARY] Successfully parsed " << allApplicants.size() 
         << " out of " << cvFiles.size() << " CV files\n" << endl;
    
    if (allApplicants.empty()) {
        cerr << "[ERROR] No valid CVs were parsed!" << endl;
        return;
    }
    
    // Filter candidates
    cout << "[FILTER] Applying minimum requirements filter..." << endl;
    
    vector<Applicant> qualifiedApplicants;
    
    for (auto& applicant : allApplicants) {
        if (jobReq.meetsMinimumRequirements(applicant)) {
            qualifiedApplicants.push_back(applicant);
        }
    }
    
    cout << "[FILTER] Qualified: " << qualifiedApplicants.size() << " candidates\n" << endl;
    
    if (qualifiedApplicants.empty()) {
        cerr << "[ERROR] No candidates met minimum requirements!" << endl;
        return;
    }
    
    // Score candidates
    cout << "[SCORING] Calculating scores..." << endl;
    scoringEngine.scoreAllApplicants(qualifiedApplicants, jobReq);
    cout << "[SUCCESS] Scoring completed!\n" << endl;
    
    // Rank candidates
    cout << "[RANKING] Selecting top " << topK << " candidates..." << endl;
    vector<Applicant> topCandidates = 
        rankingSystem.getTopKUsingHeap(qualifiedApplicants, topK);
    cout << "[SUCCESS] Top " << topCandidates.size() << " identified!\n" << endl;
    
    // Display results
    printBanner("TOP CANDIDATES");
    
    cout << left << setw(6) << "Rank" 
         << setw(25) << "Name"
         << setw(12) << "Score"
         << setw(10) << "CGPA"
         << setw(12) << "Experience"
         << "\n";
    cout << string(70, '-') << "\n";
    
    for (size_t i = 0; i < topCandidates.size(); i++) {
        cout << left << setw(6) << ("#" + to_string(i + 1))
             << setw(25) << topCandidates[i].getName().substr(0, 23)
             << setw(12) << fixed << setprecision(2) 
             << (to_string(topCandidates[i].getTotalScore()) + "/100")
             << setw(10) << topCandidates[i].getCGPA()
             << setw(12) << (to_string(topCandidates[i].getExperienceYears()) + " yrs")
             << "\n";
    }
    
    cout << string(70, '-') << "\n\n";
    
    // Generate reports
    cout << "[REPORTS] Generating output files..." << endl;
    
    fileHandler.createDirectory("data/output");
    fileHandler.saveTopCandidates(topCandidates, "data/output/top_candidates.txt");
    fileHandler.exportToCSV(topCandidates, "data/output/top_candidates.csv");
    
    printBanner("SCREENING COMPLETED");
    
    cout << "Total Applicants: " << allApplicants.size() << "\n";
    cout << "Qualified: " << qualifiedApplicants.size() << "\n";
    cout << "Top Selected: " << topCandidates.size() << "\n\n";
    cout << "Reports saved in: data/output/\n\n";
    
    if (!topCandidates.empty()) {
        cout << " TOP RECOMMENDATION: " << topCandidates[0].getName() << "\n";
        cout << "   Score: " << fixed << setprecision(2) 
             << topCandidates[0].getTotalScore() << "/100\n";
        cout << "   Status: HIGHLY RECOMMENDED\n\n";
    }
}

int main() {
    system("clear");  // Clear screen on macOS
    
    printBanner("SMARTHIRE - CV SCREENING SYSTEM");
    
    cout << "Welcome to SmartHire CV Screening System!\n";
    cout << "macOS Version with Poppler PDF Support\n\n";
    
    // Check if pdftotext is available
    cout << "[SYSTEM CHECK] Checking PDF support..." << endl;
    if (system("which pdftotext > /dev/null 2>&1") == 0) {
        cout << "[SUCCESS] pdftotext found! PDF support enabled.\n" << endl;
    } else {
        cout << "[WARNING] pdftotext not found!" << endl;
        cout << "[INFO] Install with: brew install poppler\n" << endl;
    }
    
    while (true) {
        displayMenu();
        
        int choice;
        cout << "\nEnter your choice: ";
        cin >> choice;
        
        switch (choice) {
            case 1: {
                int topK = 4;
                cout << "\nHow many top candidates? (default 4): ";
                cin >> topK;
                if (topK <= 0) topK = 4;
                runScreening(topK);
                break;
            }
            
            case 2: {
                displaySystemInfo();
                break;
            }
            
            case 3: {
                cout << "\n[EXIT] Thank you for using SmartHire!\n";
                cout << "[EXIT] Goodbye!\n" << endl;
                return 0;
            }
            
            default: {
                cerr << "\n[ERROR] Invalid choice! Try again.\n" << endl;
                break;
            }
        }
        
        cout << "\nPress Enter to continue...";
        cin.ignore();
        cin.get();
        system("clear");
    }
    
    return 0;
}