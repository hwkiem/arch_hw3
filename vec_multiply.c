#include <stdio.h>
#include <string.h>
#include <immintrin.h>
#include <time.h>

int main()
{
    int size = 8;
    int *a = (int *)calloc(8, sizeof(int));
    int *b = (int *)calloc(8, sizeof(int));
    int ans[16] = {0}; // product
    int i, j, tmp;

    char s1[size + 1], s2[size + 1]; // strings for getting numbers
    printf("enter first number: ");
    scanf(" %s", s1);
    printf("enter second number: ");
    scanf(" %s", s2);
    int l1 = strlen(s1);
    int l2 = strlen(s2);

    for (i = l1 - 1, j = 0; i >= 0; i--, j++) // loops!
    {
        a[j] = s1[i] - '0'; // get int from char
    }
    for (i = l2 - 1, j = 0; i >= 0; i--, j++)
    {
        b[j] = s2[i] - '0';
    }

    // time measurement
    clock_t start = clock();

    // compute product over both
    __m256i agg_vec_high = _mm256_setzero_si256();
    __m256i agg_vec_low = _mm256_setzero_si256();
    for (i = 0; i < size; i++) // loop over b
    {
        __m256i b_vec, a_vec, out_vec = _mm256_setzero_si256();
        // static vec
        b_vec = _mm256_set1_epi32((b[i]));
        // load a vec
        a_vec = _mm256_loadu_si256((__m256i *)a);

        // out buffer for storing multiplication results
        int out_arr[16] = {0};
        // multiply low
        out_vec = _mm256_mullo_epi32(a_vec, b_vec);
        // aligning outvec for dot product
        _mm256_storeu_si256((__m256i *)&out_arr[i], out_vec);
        // reload alligned out vectors
        __m256i out_vec_high = _mm256_loadu_si256((__m256i *)out_arr);
        __m256i out_vec_low = _mm256_loadu_si256((__m256i *)&out_arr[8]);
        // dot out vectors with aggregator
        agg_vec_high = _mm256_add_epi32(out_vec_high, agg_vec_high);
        agg_vec_low = _mm256_add_epi32(out_vec_low, agg_vec_low);
    }
    // store aggregate to the ans buffer
    _mm256_storeu_si256((__m256i *)ans, agg_vec_high);
    _mm256_storeu_si256((__m256i *)&ans[8], agg_vec_low);

    free(a);
    free(b);

    // carry prop on the ans buffer
    for (i = 0; i < 2 * size - 1; i++)
    {
        tmp = ans[i] / 10;
        ans[i] = ans[i] % 10;
        ans[i + 1] = ans[i + 1] + tmp;
    }

    // timed block is just product calc
    clock_t end = clock();
    double elapsed = ((double)(end - start)) / CLOCKS_PER_SEC;

    for (i = 2 * size; i >= 0; i--)
    {
        if (ans[i] > 0)
            break;
    }

    // print out the product
    printf("Product : ");
    for (; i >= 0; i--)
    {
        printf("%d", ans[i]);
    }
    printf("\n");
    printf("Time measured: %f seconds.\n", elapsed);

    return 0;
}
