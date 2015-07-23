#include <stdio.h>

//prototypes
void swapnumbers(int* x, int* y);

int main(void)
{
    int num1;
    int num2;

    printf("Please provide num1: ");
    scanf("%i", &num1);

    printf("Please provide num2: ");
    scanf("%i", &num2);

    printf("Swapping.....\n");

    swapnums(&num1, &num2);

    printf("Num1 is now %i\n", num1);
    printf("Num2 is now %i\n", num2);
}

void swapnums(int* x, int* y)
{
    int temp;
    temp = *x;
    *x = *y;
    *y = temp;

}
