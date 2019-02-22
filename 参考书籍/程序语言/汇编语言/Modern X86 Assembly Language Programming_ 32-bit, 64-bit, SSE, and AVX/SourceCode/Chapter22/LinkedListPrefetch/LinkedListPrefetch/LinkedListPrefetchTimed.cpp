#include "stdafx.h"
#include "LinkedListPrefetch.h"
#include "ThreadTimer.h"

void LinkedListPrefetchTimed(void)
{
    ThreadTimer::SetThreadAffinityMask();

    const int num_it = 500;
    const int num_alg = 3;
    const double et_scale = 1.0e6;
    double et[num_it][num_alg];
    ThreadTimer tt;

    const int num_nodes = 20000;
    LlNode* list1 = LlCreate(num_nodes);
    LlNode* list2a = LlCreate(num_nodes);
    LlNode* list2b = LlCreate(num_nodes);

    for (int i = 0; i < num_it; i++)
    {
        tt.Start();
        LlTraverseCpp(list1);
        tt.Stop();
        et[i][0] = tt.GetElapsedTime() * et_scale;

        tt.Start();
        LlTraverseA_(list2a);
        tt.Stop();
        et[i][1] = tt.GetElapsedTime() * et_scale;

        tt.Start();
        LlTraverseB_(list2b);
        tt.Stop();
        et[i][2] = tt.GetElapsedTime() * et_scale;
    }

#ifdef _WIN64
    const char* fn = "__LinkedListPrefetch64.csv";
#else
    const char* fn = "__LinkedListPrefetch32.csv";
#endif

    int node_fail;

    if (!LlCompare(num_nodes, list1, list2a, list2b, &node_fail))
    {
        printf("\nResults for LinkedListPrefetchTimed()\n");
        printf("  Linked list compare FAILED - node_fail = %d\n", node_fail);
    }

    ThreadTimer::SaveElapsedTimeMatrix(fn, (double*)et, num_it, num_alg);
    printf("\nBenchmark times saved to file %s\n", fn);
}
