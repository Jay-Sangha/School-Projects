#include "mathlib.h"

#include <math.h>
#include <stdio.h>

int main(void) {

    FILE *Sine = fopen("sin.csv", "w");

    fprintf(Sine, "x, y\n");

    for (double x = 0; x <= 2.0 * M_PI; x += 0.1) {
        double n = sin(x) - Sin(x);

        fprintf(Sine, "%f, %.10f\n", x, n);
    }

    fclose(Sine);

    FILE *Cosine = fopen("cos.csv", "w");

    fprintf(Cosine, "x, y\n");

    for (double x = 0; x <= 2.0 * M_PI; x += 0.1) {
        double n = cos(x) - Cos(x);

        fprintf(Cosine, "%f, %.10f\n", x, n);
    }

    fclose(Cosine);

    FILE *Tangent = fopen("tan.csv", "w");

    fprintf(Tangent, "x, y\n");

    for (double x = 0; x <= 2.0 * M_PI; x += 0.1) {
        double n = tan(x) - Tan(x);

        fprintf(Tangent, "%f, %.10f\n", x, n);
    }

    fclose(Tangent);

    return 0;
}
