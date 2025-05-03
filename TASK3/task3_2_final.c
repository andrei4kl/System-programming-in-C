#include <stdio.h>
void main(void)
{
    float x = 5.0;
    printf("x = %f, ", x);
    float y = 6.0;
    printf("y = %f\n", y);
    float *xp = &y;
    float *yp = &y;
    printf("Rezult: %f\n", *xp + *yp);
}
