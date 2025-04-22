#include <stdio.h>
void main(void)
{   
    int number = 0;
    int number1 = 0;
    
    unsigned int counter = 0;

    printf("\nTask 1, student: Klyuzhin A.S.\n");
    printf("\nEnter the first integer: ");
    scanf("%d", &number);   // считываем первое число (подходит как положительное, так и отрицательное)
    if(number >= 0)
    {
        printf("\nEnter the second integer between 0 and 255: ");
        scanf("%d", &number1);   // считываем второе число
        while((0 > number1)||(255 < number1))    // исключаем ввод ошибочного значения второго числа, выходящего за размер одного 8 битного байта
        {
            printf("\nThe second integer entered is out of range, please try again: ");
            scanf("%d", &number1);   // считываем второе число повторно
        }
    }
    else{}
    
    
    
 
// первая и вторая части задания
       
    printf("\nYour first integer is in binary form:                            "); 
     
      
    for(unsigned int n = (8*sizeof(number)); n > 0; n--)     // двигаемся от старшего бита к младшему
    {   
        int tempsimb = 0;
        tempsimb = (number>>(n-1));
        tempsimb = (1&tempsimb);
        printf("%d",tempsimb);
         
        if((number > 0)&&(1 == tempsimb))   // считаем единицы, если число положительное
        {
            counter = counter + 1;
        }
        else{}          
    }
    
    
                  

// третья и четвертая части задания
    if(number >= 0)
    {    
        printf("\nYour second integer is in binary form:                           "); 
           
        for(unsigned int n = (8*sizeof(number1)); n > 0; n--)     // двигаемся от старшего бита к младшему 
        {   
            int tempsimb = 0;
            tempsimb = (number1>>(n-1));
            tempsimb = (1&tempsimb);
            printf("%d",tempsimb);
        }
                  
        for(unsigned int i = 8; i > 0; i--)    
        {   
            unsigned int mask_clr_bit = 0;
            unsigned int mask_wrt_bit = 0;
            
            // очищаем нужный бит в первом числе (записываем в него ноль)
            mask_clr_bit = 1<<(i+15);
            number = number&(~mask_clr_bit);  
            // заменяем значение бита в третьем байте первого числа на значение соответствующего бита из первого байта второго числа
            mask_wrt_bit = 1<<(i-1);                // маска для извлечения бита из первого байта второго числа
            mask_wrt_bit = number1&mask_wrt_bit;    // извлекаем необходимый бит из первого байта второго числа, все остальные биты обнуляются   
            mask_wrt_bit = mask_wrt_bit<<16;    // сдвигаем полученный бит из первого байта в третий байт
            
            number = number|mask_wrt_bit;
        }
        
        
        
        printf("\nThe first number with the third byte replaced is in binary form: "); 
             
        for(unsigned int n = (8*sizeof(number)); n > 0; n--)     // двигаемся от старшего бита к младшему 
        {   
            int tempsimb = 0;
            tempsimb = (number>>(n-1));
            tempsimb = (1&tempsimb);
            printf("%d",tempsimb);
        }       
        
        printf("\nThe number of ones in the binary representation of the first entered integer: %d\n",counter);
    
    }
    else
    {
        printf("\nThe first number entered was negative, so the replacement of byte 3 with the second number entered was not performed.\n");
    }
     
   
    printf("\nThe END.\n");
}