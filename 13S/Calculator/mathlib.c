#include "mathlib.h"

#include <math.h>
#include <stdio.h>

double Sqrt(double x) {
    if (x < 0) {
        return nan("nan");
    }

    double old = 0.0;
    double new = 1.0;

    while (Abs(old - new) > EPSILON) {
        old = new;
        new = 0.5 * (old + (x / old));
    }

    return new;
}

double Abs(double x) {
    double y;
    if (x < 0) {
        y = -x;
    } else {
        y = x;
    }
    return y;
}

double Cos(double x) {
    double result = 1;
    double calcul = 1;

    x = fmod(x, 2.0 * M_PI);

    for (int i = 1; i <= 64; ++i) {
        calcul *= -x * x / (2 * i * (2 * i - 1));
        result += calcul;
    }

    result = fmod(result, 2.0 * M_PI);

    return result;
}

/**

double Sin(double x) {
    double result = x;
    double holder = x;

    x = fmod(x, 2.0 * M_PI);

    for (int i = 1; i <= 64; ++i) {
        holder *= -x * x / ((2 * i) * (2 * i + 1));
        result += holder;
    }

    result = fmod(result, 2.0 * M_PI);

    return result;
}

**/

double Sin(double x) {
    x = fmod(x, 2.0 * M_PI);

    int y = 1;
    double result = x;
    double holder = x;
    int swap = -1;

    while (1) {
        holder = holder * ((x * x) / ((2 * y) * (2 * y + 1)));
        result += swap * holder;
        swap = -swap;
        y++;

        if (Abs(holder) < EPSILON) {
            holder = holder * ((x * x) / ((2 * y) * (2 * y + 1)));
            result += swap * holder;
            break;
        }
    }
    result = fmod(result, 2.0 * M_PI);

    return result;
}

double Tan(double x) {
    double y = (Sin(x) / Cos(x));

    return y;
}
