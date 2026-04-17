#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
====================================================
   MINDCARE SYSTEM (FINAL PROJECT)
----------------------------------------------------
Data Structures Used:
1. Array  -> store results & questions
2. Stack  -> undo last assessment (LIFO)
3. Queue  -> appointment system (FIFO)
====================================================
*/

// ---------- CONSTANTS ----------
#define MAX_NAME 50
#define MAX_RESULTS 50
#define MAX_STACK 50
#define MAX_QUEUE 50
#define MAX_Q 5

// ---------- COLORS ----------
#define RED "\033[1;31m"
#define GREEN "\033[1;32m"
#define YELLOW "\033[1;33m"
#define CYAN "\033[1;36m"
#define RESET "\033[0m"

// ================= STRUCTURES =================

typedef struct {
    char username[MAX_NAME];
    char password[MAX_NAME];
    char role[10];
} User;

typedef struct {
    char username[MAX_NAME];
    int stress;
    int anxiety;
    int sleep;
    int depression;
} Result;

typedef struct {
    Result data[MAX_STACK];
    int top;
} Stack;

typedef struct {
    char username[MAX_NAME];
} QueueItem;

typedef struct {
    QueueItem items[MAX_QUEUE];
    int front, rear;
} Queue;

// ================= GLOBAL VARIABLES =================

User users[] = {
    {"patient1", "123", "patient"},
    {"patient2", "123", "patient"},
    {"doctor", "123", "doctor"}
};

int userCount = 3;

Result results[MAX_RESULTS];
int resultCount = 0;

Stack undoStack = {.top = -1};
Queue appointmentQueue = {.front = 0, .rear = -1};

// ================= QUESTIONS =================

char *stressQ[MAX_Q] = {
    "Stress: Pressure (1-5): ",
    "Stress: Hard to relax (1-5): ",
    "Stress: Overwhelmed (1-5): ",
    "Stress: Irritation (1-5): ",
    "Stress: Work load (1-5): "
};

char *anxietyQ[MAX_Q] = {
    "Anxiety: Nervous (1-5): ",
    "Anxiety: Worry (1-5): ",
    "Anxiety: Panic (1-5): ",
    "Anxiety: Restless (1-5): ",
    "Anxiety: Fear (1-5): "
};

char *sleepQ[MAX_Q] = {
    "Sleep: Trouble sleeping (1-5): ",
    "Sleep: Wake up (1-5): ",
    "Sleep: Tired (1-5): ",
    "Sleep: Late sleep (1-5): ",
    "Sleep: Day sleep (1-5): "
};

char *depressionQ[MAX_Q] = {
    "Depression: Sad (1-5): ",
    "Depression: No interest (1-5): ",
    "Depression: Low energy (1-5): ",
    "Depression: Hopeless (1-5): ",
    "Depression: No motivation (1-5): "
};

// ================= FUNCTION DECLARATIONS =================

int login(User *u);
void patientMenu(User u);
void doctorMenu();
void takeAssessment(User u);

void push(Result r);
Result pop();
int isEmpty();

void enqueue(char name[]);
void dequeue();

void giveSuggestion(int s, int a, int sl, int d);
void doctorDecision(int total);
void printLevel(int score);

void viewAll();
void viewOne();

// ================= MAIN =================

int main() {
    int choice;
    User loggedUser;

    while(1) {
        printf(CYAN "\n==== MINDCARE SYSTEM ====\n" RESET);
        printf("1. Patient\n2. Doctor\n3. Exit\nChoose option (1/2/3): ");
        scanf("%d", &choice);

        if(choice == 3) break;

        if(login(&loggedUser)) {
            if(choice == 1 && strcmp(loggedUser.role, "patient") == 0) {
                printf(GREEN "Patient login successful!\n" RESET);
                patientMenu(loggedUser);
            }
            else if(choice == 2 && strcmp(loggedUser.role, "doctor") == 0) {
                printf(GREEN "Doctor login successful!\n" RESET);
                doctorMenu();
            }
            else {
                printf(RED "Role mismatch!\n" RESET);
            }
        }
    }

    return 0;
}

// ================= LOGIN =================

int login(User *u) {
    char name[MAX_NAME], pass[MAX_NAME];

    printf("Username: ");
    scanf("%s", name);
    printf("Password: ");
    scanf("%s", pass);

    for(int i=0;i<userCount;i++) {
        if(strcmp(name, users[i].username)==0 &&
           strcmp(pass, users[i].password)==0) {
            *u = users[i];
            printf(GREEN "Login successful!\n" RESET);
            return 1;
        }
    }

    printf(RED "Invalid login!\n" RESET);
    return 0;
}

// ================= PATIENT =================

void patientMenu(User u) {
    int choice;

    printf("\nWelcome %s\n", u.username);
    printf("1. Take Assessment\n2. Request Appointment\nChoose option (1/2): ");
    scanf("%d", &choice);

    if(choice == 1)
        takeAssessment(u);
    else if(choice == 2) {
        enqueue(u.username);
        printf(GREEN "Appointment requested!\n" RESET);
    }
}

// ================= DOCTOR =================

void doctorMenu() {
    int ch;

    do {
        printf(GREEN "\n--- DOCTOR PANEL ---\n" RESET);
        printf("1. View All\n2. View One\n3. Process Appointment\n4. Undo\n5. Exit\n");
        printf("Choose option (1/2/3/4/5): ");
        scanf("%d", &ch);

        switch(ch) {
            case 1: viewAll(); break;
            case 2: viewOne(); break;
            case 3: dequeue(); break;
            case 4:
                if(!isEmpty()) {
                    pop();
                    resultCount--;
                    printf(YELLOW "Last record removed!\n" RESET);
                } else printf("Nothing to undo\n");
                break;
        }

    } while(ch!=5);
}

// ================= ASSESSMENT =================

void takeAssessment(User u) {
    int stress=0, anxiety=0, sleep=0, depression=0, ans;

    printf("\n--- Stress ---\n");
    for(int i=0;i<MAX_Q;i++){ printf("%s", stressQ[i]); scanf("%d",&ans); stress+=ans;}

    printf("\n--- Anxiety ---\n");
    for(int i=0;i<MAX_Q;i++){ printf("%s", anxietyQ[i]); scanf("%d",&ans); anxiety+=ans;}

    printf("\n--- Sleep ---\n");
    for(int i=0;i<MAX_Q;i++){ printf("%s", sleepQ[i]); scanf("%d",&ans); sleep+=ans;}

    printf("\n--- Depression ---\n");
    for(int i=0;i<MAX_Q;i++){ printf("%s", depressionQ[i]); scanf("%d",&ans); depression+=ans;}

    Result r;
    strcpy(r.username, u.username);
    r.stress = stress;
    r.anxiety = anxiety;
    r.sleep = sleep;
    r.depression = depression;

    results[resultCount++] = r;
    push(r);

    printf(GREEN "\nAssessment Completed!\n" RESET);

    giveSuggestion(stress, anxiety, sleep, depression);
}

// ================= STACK =================

void push(Result r){ undoStack.data[++undoStack.top]=r; }
Result pop(){ return undoStack.data[undoStack.top--]; }
int isEmpty(){ return undoStack.top==-1; }

// ================= QUEUE =================

void enqueue(char name[]){
    strcpy(appointmentQueue.items[++appointmentQueue.rear].username,name);
}

void dequeue(){
    if(appointmentQueue.front<=appointmentQueue.rear)
        printf("Processing %s\n",appointmentQueue.items[appointmentQueue.front++].username);
    else printf("No appointments\n");
}

// ================= LOGIC =================

void printLevel(int score){
    if(score>20) printf("High");
    else if(score>10) printf("Moderate");
    else printf("Low");
}

void giveSuggestion(int s,int a,int sl,int d){
    printf(CYAN "\nSuggestions:\n" RESET);

    if(s>15) printf(" Reduce stress\n");
    if(a>15) printf("Relaxation needed\n");
    if(sl>15) printf("Improve sleep\n");
    if(d>15) printf("Counseling recommended\n");
}

void doctorDecision(int total){
    printf("Decision: ");
    if(total>70) printf(RED "CRITICAL\n" RESET);
    else if(total>50) printf(YELLOW "HIGH RISK\n" RESET);
    else if(total>30) printf(YELLOW "MODERATE\n" RESET);
    else printf(GREEN "LOW\n" RESET);
}

// ================= VIEW =================

void viewAll(){
    for(int i=0;i<resultCount;i++){
        int total = results[i].stress + results[i].anxiety +
                    results[i].sleep + results[i].depression;

        printf("\nPatient: %s\n", results[i].username);

        printf("Stress: %d (", results[i].stress); printLevel(results[i].stress); printf(")\n");
        printf("Anxiety: %d (", results[i].anxiety); printLevel(results[i].anxiety); printf(")\n");
        printf("Sleep: %d (", results[i].sleep); printLevel(results[i].sleep); printf(")\n");
        printf("Depression: %d (", results[i].depression); printLevel(results[i].depression); printf(")\n");

        printf("Total: %d\n", total);

        doctorDecision(total);
    }
}

void viewOne(){
    char name[MAX_NAME];
    printf("Enter name: ");
    scanf("%s", name);

    for(int i=0;i<resultCount;i++){
        if(strcmp(name,results[i].username)==0){
            int total = results[i].stress + results[i].anxiety +
                        results[i].sleep + results[i].depression;

            printf("\nPatient: %s Total: %d\n", name, total);
            doctorDecision(total);
            return;
        }
    }
    printf("Not found\n");
}
