#define N 5
#include <stdio.h>

void main()
{   
    printf("\nTask 2, student: Klyuzhin A.S.\n");
       
    int mas[N];
    int mas1[N][N];
    int mas2[N][N];

// часть No1
    printf("\n---Part1---\n\n");
    printf("Matrix 1:\n\n");
    for(unsigned int str = 0; str < N; str++)
    {
        for(unsigned int count = 1; count<=N; count++)
        {
            printf("\t%d", (str*N + count));
        }
        printf("\n");
    }
    
// часть No2
    // заполняем массив
    
    printf("\n\n---Part2---\n\n");
    for(unsigned int n = 0; n < N; n++)
    {
        mas[n] = n + 1; 
    }

    // выводим первоначальный массив
    
    printf("Massive No1:");
    for(unsigned int n = 0; n < N; n++)
    {
        printf("\t%d", mas[n]);
    }

    // меняем местами элементы массива
    
    for(unsigned int n = 0; n < (N/2 + N%2); n++)
    {   
        unsigned int buffer = 0;
        buffer = mas[N-1-n]; 
        mas[(N-1)-n] = mas[n];
        mas[n] = buffer;
    }
     // выводим отображенный массив
    
    printf("\n\nMassive No2:");
    
    for(unsigned int n = 0; n < N; n++)
    {
        printf("\t%d", mas[n]);
    }   

// часть No3
    
    printf("\n\n---Part3---\n\n");
    printf("Matrix 2:\n\n");

    for(unsigned int i = 0; i< N; i++)
    {
        for(unsigned int j = 0; j< N; j++)
        {

            if(i<(N-1-j))
            {
                mas1[i][j] = 0;
            }
            else
            {
                mas1[i][j] = 1;
            }
        }
    }
// вывод матрицы к заданию No3

    for(unsigned int i = 0; i< N; i++)
    {
        for(unsigned int j = 0; j< N; j++)
        {
            printf("\t%d", mas1[i][j]);
        }
        printf("\n");
    }
// часть No4

    printf("\n---Part4---\n");

    // инициализируем массив нулями, стираем мусор
    for(unsigned int strg = 0; strg < N; strg++)
    {
        for(unsigned int column = 0; column < N; column++)
        {
            mas2[strg][column] = 0;
        }
    }
    
    
    // заполняем массив "улиткой"
   
    unsigned int counter = 0;
   
    for(unsigned int a = 0; a < (N/2); a++)
    {   
        
        unsigned int str = a;
        unsigned int col = a;

        // движение вправо
        for(unsigned int c = col; c<(N-1-a);c++)
        {   
            counter = counter + 1;
            mas2[str][c] = counter; 
            
        }
        col = (N-1)-a;
        // движение вниз
        for(unsigned int s = str; s < (N-1-a); s++)
        {   
            counter = counter + 1;
            mas2[s][col] = counter;
            
        }
        str = (N-1)-a;
        
        
        // движение влево
        for(unsigned int c1 = col; c1 > a; c1--)
        {   
            counter = counter + 1;
            mas2[str][c1] = counter;
        }
        col = a;
        
        // движение вверх        
        for(unsigned int s1 = str; s1 > a; s1--)
        {
               counter = counter + 1;
               mas2[s1][col] = counter;  
               
        }
        
    }
    //для нечетного числа N записываем последний (центральный) элемент матрицы вручную
    if(0 != (N%2))
    {
        mas2[N/2][N/2] = N*N;
    }

    // выводим массив "улитку" к заданию 4
    printf("\nMatrix 3:\n\n");
    for(unsigned int strg = 0; strg < N; strg++)
    {
        for(unsigned int column = 0; column < N; column++)
        {
            printf("\t%d", mas2[strg][column]);
        }
        printf("\n");
    }
    


    printf("\n\nThe END.\n");
}