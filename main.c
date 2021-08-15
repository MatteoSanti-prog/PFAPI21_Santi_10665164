#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define INF -1
#define CONST 3000
//structure of a graph
typedef struct {
    unsigned int order;//it represents the number of 'arrival' of the graph
    unsigned int path;//it represents the sum of all minimum spanning trees
    struct Matrix *next;//pointer to the next element of the list (it's ordered from minimum path to the maximum one)
} Matrix;

//pointer to an element of the graphList
typedef Matrix *Pointer;

//structure of a node
typedef struct{
    unsigned int node;//it represents the index of the node
    unsigned int distance;//it represents the current distance of the node from node 0
    struct Node *next;//pointer to the next element of the list
}Node;
//pointer to an element of the nodeList
typedef Node *PointerNode;

//this function is a user-defined atoi
unsigned int conversion(const char string[]){
    unsigned int temp=0;
    for(int i=0;string[i]!='\0';i++){
        temp=temp*10+string[i]-'0';
    }
    return temp;
}

//this function parses strings into integers
unsigned int parseInt(unsigned int matrix[], const char string[], unsigned int count1, unsigned int d){
    unsigned int allZero=0;
    char temp[10];
    unsigned int i=0;
    unsigned int counter;
    unsigned int element=0;
    for(counter=0;string[counter]!='\0';counter++){
        if(string[counter]==',' || string[counter]=='\n'){
            temp[i]='\0';
            matrix[(count1*d)+element]=conversion(temp);
            if(count1==0 && element!=0 && matrix[(count1*d)+element]==0){
                allZero++;
            }
            element++;
            i=0;
            continue;
        }
        else{
            temp[i]=string[counter];
            i++;
        }
    }
    return allZero;
}

//this function initializes matrix: its parameters are the returned matrix and its dimension
int initializeMatrix(unsigned int matrix[], unsigned int d){
    unsigned int count1;
    int temp=1;
    char string[CONST];
    for(count1=0;count1<d;count1++){
        if(fgets(string,CONST,stdin)==NULL){
            exit(1);
        }
        if(string[0]=='\n'){
            count1--;
            continue;
        }
        if(parseInt(matrix, string, count1, d)==d && count1==0){
            temp=0;
        }
    }
    return temp;
}


//this function initializes the ranking of graphs
void initializeRanking(Pointer *ranking){
    *ranking=NULL;
}

//this function initializes the list of all nodes
void initializeNode(PointerNode *listOfNodes){
    *listOfNodes=NULL;
}

//this function inserts a new node in the list
void insertNode(PointerNode *listOfNodes, unsigned int i){
    Node *punt;
    punt=malloc(sizeof(Node));
    punt->node=i;
    if(i==0){
        punt->distance=0;
    }else{
        punt->distance=INF;
    }
    punt->next= (struct Node *) *listOfNodes;
    *listOfNodes=punt;
}

//this function inserts a new graph in base of its minimum path's length
void insertInOrder(Pointer *ranking, unsigned int order, unsigned int path, unsigned int k){
    unsigned int counter=0;
    Matrix *punt, *currentPunt, *previousPunt;
    previousPunt=NULL;
    currentPunt=*ranking;
    while(currentPunt!=NULL && path>currentPunt->path && counter<k){
        previousPunt=currentPunt;
        currentPunt=(Matrix*)currentPunt->next;
        counter++;
    }
    while(currentPunt!=NULL && path==currentPunt->path && order>currentPunt->order && counter<k){
        previousPunt=currentPunt;
        currentPunt=(Matrix*)currentPunt->next;
        counter++;
    }
    if(counter<k){
        punt=malloc(sizeof(Matrix));
        punt->path=path;
        punt->next= (struct Matrix *) currentPunt;
        punt->order=order;
        if(previousPunt!=NULL){
            previousPunt->next= (struct Matrix *) punt;
        }
        else{
            *ranking=punt;
        }
    }
}


//this function returns the node with minimum distance which is the first of the list.

void deleteElement(PointerNode *listOfNodes){
    if(*listOfNodes!=NULL){
        Node *punt;
        punt=*listOfNodes;
        *listOfNodes= (PointerNode) (*listOfNodes)->next;
        free(punt);
    }
}

//this function checks if there are no more nodes
int emptyList(PointerNode *listOfNodes){
    if(*listOfNodes==NULL){
        return 1;
    }
    else{
        return 0;
    }
}

void deleteNode(PointerNode *listOfNodes, unsigned int node){
    Node *punt;
    if(*listOfNodes!=NULL){
        if((*listOfNodes)->node==node){
            punt=*listOfNodes;
            *listOfNodes= (PointerNode) (*listOfNodes)->next;
            free(punt);
        }
        else{
            deleteNode((PointerNode *) &((*listOfNodes)->next), node);
        }
    }
}

void updateDistance(PointerNode *listOfNodes, unsigned int node, unsigned int newDistance){
    Node *punt, *currentPunt, *prevPunt;
    prevPunt=NULL;
    currentPunt=*listOfNodes;
    while(currentPunt!=NULL && newDistance>currentPunt->distance){
        prevPunt=currentPunt;
        currentPunt= (Node *) currentPunt->next;
    }
    punt=malloc(sizeof(Node));
    punt->node=node;
    punt->distance=newDistance;
    punt->next= (struct Node *) currentPunt;
    if(prevPunt!=NULL){
        prevPunt->next= (struct Node *) punt;
    }
    else{
        *listOfNodes=punt;
    }
    deleteNode((PointerNode *) &(punt->next), node);
}

//this function returns the sum of all minimum paths, and it implements Dijkstra's algorithm
unsigned long long int graphCost(unsigned const int matrix[], unsigned int d, PointerNode listOfNodes, unsigned int distances[]){
    unsigned int j;//generic node
    unsigned int sum=0;
    unsigned int chosen;//node with minimum distance
    for(j=0;j<d;j++){
        insertNode(&listOfNodes,d-j-1);
        if(j==0){
            distances[j]=0;
        }
        else{
            distances[j]=INF;
        }
    }
    while(emptyList(&listOfNodes)==0){
        chosen=listOfNodes->node;//node with minimum distance from node 0
        deleteElement(&listOfNodes);
        if(distances[chosen]==INF){
            sum+=0;
            break;
        }
        else{
            sum+=distances[chosen];
        }
        if(emptyList(&listOfNodes)==0){
            for(j=0;j<d;j++){//here we are considering each element of the matrix's row associated to the chosen node starting from 1 because we don't care about node 0
                if(matrix[d*chosen+j]!=0 && chosen!=j){//here I consider only the node that are reachable from the chosen node
                    if(distances[chosen]+matrix[d*chosen+j]<distances[j]){
                        distances[j]=distances[chosen]+matrix[d*chosen+j];
                        updateDistance(&listOfNodes, j, distances[chosen]+matrix[d*chosen+j]);
                    }
                }
            }
        }
    }
    while(emptyList(&listOfNodes)==0){
        deleteElement(&listOfNodes);
    }
    return sum;
}

//this function displays the current ranking
void printRanking(Pointer *ranking, unsigned int k){
    if(ranking!=NULL){
        int counter=0;
        Matrix* punt=*ranking;
        if(punt!=NULL && counter<k){
            printf("%d", punt->order);
            punt=(Matrix *)punt->next;
            counter++;
            while(punt!=NULL && counter<k){
                printf(" %d", punt->order);
                punt=(Matrix *)punt->next;
                counter++;
            }
        }
        printf("\n");
    }
}

//this function free the last remaining elements of the ranking
void freeMem(Pointer *ranking){
    Pointer punt;
    while(*ranking!=NULL){
        punt=*ranking;
        *ranking= (Pointer) (*ranking)->next;
        free(punt);
    }
}

int main() {
    unsigned int counter=0;//number of graphs read before the current one
    unsigned int d, k;//d=number of nodes, k=length of the ranking
    char command[15];//array used to store the command
    //unsigned int* matrix;//use malloc in order to initialize the matrix because i don't know its dimension at compile time
    Pointer ranking;//ranking ordered by minimum path
    PointerNode listOfNodes;
    if(scanf("%i %i", &d, &k)==EOF){
        exit(1);
    }
    unsigned int matrix[d*d];
    unsigned int distances[d];
    initializeRanking(&ranking);
    initializeNode(&listOfNodes);
    while(scanf("%s", command)!=EOF){
        if(strcmp(command,"AggiungiGrafo")==0){
            if(d!=0) {
                if(initializeMatrix(matrix, d)==0){//initialization of the matrix
                    insertInOrder(&ranking, counter, 0, k);
                }
                else{
                    insertInOrder(&ranking, counter, graphCost(matrix, d, listOfNodes, distances), k);
                }
                counter++;
            }
        }
        else if(strcmp(command,"TopK")==0){
            printRanking(&ranking, k);
        }
    }
    freeMem(&ranking);
    free(listOfNodes);
    return 0;
}
