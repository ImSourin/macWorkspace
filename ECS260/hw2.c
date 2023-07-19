#include "klee/klee.h"
#include "assert.h"

long long power2bitwise(long long x) {
    return 1<<x;
}

long long powerModExp(long long x, long long y) {
    long long res = 1;
    while (y > 0) {
        if (y % 2 == 1)
            res = (res * x);
        y = y >> 1;
        x = (x * x);
    }
    return res;
}

int main() {
    long long a;
    klee_make_symbolic(&a, sizeof(a), "a");
    if(a >= 0 && a < 63){
        assert(power2bitwise(a) == powerModExp(2, a));
    } else {
        printf("Exponent out of range\n");
    }
    return 0;
}
