

#include <iostream>
#include<string.h>
#include<stdio.h>
#include<malloc.h>
#include<string.h>
#include <chrono>
#include<math.h>
#include<conio.h>
#include<time.h>

#define SIZE 100 
using namespace std;
char SouRules[4][3] = {"AC","AB","BB","E"};
char DesRules[4][3] = {"E","BC","E"};
typedef struct Node_array
{
    char state[SIZE];
    struct Node_array* parent; // tree search

    int path_cost;
    int rule;
    int g;  //giá trị heuristic
    struct Node_array* nextNode; // list


} Node;

///



typedef struct NodeList {
    unsigned int nodeCount;    //the number of nodes in the list
    Node* head;            //pointer to the first node in the list
    Node* tail;            //pointer to the last node in the list
};

int is_equal(char a[], char b[])
{
    int flag;
    flag = strcmp(a, b);
    if (flag == 0)
        return 1;
    return 0;
    
}

NodeList* FIFO_initial() {
    //
    NodeList* list = new NodeList();
    list->head = list->tail = NULL;
    list->nodeCount = 0;
    return list;
}

void FIFO_add(NodeList* list, Node* node) {
    if (list->head != NULL)
    {
        list->tail->nextNode = node;
        list->tail = node;
        list->tail->nextNode = NULL;
        list->nodeCount += 1;
    }
    else
    {
        node->nextNode = NULL;
        list->head = list->tail = node;
        list->nodeCount += 1;
    }
}

void FIFO_addHead(NodeList* list, Node* node) {
    if (list->head != NULL)
    {
        node->nextNode = list->head;
        list->head = node;
        list->nodeCount += 1;
    }
    else
    {
        node->nextNode = NULL;
        list->head = list->tail = node;
        list->nodeCount += 1;
    }
}

Node* FIFO_pop(NodeList* list) {
    //
    if (list->nodeCount != 0)
    {
        Node* n = list->head;
        list->head = n->nextNode;
        list->nodeCount -= 1;
        if (list->nodeCount == 0)
        {
            list->head = list->tail = NULL;
        }
        return n;
    }
    return NULL;
}

void FIFO_AddTheoThuTuTangDan(NodeList* list, Node* node)
{
    if (list->head == NULL)
    {
        node->nextNode = NULL;
        list->head = node;
        list->tail = list->head;
        list->nodeCount += 1;
        return;
    }
    else
    {
        Node* p = list->head;
        while (p != NULL)
        {
            if (list->head == list->tail)
            {
                list->head->nextNode = node;
                list->tail = node;
                list->nodeCount++;
                return;
            }
            if (p->nextNode == NULL)
            {
                node->nextNode = NULL;
                list->tail->nextNode = node;
                list->tail = node;
                list->nodeCount += 1;
                return;
            }
            if (p->nextNode->g >= node->g)
            {
                node->nextNode = p->nextNode;
                p->nextNode = node;
                list->nodeCount++;
                return;
            }
            p = p->nextNode;
        }
    }

}

void DelNode(NodeList* list, char state[])
{
    if (list->head == NULL)
        return;
    Node* p = list->head;
    while (p != NULL)
    {
        if (list->head == list->tail)
        {
            free(p);
            list = FIFO_initial();
            return;
        }
        else if (is_equal(p->nextNode->state, state))
        {
            Node* q = p->nextNode;
            p->nextNode = p->nextNode->nextNode;
            free(q);
            list->nodeCount--;
            return;
        }
        else if (p->nextNode == list->tail)
        {
            p->nextNode = NULL;
            free(list->tail);
            list->tail = p;
            list->nodeCount--;
        }
        p = p->nextNode;
    }
}

/*int isGoal(char current[], char goal[])
{
    Node* test = current;
    
    for(int i=0;i<strlen(test->state);i++)
    {
        if(test->state[i]!=goal->state[0])
        {
            return 0;
        }
    }
    return 1;
}*/
void backString(Node* str, int point)
{
    int i = 0;
    char temp;
    for (i = point; i < strlen(str->state) - 1; i++)
    {
        temp = str->state[i + 1];
        str->state[i] = temp;
    }
    str->state[i] = '\0';
}
void rulesApply(Node* str, int point, int rulesApply)
{
    if (rulesApply == 3)//loại bỏ chữ E
    {
        backString(str, point);
    }
    else if (strlen(DesRules[rulesApply]) < 2)//biến thành chữ E
    {
        str->state[point] = DesRules[rulesApply][0];
        backString(str, point + 1);
    }
    else if (strlen(DesRules[rulesApply]) == 2)//thay thế bằng 1 cặp
    {
        str->state[point] = DesRules[rulesApply][0];
        str->state[point+1] = DesRules[rulesApply][1];
    }
    
}
Node* ChildNode(Node* current,int point, int ruleApply)
{
    Node* Child = new Node();
    strcpy_s(Child->state, current->state);
    Child->rule= ruleApply;
    Child->path_cost = current->path_cost + 1;
    //Child->g = strlen(Child->state) - tongSoE(Child->state) + Child->path_cost
    Child->parent = current;
    Child->nextNode = NULL;
    rulesApply(Child, point, ruleApply);
    return Child;
}
void Solution(Node* node) {
    //
    Node* current = node;
    Node* temp = new Node();
    NodeList* path = FIFO_initial();
    if (current->parent == NULL)
    {
        cout << "\nStart is Goal\n";
        return;
    }
    while (current != NULL)
    {
        FIFO_addHead(path, current);
        current = current->parent;
    }
    current = path->head;
    cout << "RuleApply: \t\t ";
    cout << current->state << "\n";
    current = current->nextNode;
    while (current != NULL)
    {
        if (current->rule != 3)
        {
            cout << "RuleApply: ";
            cout << SouRules[current->rule] << "->" << DesRules[current->rule] << "\t ";
            cout << current->state << "\n";
            current = current->nextNode;
        }
        else {
            cout << "RuleApply: ";
            cout << SouRules[current->rule] << "->" << "\t" << "\t ";
            cout << current->state << "\n";
            current = current->nextNode;
        }
    }
}
bool checkExist(NodeList* list, Node* node) 
{
    //
    Node* n = list->head;
    while (n != NULL)
    {
        if (n == NULL)
        {
            return false;
        }
        if (strcmp(n->state, node->state) == 0)
        {
            return true;
        }
        n = n->nextNode;
    }
    return false;
}

void breadthFirstSearch(Node* root, Node* goal) {
    NodeList* open = FIFO_initial();
    NodeList* close = FIFO_initial();
    char temp[3];
    FIFO_add(open, root);
    while (open->nodeCount > 0)
    {
        Node* current = FIFO_pop(open);
        FIFO_add(close, current);
        if (strcmp(current->state,goal->state) == 0)
        {
            cout << "\nPath: \n";
            Solution(current);
            return;
        }
        //successor ở đây!!
        for (int i = 0; i < strlen(current->state) - 1; i++)
        {
            
            //Chạy thuật toán i->strlen
            //để rule 1 || 2 chạy if SouRules[0||1][0] (A) == current->state[i] 
                                //if SouRules[0||1][1] == current->state[i+1]
                                //node Child=ChildNode(current,i,0||1)
            if (current->state[i] == 'E' && i < strlen(current->state)-1)
            {
                Node* child = ChildNode(current, i, 3);
                if (!checkExist(close, child) && !checkExist(open, child))
                {
                    FIFO_add(open, child);
                }
            }
            temp[0] = current->state[i];
            temp[1] = current->state[i + 1];
            temp[2] = '\0';
            for (int j = 0; j < 3; j++)
            {
                if (strcmp(temp, SouRules[j]) == 0)
                {
                    Node* child = ChildNode(current, i, j);
                    if (!checkExist(close, child) && !checkExist(open, child))
                    {
                        FIFO_add(open, child);
                    }
                    break;
                }
            }
            
        }
        //tới đây
    }
    cout << "Không có hướng giải chuỗi này thành E!";
}

int main()
{
    Node* Goal, * Start;
    Goal = (Node*)malloc(sizeof(Node));
    Start = (Node*)malloc(sizeof(Node));
    Goal->state[0] = 'E';
    Goal->state[1] = '\0';
    std::cout << "Goal string: "<<Goal->state << "\nInput start string: \n";
    scanf_s("%s", Start->state,SIZE-1);

    Start->rule = NULL;
    Start->path_cost = 0;
    Start->parent = NULL;
    Start->nextNode = NULL;

    //BFS
    cout << "BFS: \n";
    clock_t start = clock();
    breadthFirstSearch(Start, Goal);
    clock_t finish = clock();
    double duration = (double)(finish - start) / CLOCKS_PER_SEC;
    printf("Thoi gian thuc thi: %.5lf", duration);
    
}

