#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define MAXWORD 100

typedef struct Node{
    char *data;
    int index;
    struct Node *NextRow;
} Node;

typedef struct Col{
    char *ColName;
    Node *p;
} Col;

Col * CreateTable(int c){
    Col *temp = (Col *)malloc(c * sizeof(Col));
    for(int i=0; i<c; i++){
        temp[i].p = NULL;
    }
    return temp;
}

void ReadNames(Col *table, int c){
    puts("");
    for(int i=0; i<c; i++){
        printf("Enter the name of column No. %d: ", i+1);
        char name[MAXWORD];
        scanf("%s", name);
        table[i].ColName = strdup(name);
    }
    puts("");
}

void FreeNode(Node *p){
    if(p != NULL){
        if(p->NextRow){
            FreeNode(p->NextRow);
        }
        free(p);
    }
}

void FreeTable(Col *table, int num){
    for(int i=0; i<num; i++){
        if(table[i].p != NULL){
            FreeNode(table[i].p);
        }
    }
    free(table);
}

void insert(Node **head, char *data, int idx){
    if(*head == NULL){
        *head = (Node *)malloc(sizeof(Node));
        (*head)->data = strdup(data);
        (*head)->index = idx;
        (*head)->NextRow = NULL;
    }else{
        insert(&(*head)->NextRow, data, idx);
    }
}

void NumOfRows(Node *head, int *n){
    if(head != NULL){
        (*n)++;
        NumOfRows(head->NextRow, n);
    }
}

void PrintColumn(Node *head){
    if(head != NULL){
        printf("%d\t%s\n",head->index, head->data);
        PrintColumn(head->NextRow);
    }
}

void PrintRow(Col *table, int NumberOfColumns, FILE *f, int RowIndex){
    for(int i=0; i < NumberOfColumns && table[i].p != NULL; i++){
        Node *current = table[i].p;
        while(current->index != RowIndex && current->NextRow != NULL){
            current = current->NextRow;
        }
        fprintf(f, "%s\t", current->data);
    }
    fprintf(f, "\n");
}

void PrintTable(Col *table, int c){
    FILE *outputFile = fopen("table.txt", "w");

    for(int i=0; i<c; i++){
        fprintf(outputFile, "%s\t", table[i].ColName);
    }fprintf(outputFile, "\n");

    int n=0;
    NumOfRows(table[0].p, &n);
    for(int i=1; i<=n; i++){
        PrintRow(table, c, outputFile, i);
    }

    fclose(outputFile);
}

int main(){
    // int c;
    // printf("Enter the number of columns: ");
    // scanf("%d", &c);

    Col *table = CreateTable(2);
    ReadNames(table, 2);

    insert(&(table[0].p), "A", 1);
    insert(&(table[0].p), "A-", 2);
    insert(&(table[0].p), "B+", 3);
    insert(&(table[1].p), "95", 1);
    insert(&(table[1].p), "90", 2);
    insert(&(table[1].p), "87", 3);

    //PrintColumn(table[0].p);
    PrintTable(table, 2);
    puts("Table was printed to the file \"Data.txt\"\n");

    FreeTable(table, 2);
    return 0;
}