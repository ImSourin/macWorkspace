//Teammates: Sourin Chakrabarti, Ujwal Pratap Krishna Koluvakolanu Thyagarajan
#include <stdio.h>
#include <stdlib.h>

/***
** Implement the function below. Function check_bounds takes as input
** the size of the array being accessed, the index of the accessed
** element, and the line number and file in which this access
** occurs. If the access is out of bounds, then the function prints an
** error message.
***/



void check_bounds(long int index, long int size, const char* filename, int line) {
    if (index < 0 || (size > 0 && index >= size)) {
        fprintf(stderr, "%s:%d: Array Out of Bounds Error \n", filename, line);
        exit(1);
    }
    return;
}
