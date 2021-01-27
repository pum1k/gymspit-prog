#include <stdio.h>

int factorial(int n)
{
    if (n < 0)
        return -1;

    if (n == 0)
        return 1;
    else
        return n * factorial(n - 1);
}

int gcd(int a, int b)
{
    if (b == 0)
        return a;
    else
        return gcd(b, a % b);
}

int main(int argc, char const *argv[])
{
    printf("factorial 5: %d\n", factorial(5));

    printf("GCD 15, 10: %d\n", gcd(15, 10));

    return 0;
}
