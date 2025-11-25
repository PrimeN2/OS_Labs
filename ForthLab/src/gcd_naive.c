#include "contracts.h"

int gcd(int a, int b) {
    int min = a < b ? a : b;
    int result = 1;
    for (int i = 2; i <= min; ++i) {
        if (a % i == 0 && b % i == 0) {
            result = i;
        }
    }
    return result;
}
