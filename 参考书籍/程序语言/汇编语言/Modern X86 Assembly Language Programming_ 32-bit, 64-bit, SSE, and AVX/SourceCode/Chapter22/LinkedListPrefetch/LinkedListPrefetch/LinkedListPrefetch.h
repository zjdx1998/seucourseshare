#pragma once
#include "MiscDefs.h"

// This structure must match the corresponding structure definition
// in LinkedListPrefetch.inc.
typedef struct llnode
{
    double ValA[4];
    double ValB[4];
    double ValC[4];
    double ValD[4];
    Uint8 FreeSpace[376];

    llnode* Link;

#ifndef _WIN64
    Uint8 Pad[4];
#endif

} LlNode;

extern void LlTraverseCpp(LlNode* p);
extern LlNode* LlCreate(int num_nodes);
extern bool LlCompare(int num_nodes, LlNode* l1, LlNode* l2, LlNode* l3, int* node_fail);

extern "C" void LlTraverseA_(LlNode* p);
extern "C" void LlTraverseB_(LlNode* p);

extern void LinkedListPrefetchTimed(void);
