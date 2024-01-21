#include <stdio.h>
#include "sz_math.h"

int test() {
    Vector2 a = { 1, 2 };
    Vector2 b = { 3, 4 };
    
    Vector2 add = Vector2Add(a, b);
    Vector2 scale = Vector2Scale(a, 5);

    printf("%f %f\n", add.x, add.y);
    printf("%f %f\n", scale.x, scale.y);
    printf("%f\n", Vector2Dot(a, b));

}
