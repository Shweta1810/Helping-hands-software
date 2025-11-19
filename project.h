/* project.h */
#ifndef PROJECT_H
#define PROJECT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NAME_LEN 64

/* ---------- Enums ---------- */
typedef enum { PACKAGING = 1, DELIVERY, SORTING } Sector;
typedef enum { DAY = 1, NIGHT } Timing;

/* ---------- Structs ---------- */
typedef struct Helper {
    int id;
    int preference_num;
    Sector sector;
    int wages;
    Timing timing;
    int extra_preference_flag;
    struct Helper *next;
} Helper;

typedef struct Neigh {
    char branch_name[NAME_LEN];
    struct Neigh *next;
} Neigh;

typedef struct Branch {
    char name[NAME_LEN];
    Helper *helpers_head;
    Neigh *adj;
    struct Branch *left, *right;
} Branch;

typedef struct QNode {
    int helper_id;
    struct QNode *next;
} QNode;

typedef struct Queue {
    QNode *front, *rear;
} Queue;

/* ---------- Globals ---------- */
extern Branch *root;
extern int nextHelperID;

/* ---------- Queue Functions ---------- */
Queue *createQueue();
void enqueueID(Queue *, int);
int dequeueID(Queue *);
int queueEmpty(Queue *);
void freeQueue(Queue *);

/* ---------- Input ---------- */
void read_line(char *, int);
void clearInputBuffer(void);

/* ---------- Branch Operations ---------- */
Branch *createBranchNode(const char *);
Branch *insertBranch(Branch *, const char *);
Branch *findBranch(Branch *, const char *);
Branch *removeBranchNode(Branch *, const char *);
Branch *minBranchNode(Branch *);
void addEdge(Branch *, const char *);

/* ---------- Helper Operations ---------- */
void insertHelperToBranch(Branch *, Helper *);
int removeHelperFromBranch(Branch *, int);
int removeHelperByID_traverse(Branch *, int);
int searchHelperByID(Branch *, int);

/* ---------- Menu Options ---------- */
void optionAddBranch();
void optionRemoveBranch();
void optionRegisterHelpers();
void optionRemoveHelperByID();
void optionViewBranches();
void optionHelpersByBranch();
void optionTotalHelpersDetails();
void optionSearchHelperByNumber();
void optionConnectTwoBranches();

#endif
