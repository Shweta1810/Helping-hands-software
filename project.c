#include<stdio.h>
#include "project.h"
#include <ctype.h>
#include<stdlib.h>
#include<string.h>

/* ---------- Globals ---------- */
Branch *root = NULL;
int nextHelperID = 1;

/* ---------- Queue Functions ---------- */
Queue *createQueue() {
    Queue *q = malloc(sizeof(Queue));
    q->front = q->rear = NULL;
    return q;
}

void enqueueID(Queue *q, int id) {
    QNode *n = malloc(sizeof(QNode));
    n->helper_id = id;
    n->next = NULL;
    if (!q->rear)
        q->front = q->rear = n;
    else {
        q->rear->next = n;
        q->rear = n;
    }
}

int dequeueID(Queue *q) {
    if (!q->front) return -1;
    QNode *t = q->front;
    int id = t->helper_id;
    q->front = t->next;
    if (!q->front) q->rear = NULL;
    free(t);
    return id;
}

int queueEmpty(Queue *q) {
    return q->front == NULL;
}

void freeQueue(Queue *q) {
    while (!queueEmpty(q)) dequeueID(q);
    free(q);
}

/* ---------- Input ---------- */
void read_line(char *buf, int size) {
    fgets(buf, size, stdin);
    buf[strcspn(buf, "\r\n")] = '\0';
}

/* ---------- Branch Functions ---------- */
Branch *createBranchNode(const char *name) {
    Branch *b = malloc(sizeof(Branch));
    strncpy(b->name, name, NAME_LEN - 1);
    b->name[NAME_LEN - 1] = '\0';
    b->helpers_head = NULL;
    b->adj = NULL;
    b->left = b->right = NULL;
    return b;
}

Branch *insertBranch(Branch *node, const char *name) {
    if (!node) {
        printf("Branch added successfully!\n");
        return createBranchNode(name);
    }

    int cmp = strcmp(name, node->name);

    if (cmp < 0)
        node->left = insertBranch(node->left, name);
    else if (cmp > 0)
        node->right = insertBranch(node->right, name);
    else
        printf("Branch already exists.\n");

    return node;
}

Branch *findBranch(Branch *node, const char *name) {
    if (!node) return NULL;
    int cmp = strcmp(name, node->name);
    if (cmp == 0) return node;
    return (cmp < 0) ? findBranch(node->left, name) : findBranch(node->right, name);
}

Branch *minBranchNode(Branch *node) {
    while (node && node->left)
        node = node->left;
    return node;
}

Branch *removeBranchNode(Branch *node, const char *name) {
    if (!node) return NULL;

    int cmp = strcmp(name, node->name);

    if (cmp < 0)
        node->left = removeBranchNode(node->left, name);
    else if (cmp > 0)
        node->right = removeBranchNode(node->right, name);
    else {
        Helper *h = node->helpers_head;
        while (h) {
            Helper *t = h->next;
            free(h);
            h = t;
        }

        Neigh *n = node->adj;
        while (n) {
            Neigh *t = n->next;
            free(n);
            n = t;
        }

        if (!node->left) {
            Branch *r = node->right;
            free(node);
            return r;
        }
        if (!node->right) {
            Branch *l = node->left;
            free(node);
            return l;
        }

        Branch *temp = minBranchNode(node->right);
        strcpy(node->name, temp->name);
        node->helpers_head = temp->helpers_head;
        node->adj = temp->adj;

        node->right = removeBranchNode(node->right, temp->name);
    }

    return node;
}

/* ---------- Helper Operations ---------- */
void insertHelperToBranch(Branch *b, Helper *h) {
    Helper **pp = &b->helpers_head;
    while (*pp && (*pp)->preference_num <= h->preference_num)
        pp = &(*pp)->next;

    h->next = *pp;
    *pp = h;
}

int removeHelperFromBranch(Branch *b, int id) {
    Helper **pp = &b->helpers_head;
    while (*pp) {
        if ((*pp)->id == id) {
            Helper *t = *pp;
            *pp = t->next;
            free(t);
            return 1;
        }
        pp = &(*pp)->next;
    }
    return 0;
}

int removeHelperByID_traverse(Branch *node, int id) {
    if (!node) return 0;
    if (removeHelperFromBranch(node, id)) return 1;
    return removeHelperByID_traverse(node->left, id) ||
           removeHelperByID_traverse(node->right, id);
}

int searchHelperByID(Branch *node, int id) {
    if (!node) return 0;

    Helper *h = node->helpers_head;
    while (h) {
        if (h->id == id) {
            printf("Found in %s → ID:%d Pref:%d Sector:%d Wage:%d Timing:%d\n",
                node->name, h->id, h->preference_num, h->sector, h->wages, h->timing);
            return 1;
        }
        h = h->next;
    }

    return searchHelperByID(node->left, id) ||
           searchHelperByID(node->right, id);
}

void addEdge(Branch *from, const char *to_name) {
    Neigh *n = malloc(sizeof(Neigh));
    strcpy(n->branch_name, to_name);
    n->next = from->adj;
    from->adj = n;
}

/* ---------- Menu Option Functions ---------- */
void optionAddBranch() {
    char name[NAME_LEN];
    printf("Enter branch name: ");
    read_line(name, NAME_LEN);
    root = insertBranch(root, name);
}

void optionRemoveBranch() {
    char name[NAME_LEN];
    printf("Enter branch to remove: ");
    read_line(name, NAME_LEN);
    if (!findBranch(root, name)) {
        printf("Branch not found.\n");
        return;
    }
    root = removeBranchNode(root, name);
    printf("Branch removed.\n");
}

void optionRegisterHelpers() {
    char branchName[NAME_LEN];
    int count, pref_start, s, wages, t, extra_flag;

    printf("Enter branch: ");
    read_line(branchName, NAME_LEN);
    Branch *b = findBranch(root, branchName);
    if (!b) {
        printf("Branch not found.\n");
        return;
    }

    printf("How many helpers? ");
    scanf("%d", &count); getchar();

    printf("Starting preference number: ");
    scanf("%d", &pref_start); getchar();

    printf("Sector (1-Pack, 2-Del, 3-Sort): ");
    scanf("%d", &s); getchar();

    printf("Wages: ");
    scanf("%d", &wages); getchar();

    printf("Timing (1-Day, 2-Night): ");
    scanf("%d", &t); getchar();

    printf("Extra flag (1/0): ");
    scanf("%d", &extra_flag); getchar();

    if (extra_flag) wages += 100;

    Queue *q = createQueue();
    for (int i = 0; i < count; i++)
        enqueueID(q, nextHelperID++);

    int pref = pref_start;

    while (!queueEmpty(q)) {
        Helper *h = malloc(sizeof(Helper));
        h->id = dequeueID(q);
        h->preference_num = pref++;
        h->sector = (s == 2 ? DELIVERY : (s == 3 ? SORTING : PACKAGING));
        h->wages = wages;
        h->timing = (t == 2 ? NIGHT : DAY);
        h->extra_preference_flag = extra_flag;
        h->next = NULL;

        insertHelperToBranch(b, h);

        printf("Helper %d added to %s\n", h->id, branchName);
    }

    freeQueue(q);
}

void optionRemoveHelperByID() {
    int id;
    printf("Enter helper ID to remove: ");
    scanf("%d", &id); getchar();

    if (removeHelperByID_traverse(root, id))
        printf("Helper removed.\n");
    else
        printf("Helper not found.\n");
}

void optionViewBranches() {
    void inorder(Branch *n) {
        if (!n) return;
        inorder(n->left);
        printf("%s\n", n->name);
        inorder(n->right);
    }
    inorder(root);
}

void optionHelpersByBranch() {
    char name[NAME_LEN];
    printf("Enter branch name: ");
    read_line(name, NAME_LEN);

    Branch *b = findBranch(root, name);
    if (!b) {
        printf("Branch not found.\n");
        return;
    }

    Helper *h = b->helpers_head;
    if (!h) {
        printf("No helpers.\n");
        return;
    }

    while (h) {
        printf("ID:%d Pref:%d Wage:%d Sector:%d Timing:%d\n",
               h->id, h->preference_num, h->wages, h->sector, h->timing);
        h = h->next;
    }
}

void optionTotalHelpersDetails() {
    void inorder(Branch *n) {
        if (!n) return;
        inorder(n->left);

        int c = 0;
        Helper *h = n->helpers_head;
        while (h) { c++; h = h->next; }

        printf("%s → %d helpers\n", n->name, c);

        inorder(n->right);
    }
    inorder(root);
}

void optionSearchHelperByNumber() {
    int id;
    printf("Enter helper ID to search: ");
    scanf("%d", &id); getchar();

    if (!searchHelperByID(root, id))
        printf("Not found.\n");
}

void optionConnectTwoBranches() {
    char a[NAME_LEN], b[NAME_LEN];

    printf("Enter source: ");
    read_line(a, NAME_LEN);

    printf("Enter destination: ");
    read_line(b, NAME_LEN);

    Branch *ba = findBranch(root, a);
    Branch *bb = findBranch(root, b);

    if (!ba || !bb) {
        printf("Branch not found.\n");
        return;
    }

    addEdge(ba, bb->name);
    addEdge(bb, ba->name);

    printf("Connected %s <-> %s\n", a, b);
}
