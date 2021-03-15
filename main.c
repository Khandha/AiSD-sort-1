#include <assert.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int ns[] = {
        10000, 20000, 30000, 40000, 50000, 60000, 70000, 80000, 90000, 100000};

void fill_random(int *A, int n)
{
    for (int i = 0; i < n; i++)
    {
        A[i] = rand() % n;
    }
}

void fill_increasing(int *A, int n)
{
    for (int i = 0; i < n; i++)
    {
        A[i] = i;
    }
}

void fill_decreasing(int *A, int n)
{
    for (int i = 0; i < n; i++)
    {
        A[i] = n - i;
    }
}

void fill_vshape(int *A, int n)
{
    int m;
    m = n / 2 + n % 2;
    for (int i = 0; i < m; i++)
    {
        A[i] = n / 2 - i;
    }
    for (int i = m; i < n; i++)
    {
        A[i] = A[i - 1] + 1;
    }
}

int Min_element(int *A, int i, int j)
{
    int min = i;
    for (int n = i + 1; n <= j; n++)
    {
        if (A[n] < A[min])
        {
            min = n;
        }
    }
    return min;
}

void Swap(int *A, int i, int j)
{
    int temp = A[i];
    A[i] = A[j];
    A[j] = temp;
}

void selection_sort(int *A, int n)
{
    for (int i = 0; i < n; i++)
    {
        int min = Min_element(A, i, n - 1);
        Swap(A, i, min);
    }
}

void insertion_sort(int *A, int n)
{
    for (int i = 1; i < n; i++)
    {
        int temp = A[i];
        int j = i - 1;
        while ((j >= 0) && (A[j] > temp))
        {
            A[j + 1] = A[j];
            j--;
        }
        A[j + 1] = temp;
    }
}

int Partition(int *A, int p, int r)
{
    int x = A[r];
    int i = p - 1;
    for (int j = p; j <= r; j++)
    {
        if (A[j] <= x)
        {
            i = i + 1;
            Swap(A, i, j);
        }
    }
    return i;
}
void quick_sort(int *A, int p, int r)
{
    if (p < r)
    {
        int q = Partition(A, p, r);
        quick_sort(A, p, q - 1);
        quick_sort(A, q + 1, r);
    }
}

void quick_sort_all(int *A, int n)
{
    quick_sort(A, 0, n - 1);
}
int randomized_partition(int *A, int p, int r)
{
    int i = (rand() % (r - p + 1));
    i += p; //find random [p, r]
    Swap(A, i, r);
    return Partition(A, p, r);
}

void randomized_quick_sort(int *A, int p, int r)
{
    if (p < r)
    {
        int q = randomized_partition(A, p, r);
        randomized_quick_sort(A, p, q - 1);
        randomized_quick_sort(A, q + 1, r);
    }
}

void randomized_quick_sort_all(int *A, int n)
{
    randomized_quick_sort(A, 0, n - 1);
}

void MaxHeapify(int *A, int i, int n)
{
    int max = i, left = 2 * i + 1, right = 2 * i + 2;
    if ((left < n) && (A[left] > A[max]))
    {
        max = left;
    }
    if ((right < n) && (A[right] > A[max]))
    {
        max = right;
    }
    if (max != i)
    {
        Swap(A, i, max);
        MaxHeapify(A, max, n);
    }
}

void heap_sort(int *A, int n)

{
    for (int i = n / 2 - 1; i >= 0; i--)
    {
        MaxHeapify(A, i, n);
    }
    for (int i = n - 1; i > 0; i--)
    {
        Swap(A, 0, i);
        MaxHeapify(A, 0, i);
    }
}

bool is_random(int *A, int n)
{
    return true;
}

bool is_increasing(int *A, int n)
{
    for (int i = 1; i < n; i++)
    {
        if (A[i] <= A[i - 1])
        {
            return false;
        }
    }
    return true;
}

bool is_decreasing(int *A, int n)
{
    for (int i = 1; i < n; i++)
    {
        if (A[i] >= A[i - 1])
        {
            return false;
        }
    }
    return true;
}

bool is_vshape(int *A, int n)
{
    if (n % 2 == 0)
    {
        return is_decreasing(A, n / 2) && is_increasing(A + n / 2, n / 2);
    }
    return is_decreasing(A, n / 2 + 1) && is_increasing(A + n / 2, n / 2 + 1);
}

bool is_sorted(int *A, int n)
{
    for (int i = 1; i < n; i++)
    {
        if (A[i] < A[i - 1])
        {
            return false;
        }
    }
    return true;
}

char *bool_to_string(bool b)
{
    return b ? "Y" : "N";
}

void (*fill_functions[])(int *, int) = {fill_random, fill_increasing, fill_decreasing, fill_vshape};
bool (*check_functions[])(int *, int) = {is_random, is_increasing, is_decreasing, is_vshape};
void (*sort_functions[])(int *, int) = {selection_sort, insertion_sort, quick_sort_all, randomized_quick_sort_all, heap_sort};

char *fill_names[] = {"Random", "Increasing", "Decreasing", "V-Shape"};
char *sort_names[] = {"SelectionSort", "InsertionSort", "QuickSort", "RandomizedQuickSort", "HeapSort"};

int main()
{
    FILE *file = fopen("results.csv", "w");
    if (!file)
    {
        return false;
    }
    for (unsigned int i = 0; i < sizeof(sort_functions) / sizeof(*sort_functions); i++)
    {
        void (*sort)(int *, int) = sort_functions[i];

        for (unsigned int j = 0; j < sizeof(fill_functions) / sizeof(*fill_functions); j++)
        {
            void (*fill)(int *, int) = fill_functions[j];
            bool (*check)(int *, int) = check_functions[j];

            for (unsigned int k = 0; k < sizeof(ns) / sizeof(*ns); k++)
            {
                int n = ns[k];
                int *A = calloc(n, sizeof(*A));

                fill(A, n);
                bool is_filled_ok = check(A, n);

                clock_t begin = clock();
                sort(A, n);
                clock_t end = clock();
                double seconds = (double)(end - begin) / (double)CLOCKS_PER_SEC;

                bool is_sorted_ok = is_sorted(A, n);

                printf("%-20s %-11s %-10d %-4s %-4s %g\n", sort_names[i], fill_names[j], n, bool_to_string(is_filled_ok), bool_to_string(is_sorted_ok), seconds);

                fprintf(file, "%-20s, %-11s, %-10d, %g,\n", sort_names[i], fill_names[j], n, seconds);

                free(A);
            }
        }
    }
    fclose(file);

    return 0;
}