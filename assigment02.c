#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX 1000

int main() {
    char input[MAX];

    printf("Enter tree: ");
    scanf("%999s", input);

    int len = strlen(input);

    /* ---------------------------
       1. Basic validation
       --------------------------- */

    if (len == 0 || !isupper(input[0])) {
        printf("Invalid tree\n");
        return 0;
    }

    int balance = 0;
    int nodeCount = 0;
    int leafCount = 0;
    int nonLeafCount = 0;
    int maxDegree = 0;
    int maxDepth = 0;

    /*
       nodeStack:
       현재 노드의 부모 관계를 기억
    */
    char nodeStack[MAX];
    int nodeTop = -1;

    /*
       counterStack:
       각 노드의 자식 수를 기억
    */
    int counterStack[MAX];
    int counterTop = -1;

    /*
       C node information
    */
    char cParent = '-';
    char cChildren[MAX];
    int cChildCount = 0;

    /*
       depth information for tree printing
    */
    int depth = 0;

    for (int i = 0; i < len; i++) {

        char ch = input[i];

        /* ---------------------------
           Node
           --------------------------- */
        if (isupper(ch)) {

            nodeCount++;

            /*
               현재 노드의 parent
            */
            if (nodeTop >= 0) {
                char parent = nodeStack[nodeTop];

                if (ch == 'C') {
                    cParent = parent;
                }

                /*
                   부모의 child count 증가
                */
                if (counterTop >= 0) {
                    counterStack[counterTop]++;
                }
            }

            /*
               새로운 노드를 stack에 push
            */
            nodeTop++;
            nodeStack[nodeTop] = ch;

            /*
               새로운 노드의 child counter
            */
            counterTop++;
            counterStack[counterTop] = 0;

            /*
               현재 depth
            */
            if (depth > maxDepth) {
                maxDepth = depth;
            }
        }

        /* ---------------------------
           Open parenthesis
           --------------------------- */
        else if (ch == '(') {

            balance++;

            /*
               '(' 바로 앞의 노드가
               child를 가질 예정
            */
        }

        /* ---------------------------
           Comma
           --------------------------- */
        else if (ch == ',') {

            /*
               sibling separator
               별도의 작업은 필요 없음
            */
        }

        /* ---------------------------
           Close parenthesis
           --------------------------- */
        else if (ch == ')') {

            if (balance <= 0) {
                printf("Invalid tree\n");
                return 0;
            }

            balance--;

            /*
               현재 노드의 child 수
            */
            if (counterTop >= 0) {

                int degree = counterStack[counterTop];

                if (degree > 0)
                    nonLeafCount++;
                else
                    leafCount++;

                if (degree > maxDegree)
                    maxDegree = degree;

                counterTop--;
            }

            /*
               현재 노드를 stack에서 제거
            */
            if (nodeTop >= 0)
                nodeTop--;

            if (depth > 0)
                depth--;
        }
    }

    /* ---------------------------
       Validation
       --------------------------- */

    if (balance != 0 ||
        nodeTop != 0 ||
        counterTop != 0) {

        printf("Invalid tree\n");
        return 0;
    }

    /*
       Root node 처리
       root은 ')'가 없기 때문에
       마지막에 따로 처리
    */
    if (counterTop >= 0) {

        int degree = counterStack[counterTop];

        if (degree > 0)
            nonLeafCount++;
        else
            leafCount++;

        if (degree > maxDegree)
            maxDegree = degree;
    }

    /* ---------------------------
       C children 찾기
       --------------------------- */

    /*
       다시 문자열을 scan하여
       C의 바로 다음 child들을 찾음
    */

    int cIndex = -1;

    for (int i = 0; i < len; i++) {
        if (input[i] == 'C') {
            cIndex = i;
            break;
        }
    }

    if (cIndex != -1) {

        /*
           C 다음에 '('가 있으면
           C는 child를 가짐
        */
        if (cIndex + 1 < len && input[cIndex + 1] == '(') {

            int i = cIndex + 2;

            while (i < len && input[i] != ')') {

                if (isupper(input[i])) {

                    cChildren[cChildCount++] = input[i];

                    /*
                       child node의 subtree를
                       건너뛰기 위해 검사
                    */
                    if (i + 1 < len && input[i + 1] == '(') {

                        int b = 0;
                        i++;

                        while (i < len) {

                            if (input[i] == '(')
                                b++;

                            else if (input[i] == ')') {
                                b--;

                                if (b == 0)
                                    break;
                            }

                            i++;
                        }
                    }
                }

                i++;
            }
        }
    }

    /* ---------------------------
       Output
       --------------------------- */

    printf("\n");
    printf("Total nodes: %d\n", nodeCount);
    printf("Leaf nodes: %d\n", leafCount);
    printf("Non-leaf nodes: %d\n", nonLeafCount);
    printf("Tree height: %d\n", maxDepth);
    printf("Tree degree: %d\n", maxDegree);

    printf("Parent of C: ");

    if (cParent == '-')
        printf("None\n");
    else
        printf("%c\n", cParent);

    printf("Children of C: ");

    if (cChildCount == 0) {
        printf("None\n");
    } else {

        for (int i = 0; i < cChildCount; i++) {
            printf("%c", cChildren[i]);

            if (i != cChildCount - 1)
                printf(", ");
        }

        printf("\n");
    }

    /* ---------------------------
       Tree visualization
       --------------------------- */

    printf("\nTree:\n");

    int currentDepth = 0;

    for (int i = 0; i < len; i++) {

        if (isupper(input[i])) {

            /*
               indentation
            */
            for (int j = 0; j < currentDepth; j++)
                printf("    ");

            if (i == 0)
                printf("%c\n", input[i]);
            else
                printf("+---%c\n", input[i]);
        }

        else if (input[i] == '(') {
            currentDepth++;
        }

        else if (input[i] == ')') {
            currentDepth--;
        }
    }

    return 0;
}