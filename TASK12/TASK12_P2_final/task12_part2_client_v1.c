//программа сервер
#include <stdio.h>  //стандартная библиотека для работы функции printf
#include <unistd.h> //библиотека для работы с системными фызовами
//#include <sys/types.h>
//#include <sys/wait.h> 
//#include <sys/stat.h>
#include <fcntl.h>        // O_RDONLY
//#include <string.h>

void main()
{
    char mass[100];
    int fd_fifo = 0;
       
        // открываем канал для записи
        
        if((fd_fifo = open("/tmp/my_channel", O_RDONLY)) == -1)
        {
            printf("Ошибка открытия канала.\n");
        }
        else
        {
          int ind = 0;
          int flag = 0;
          ssize_t status = 1; 
         
          while(status > 0 )
          {

           status = read(fd_fifo, &mass[ind], 1);
           //printf("status =%ld, mass[%d] = %c\n", status, ind, mass[ind]);
           ind++;
           
          }
        printf("Из канала было считано: %s\n", mass);    
        unlink("/tmp/my_channel");   //удаляем канал
        }
 


printf("Завершение работы клиента.\n"); 
        
}