
#include <stdio.h>  //стандартная библиотека для работы функции printf
#include <unistd.h> //библиотека для работы с системными фызовами
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <stdlib.h>

#define PATH1 "/usr/bin/"   // путь к папке с утилитами, которые будем запускать
#define LEN_BUF 100         // максимальная длина вводимой пользователем строки строки

// функция для считывания строки из терминала
void read_data(char* massive, unsigned int MAXIMUM)
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
        if(pm1 < (massive + MAXIMUM - 1))
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

void main()
{
    pid_t pid = 0;                    // PID главного просесса (т.е. ID этой программы при запуске)
    pid_t ppid = 0;                   // PPID главноего процесса (т.е. родительский ID относительно этой программы)
    pid_t pid_new_process = 0;        // PID порожденного процесса, созданного в ходе выполнения этой программы (получаем в процессе создания процесса)
    pid_t status = 0;                 // сохраняем статус завершения работы порожденного процесса (потомка)    
    
    
    int flag_exit = 0;      // флаг для выхода из программы
    int stat_exec = 0;      // статус выполнения команды exec 

    char name_programm[20];        // команда (имя запускаемой программы)
    char parametr1[10];             // параметр 1 для запускаемой программы
    char parametr2[10];             // параметр 2 для запускаемой программы
    char name_exit[] = "exit";        // команда выхода из программы
    char path[100];                     
    
    char my_buffer[LEN_BUF];             // буфер для считывания команды 
    char *sp;                            // указатель, используется функцией strtok для разбиения строки на части

    pid = getpid();
    ppid = getppid();

   // printf("Главный процесс.\nPID = %d, PPID = %d.\n", pid, ppid);
    
    while(flag_exit == 0)
    {   
        parametr1[0] = 0;         //по этому значению буду определять вводился ли аргумент и надо ли его передавать при вызове execl
        parametr1[1] = '\0';        //устанавливаю конец строки на второй элемент массива
        
        parametr2[0] = 0;         //по этому значению буду определять вводился ли аргумент и надо ли его передавать при вызове execl
        parametr2[1] = '\0';        //устанавливаю конец строки на второй элемент массива

        name_programm[0] = 0;
        name_programm[1] = '\0';

        printf("Формат ввода команды: команда -параметр1 -параметр2\n");
        printf("ankl>");
        fflush(NULL);       // проталкиваем текст из предыдущего printf из буфера библиотеки в буфер ядра

        read_data(my_buffer, LEN_BUF);      // считываем строку   
        
        //printf("\nmy_buffer = %s\n", my_buffer);

        //выделяем команду из введенной строки
        sp = strtok(my_buffer, " ");    
        
        if(sp != NULL)
        {
          strcpy(name_programm, sp);
          sp = strtok(NULL, " ");
        }
        else
        {

        }
        
        // выделяем первый параметр из остатка строки
        if(sp != NULL)
        {
          strcpy(parametr1, sp);
          sp = strtok(NULL, " "); 
        }
        else
        {}
       
        // выделяем второй параметр из остатка строки
        if(sp != NULL)
        {
            strcpy(parametr2, sp);
            sp = strtok(NULL, " "); 
        }
        else
        {}

        // очищаем остатки строки
        while(sp != NULL)
        {
          sp = strtok(NULL, " ");  
        }
  

    
        //printf("\nname_programm = %s\n", name_programm);
        //printf("\nparametr1 = %s\n", parametr1);
    
       
        if(strcmp(name_programm, name_exit) != 0)       // проверяю не хочет ли пользователь выйти
        {    

            pid_new_process = fork();       // создаю процесс и записываю возвращенное значение(PID, ошибка)
    
            //printf("pid_new_process = %d\n", pid_new_process); 

            if(pid_new_process == -1)
            {
                printf("Ошибка создания нового процесса.\n");
            }
            else if(pid_new_process == 0)
            {
                //printf("Сейчас мы находимся в порожденном процессе.\n");
                strcpy(path,PATH1);
                strcat(path, name_programm);
                //printf("\npath = %s", path);

                if(parametr1[0] == 0)
                {
                    stat_exec = execl(path, path, NULL);  // указываю путь на образ процесса (выполнить файл)
                }
                else
                { 
                    if(parametr2[0] == 0)
                    {
                        stat_exec = execl(path, path, parametr1, NULL);  // указываю на образ процесса (выполнить файл), а также один аргумент   
                    }
                    else
                    {
                        stat_exec = execl(path, path, parametr1, parametr2, NULL);
                    }
                }
            }
            else
            {
                waitpid(pid_new_process, &status, 0); // ожидаем завершение порожденного процесса   
            }
        }
        else
        {
            flag_exit = 1;
        }
        //---------------------------------------  
        // если команде exec не удалось заменить образ процесса, то остаются два одинаковых процесса.
        // второй процесс завершаем самостоятельно, используя переменную stat_exec
        if(stat_exec == -1)   
        {
            exit(stat_exec);
        } 
        else{} 
        //---------------------------------------
    }
    
    printf("END\n");

}