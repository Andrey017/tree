#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct node{
    struct node *parent;
    struct node *right;
    struct node *left;
    int number;
} node;

node* createroot(int number);
int addnode(int number, node* current, int target, char type, int launchflag);
void checkprint(node *tree, int n);
void printmas(node *tree);
void statistics(node *tree);
int maximaldepth(node *tree, int n);
int fisheap(node *tree, int curdepth);
void writenode(node *tree, FILE *file);
void loadnode(node *tree, FILE *file);
void removetree(node *tree);
int removeelement(node *tree, int numtos, char typedel);

#define PARENT 1
#define SIBLING 2
#define SELF 3
#define DESCENDANTS 4
#define FAMILY 5

int leavesum, internalnum, nodesnum, isheap, maxdepth;


int main(int argc, char *argv[])
{
    int i, numtow, numtos;
    node *tree = 0;
    FILE *file = 0;
    char command[500],flag = 8, *c=0;

    if (argc > 1){
        if ((file = fopen(argv[1], "rb")) == 0)
            printf("Error opening\n");
        else {
            fread(&numtow, sizeof(int), 1, file);
            tree = createroot(numtow);
            loadnode(tree, file);
            fclose(file);
        }
    }

    else
        printf("No tree was loaded from command line arguments\n");

    while (flag){
        leavesum = internalnum = nodesnum = maxdepth = 0;
        statistics(tree);
        maxdepth = maximaldepth(tree, 0);
        isheap = fisheap(tree, 0);
        //checkprint(tree, 0);
        //printf("Numbers of nodes: %d. \nNumbers of internal nodes: %d.\nNumber of leaves: %d.Maximal depth of tree: %d.\n", nodesnum,internalnum, leavesum, maxdepth);
        //if (isheap) printf("The tree is not binary heap\n");
        //else printf("The tree is binary heap\n");
        printf("Enter command: \n");
        scanf("%s", command);

        if (!strcmp(command, "add")){
            scanf("%s", command);
            if (!strcmp(command, "root")){
                if (tree == 0){
                    flag = 10;
                    scanf("%s", command);
                    i=0;
                    while (command[i]){
                        if ((!(command[i]>='0'))||(!(command[i]<='9')))
                            flag = 2;
                        ++i;
                    }

                    if (flag == 2){
                        printf("Not correct number\n");
                    }
                    else if (flag == 10){
                        sscanf(command, "%d", &numtow);
                        tree = createroot(numtow);
                    }
                    flag = 8;
                }
                else
                    printf("Root already exists\n");
            }

            else if (!strcmp(command, "node")){
                flag = 10;
                scanf("%s", command);
                i = 0;
                while (command[i]){
                    if ((!(command[i]>='0'))||(!(command[i]<='9')))
                        flag = 2;
                    ++i;
                }

                if (flag == 2){
                    printf("Not correct numbers\n");
                }
                else if (flag == 10){
                    sscanf(command, "%d", &numtow);
                    scanf("%s", command);
                    if (!strcmp(command, "parent")){
                        scanf("%s", command);
                        if (!strcmp(command, "root")){
                            if (tree){
                                if (!tree->left){
                                    tree->left = createroot(numtow);
                                    tree->left->parent = tree;
                                }
                                else if (!tree->right){
                                    tree->right = createroot(numtow);
                                    tree->right->parent = tree;
                                }
                                else
                                    printf("%s", "Root has no free descendants\n");
                            }
                            else
                                printf("There is no root\n");
                        }
                        else{
                            flag = 10;
                            i = 0;
                            while (command[i]){
                                if ((!(command[i]>='0'))||(!(command[i]<='9')))
                                    flag = 2;
                                ++i;
                            }
                            if (flag == 2){
                                printf("Not correct number\n");
                            }
                            else if (flag == 10){
                                sscanf(command, "%d", &numtos);
                                if (addnode(numtow, tree, numtos, PARENT, 1));
                                else printf("No element with numbers %d or it already has 2 descendants\n", numtos);
                            }
                        }
                    }
                    else if (!strcmp(command, "sibling")){
                        scanf("%s", command);
                        flag = 10;
                        i = 0;
                        while (command[i]){
                            if ((!(command[i]>='0'))||(!(command[i]<='9')))
                                flag = 2;
                            ++i;
                        }
                        if (flag == 2){
                            printf("Not correct numbers\n");
                        }
                        else if (flag == 10){
                            sscanf(command, "%d", &numtos);
                            if (addnode(numtow, tree, numtos, SIBLING, 1));
                            else printf("No element with number %d or it already has a sibling\n", numtos);
                        }
                    }
                    else
                        printf("%s", "Wrong command\n");
                    flag = 8;
                }
                flag = 8;
            }
            else
                printf("%s", "Wrong commad\n");
        }

        else if (!strcmp(command, "rem")){
            scanf("%s", command);
            if (!strcmp(command, "whole")){
                removetree(tree);
                tree = 0;
                printf("The tree was delete\n");
            }
            else if (!strcmp(command, "node")){
                flag = 10;
                scanf("%s", command);
                i = 0;
                while (command[i]){
                    if ((!(command[i]>='0'))||(!(command[i]<='9')))
                        flag = 2;
                    ++i;
                }
                if (flag == 2){
                    printf("Not correct number");
                }
                else if (flag == 10){
                    sscanf(command, "%d", &numtos);
                    scanf("%s", command);
                    if (!strcmp(command, "self")){
                        if (!removeelement(tree, numtos, SELF)) printf("Element not found or has descendant\n");
                    }
                    else if (!strcmp(command, "sibling")){
                        if (!removeelement(tree, numtos, SIBLING)) printf("Element not found, has no sibling\n");
                    }
                    else if (!strcmp(command, "descendants")){
                        if (!removeelement(tree, numtos, DESCENDANTS)) printf("Element not found\n");
                    }
                    else if (!strcmp(command, "family")){
                        if (!removeelement(tree, numtos, FAMILY)) printf("Element not found");
                    }
                    else
                        printf("%s", "Wrong command\n");
                }
                else
                    printf("UNKNOWN ERROR\n");
                flag = 8;
            }
            else printf("%s", "Wrong command\n");
        }
        else if (!strcmp(command, "save")){
            fgets(command, sizeof(command), stdin);
            c = command;
            while((*(c))!='\n') ++c;
            *c = '\0';
            if ((file = fopen(command, "wb"))!= 0){
                writenode(tree, file);
                fclose(file);
            }
            else
                printf("Error creating file\n");
        }
        else if (!strcmp(command, "load")){
            fgets(command, sizeof(command), stdin);
            c = command;
            while ((*(c))!='\n') ++c;
            *c = '\0';
            if ((file = fopen(command, "rb"))!= 0){
                if (tree){
                    removetree(tree);
                    tree = 0;
                }
                fread(&numtow, sizeof(int), 1, file);
                tree = createroot(numtow);
                loadnode(tree, file);
                statistics(tree);
                maxdepth = maximaldepth(tree, 0);
                fclose(file);
            }
            else
                printf("File not\n");
                checkprint(tree, 0);
                printf("Numbers of nodes: %d. \nNumbers of internal nodes: %d.\nNumber of leaves: %d.\nMaximal depth of tree: %d.\n", nodesnum,internalnum, leavesum, maxdepth);

        }
        else if (!strcmp(command, "help")){
            printf("HELP");
            fflush(stdin);
            getchar();
        }

        else if (!strcmp(command, "show")){
            checkprint(tree, 0);
            printf("Numbers of nodes: %d. \nNumbers of internal nodes: %d.\nNumber of leaves: %d.Maximal depth of tree: %d.\n", nodesnum,internalnum, leavesum, maxdepth);

        }
        else if (!strcmp(command, "exit"))
            return 0;
        else{
            printf("%s", "Wrong command\n");
        }
        fflush(stdin);
    }

    return 0;
}

int removeelement(node *tree, int numtos, char typedel){
    static int flag;
    int tmp = 0;
    if (tree){
        if (typedel == SELF){
            if (!flag)
                if (tree->left){
                    if (tree->left->number == numtos){
                        if ((tree->left->left == 0)&&(tree->left->right ==0)){
                            tmp = flag = 1;
                            free(tree->left);
                            tree->left = 0;
                        }
                    }
                }
            if (!flag)
                if (tree->right){
                    if (tree->right->number == numtos){
                        if ((tree->right->left == 0)&&(tree->right->right == 0)){
                            tmp = flag = 1;
                            free(tree->right);
                            tree->right = 0;
                        }
                    }
                }
        }

        else if (typedel == SIBLING){
            if (!flag)
                if (tree->left){
                    if (tree->left->number == numtos){
                        if (tree->right)
                            if ((tree->right->left == 0)&&(tree->right->right == 0)){
                                tmp = flag = 1;
                                free(tree->right);
                                tree->right = 0;
                            }
                    }
                }
            if (!flag)
                if (tree->right){
                    if (tree->right->number == numtos){
                        if (tree->left)
                            if ((tree->left->left == 0)&&(tree->left->right == 0)){
                                tmp = flag = 1;
                                free(tree->left);
                                tree->left = 0;
                            }
                    }
                }
        }

        else if (typedel == DESCENDANTS){
            if (!flag)
                if (tree->left){
                    if (tree->left->number == numtos){
                        if ((tree->left->left)){
                            removetree(tree->left->left);
                            tree->left->left = 0;
                        }
                        if ((tree->left->right)){
                            removetree(tree->left->right);
                            tree->left->right = 0;
                        }
                        tmp = flag = 1;
                    }
                }
            if (!flag)
                if (tree->right){
                    if (tree->right->number == numtos){
                        if ((tree->right->left)){
                            removetree(tree->right->left);
                            tree->right->left = 0;
                        }
                        if ((tree->right->right)){
                            removetree(tree->right->right);
                            tree->right->right = 0;
                        }
                        tmp = flag = 1;
                    }
                }
        }

        else if (typedel == FAMILY){
            if (!flag)
                if (tree->left){
                    if (tree->left->number == numtos){
                        removetree(tree->left);
                        tree->left = 0;
                        tmp = flag =1;
                    }
                }
            if (!flag)
                if (tree->right){
                    if (tree->right->number == numtos){
                        removetree(tree->right);
                        tree->right = 0;
                        tmp = flag = 1;
                    }
                }
        }

        else
            printf("Wrong 3 parameter of removeelemnt\n");
    }
    else
        return 0;
    if (!flag) tmp = removeelement(tree->left, numtos, typedel);
    if (!flag) tmp = removeelement(tree->right, numtos, typedel);
    if (!tree->parent) flag =0;
    return tmp;
}

void removetree(node *tree){
    if (!tree) return;
    if (tree->left)
        removetree(tree->left);
    if (tree->right)
        removetree(tree->right);
    free(tree);
}

void loadnode(node *tree, FILE *file){
    int targetn, number;
    if (!fread(&targetn, sizeof(int), 1, file)) return;
    if (!fread(&number, sizeof(int), 1, file)) return;
    addnode(number, tree, targetn, PARENT, 1);
    loadnode(tree, file);
}

void writenode(node *tree, FILE *file){
    if (tree->parent){
        fwrite(&tree->parent->number, sizeof(int), 1, file);
        fwrite(&tree->number, sizeof(int), 1, file);
    }
    else {
        fwrite(&tree->number, sizeof(int), 1, file);
    }
    if (tree->left) writenode(tree->left, file);
    if (tree->right) writenode(tree->right, file);
}

int fisheap(node *tree, int curdepth){
    extern int maxdepth;
    static int flag, flage1;
    int tmp = 0;
    ++curdepth;
    if (tree){
        if ((!flag))
            if (tree->parent)
                if ((tree->parent->number)<(tree->number))
                    flag = tmp = 1;
        if (!flag)
            if ((curdepth+1)<(maxdepth)){
                if (!((tree->left)&&(tree->right)))
                    tmp = flag = 1;
            }
        if (curdepth == maxdepth)
            if (flage1)
                tmp = flag = 1;
        if (!flag)
            tmp = fisheap(tree->left, curdepth);

        if (!flag)
            tmp = fisheap(tree->right, curdepth);
    }

    else{
        if (!flag)
            if (curdepth == maxdepth)
                flage1 = 1;
    }
    if (curdepth == 1){
        flag = 0;
        flage1 = 0;
    }
    return tmp;
}

int maximaldepth(node *tree, int n){
    int a = 0, b = 0;
    if (tree){
        ++n;
        a = maximaldepth(tree->left, n);
        b = maximaldepth(tree->right, n);
        if (a>n) n=a;
        if (b>n) n=b;
    }

    return n;
}

void statistics(node *tree){
    extern int leavesum, internalnum, nodesnum;
    if (tree){
        ++nodesnum;
        if ((tree->right)||(tree->left))
            ++internalnum;
        else
            ++leavesum;
        if (tree->right)
            statistics(tree->right);
        if (tree->left)
            statistics(tree->left);
    }
}

node* createroot(int number){
    node *temp;
    temp = (node*)malloc(sizeof(node));
    temp->left = temp->right = temp->parent = 0;
    temp->number = number;
    return temp;
}

int addnode(int number, node *current, int target, char type, int launchflag){
    static int flag;
    int ntmp = 0;
    node *temp;

    if (flag) return 1;
    if (!current){
        printf("Node is empty\n");
        return 0;
    }

    if (type == PARENT){
        if (current->number == target){
            if (current->left == 0){
                temp = (node*)malloc(sizeof(node));
                temp->left = temp->right = 0;
                temp->parent = current;
                current->left = temp;
                temp->number = number;
                flag = ntmp = 1;
            }
            else if (current->right == 0){
                temp = (node*)malloc(sizeof(node));
                temp->left = temp->right = 0;
                temp->parent = current;
                current->right = temp;
                temp->number = number;
                flag = ntmp = 1;
            }
        }

        if (!flag){
            if (current->left){
                if (current->right){
                    ntmp = addnode(number, current->left, target, PARENT, 0);
                    if (!flag) ntmp = addnode(number, current->right, target, PARENT, 0);
                }
                else {
                    ntmp = addnode(number, current->left, target, PARENT, 0);
                }
            }
            else {
                if (current->right)
                    ntmp = addnode(number, current->right, target, PARENT, 0);
            }
        }
    }

    else if (type == SIBLING){
        if (current->left){
            if (current->right){
                ntmp = addnode(number, current->left, target, SIBLING, 0);
                if (!flag) ntmp = addnode(number, current->right, target, SIBLING, 0);
            }
            else{
                if (current->left->number == target){
                    temp = (node*)malloc(sizeof(node));
                    temp->left = temp->right = 0;
                    temp->parent = current;
                    current->right = temp;
                    temp->number = number;
                    flag = ntmp = 1;
                }
                else{
                    ntmp = addnode(number, current->left, target, SIBLING, 0);
                }
            }
        }
        else if (current->right){
            if (current->left){
                ntmp = addnode(number, current->left, target, SIBLING, 0);
                if (!flag) ntmp = addnode(number, current->right, target, SIBLING, 0);
            }
            else {
                if (current->right->number == target){
                    temp = (node*)malloc(sizeof(node));
                    temp->left = temp->right = 0;
                    temp->parent = current;
                    current->left = temp;
                    temp->number = number;
                    flag = ntmp = 1;
                }
                else {
                    ntmp = addnode(number, current->right, target, SIBLING, 0);
                }
            }
        }
        else
            return 0;
    }
    else {
        printf("%s", "Wrong 4 argument of addnode\n");
        return 0;
    }
    if ((flag)&&(launchflag)) flag = 0;
    return (ntmp);
}

void printmas(node *tree){
    if (tree){
        printmas(tree->left);
        printf("%d", tree->number);
        printmas(tree->right);
    }
}

void checkprint(node *tree, int n){
    int i;
    if (tree){
        checkprint(tree->right, n+3);
        for (i=0; i<n; i++)
            putchar(' ');
        printf("%d\n", tree->number);
        checkprint(tree->left, n+3);
    }
}
