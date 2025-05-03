#include <stdio.h>

void main(void)
{   
   printf("\n\nStudent: Klyuzhin A.S.\n\n");
   printf("task3_3\n\n"); 
   int mas[10] = {1,2,3,4,5,6,7,8,9,10};
   int* mp =&mas[0];
   printf("Massive : ");
   
   
   for(unsigned char index = 0; index < 10; index++)
   {
    printf(" %d", *mp); // извлекаем элемент массива из ячейки памяти по адресу хранящемуся в mp
    mp++;   // переходим к следующей ячейке массива, увеличивая адрес в указателе
    
    //
    if(index < 9)
    {
        printf(",");
    }
    else
    {
        printf(".\n");
    }
   }

   printf("\nThe END.");
}
