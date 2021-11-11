#include <stdio.h>
#include <string.h>
#include <time.h>

int main()
{
    int a[8], b[8];   // int arrays for holding digits
    int ans[16] = {}; // product
    int i, j, tmp;

    char s1[9], s2[9]; // strings for getting numbers
    printf("enter first number: ");
    scanf(" %s", s1);
    printf("enter second number: ");
    scanf(" %s", s2);
    int l1 = strlen(s1); // lengths of both
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
    for (i = 0; i < l2; i++) // loop over b
    {
        for (j = 0; j < l1; j++) // loop over a
        {
            ans[i + j] += b[i] * a[j];
        }
    }

    // carry prop
    for (i = 0; i < l1 + l2; i++)
    {
        tmp = ans[i] / 10;
        ans[i] = ans[i] % 10;
        ans[i + 1] = ans[i + 1] + tmp;
    }

    // timed block is just product calc
    clock_t end = clock();
    double elapsed = ((double)(end - start)) / CLOCKS_PER_SEC;

    for (i = l1 + l2; i >= 0; i--)
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
