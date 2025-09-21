//программа сервер
#include <stdio.h>  //стандартная библиотека для работы функции printf 
#include <unistd.h> //библиотека для работы с системными фызовами
//#include <sys/types.h>
//#include <sys/wait.h>               
#include <sys/stat.h>               // mkfifo()
#include <fcntl.h>                  // S_IRUSR, S_IWUSR, O_WRONLY
#include <string.h>                 // strlen()

void main()
{
    char mass[] = "Hi!";
    int fd_fifo = 0;


   unlink("/tmp/my_channel");   // если уже существует файл с таким именем то удаляем его

   // создаем именованный канал для чтения и записи
   if((mkfifo("/tmp/my_channel", S_IRUSR|S_IWUSR)) == -1)
    {
        printf("Ошибка при создании канала fifo.\n");
       
    }
    else
    {
        // открываем канал для записи
        
        printf("Создаем канал fifo. Ждём пока к каналу подключится клиент...\n");
        if((fd_fifo = open("/tmp/my_channel", O_WRONLY)) == -1)
        {
            printf("Ошибка открытия канала.");
        }
        else
        {
            printf("К каналу подключился клиент, записываем в канал данные: %s\n", mass);
            write(fd_fifo, mass, strlen(mass));
            
        }
 
    }

   printf("Клиент считал данные. Завершаем работу сервера.\n"); 
        
}