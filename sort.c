#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define THRESHOLD 16
#define iter size_t
#define dist size_t
#define value int
typedef int (*comparator)(value, value);

static value* arr;

#define cmpm(a,b) ((a) < (b))
#define set(i,v) (arr[(i)] = (v))
#define get(i) (arr[(i)])

/* static inline void set(iter i, value val) */
/* { */
/*     arr[i] = val; */
/* } */

/* static inline value get(iter i) */
/* { */
/*     return arr[i]; */
/* } */

static inline void swap(iter a, iter b)
{
    value tmp;
    tmp = arr[a];
    arr[a] = arr[b];
    arr[b] = tmp;
}

void push_heap(iter begin, dist hole, dist top, value val, comparator cmpfp)
{
    dist parent = (hole - 1) / 2;
    while (hole > top && cmpm(get(begin + parent), val))
    {
        set(begin + hole, get(begin + parent));
        hole = parent;
        parent = (hole - 1) / 2;
    }
    set(begin + hole, val);
}


void adjust_heap(iter begin, dist hole, dist len, value val, comparator cmpfp)
{
    dist top, child;

    top = hole;
    child = hole;
    while (child < (len - 1) / 2)
    {
        child = 2 * (child + 1);
        if (cmpm(get(begin + child), get(begin + (child - 1))))
            child--;

        set(begin + hole, get(begin + child));
        hole = child;
    }

    if ((len & 1) == 0 && child == (len - 2) / 2)
    {
        child = 2 * (child + 1);
        set(begin + hole, get(begin + (child - 1)));
        hole = child - 1;
    }

    push_heap(begin, hole, top, val, cmpfp);
}

void make_heap(iter begin, iter end, comparator cmpfp)
{
    dist len, parent;
    value val;

    len = end - begin;
    if (len < 2)
        return;

    parent = (len - 2) / 2;
    while (1)
    {
        val = get(begin + parent);
        adjust_heap(begin, parent, len, val, cmpfp);
        if (parent == 0)
            return;
        parent--;
    }
}

static inline void pop_heap(iter begin, iter end, iter result, comparator cmpfp)
{
    value val = get(result);
    set(result, get(begin));
    adjust_heap(begin, 0, end - begin, val, cmpfp);
}

void heap_select(iter begin, iter mid, iter end, comparator cmpfp)
{
    iter i;
    make_heap(begin, mid, cmpfp);
    for (i = mid; i < end; ++i)
    {
        if (cmpm(get(i), get(begin)))
            pop_heap(begin, mid, i, cmpfp);
    }
}

void heap_sort(iter begin, iter end, comparator cmpfp)
{
    while (end - begin > 1)
    {
        --end;
        pop_heap(begin, end, end, cmpfp);
    }
}

void partial_sort(iter begin, iter mid, iter end, comparator cmpfp)
{
    heap_select(begin, mid, end, cmpfp);
    heap_sort(begin, mid, cmpfp);
}


iter partition(iter begin, iter end, iter pivot, comparator cmpfp)
{
    while(1)
    {
        while(cmpm(get(begin), get(pivot)))
            ++begin;
        --end;
        while(cmpm(get(pivot), get(end)))
            --end;
        if (!(begin < end))
            return begin;
        swap(begin, end);
        ++begin;
    }
}

void move_median_to_begin(iter result, iter a, iter b, iter c, comparator cmpfp)
{
    if (cmpm(get(a), get(b)))
    {
        if (cmpm(get(b), get(c)))
            swap(result, b);
        else if (cmpm(get(a), get(c)))
            swap(result, c);
        else
            swap(result, a);
    }
    else if (cmpm(get(a), get(c)))
        swap(result, a);
    else if (cmpm(get(b), get(c)))
        swap(result, c);
    else
        swap(result, b);
}

iter partition_pivot(iter begin, iter end, comparator cmpfp)
{
    iter mid = begin + (end - begin) / 2;
    move_median_to_begin(begin, begin + 1, mid, end - 1, cmpfp);
    return partition(begin + 1, end, begin, cmpfp);
}

void introsort_loop(iter begin, iter end, iter depthlimit, comparator cmpfp)
{
    iter pivot;
    while (end - begin > THRESHOLD)
    {
        if (depthlimit == 0)
        {
            partial_sort(begin, end, end, cmpfp);
            return;
        }

        --depthlimit;
        pivot = partition_pivot(begin, end, cmpfp);
        introsort_loop(pivot, end, depthlimit, cmpfp);
        end = pivot;
    }
}

void unguarded_linear_insert(iter end, comparator cmpfp)
{
    value val = get(end);
    iter next = end;
    --next;
    while (cmpm(val, get(next)))
    {
        set(end, get(next));
        end = next;
        --next;
    }
    set(end, val);
}

static inline void copy_backwards(iter begin, iter last, iter dlast)
{
    while (begin != last)
    {
        set(--dlast, get(--last));
    }
}

void insertion_sort(iter begin, iter end, comparator cmpfp)
{
    iter i;
    value val;
    if (begin == end) return;
    for (i = begin + 1; i != end; ++i)
    {
        if (cmpm(get(i), get(begin)))
        {
            val = get(i);
            copy_backwards(begin, i, i + 1);
            set(begin, val);
        }
        else
        {
            unguarded_linear_insert(i, cmpfp);
        }
    }
}

void unguarded_insertion_sort(iter begin, iter end, comparator cmpfp)
{
    iter i;
    for (i = begin; i != end; ++i)
        unguarded_linear_insert(i, cmpfp);
}

void final_insertion_sort(iter begin, iter end, comparator cmpfp)
{
    if (end - begin > THRESHOLD)
    {
        insertion_sort(begin, end + THRESHOLD, cmpfp);
        unguarded_insertion_sort(begin + THRESHOLD, end, cmpfp);
    }
    else
    {
        insertion_sort(begin, end, cmpfp);
    }
}

double log2(double n)
{
    return log(n) / log(2.0);
}

void sort(iter begin, iter end, comparator cmpfp)
{
    if (begin != end)
    {
        introsort_loop(begin, end, log2((double)(end - begin)) * 2, cmpfp);
        final_insertion_sort(begin, end, cmpfp);
    }
}


#define BUFSIZE 1000000

static inline int compare(int a, int b)
{
    return a < b;
}

int main(void)
{
    int i;
    clock_t t1, t2;
    double res;
    srand48(11);

    arr = malloc(sizeof(int) * BUFSIZE);
    for (i = 0; i < BUFSIZE; ++i)
        arr[i] = lrand48() % 65536;

    t1 = clock();
    sort(0, BUFSIZE, &compare);
    t2 = clock();
    res = (double)(t2 - t1);

    res = 1000.0 * (res / CLOCKS_PER_SEC);
    printf("res: %f\n", res);


    for (i = 0; i < BUFSIZE - 1; ++i)
    {
        if (get(i) > get(i+1))
        {
            printf("NOT SORTED\n");
            break;
        }
    }

    free(arr);
    return 0;
}

