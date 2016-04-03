#include <time.h>
#include <stdio.h>
#include <stdlib.h>

#include <algorithm>
#include <vector>


#define BUFSIZE 1000000
int main(void)
{
    int i;
    clock_t t1, t2;
    double res;
    srand48(11);

    std::vector<int> arr;
    for (i = 0; i < BUFSIZE; ++i)
        arr.push_back(lrand48() % 65536);

    t1 = clock();
    std::sort(arr.begin(), arr.end());
    t2 = clock();
    res = (double)(t2 - t1);

    res = 1000.0 * (res / CLOCKS_PER_SEC);
    printf("res: %f\n", res);

    for (i = 0; i < BUFSIZE - 1; ++i)
    {
        if (arr[i] > arr[i+1])
        {
            printf("NOT SORTED\n");
            break;
        }
    }

    return 0;
}
