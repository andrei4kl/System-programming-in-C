
#include <stdio.h>
#include <stdlib.h>
#include "../mymath/calc.h"   // указываю относительный путь в каталог c моей библиотекой
#define MAX 15
char choice [2] = {'0', '\0'};  // массив для хранения переменной выбора пункта меню, чтобы использовать универсальную функцию ввода read_data

// функция считывания строки из консоли, принимает указатель на массив и размер массива
void read_data(char* massive, unsigned int maximum)
{
  char* pm1 = massive;
    while(1)
    {   
        *pm1 = getchar();
        if(('\n' != *pm1))
        {   
            pm1++;
        }
        else
        {   
            *pm1 = '\0';    // заменяем символ перевода строки на символ окончания строки (для дальнейшей адекватной работы printf)
            break;
        }
        if(pm1 < (massive + maximum - 1))
            {

            }
        else
            {
                *pm1 = '\0';    // заменяем символ перевода строки на символ окончания строки (для дальнейшей адекватной работы printf)
                while(getchar() != '\n');   // очищаем буфер потока stdin
                break; 
            }
    }

}
// функция переводит последовательность символов от 0 до 9 в число типа int
// принимает указатель на массив символов (последний символ в котором "\0"), возвращает целое число
// контролирует введенные символы, в случае ошибки возвращает 0
int simbol_to_integer(char* mas_simbol)
{

    unsigned int quantity_simbols = 0;   // количество фактически введенных ASCII символов (до служебного символа \0)
    unsigned int index = 0;
    unsigned int flag_conf = 0; // флаг проверки введенных символов
    int integer_ret = 0; // переменная для возвращаемого функцией результата преобразования

    while((*(mas_simbol + index) != '\0'))   // вычислаяем количество введенных символов (использованных ячеек массива)
    {
        quantity_simbols++;
        index++;
    }
    if((quantity_simbols > 0))   // проверка на минимальное количество введенных символов
    {
        for(unsigned int index2 = 0; index2 < quantity_simbols; index2++)    // проверка введенных сиволов на соответствие кодам цифр от 0 до 9 согласно таблице ASCII
        {
            if((*(mas_simbol+index2) > '/')&&(*(mas_simbol+index2) < ':'))
            {
                flag_conf = 1;
            }
            else
            {
                flag_conf = 0;
                break;
            }

        }

        if(1 == flag_conf)
        {
            for(unsigned int ind = 0; ind < quantity_simbols; ind++)    // переводим последовательность символов в число
            {
                integer_ret = integer_ret*10 + (*(mas_simbol + ind) - 48);
            }
        }
        else
        {
           printf("\nYou entered an incorrect number.\n"); 
        }
    }
    else
    {
        printf("\nYou entered an incorrect number.\n");
    }
    return integer_ret;
}
/*
int addition(int a, int b)      // функция сложения
{
    int rez = 0;
    rez = a + b;
    return rez;
}

int subtraction(int a, int b)       // функция вычитания
{
    int rez = 0;
    rez = a - b;
    return rez;
}

int multiplication(int a, int b)      // функция умножения
{
    int rez = 0;
    rez = a*b;
    return rez;
}

int division (int a, int b)     // целочисленное деление, остаток отбрасывается
{
    int rez = 0;
    rez = a/b;
    return rez;
}
*/
void main(void)
{  
   printf("\n\nStudent: Klyuzhin A.S.\n\n");
   printf("task7 - static lib\n\n"); 
   
   
   char in_integer[MAX];     // массив для хранения вводимых чисел в виде последовательности символов ASCII
   
   int first_integer = 0;       // первое число для вычислений
   int second_integer = 0;      // второе число для вычислений
   int result_math = 0;     // переменная для хранения результата вычисления
   
   while('5' != choice[0])
   {
    printf("\n");
    printf("1) Addition\n");
    printf("2) Subtraction\n");
    printf("3) Multiplication\n");
    printf("4) Division\n");
    printf("5) Exit\n");

    printf("\nEnter the number: ");    

    read_data(&choice[0], 2);  // считываем число и символ конца строки в массив
    if((choice[0] < '1')||(choice[0] > '5'))
    {
        printf("\nYou have entered an incorrect number, enter a number from 1 to 5 and press Enter.\n");
        choice[0] = '0'; //сбрасываем неверно введенное значение к исходному
    }
    else if (choice[0] != '5') // если был введен код символа "5", то выходим и ничего не делаем
    {
      printf("\nEnter the first integer: ");
      read_data(in_integer, MAX);                       // вводим число из консоли в массив in_integer максимального размера = MAX символов ASCII
      first_integer = simbol_to_integer(in_integer);    // Формируем число типа int из набора введенных символов
     
      printf("\nEnter the second integer: ");
      read_data(in_integer, MAX);                       // вводим число из консоли в массив in_integer максимального размера = MAX символов ASCII
      second_integer = simbol_to_integer(in_integer);    // Формируем число типа int из набора введенных символов
      
      switch(choice[0])
      {
        case '1':
        result_math = addition(first_integer, second_integer);  // вызываем функцию сложения
        break;

        case '2':
        result_math = subtraction(first_integer, second_integer);  // вызываем функцию вычитания
        break;
        
        case '3':
        result_math = multiplication(first_integer, second_integer);  // вызываем функцию умножения
        break;

        case '4':
        result_math = division(first_integer, second_integer);  // вызываем функцию целочисленного деления
        break;
      }
      printf("\nCalculation result = %d\n", result_math);
    } 
    else{}    
    
   }
   
   printf("\n\nThe END.");
}



