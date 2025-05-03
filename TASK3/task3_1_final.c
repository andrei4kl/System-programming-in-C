#include <stdio.h>
void main(void)
{   
    int number = 0;
    int number1 = 0;
    
    char* ptr;

    printf("\nTask 3_1, student: Klyuzhin A.S.\n");

    printf("\nEnter the first positive integer: ");
    scanf("%d", &number);   // считываем первое число (подходит как положительное, так и отрицательное)
    if(number < 0)
    {
        while(number < 0)    // исключаем ввод ошибочного значения
        {
            printf("\nThe number entered is less than zero try entering it again: ");
            scanf("%d", &number);   // считываем первое число повторно
        }
    }
    else{}
    
    printf("\nEnter the second integer between 0 and 255: ");
    scanf("%d", &number1);   // считываем второе число
    while((number1 < 0)||(number1 > 255))    // исключаем ввод ошибочного значения второго числа, выходящего за размер одного 8 битного байта
    {
        printf("\nThe second integer entered is out of range, please try again: ");
        scanf("%d", &number1);   // считываем второе число повторно
    }
    printf("\nnumber: %d", number);
    printf("\nnumber1: %d", number1);

    
 
// первая часть задания
       
    printf("\nYour first integer is in binary form:                "); 
     
      
    for(unsigned int n = (8*sizeof(number)); n > 0; n--)     // двигаемся от старшего бита к младшему
    {   
        int tempsimb = 0;
        tempsimb = (number>>(n-1));
        tempsimb = (1&tempsimb);
        printf("%d",tempsimb);
    }
    
    printf("\nYour second integer is in binary form:               "); 
          
    for(unsigned int n = (8*sizeof(number1)); n > 0; n--)     // двигаемся от старшего бита к младшему 
    {   
        int tempsimb = 0;
        tempsimb = (number1>>(n-1));
        tempsimb = (1&tempsimb);
        printf("%d",tempsimb);
    }
     
    ptr = (char*)&number+2;  // инициируем указатель адресом 3 байта первого числа (адрес начала массива + смещение 2 байта)
   // printf("ptr = %p",ptr);
    
    *ptr = number1;      // записываем число 1 (размером 1 байт) по адресу на который ссылается указатель
   //printf("*ptr = %d",*ptr);
   
    printf("\nThe first number with replacement of the third byte: "); 
    
    for(unsigned int n = (8*sizeof(number)); n > 0; n--)     // двигаемся от старшего бита к младшему 
    {   
        int tempsimb = 0;
        tempsimb = (number>>(n-1));
        tempsimb = (1&tempsimb);
        printf("%d",tempsimb);
    }

    printf("\nThe END.\n");
}