
#include <stdio.h>
#define MAX 100 // количество элементов в массиве структур (не более 999 элементов, ограничение адресации массива confirmation при использовнии функции del_abonent)
#define MAX_field 10 // размер массива в структуре
struct abonent
{
    char name[MAX_field];
    char second_name[MAX_field];
    char tel[MAX_field];
}; 

struct abonent mas[MAX];    // объявляем массив структур
char choice [2] = {'0', '\0'};  // массив для хранения переменной выбора пункта меню, чтобы использовать универсальную функцию ввода read_data
char compare[MAX_field];    // для хранения слова при поиске по имени
char confirmation[4] = {'0','0','0','\0'};  //// массив для хранения переменной подтверждения удаления, чтобы использовать универсальную функцию ввода read_data


// функция считывания строки из консоли, принимает указатель на массив и размер массива
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



//функция поиска пустой ячейки массива
//возвращает порядковый номер свободной ячейки
//и ноль если нет свободных ячеек
int search_empty_cell(void)
{
    int ret = 0;
        
    // перебираем элементы массива пока не попадется свободная ячейка, если нет свободных, то выводим ноль
    for(unsigned int i = 0 ; i < MAX; i++)
    {
        if('0' == mas[i].name[0])
        {
           if(('0' == mas[i].second_name[0])&&('0' == mas[i].tel[0]))
            {
                ret = i + 1; 
                break;
            }
            else
            {}
        }
        else
        {}
    }

   return ret;
}
// функция добавления нового абонента в справочник
void add_abonent()
{   
    printf("\nFunction  - Add abonent.\n");
    int number_cell;
    number_cell = search_empty_cell();
    if(number_cell > 0)
    {
      printf("\nAbonent No%d\n",number_cell); 
      printf("\nName: ");
      read_data(mas[number_cell-1].name, MAX_field);
      
      printf("\nSecond_name: ");
      read_data(mas[number_cell-1].second_name, MAX_field);
     
      printf("\nTel: ");
      read_data(mas[number_cell-1].tel, MAX_field);
      
    }
    else
    {
       printf("\nThe phone book is full.\n"); 
    }
    
}


// функция удаления абонента из справочника
void del_abonent()
{   
    unsigned int quantity_num_confirmation = 0;   // количество фактически введенных ASCII символов (до служебного символа \0)
    unsigned int index = 0;
    unsigned int num_record_for_del = 0;  //     номер удаляемой ячейки от 1 до MAX, если 0 то ничего не делать
    unsigned int flag_conf = 0; // флаг проверки введенных символов
    
    
    printf("\nFunction  - Del abonent.\n");
    printf("\nEnter the number of the record from 1 to %d you want to delete: ", MAX);
    read_data(confirmation, 4);  // считываем номер записи для удаления из записной книги
    
    while((confirmation[index] != '\0')&&(index < 4))   // вычислаяем количество введенных символов (использованных ячеек массива)
    {
        quantity_num_confirmation++;
        index++;
    }
    
    
    if((quantity_num_confirmation > 0))   // проверка на минимальное количество введенных символов
    {
        for(unsigned int index2 = 0; index2 < quantity_num_confirmation; index2++)    // проверка введенных сиволов на соответствие кодам цифр от 0 до 9 согласно таблице ASCII
        {
            if((confirmation[index2] > '/')&&(confirmation[index2] < ':'))
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
            switch(quantity_num_confirmation)
            {   
            
            case 1:
            num_record_for_del = confirmation[0] - 48;  // восстанавливаем DEC число из кода символа ASCII, 48 - код нуля
            //printf("\nYou entered a number: %d.\n", num_record_for_del);
            break;

            case 2:
            num_record_for_del = (confirmation[0] - 48)*10 + (confirmation[1] - 48);
            //printf("\nYou entered a number: %d.\n", num_record_for_del);
            break;

            case 3:
            num_record_for_del = (confirmation[0] - 48)*100 + (confirmation[1] - 48)*10 + (confirmation[0] - 48);
            //printf("\nYou entered a number: %d.\n", num_record_for_del);
            break;
            }
        }
        else
        {
           //printf("\nYou entered an incorrect number.\n"); 
        }
    }
    else
    {
        //printf("\nYou entered an incorrect number.\n");
    }
    
        
    if(((num_record_for_del)<=MAX)&&((num_record_for_del)>0))
    {
      printf("\nSubscriber's data under number %d in the phone book has been deleted.\n", num_record_for_del);
      for(unsigned int ind1 = 0; ind1<MAX_field; ind1++)
        {
         if(ind1 < MAX_field-1)
         {   
         mas[num_record_for_del-1].name[ind1] = '0';
         mas[num_record_for_del-1].second_name[ind1] = '0';
         mas[num_record_for_del-1].tel[ind1] = '0';
         }
         else
         {
         mas[num_record_for_del-1].name[ind1] = '\0';
         mas[num_record_for_del-1].second_name[ind1] = '\0';
         mas[num_record_for_del-1].tel[ind1] = '\0';  
         }
        }  
    }
    else
    {
      printf("\nYou entered an incorrect number.\n");  
    }
}


// поиск абонента в справочнике по имени
int search_abonent(void)
{
    printf("\nFunction  - Search abonent.\n");
    unsigned int counter_compare = 0;   // счетчик совпадений
    
    printf("\nname for search: ");
    read_data(compare, MAX_field);  // считываем слово для поиска в записной книге

    for(unsigned int index1 = 0; index1 < MAX; index1++)
    {
        unsigned int index2 = 0;
        unsigned int flag_ok = 1;
        
        while((index2 < MAX_field)&&(index2 >= 0)&&(0 != flag_ok))
        {
            
            if('\0' == compare[index2]) 
            {
                break;  // покинуть цикл, т.к. дальнейшее сравнение не имеет смысла, в ячейках массива после символа конца строки лежит мусор
            }
            else
            {
                if(mas[index1].name[index2] == compare[index2])
                {
                    index2++;
               
                }
                else
                {
                    flag_ok = 0;
                    index2 = 0;  
                }
            }
        }
        
        if(1 == flag_ok)
        {
            counter_compare++;  // записываем совпадение в счетчик
            printf("\nAbonent No%d\n", (index1+1));
            printf("name       : %s\n", mas[index1].name);
            printf("second name: %s\n", mas[index1].second_name);
            printf("tel        : %s\n", mas[index1].tel);
            
        }
        else{}


    }
    printf("\n A total of %d matches were detected.\n", counter_compare);
    
}


// вывод всей книги абонентов в консоль
void print_all_abonent()
{
    printf("\nFunction  - Print all abonent.\n");
    for(unsigned int index = 0; index < MAX; index++)
    {
        printf("\nAbonent No%d\n", (index+1));
        printf("name       : %s\n", mas[index].name);
        printf("second name: %s\n", mas[index].second_name);
        printf("tel        : %s\n", mas[index].tel);
    }
}




void main(void)
{   
   printf("\n\nStudent: Klyuzhin A.S.\n\n");
   printf("task3_4\n\n"); 
   // записываем код символа нуля в ячейки массивов внутри структур
   for(unsigned int ind = 0; ind < MAX ; ind ++)
   {
       for(unsigned int ind1 = 0; ind1<MAX_field; ind1++)
        {
         if(ind1 < MAX_field-1)
         {   
         mas[ind].name[ind1] = '0';
         mas[ind].second_name[ind1] = '0';
         mas[ind].tel[ind1] = '0';
         }
         else
         {
         mas[ind].name[ind1] = '\0';
         mas[ind].second_name[ind1] = '\0';
         mas[ind].tel[ind1] = '\0';  
         }
        }
         
   }

   while('5' != choice[0])
   {
    printf("\n");
    printf("1) Add a subscriber\n");
    printf("2) Delete a subscriber\n");
    printf("3) Search for a subscriber by name\n");
    printf("4) Output of all records\n");
    printf("5) Exit\n");
        
    read_data(&choice[0], 2);  // считываем число и символ конца строки в массив
    if((choice[0] < '1')||(choice[0] > '5'))
    {
        printf("\nYou have entered an incorrect number, enter a number from 1 to 5 and press Enter.\n");
        choice[0] = '0'; //сбрасываем неверно введенное значение к исходному
    }
    else
    {
      switch(choice[0])
      {
        case '1':
        add_abonent();
        break;

        case '2':
        del_abonent();
        break;
        
        case '3':
        search_abonent();
        break;

        case '4':
        print_all_abonent();
        break;
      }

    }     

   }

 printf("\n\nThe END.");
}



