
#include <termios.h>
#include <sys/ioctl.h>
#include <signal.h>
#include <stdlib.h>
#include <ncurses.h>
#include <dirent.h>
#include <string.h>
//#include <stdio.h>  //стандартная библиотека для работы функции printf
#include <unistd.h> //библиотека для работы с системными фызовами open, write, read
#include <fcntl.h>  // определения для параметров управления файлами
#include <sys/stat.h>
#include <sys/types.h>

#define BIG_FRAME_S 20
#define BIG_FRAME_C 80
#define MAX_WND_NAME 12
#define MAX_lengh_name (BIG_FRAME_C-4)

int position_name0_left_y = 0;   // координата "y" левого верхнего окна "wnd_name_left" 
int position_name0_left_x = 0;   // координата "x" левого верхнего окна "wnd_name_left" 
int position_name0_right_y = 0;   // координата "y" правого верхнего окна "wnd_name_right" 
int position_name0_right_x = 0;   // координата "x" правого верхнего окна "wnd_name_right" 

int position_wnd_prev_left_x = 0;  // координата "x" левого окна "wnd_prev_left"
int position_wnd_prev_left_y = 0;  // координата "y" левого окна "wnd_prev_left"
int position_wnd_prev_right_x = 0; // координата "x" правого окна "wnd_prev_right"
int position_wnd_prev_right_y = 0; // координата "y" правого окна "wnd_prev_right"

int curs_x = 0; // текущие координаты курсора
int curs_y = 0;

int counter_name_in_dir_left = 0;   // полное количество имен в списке каталога (количество возможных запросов readdir чтобы не получить ошибку)
int counter_name_in_dir_right = 0;

int now_num_of_pages_left = 0;       // текущий номер страницы имен (в каталоге неопределенное количество файлов, а количество окон для вывода ограничено, возникает необходимость разбить данные на части)
int now_num_of_pages_right = 0;
int full_num_of_pages_left = 1;       // полное (необходимое) количество страниц имен в открытом каталоге
int full_num_of_pages_right = 1;

int counter_name_left;   // переменная хранит количество задействованных окон "ИМЯ"  при выводе информации из текущей страницы (максимум MAX_WND_NAME имен) внутри каталога
int counter_name_right;

   //------------  перенес переменные из функции main сюда
   DIR * dir_left_pointer;   // указатель на структуру типа DIR открытого каталога 
   DIR * dir_right_pointer;

   struct dirent * dp_prev_left;  // указатель на структуру типа dirent для хранения адреса структуры
                                  // содержащей информацию об иноде (более высокого уровня)
   struct dirent * dp_prev_right; 
   
   struct dirent * dp_now_left;   // указатель на структуру типа dirent для хранения адреса структуры содержащей информацию (адрес?) об иноде текущего каталога
   struct dirent * dp_now_right;

   struct dirent * path_left;       // для сохранения и дальнейшего вывода информации о полном пути к текущему каталогу
   struct dirent * path_right;

   struct dirent * dp_left[MAX_WND_NAME];       // указатель на структуру типа dirent для считывания данных из каталога (имя и инод файла)
   struct dirent * dp_right[MAX_WND_NAME];

   char text_path_left[4096];       // буфер для хранения полного пути к открытой директории
   
   char str1_path_left[BIG_FRAME_C-2];      // разбиваю полный путь на две строки, чтобы вывести в окно по отдельности, т.к. ломается графика при автопереносе слов
   char str2_path_left[BIG_FRAME_C-2];
   
   char text_path_right[4096];
   
   char str1_path_right[BIG_FRAME_C-2];      // разбиваю полный путь на две строки, чтобы вывести в окно по отдельности, т.к. ломается графика при автопереносе слов
   char str2_path_right[BIG_FRAME_C-2];

  struct stat st_info_of_file;   // структура типа stat для временного хранения информации о файле 
  
   //------------



// левая часть файлового менеджера
 WINDOW * big_frame_left;
 WINDOW * wnd_path_left;
 WINDOW * wnd_name_left[MAX_WND_NAME];
 WINDOW * wnd_size_left[MAX_WND_NAME];
 WINDOW * wnd_prev_left;      // указатель на окно для вывода каталога уровнем выше ("..")
 WINDOW * wnd_num_page_left;    // указатель на окно для вывода номера текущей отображенной в менеджере страницы имен из каталога 

// правая часть файлового менеджера   
 WINDOW * big_frame_right;
 WINDOW * wnd_path_right;
 WINDOW * wnd_name_right[MAX_WND_NAME];
 WINDOW * wnd_size_right[MAX_WND_NAME];
 WINDOW * wnd_prev_right;      // указатель на каталог уровнем выше
 WINDOW * wnd_num_page_right;

 WINDOW * wnd_key_big; // окно для создания рамки 
 WINDOW * wnd_key_small; // окно для вывода информации о последней нажатой клавише



// функция обработки сигнала об изменении размеров окна терминала, пока не работает :(
void sig_winch(int signo)
{
    struct winsize size;
    ioctl(fileno(stdout), TIOCGWINSZ, (char *) &size);
    resizeterm(size.ws_row,size.ws_col);
}
//-------------------------------------------------------------------------------------

//--------------------------------------------
// функция для начальной инициализации окон
void init_graphics()
{
   initscr();                   // включаем псевдографический режим работы терминала
   signal(SIGWINCH, sig_winch); // назначаем функцию для обработки сигнала об изменении размеров окна терминала
   //cbreak();                    // задаем режим работы с клавиатурой: не ждать конца строки, обрабатывать специальные клавиши и их комбинации
   noecho();                    // отключаем отображение вводимых с клавиатуры символов в окно терминала
   //curs_set(0);                 // отключаем видимость курсора
   curs_set(1);
   refresh();
   start_color();      // инициализируем управление цветом
   init_pair(1, COLOR_WHITE, COLOR_BLUE);
   init_pair(2, COLOR_WHITE, COLOR_BLACK);
// создание окон для файлового менеджера

//большие окна
big_frame_left = newwin(BIG_FRAME_S,BIG_FRAME_C, 1,1);  // создаем новое окно: количество строк, количество символов, левый верхний угол строка и столбец
big_frame_right = newwin(BIG_FRAME_S,BIG_FRAME_C, 1, (2+BIG_FRAME_C));
wbkgd(big_frame_left, COLOR_PAIR(1));   // задаем цвет фона из цветовой пары
wbkgd(big_frame_right, COLOR_PAIR(1));

box(big_frame_left, '|','-');  // рисуем рамку вокруг окна
box(big_frame_right, '|','-');  // рисуем рамку вокруг окна

//окно последней нажатой клавиши "большое", для создания рамки

wnd_key_big = newwin(3,BIG_FRAME_C*2 + 1,BIG_FRAME_S+1,1);
wbkgd(wnd_key_big, COLOR_PAIR(1));
box(wnd_key_big, '|','-');  // рисуем рамку вокруг окна

// окно для вывода информации о  последней нажатой клавише
wnd_key_small = derwin(wnd_key_big, 1,(BIG_FRAME_C*2 - 1), 1, 1);
wbkgd(wnd_key_small, COLOR_PAIR(1));

// окна адресов
wnd_path_left = derwin(big_frame_left, 2,(BIG_FRAME_C-2), 1, 1); 
wnd_path_right = derwin(big_frame_right, 2,(BIG_FRAME_C-2), 1, 1);
wbkgd(wnd_path_left, COLOR_PAIR(2));
wbkgd(wnd_path_right, COLOR_PAIR(2));

// окноа возврата в предыдущий каталог
wnd_prev_left = derwin(big_frame_left, 1,(MAX_lengh_name), 4, 1);
wnd_prev_right = derwin(big_frame_right, 1,(MAX_lengh_name), 4, 1);
wbkgd(wnd_prev_left, COLOR_PAIR(2));
wbkgd(wnd_prev_right, COLOR_PAIR(2));

// окна файлов и каталогов в текущей директории
for(int ind = 0; ind < MAX_WND_NAME ; ind++ )
{
  // для левой части файлового менеджера
     wnd_name_left[ind] = derwin(big_frame_left, 1,(MAX_lengh_name), (5 + ind), 1);
     wbkgd(wnd_name_left[ind], COLOR_PAIR(2));
  // для правой части файлового менеджера
     wnd_name_right[ind] = derwin(big_frame_right, 1,(MAX_lengh_name), (5 + ind), 1);
     wbkgd(wnd_name_right[ind], COLOR_PAIR(2));
}

// окна с номером текущей страницы внутри каталога
wnd_num_page_left = derwin(big_frame_left, 1,(BIG_FRAME_C-2), 6+MAX_WND_NAME, 1);
wnd_num_page_right = derwin(big_frame_right, 1,(BIG_FRAME_C-2), 6+MAX_WND_NAME, 1);
wbkgd(wnd_num_page_left, COLOR_PAIR(2));
wbkgd(wnd_num_page_right, COLOR_PAIR(2));

getbegyx(wnd_name_left[0], position_name0_left_y, position_name0_left_x);   // сохраняю координаты левого окна "имя" (левый верхний угол)
getbegyx(wnd_name_right[0], position_name0_right_y, position_name0_right_x);   // сохраняю координаты правого окна "имя" (левый верхний угол)

// обновление изображения в окне терминала
   for(int ind = 0; ind < MAX_WND_NAME ; ind++ )
   {
    wrefresh(wnd_name_left[ind]);
    wrefresh(wnd_name_right[ind]);
   }

   wrefresh(wnd_prev_left);
   wrefresh(wnd_prev_right);
   
   wrefresh(wnd_path_left);
   wrefresh(wnd_path_right);
   
   wrefresh(wnd_num_page_left);
   wrefresh(wnd_num_page_right);
  
   wrefresh(wnd_key_small);
  
   wrefresh(big_frame_left);
   wrefresh(big_frame_right);
   
   wrefresh(wnd_key_big);
   
   refresh();                    // обновить изображение в терминале

}

// функция обновления информации во всех окнах терминала
void window_refresh()
{
    for(int ind = 0; ind < MAX_WND_NAME ; ind++ )
        {
            wrefresh(wnd_name_left[ind]);
            wrefresh(wnd_name_right[ind]);
        }   

   wrefresh(wnd_prev_left);
   wrefresh(wnd_prev_right);

   wrefresh(wnd_path_left);
   wrefresh(wnd_path_right);

   wrefresh(wnd_num_page_left);
   wrefresh(wnd_num_page_right);
   wrefresh(wnd_key_small);
   wrefresh(big_frame_left);
   wrefresh(big_frame_right);
   wrefresh(wnd_key_big);
   
   refresh();  

}

// функция подсчета имен в списке каталога, возвращает количество страниц необходимых для отображения всех файлов
int counter_name_in_catalog(int left__or__right)
{   
    struct dirent * dirp = 0;
    int counter_name_dir = 0;
    int max_pages_dir = 0;

      
  
  if(left__or__right == 0)
  {
    rewinddir (dir_left_pointer);    // функция переводит файловую позицию чтения в начало каталога
    while((dirp = readdir(dir_left_pointer)) != 0)
        {
         counter_name_dir++;
        }
  }
  else
  {
    rewinddir (dir_right_pointer);    // функция переводит файловую позицию чтения в начало каталога
    while((dirp = readdir(dir_right_pointer))!= 0)
    {
        counter_name_dir++;
    }
  }

    counter_name_dir = counter_name_dir - 2; //временно отнимаю число 2, т.к. в списке каталога всегда есть две записи с именами "." и "..", которые я обрабатываю отдельно (не вывожу в окна для имён)
                                                
    // вычисляем сколько требуется страниц для отображения всех файлов из каталога
    max_pages_dir = counter_name_dir/MAX_WND_NAME; 
   if(max_pages_dir == 0)  // если результат целочисленного деления 0, то потребуется одна страница
   {
    max_pages_dir =1;      // одной страницы будет достаточно
   }
   else
   {
    if((max_pages_dir%MAX_WND_NAME) > 0)   // если остаток от деления больше нуля, т.е. количество имен больше чем вмещается на одной странице
    {
        max_pages_dir = max_pages_dir + 1; // добавляем дополнительную страницу
    }
    else{}
   }

    if(left__or__right == 0)      // сохраняем данные о полном количестве имен в списке (включая "." и "..") в глобальных переменных counter_name_in_dir_left либо в counter_name_in_dir_right
    {
       counter_name_in_dir_left = counter_name_dir+2; // возвращаю два имени, т.к. мне в других функциях потребуется полное количество имен в списке каталога
    }
    else
    {
       counter_name_in_dir_right = counter_name_dir+2; // возвращаю два имени, т.к. мне в других функциях потребуется полное количество имен в списке каталога
    }

    return max_pages_dir;
    
}
//функции изменения строки адреса каталога
void path_subtraction(char * text_path)
{   
   char * place_pointer;  // указатель на последний символ "/"
   place_pointer = strrchr(text_path, '/');    //  ищет символ в строке справа-налево
   *place_pointer = '\0'; // записать в ячейку по адресу указателя значение символа конца строки
}

void path_addition(char * text_path, char * add_text_path)
{
   strcat(text_path,"/");   // объединяет две строки и записывает  результирующую строку
   strcat(text_path, add_text_path); // добавляет в конец первой строки вторую
}
//  функция открытия директории и извлечения из нее информации в окна половины терминала
//----------------------
void write_win_manager(int left_or_right)
{   
    int flag_end_dir = 0;  // флаг конца каталога
    int index_wnd_name = 0; // индекс окна, необходим для того, чтобы не было пропусков в окнах имен, т.к. "." и ".."  записываются в отдельные окна    
    struct dirent * temp_struct_dirent;   // временный указатель для сортировки имен
    int i = 0;  // счетчик файловой позиции
    int i2 = 0;   // пока в таком виде, дополнительный индекс на случай считывания "." или ".." (нужен для возможности корректного вывода имен в окна)
    char str_info_temp[4096]; // строка для проверки атрибутов файлов (каталог или нет)
    //int ind = 0;
    
    str_info_temp[0] = '\0'; // делаю строку пустой

    if(0 == left_or_right)
  { 
    
    counter_name_left = 0;  //обнуляем счетчик использованных для записи имени окон "ИМЯ"

    rewinddir (dir_left_pointer);    // функция переводит файловую позицию чтения в начало каталога

  // задача вычитать MAX_WND_NAME имен из файла каталога, не считая имен "." и "..", в тоже время необходимо учесть, что смещение файловой позиции при считывании таких полей в том числе
  // для этого ввожу дополнительную переменную ind2
  while(i < MAX_WND_NAME*now_num_of_pages_left) // считываем в пустоту структуры из каталога, чтобы перевести файловую позицию на начало необходимой страницы now_num_of_pages_left
  {
   
   temp_struct_dirent = readdir(dir_left_pointer);
   
   if(strcmp((temp_struct_dirent->d_name), ".") == 0)      // сравнение строк
   {
       dp_now_left = temp_struct_dirent;  // сохраняем указатель на структуру типа dirent в которой находится поле d_ino с инодом текущего "файла каталога"
       i2++;
   }
   else
   {
      if(strcmp((temp_struct_dirent->d_name), "..") == 0)
      {
        dp_prev_left = temp_struct_dirent;  // сохраняем указатель на структуру типа dirent в которой находится поле d_ino с инодом предыдущего "файла каталога"
        i2++;
      }
      else
      {
        i++;
      }
   }   
  }

    
    //seekdir(dir_pointer, telldir(dir_pointer)); // устанавливаем файловую позицию  
    
    wclear(wnd_num_page_left); // заполняем окно индекса страниц пробелами
    wprintw(wnd_num_page_left,"Page %d of %d.", (now_num_of_pages_left+1), full_num_of_pages_left); // выводим информацию о номере страницы 
    
    getyx(stdscr, curs_y,curs_x);       // считываю текущие координаты курсора для последующего возврата в эту позицию
    wclear(wnd_path_left);
    move(curs_y,curs_x);    //возвращаю курсор на место
    wprintw(wnd_path_left,"%s", text_path_left); // записываем путь в левое окно
    
    
    if(dir_left_pointer != NULL)        
    {
    
    while (index_wnd_name < MAX_WND_NAME)       // считываем последовательно MAX_WND_NAME записей из директории
    {
        if(i < (counter_name_in_dir_left-i2))
        {
            // ничего не делаем
        }
        else
        {
          flag_end_dir = 1;  // выставляем флаг запрещающий дальнейшие запросы readdir так как список в каталоге закончился
        }
        
        if(flag_end_dir == 0)
        {   
            temp_struct_dirent = readdir(dir_left_pointer);
            
             
            //------------------ этот кусочек пока не используется
            if(strcmp((temp_struct_dirent->d_name), ".") == 0)      // сравнение строк
            {
                dp_now_left = temp_struct_dirent;  // сохраняем указатель на структуру типа dirent в которой находится поле d_ino с инодом текущего "файла каталога"
            }
            else{}
            //------------------
            
            if(( temp_struct_dirent != 0) && (strcmp((temp_struct_dirent->d_name), ".") != 0) && (strcmp((temp_struct_dirent->d_name), "..") != 0)) //если не достигнут конец списка и не текущий и не предыдущий каталог
            {
                        
                dp_left[index_wnd_name] = temp_struct_dirent;
              
                // проверка типа файла
                    //---------------------------------   
                    strcpy(str_info_temp, text_path_left);   // копирую путь текущего каталога в строку
                    path_addition(str_info_temp, dp_left[index_wnd_name]->d_name);  // добавляю к строке косую черту и имя файла в каталоге
                    stat(str_info_temp, &st_info_of_file);
                    //   проверить является ли имя в текущем окне каталогом, если да, то     
                    // S_IFMT - битовая маска для извлечения числа кодирующего тип файла
                    if ((st_info_of_file.st_mode & S_IFMT) == S_IFDIR)          // проверяем является ли данный файл каталогом проверяя установлен ли бит S_IFDIR
                    {
                        //сделать жирным текст в окне
                        wattron(wnd_name_left[index_wnd_name],A_BOLD);
                    }
                    else
                    {
                        //сделать текст в окне обычным
                        wattroff(wnd_name_left[index_wnd_name],A_BOLD);
                    }
                    //-----------------------------------
               
                wclear(wnd_name_left[index_wnd_name]);
                wprintw(wnd_name_left[index_wnd_name],"%s", dp_left[index_wnd_name]->d_name);
                index_wnd_name++;  // переходим к следующему свободному окну "имя"
                counter_name_left++; // увеличивааем счетчик окон с записанными именами
                
            }
            else{}

            if(strcmp((temp_struct_dirent->d_name), "..") == 0)      // сравнение строк
            {
                dp_prev_left = temp_struct_dirent;  // сохраняем указатель на структуру типа dirent в которой находится поле d_ino с инодом предыдущего "файла каталога"
            }
            else{}
            

            i++;    //увеличиваем счетчик (количество считываний readdir, не должно превышать полного количества имен в списке каталога)
        }
        else
        {
            wclear(wnd_name_left[index_wnd_name]); // стираем содержимое окна
            index_wnd_name++;  // переходим к следующему свободному окну "имя"
        }

      //---------------------------
    }
    flag_end_dir = 0; // сбрасываю флаг конца файла директории
    
    if(index_wnd_name < MAX_WND_NAME)   // если остались свободные окна "имя", то зачищаем их
    {   for(int indx=0; indx < (MAX_WND_NAME-index_wnd_name);indx++)
        {
          wclear(wnd_name_left[index_wnd_name+indx]); // очистка
          
        }
    }
    else{}
    }
    else
    {}
    move(position_wnd_prev_left_y, position_wnd_prev_left_x);  // перемещаю курсор на первое окно имени в правой части
    
 }
    else
    {
        counter_name_right = 0;  //обнуляем счетчик использованных для записи имени окон "ИМЯ"

        rewinddir(dir_right_pointer);    // функция переводит файловую позицию чтения в начало каталога
 
        while(i < MAX_WND_NAME*now_num_of_pages_right) // считываем в пустоту структуры из каталога, чтобы перевести файловую позицию на начало необходимой страницы now_num_of_pages_left
        {
         
         temp_struct_dirent = readdir(dir_right_pointer);
         
         if(strcmp((temp_struct_dirent->d_name), ".") == 0)      // сравнение строк
         {
             dp_now_right = temp_struct_dirent;  // сохраняем указатель на структуру типа dirent в которой находится поле d_ino с инодом текущего "файла каталога"
         }
         else
         {
            if(strcmp((temp_struct_dirent->d_name), "..") == 0)
            {
              dp_prev_right = temp_struct_dirent;  // сохраняем указатель на структуру типа dirent в которой находится поле d_ino с инодом предыдущего "файла каталога"
            }
            else
            {
              i++;
            }
         }   
        }        
         
          
        
        wclear(wnd_num_page_right); // очистка окна с номером страницы
        
        wprintw(wnd_num_page_right,"Page %d of %d.", (now_num_of_pages_right+1), full_num_of_pages_right); // выводим информацию о номере страницы
        
        getyx(stdscr, curs_y,curs_x);       // считываю текущие координаты курсора для последующего возврата в эту позицию
        wclear(wnd_path_right);
        move(curs_y,curs_x); 
        wprintw(wnd_path_right,"%s", text_path_right); // записываем путь в левое окно
        
        if(dir_right_pointer != NULL)        
        {
        
        while (index_wnd_name < MAX_WND_NAME)       // считываем последовательно MAX_WND_NAME записей из директории
          {
               if(i < counter_name_in_dir_right-1)
                {
                    // ничего не делаем
                }
                else
                {
                  flag_end_dir = 1;  // выставляем флаг запрещающий дальнейшие запросы readdir так как список в каталоге закончился
                }
            
            //flag_end_dir = 1;
    
            if(flag_end_dir == 0)
            {   
                temp_struct_dirent = readdir(dir_right_pointer);
                
                
            //------------------ этот кусочек пока не используется
            if(strcmp((temp_struct_dirent->d_name), ".") == 0)      // сравнение строк
            {
                dp_now_right = temp_struct_dirent;  // сохраняем указатель на структуру типа dirent в которой находится поле d_ino с инодом текущего "файла каталога"
            }
            else{}
            //------------------
                
    
                if(( temp_struct_dirent != 0) && (strcmp((temp_struct_dirent->d_name), ".") != 0) && (strcmp((temp_struct_dirent->d_name), "..") != 0)) //если не достигнут конец списка и не текущий и не предыдущий каталог
                {
                                     
                    dp_right[index_wnd_name] = temp_struct_dirent;
                    // проверка типа файла
                    //---------------------------------   
                    strcpy(str_info_temp, text_path_right);   // копирую путь текущего каталога в строку
                    path_addition(str_info_temp, dp_right[index_wnd_name]->d_name);  // добавляю к строке косую черту и имя файла в каталоге
                    stat(str_info_temp, &st_info_of_file);
                    // проверить является ли имя в текущем окне каталогом, если да, то     
                    // S_IFMT - битовая маска для извлечения числа кодирующего тип файла
                    if ((st_info_of_file.st_mode & S_IFMT) == S_IFDIR)          // проверяем является ли данный файл каталогом проверяя установлен ли бит S_IFDIR
                    {
                       // сделать жирным текст в окне
                        wattron(wnd_name_right[index_wnd_name],A_BOLD);
                    }
                    else
                    {
                        wattroff(wnd_name_right[index_wnd_name],A_BOLD);
                       // сделать текст в окне обычным
                    }
                    //-----------------------------------
                    wclear(wnd_name_right[index_wnd_name]);
                    wprintw(wnd_name_right[index_wnd_name],"%s", dp_right[index_wnd_name]->d_name);
                    index_wnd_name++;  // переходим к следующему свободному окну "имя"
                    counter_name_right++; // увеличивааем счетчик окон с записанными именами
                    
                }
                else{}
    
                if(strcmp((temp_struct_dirent->d_name), "..") == 0)      // сравнение строк
                {
                    dp_prev_right = temp_struct_dirent;  // сохраняем указатель на структуру типа dirent в которой находится поле d_ino с инодом предыдущего "файла каталога"
                    //flag_end_dir = 1;           // выставляем флаг, т.к. больше нельзя применять команду readdir (возникнет ошибка)
                }
                else{}
                
    
                i++;    //увеличиваем счетчик (количество считываний readdir, не должно превышать полного количества имен в списке каталога)
            }
            else
            {
                // функция strcpy вызывает ошибку памяти, надо проверять длину буфера приема перед копированием, переполняется буфер
                //strcpy(dp_right[index_wnd_name]->d_name, "$"); // в поле d_name всех оставшихся структур в массиве записываю строку содержащую один символ (пока произвольный), чтобы отсечь их обработку в дальнейшем
                //wnd_name_temp = wnd_name_temp + index_wnd_name;
                wclear(wnd_name_right[index_wnd_name]); // стираем содержимое окна
                //wrefresh(wnd_name_left[index_wnd_name]);
                index_wnd_name++;  // переходим к следующему свободному окну "имя"
            }

           // ind++;  //переходим к следующему окну
        }
        flag_end_dir = 0; // сбрасываю флаг конца файла директории
        
        if(index_wnd_name < MAX_WND_NAME)   // если остались свободные окна "имя", то зачищаем их
        {   for(int indx=0; indx < (MAX_WND_NAME-index_wnd_name);indx++)
            {
                wclear(wnd_name_right[index_wnd_name+indx]); // очистка
              //wrefresh(wnd_name_left[index_wnd_name+i]);
            }
        }
        else{}
        }
        else
        {}
        move(position_wnd_prev_right_y, position_wnd_prev_right_x);  // перемещаю курсор на первое окно имени в правой части

    }
//-----+++---+++---+++---+++---+++---+++


}
//----------------------
//обработка кнопок

void button(void)
{
int key_00 = 0;
int key_01 = 0;
int key_02 = 0;
int key = 0;

key=getch();        // клавиши со стрелками возвращают последовательность из трех кодов 91 ()

if(key == 9)    // нажата клавиша Tab
{   
    getyx(stdscr, curs_y,curs_x);       // считываю текущие координаты курсора для последующего возврата в эту позицию
    wclear(wnd_key_small);
    wprintw(wnd_key_small, "The TAB key was pressed, its code ASCII: %d.", key);
    wrefresh(wnd_key_small);
    move(curs_y, curs_x); // возвращаю курсор на начальную позицию

    if(curs_x == position_name0_right_x)
    {
        if(curs_y < (position_name0_left_y + counter_name_left))
        {
        move(curs_y, position_name0_left_x); // перемещаю курсор в левую половину файлового менеджера
        // +1. перейти в левый каталог на окно напротив текущего (курсор на окно)
        // 2. изменить цветовую схему окна
        }
        else
        {
          move(position_name0_left_y, position_name0_left_x); // перемещаю курсор в левую половину файлового менеджера  
        }
    }
    else
    {
        if(curs_y < (position_name0_right_y + counter_name_right))
        {
        move(curs_y, position_name0_right_x); // перемещаю курсор в правую половину файлового менеджера
        // +1. перейти в левый каталог на окно напротив текущего (курсор на окно)
        // 2. изменить цветовую схему окна
        }
        else
        {
          move(position_name0_right_y, position_name0_right_x); // перемещаю курсор в правую половину файлового менеджера  
        }
    }
}
else
{
    if(key == 27)
    {   
        key_00 = key;
        key = getch();
    }
    else{}

    if (key == 91)
    {
        key_01 = key;
        key = getch();
    }
    else{}  
}


switch(key) 
{       


            case 65:
            {
                // клавиша вверх UP (DEC: 27, 91, 65) 
                //+1. если курсор находится не в самом верхнем окне, то переместить позицию курсора вверх
                //+2. если курсор уже находится в самом верхнем окне, то переместить курсор на самое нижнее окно 
                // 3. изменить цветовую схему в выделенном окне
                
                getyx(stdscr, curs_y,curs_x);       // считываю текущие координаты курсора для последующего возврата в эту позицию
                wclear(wnd_key_small);
                wprintw(wnd_key_small, "The UP key was pressed, its code ASCII: %d, %d, %d.", key_00, key_01, key);
                wrefresh(wnd_key_small);
                move(curs_y, curs_x); // возвращаю курсор на начальную позицию
                
                getyx(stdscr, curs_y,curs_x);
                if(curs_x == position_name0_left_x)
                {
                    if(curs_y > position_name0_left_y-1)
                    {
                        move(curs_y-1, curs_x);
                    
                    }
                    else
                    {
                        move((position_name0_left_y + counter_name_left-1), curs_x); // перемещаем курсор на последнее занятое окно "имя"
                    }
                        
                    
                }
                else
                {
                    if(curs_y > position_name0_right_y-1)
                    {
                        move(curs_y-1, curs_x);
                    
                    }
                    else
                    {
                       move((position_name0_right_y+counter_name_right-1), curs_x); // перемещаем курсор на последнее занятое окно "имя"
                    }
                }

                 
                break;
            }
            case 66: 
            {
                // клавиша вниз Down (DEC: 27, 91, 66)
                // +1. переместить позицию курсора вниз, если не достигнуто нижнее окно с именем
                // +2. если курсор уже находится в последнем окне, то переместить курсор на самое верхнее окно ".."
                // 3. изменить цветовую схему окна в котором находится курсор
                getyx(stdscr, curs_y,curs_x);       // считываю текущие координаты курсора для последующего возврата в эту позицию
                wclear(wnd_key_small);
                wprintw(wnd_key_small, "The DOWN key was pressed, its code ASCII: %d, %d, %d.", key_00, key_01, key);
                wrefresh(wnd_key_small);
                move(curs_y, curs_x); // возвращаю курсор на начальную позици
                
                getyx(stdscr, curs_y,curs_x);
                if(curs_x == position_name0_left_x)
                    {
                        if(curs_y < (position_name0_left_y + counter_name_left-1))
                            {
                                move((curs_y + 1), curs_x);
                    
                            }
                        else
                            {
                                move((position_name0_left_y - 1), curs_x);
                    
                            }
                    }
                    else
                    {
                        if(curs_y < (position_name0_right_y + counter_name_right-1))
                        {
                            move((curs_y + 1), curs_x);
                
                        }
                    else
                        {
                            move((position_name0_right_y - 1), curs_x);
                
                        }
                    }                

                break;
            }
           
           // две кнопки обработки Page UP и Page Down для перехода между страницами
           case 53:
           {
            
            key_02 = getch();
            if(key_02 == 126)
            {

            // переход на страницу выше Page Up (DEC: 27, 91, 53, 126)
            getyx(stdscr, curs_y,curs_x);       // считываю текущие координаты курсора для последующего возврата в эту позицию
            wclear(wnd_key_small);
            wprintw(wnd_key_small, "The Page UP key was pressed, its code ASCII: %d, %d, %d, %d.", key_00, key_01, key, key_02);
            wrefresh(wnd_key_small);
            move(curs_y, curs_x); // возвращаю курсор на начальную позици
            // 1. если текущая страницы не первая,для этой половины менеджера,
            //    то перейти на предыдущую страницу обновив все окна имен и окно номера страницы
           
            getyx(stdscr, curs_y,curs_x);       // считываю текущие координаты курсора
            if(curs_x == position_name0_left_x) // выбор половины окна (левая или правая)
            {   
                
                if(now_num_of_pages_left > 0)
                {   // 2. вызвать функцию заполнения данных в окна для новой страницы
                    // 3. обновить изображение на экране
                    now_num_of_pages_left = now_num_of_pages_left - 1;
                    write_win_manager(0);
                    window_refresh();
                }
                else
                {}
             
            }
            else
            {
                
                if(now_num_of_pages_right > 0)
                {
                    // 2. вызвать функцию заполнения данных в окна для новой страницы
                    // 3. обновить изображение на экране
                    now_num_of_pages_right = now_num_of_pages_right - 1;
                    write_win_manager(1);
                    window_refresh(); 
                }
                else
                {}
                
            }
            }
            else{}
            break;
           }

           case 54:
           {

           key_02 = getch();
           if(key_02 == 126)
            {
              // переход на страницу ниже Page Down (DEC: 27, 91, 54, 126)
              getyx(stdscr, curs_y,curs_x);       // считываю текущие координаты курсора для последующего возврата в эту позицию
              wclear(wnd_key_small);
              wprintw(wnd_key_small, "The Page Down key was pressed, its code ASCII: %d, %d, %d, %d.", key_00, key_01, key, key_02);
              wrefresh(wnd_key_small);
              move(curs_y, curs_x); // возвращаю курсор на начальную позиции

              // 1. если текущий номер страницы меньше чем максимум для этой половины менеджера,
              //    то перейти на следующую страницу обновив все окна имен и окно номера страницы
            getyx(stdscr, curs_y,curs_x);       // считываю текущие координаты курсора
            if(curs_x == position_name0_left_x)
            {
                if(now_num_of_pages_left < full_num_of_pages_left-1)
                {
                    now_num_of_pages_left = now_num_of_pages_left + 1;
                    write_win_manager(0);
                    window_refresh(); 
                }
                else
                {}
                // 2. вызвать функцию заполнения данных в окна для новой страницы
                // 3. обновить изображение на экране
            }
            else
            {
                if(now_num_of_pages_right < full_num_of_pages_right-1)
                {
                    now_num_of_pages_right = now_num_of_pages_right + 1;
                    write_win_manager(1);
                    window_refresh(); 
                }
                else
                {}
                // 2. вызвать функцию заполнения данных в окна для новой страницы
                // 3. обновить изображение на экране
            }
            }
            else{}
            break;
           }
          
}
// если была нажата клавиша Enter
if(key == 10)
{
    
    getyx(stdscr, curs_y,curs_x);       // считываю текущие координаты курсора для последующего возврата в эту позицию
    wclear(wnd_key_small);
    wprintw(wnd_key_small, "The ENTER key was pressed, its code ASCII: %d.", key);
    wrefresh(wnd_key_small);
    move(curs_y, curs_x); // возвращаю курсор на начальную позици

    getyx(stdscr, curs_y,curs_x);               // обновляем информацию о местоположении курсора
   
    if(curs_x == position_name0_left_x)         // курсор находится в левом столбике имен
    {   
        if(curs_y == position_wnd_prev_left_y)    // курсор стоит на окне ".." 
        {           
          // 1. изменить сохраненный путь к каталогу вырезав с конца имя текущего каталога и косую черту
           path_subtraction(text_path_left); // функция принимает указатель на массив в котором хранится строка (путь к каталогу) и удаляет из этой строки все символы начиная с последней косой черты 
          // 2. закрыть предыдущий каталог используя указатель типа DIR на него
          closedir(dir_left_pointer);
          //2.1 вернуть номер текущей страницы на первую (иначе в новом каталоге будет попытка открыть несуществующую страницу и возникнет ошибка)
            now_num_of_pages_left = 0;
          // 3. открыть новый каталог по сохраненному адресу
          dir_left_pointer = opendir(text_path_left); // открываем файл каталога по адресу
          //3.1 подсчитать количество страниц необходимых для вывода имен из каталога, сохранить в full_num_of_pages_left
          full_num_of_pages_left = counter_name_in_catalog(0);
          // 4. вызвать функцию заполнения данных в окна начиная с первой страницы для нового каталога
          write_win_manager(0); //вызываю функцию обновления данных в окнах (аргумент 0 соответствует левой части в файловом менеджере)
          
        }
        else
        {
          
            // 1. изменить сохраненный путь к каталогу добавив в него косую черту и имя нового, вложенного в него каталога
            path_addition(text_path_left,(dp_left[curs_y - position_name0_left_y]->d_name)); // функция принимает указатель на массив в котором хранится строка (путь к каталогу) и добавляет в конце этой строки косую черту и имя вложенного каталога 
           
            // проверка типа файла
            stat(text_path_left, &st_info_of_file);
            // проверить является ли имя в текущем окне каталогом, если да, то     
            // S_IFMT - битовая маска для извлечения числа кодирующего тип файла
            if ((st_info_of_file.st_mode & S_IFMT) == S_IFDIR)          // проверяем является ли данный файл каталогом проверяя установлен ли бит S_IFDIR
            {
                // 2. закрыть предыдущий каталог используя указатель типа DIR на него
                 closedir(dir_left_pointer);
                 //2.1 вернуть номер текущей страницы на первую (иначе в новом каталоге будет попытка открыть несуществующую страницу и возникнет ошибка)
                 now_num_of_pages_left = 0;
                 // 3. открыть новый каталог по сохраненному адресу
                 dir_left_pointer = opendir(text_path_left); // открываем файл каталога по адресу 
                 //3.1 подсчитать количество страниц необходимых для вывода имен из каталога, сохранить в full_num_of_pages_left
                 full_num_of_pages_left = counter_name_in_catalog(0);
                 // 4. вызвать функцию заполнения данных в окна начиная с первой страницы для нового каталога
                 write_win_manager(0);
            }
            else
            {
                 path_subtraction(text_path_left); // возвращаем старый адрес, т.к. имя в текущем окне не является каталогом
            }
         
           
        }
    }
    else
    {

        if(curs_y == position_wnd_prev_right_y)    // курсор стоит на окне ".."
        {   
            // 1. изменить сохраненный путь к каталогу добавив в него косую черту и имя нового, вложенного в него каталога 
            path_subtraction(text_path_right); // функция принимает указатель на массив в котором хранится строка (путь к каталогу) и удаляет из этой строки все символы начиная с последней косой черты 
            // 2. закрыть предыдущий каталог используя указатель типа DIR на него
            closedir(dir_right_pointer);
            //2.1 вернуть номер текущей страницы на первую (иначе в новом каталоге будет попытка открыть несуществующую страницу и возникнет ошибка)
            now_num_of_pages_right = 0;
            // 3. открыть новый каталог по сохраненному адресу
            dir_right_pointer = opendir(text_path_right); // открываем файл каталога по текущему адресу 
            //3.1 подсчитать количество страниц необходимых для вывода имен из каталога, сохранить в full_num_of_pages_right
            full_num_of_pages_right = counter_name_in_catalog(1);
            // 4. вызвать функцию заполнения данных в окна начиная с первой страницы для нового каталога
            write_win_manager(1);
            
        }
        else
        {
            
            // 1. изменить сохраненный путь к каталогу добавив в него косую черту и имя нового, вложенного в него каталога 
            // значение dp_right
            path_addition(text_path_right,(dp_right[curs_y - position_name0_right_y]->d_name)); // функция принимает указатель на массив в котором хранится строка (путь к каталогу) и добавляет в конце этой строки косую черту и имя вложенного каталога
            
            stat(text_path_right, &st_info_of_file);
            // проверить является ли имя в текущем окне каталогом, если да, то   
            // S_IFMT - битовая маска для извлечения числа кодирующего тип файла
            if ((st_info_of_file.st_mode & S_IFMT) == S_IFDIR)          // проверяем является ли данный файл каталогом проверяя установлен ли бит S_IFDIR
            {
                // 2. закрыть предыдущий каталог используя указатель типа DIR на него
                closedir(dir_right_pointer);
                //2.1 вернуть номер текущей страницы на первую (иначе в новом каталоге будет попытка открыть несуществующую страницу и возникнет ошибка)
                now_num_of_pages_right = 0;
                // 3. открыть новый каталог по сохраненному адресу
                dir_right_pointer = opendir(text_path_right); // открываем файл каталога по текущему адресу
                //3.1 подсчитать количество страниц необходимых для вывода имен из каталога, сохранить в full_num_of_pages_right
                full_num_of_pages_right = counter_name_in_catalog(1); 
                // 4. вызвать функцию заполнения данных в окна начиная с первой страницы для нового каталога (передаем в нее указатель на открытую директорию, указатель на массив структур dirent, номер страницы имен)
                write_win_manager(1);
            }
            else
            {
               path_subtraction(text_path_right); // возвращаем старый адрес, т.к. имя в текущем окне не является каталогом 
            }
        
        }
    }
window_refresh();   
}
else{}

}
void main()
{  
    
   init_graphics();     // функция инициализации графического оформления терминала

   // изначальное заполнение таблиц файлового менеджера 
   //-------------------------------------------------------------------

   // левая сторона
   getcwd(text_path_left, 255); // узнаем и сохраняем абсолютное имя пути к текущему каталогу в левом окне
       
   dir_left_pointer = opendir(text_path_left); // открываем файл каталога по текущему адресу (каталог из которого запущен процесс (эта программа))
   // подсчитать количество страниц необходимых для вывода имен из каталога, сохранить в full_num_of_pages_left
   full_num_of_pages_left = counter_name_in_catalog(0);
 //----- 
   wclear(wnd_num_page_left); // заполняем окно индекса страниц пробелами
   wprintw(wnd_num_page_left,"Page %d of %d.", (now_num_of_pages_left+1), full_num_of_pages_left); // выводим информацию о номере страницы
//------

   // вызвать функцию заполнения данных в окна начиная с первой страницы для нового каталога
   write_win_manager(0);
   wprintw(wnd_prev_left,"%s",".."); // отображаем точки всегда в верхнем окне, (перейти в каталог на уровень выше)

   // правая сторона
   getcwd(text_path_right, 255); // узнаем и сохраняем абсолютное имя пути к текущему каталогу в правом окне
   dir_right_pointer = opendir(text_path_right);
   //подсчитать количество страниц необходимых для вывода имен из каталога, сохранить в full_num_of_pages_right
   full_num_of_pages_right = counter_name_in_catalog(1);
//------ 
   wclear(wnd_num_page_right); // заполняем окно индекса страниц пробелами
   wprintw(wnd_num_page_right,"Page %d of %d.", (now_num_of_pages_right+1), full_num_of_pages_right); // выводим информацию о номере страницы
//------
   //вызвать функцию заполнения данных в окна начиная с первой страницы для нового каталога
   write_win_manager(1);
   wprintw(wnd_prev_right,"%s",".."); // отображаем точки всегда в верхнем окне, (перейти в каталог на уровень выше)
   //------------------------------------------------------------------
  
 
      
   window_refresh();    // обновляем информацию в окнах терминала
  
   getbegyx(wnd_prev_left, position_wnd_prev_left_y, position_wnd_prev_left_x);   // запрашиваю координаты верхнего левого окна ".."
   getbegyx(wnd_prev_right, position_wnd_prev_right_y, position_wnd_prev_right_x);   // запрашиваю координаты верхнего правого окна ".."

   move(position_name0_left_y, position_name0_left_x);  // перемещаю курсор на первое окно имени в левой части
 
 // временное зацикливание 
  
  while(1)
  {

  button();
  }

  endwin();
 
  getch();  
 
   
exit(EXIT_SUCCESS);
   
   
 
}