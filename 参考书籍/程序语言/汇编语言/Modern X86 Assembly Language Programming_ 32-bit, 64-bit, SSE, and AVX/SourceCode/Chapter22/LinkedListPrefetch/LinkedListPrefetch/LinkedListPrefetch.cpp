#include "stdafx.h"
#include "LinkedListPrefetch.h"
#include <stdlib.h>
#include <math.h>
#include <stddef.h>

bool LlCompare(int num_nodes, LlNode* l1, LlNode* l2, LlNode* l3, int* node_fail)
{
    const double epsilon = 1.0e-9;

    for (int i = 0; i < num_nodes; i++)
    {
        *node_fail = i;

        if ((l1 == NULL) || (l2 == NULL) || (l3 == NULL))
            return false;

        for (int j = 0; j < 4; j++)
        {
            bool b12_c = fabs(l1->ValC[j] - l2->ValC[j]) > epsilon;
            bool b13_c = fabs(l1->ValC[j] - l3->ValC[j]) > epsilon;
            if (b12_c || b13_c)
                return false;

            bool b12_d = fabs(l1->ValD[j] - l2->ValD[j]) > epsilon;
            bool b13_d = fabs(l1->ValD[j] - l3->ValD[j]) > epsilon;
            if (b12_d || b13_d)
                return false;
        }

        l1 = l1->Link;
        l2 = l2->Link;
        l3 = l3->Link;
    }

    *node_fail = -2;
    if ((l1 != NULL) || (l2 != NULL) || (l3 != NULL))
        return false;

    *node_fail = -1;
    return true;
}

void LlPrint(LlNode* p, FILE* fp, const char* msg)
{
    int i = 0;
    const char* fs = "%14.6lf %14.6lf %14.6lf %14.6lf\n";

    if (msg != NULL)
        fprintf(fp, "%s\n", msg);

    while (p != NULL)
    {
        fprintf(fp, "\nLlNode %d [0x%p]\n", i, p);
        fprintf(fp, "  ValA: ");
        fprintf(fp, fs, p->ValA[0], p->ValA[1], p->ValA[2], p->ValA[3]);

        fprintf(fp, "  ValB: ");
        fprintf(fp, fs, p->ValB[0], p->ValB[1], p->ValB[2], p->ValB[3]);

        fprintf(fp, "  ValC: ");
        fprintf(fp, fs, p->ValC[0], p->ValC[1], p->ValC[2], p->ValC[3]);

        fprintf(fp, "  ValD: ");
        fprintf(fp, fs, p->ValD[0], p->ValD[1], p->ValD[2], p->ValD[3]);

        i++;
        p = p->Link;
    }
}

LlNode* LlCreate(int num_nodes)
{
    LlNode* first = NULL;
    LlNode* last = NULL;

    srand(83);
    for (int i = 0; i < num_nodes; i++)
    {
        LlNode* p = (LlNode*)_aligned_malloc(sizeof(LlNode), 64);
        p->Link = NULL;

        if (i == 0)
            first = last = p;
        else
        {
            last->Link = p;
            last = p;
        }

        for (int i = 0; i < 4; i++)
        {
            p->ValA[i] = rand() % 500 + 1;
            p->ValB[i] = rand() % 500 + 1;
            p->ValC[i] = 0;
            p->ValD[i] = 0;
        }
    }

    return first;
}

void LlTraverseCpp(LlNode* p)
{
    while (p != NULL)
    {
        for (int i = 0; i < 4; i++)
        {
            p->ValC[i] = sqrt(p->ValA[i] * p->ValA[i] + p->ValB[i] * p->ValB[i]);
            p->ValD[i] = sqrt(p->ValA[i] / p->ValB[i] + p->ValB[i] / p->ValA[i]);
        }
        p = p->Link;
    }
}

void LinkedListPrefetch(void)
{
    const int num_nodes = 8;
    LlNode* list1 = LlCreate(num_nodes);
    LlNode* list2a = LlCreate(num_nodes);
    LlNode* list2b = LlCreate(num_nodes);

#ifdef _WIN64
    const char* platform = "X86-64";
    size_t sizeof_ll_node = sizeof(LlNode);
    const char* fn = "__LinkedListPrefetchResults64.txt";
#else
    const char* platform = "X86-32";
    size_t sizeof_ll_node = sizeof(LlNode);
    const char* fn = "__LinkedListPrefetchResults32.txt";
#endif

    printf("\nResults for LinkedListPrefetch\n");
    printf("Platform target:  %s\n", platform);
    printf("sizeof(LlNode):   %d\n", sizeof_ll_node);
    printf("LlNode member offsets\n");
    printf("  ValA:           %d\n", offsetof(LlNode, ValA));
    printf("  ValB:           %d\n", offsetof(LlNode, ValB));
    printf("  ValC:           %d\n", offsetof(LlNode, ValC));
    printf("  ValD:           %d\n", offsetof(LlNode, ValD));
    printf("  FreeSpace:      %d\n", offsetof(LlNode, FreeSpace));
    printf("  Link:           %d\n", offsetof(LlNode, Link));
    printf("\n");

    LlTraverseCpp(list1);
    LlTraverseA_(list2a);
    LlTraverseB_(list2b);

    int node_fail;

    if (!LlCompare(num_nodes, list1, list2a, list2b, &node_fail))
        printf("\nLinked list compare FAILED - node_fail = %d\n", node_fail);
    else
        printf("\nLinked list compare OK\n");

    FILE* fp;
    if (fopen_s(&fp, fn, "wt") == 0)
    {
        LlPrint(list1, fp, "\n----- list1 -----");
        LlPrint(list2a, fp, "\n ----- list2a -----");
        LlPrint(list2b, fp, "\n ----- list2b -----");
        fclose(fp);

        printf("\nLinked list results saved to file %s\n", fn);
    }
}

int _tmain(int argc, _TCHAR* argv[])
{
    LinkedListPrefetch();
    LinkedListPrefetchTimed();
    return 0;
}
