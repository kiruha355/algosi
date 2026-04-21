#include <stdbool.h>
#include <stdio.h>
int main(void)
{
    short f = 32 % 4;
    int i = 5, j = 5, k = 5;

    bool res_1 = i < 5;
    bool res_2 = i >= 5;
    bool res_3 = f;
    bool res_4 = i++ > 5;
    bool res_5 = ++j > 5;
    bool res_6 = 10 < k * 2;
    bool res_7 = 12 == k * 3;
    printf("%d%d%d%d%d%d%d",res_1,res_2,res_3,res_4,res_5,res_6,res_7);
    return 0;
}