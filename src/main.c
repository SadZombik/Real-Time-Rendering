#include <stdio.h>
#include "math/sz_math.h"

int main() {
    Vector4 v1 = { 0, 1, 2, 3 };
    Vector4 v2 = { 4, 5, 6, 7 };
    Vector4 v3 = Vector4Add(v1, v2);
    Vector4 v4 = Vector4Scale(v1, 5);
    
    PrintVector4(&v1, "v1");
    PrintVector4(&v2, "v2");
    PrintVector4(&v3, "Add");

    printf("Dot: %f\n", Vector4Dot(v1, v2));
    PrintVector4(&v4, "Scale5");


    return 0;
}
