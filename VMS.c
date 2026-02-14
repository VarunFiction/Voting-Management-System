#include<stdio.h>
#include<string.h>

#define MAX_NAME_LEN 50
#define MAX_PASS_LEN 20

// STAFF PASSWORD 
#define STAFF_PASSWORD "admin123"

//  STATIC COUNTS 
#define VOTER_COUNT 5
#define CANDIDATE_COUNT 3

//  DATA STRUCTURES 
typedef struct {
    int id;
    char name[MAX_NAME_LEN];
    char password[MAX_PASS_LEN];
    int hasVoted;
} Voter;

typedef struct {
    char name[MAX_NAME_LEN];
    int votes;
} Candidate;

//  GLOBAL DATABASE 
Voter voters[VOTER_COUNT] = {
    {101, "Tanishk ",   "pass1", 0},
    {102, "Varun",     "pass2", 0},
    {103, "Aditya", "pass3", 0},
    {104, "Ashutosh ",   "pass4", 0},
    {105, "Vijay",     "pass5", 0}
};

Candidate candidates[CANDIDATE_COUNT] = {
    {"Narendra Modi", 0},
    {"Rahul Gandhi", 0},
    {"Amit Shah",  0}
};

int votesNOTA = 0;


//  UTILITIES 
int findVoterIndexById(int id) {
    for (int i = 0; i < VOTER_COUNT; i++) {
        if (voters[i].id == id) return i;
    }
    return -1;
}

void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) { }
}

//  VOTER FLOW 
void showCandidates() {
    printf("\n------------- CANDIDATES LIST -------------\n");
    for (int i = 0; i < CANDIDATE_COUNT; i++) {
        printf("%d. %s\n", i + 1, candidates[i].name);
    }
    printf("%d. None of the Above (NOTA)\n", CANDIDATE_COUNT + 1);
}

void castVoteForVoter(int voterIdx) {
    if (voters[voterIdx].hasVoted) {
        printf("\nYou have already voted. Second voting attempt denied!\n");
        return;
    }

    showCandidates();
    printf("\nEnter your vote (1-%d): ", CANDIDATE_COUNT + 1);
    int ch = 0;
    if (scanf("%d", &ch) != 1) {
        printf("Invalid input.\n");
        clearInputBuffer();
        return;
    }
    clearInputBuffer();

    if (ch >= 1 && ch <= CANDIDATE_COUNT) {
        candidates[ch - 1].votes++;
        printf("\nYou voted for %s successfully \n", candidates[ch - 1].name);
        voters[voterIdx].hasVoted = 1;
    } else if (ch == CANDIDATE_COUNT + 1) {
        votesNOTA++;
        printf("\nYou selected NOTA. Vote recorded \n");
        voters[voterIdx].hasVoted = 1;
    } else {
        printf("Invalid choice! Vote not counted.\n");
    }
}

void voterLoginFlow() {
    int id;
    char pass[MAX_PASS_LEN];

    printf("\n------------- VOTER LOGIN -------------\n");
    printf("Enter Voter ID: ");
    if (scanf("%d", &id) != 1) {
        printf("Invalid input.\n");
        clearInputBuffer();
        return;
    }
    clearInputBuffer();

    printf("Enter Password: ");
    if (scanf("%15s", pass) != 1) {
        printf("Invalid input.\n");
        clearInputBuffer();
        return;
    }
    clearInputBuffer();

    int idx = findVoterIndexById(id);
    if (idx == -1 || strcmp(voters[idx].password, pass) != 0) {
        printf("\n Login failed. Invalid Voter ID or Password.\n");
        return;
    }

    printf("\nLogin Successful \n");

    if (voters[idx].hasVoted) {
        printf("\nChecking Voting Status...\n");
        printf(" You have already voted. Second voting attempt denied!\n");
        return;
    }

    castVoteForVoter(idx);
}

//  STAFF FLOW (RESULTS ONLY) 
void showVoteCountForStaff() {
    printf("\n------------- VOTE COUNT (STAFF ONLY) -------------\n");
    for (int i = 0; i < CANDIDATE_COUNT; i++) {
        printf("%-15s : %d votes\n", candidates[i].name, candidates[i].votes);
    }
    printf("%-15s : %d votes\n", "NOTA", votesNOTA);
}

void showLeaderForStaff() {
    int leaderIdx = -1;
    int leaderVotes = -1;
    int tie = 0;

    // consider candidates only (NOTA typically not "leading" winner)
    for (int i = 0; i < CANDIDATE_COUNT; i++) {
        if (candidates[i].votes > leaderVotes) {
            leaderVotes = candidates[i].votes;
            leaderIdx = i;
            tie = 0;
        } else if (candidates[i].votes == leaderVotes) {
            tie = 1;
        }
    }

    printf("\n---------------------------------------\n");
    if (leaderVotes <= 0) {
        printf("No votes yet.\n");
    } else if (tie) {
        printf("Currently, there is a tie among top candidates.\n");
    } else {
        printf("Leading Candidate: %s with %d votes\n",
               candidates[leaderIdx].name, leaderVotes);
    }
    printf("---------------------------------------\n");
}

void staffLoginFlow() {
    char pwd[64];
    printf("\n------------- STAFF LOGIN -------------\n");
    printf("Enter Staff Password: ");
    if (scanf("%63s", pwd) != 1) {
        printf("Invalid input.\n");
        clearInputBuffer();
        return;
    }
    clearInputBuffer();

    if (strcmp(pwd, STAFF_PASSWORD) != 0) {
        printf("\n  Access denied. Wrong password.\n");
        return;
    }

    printf("\nLogin Verified \n");

    int choice;
    while (1) {
        printf("\n1. View Vote Count\n");
        printf("2. View Leading Candidate\n");
        printf("3. Back to Main Menu\n");
        printf("Enter choice: ");
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input.\n");
            clearInputBuffer();
            continue;
        }
        clearInputBuffer();

        if (choice == 1) {
            showVoteCountForStaff();
        } else if (choice == 2) {
            showLeaderForStaff();
        } else if (choice == 3) {
            break;
        } else {
            printf("Invalid choice. Try again.\n");
        }
    }
}

//  MAIN MENU 
int main() {
    int choice;

    while (1) {
        printf("\n------------------------------------------------------\n");
        printf("                VOTING MANAGEMENT SYSTEM\n");
        printf("------------------------------------------------------\n");
        printf("1. Voter Login\n");
        printf("2. Staff Login\n");
        printf("3. Exit\n");
        printf("\nEnter choice: ");

        if (scanf("%d", &choice) != 1) {
            printf("Invalid input.\n");
            clearInputBuffer();
            continue;
        }
        clearInputBuffer();

        switch (choice) {
            case 1: voterLoginFlow(); break;
            case 2: staffLoginFlow(); break;
            case 3: printf("\nSystem Closed. Thank You!\n"); 
            return 0;
            default: printf("Invalid choice! Try again.\n");
        }
    }
}