
#include <stdio.h>
#include <stdlib.h>
// количество элементов в списке (не более 999 элементов, ограничение адресации массива confirmation при использовнии функции del_abonent)
// нет ограничений по записи одинаковых абонентов
#define MAX_field 10 // размер массива в структуре
struct abonent
{
    char name[MAX_field];
    char second_name[MAX_field];
    char tel[MAX_field];
    struct abonent *next;
    struct abonent *prev;
}; 

struct abonent* p_head_abonent = NULL; // создаем указатель на первый элемент типа "struct abonent" в двухсвязном списке, инициализируем его как NULL, т.к. еще не создано ни одного элемента в списке

struct abonent *create_abonent(char* name1, char* name2, char* phone)    //функция создания нового абонента в списке
{
    struct abonent* new_abonent = (struct abonent*)malloc(sizeof(struct abonent));  // динамическое выделение памяти без инициализации
              
    for (int ind = 0; ind < MAX_field; ind++)                   // переписываю строку из временного в массива в массив внутри структуры
    {
        new_abonent->name[ind] = *(name1 + ind);              // доступ к полю структуры через имя указателя на нее
    }

    for (int ind = 0; ind < MAX_field; ind++)
    {
        new_abonent->second_name[ind] = *(name2 + ind);              // доступ к полю структуры через имя указателя на нее
    }

    for (int ind = 0; ind < MAX_field; ind++)
    {
        new_abonent->tel[ind] = *(phone + ind);              // доступ к полю структуры через имя указателя на нее
    }


    new_abonent->next = NULL;               //предварительно инициализируем поле значением NULL
    new_abonent->prev = NULL;               //предварительно инициализируем поле значением NULL
    return new_abonent;                     // функция возвращает указатель на созданную структуру (новый элемент в двухсвязном списке)
}
//функция добавляет новый элемент в конец списка 
// принимает имя, фамилию, телефон и указатель на первый элемент списка
void save_new_abonent(struct abonent** head, char* name1, char* name2, char* phone)  
{
    struct abonent* new_abonent = create_abonent(name1,name2,phone); // функция create_abonent создает новый элемент списка и возвращает адрес на него

    if (*head == NULL)  // Проверяется указатель на первый элемент массива (если еще не было создано ни одного элемента, то заходим в условие)
    {
        *head = new_abonent;    // записываем адрес созданного элемента списка в качестве первого, т.к. до этого ни одного элемента не было создано
        return;                 // завершаем выполнение функции, т.к. нет ни следующего ни предыдущего элемента списка и поля next и prev в этом элементе останутся равными NULL
    }

    // если созданный элемент списка не является первым, то

    struct abonent* temp = *head;       // сохраняем значение из указателя head на первую структуру в списке во временный указатель temp 
    
    while (temp->next != NULL)          // пока поле next в текущем элементе списка (т.е. в структуре на которую указывает текущее значение temp) не равно NULL (т.е. это не последний элемент в списке)
    {                                   
        temp = temp->next;              // перезаписываем в указатель temp значение из поля next в текущем элементе списка (т.е. temp теперь будет указывать на следующий элемент списка)
    }
    // после перебора всех элементов списка: temp указывает на последний, а в поле next этого элемента содержится значение NULL (индикатор последнего элемента списка) 
    // можно добавить вновь созданный элемент в конец списка путем записи в поле next последнего уже находящегося в списке элемента адреса нашего созданного нового элемента списка
    
    temp->next = new_abonent;               // изменяем поле next последнего элемента в списке на адрес нового элемента
    new_abonent->prev = temp;               // записываем в поле prev нового элемента адрес последнего элемента который был в списке
}

// функция удаления первого элемента списка, принимает адрес указателя на первый элемент списка
void del_first_element(struct abonent** head)
{
    struct abonent* temp_del_first = *head; // создаем временный указатель на первый элемент списка 
    *head = (*head)->next;                  // перезаписываем значение адреса первого элемента на адрес следующего элемента в списке
    if (*head != NULL)                      // если новый адрес первого элемента в списке не равен нулю, т.е. существует такой элемент в списке
    {
        (*head)->prev = NULL;               // записать в поле prev нового "первого" элемента списка значение NULL
    }
    free(temp_del_first); // освободить память в объеме структуры abonent начиная с адреса на который указывает temp
}
// функция удаления элемента на определенной позиции
void del_number_element(struct abonent** head, int position)
{
    struct abonent* temp = *head;      // создаем временный дополнительный указатель на первый элемент в списке

    if (position == 1)                  // если необходимо удалить первый элемент в списке
    {
        del_first_element(head);        // вызываем функцию удаления первого элемента списка
        return;
    }
    for (int i = 1; temp != NULL && i <position; i++)       // переходим к адресу соответствующему позиции "элемента для удаления" в списке
    {
        temp = temp->next;
    }
    if (temp->next != NULL)         // Если следующий элемент в списке существует, то
    {
        (temp->next)->prev = temp->prev;    // в поле prev следующего элемента записываем значение prev из текущего (удаляемого)
    }
    if (temp ->prev != NULL)        // Если предыдущий элемент в списке существует, то
    {
        (temp->prev)->next = temp->next;    // в поле next предыдущего элемента записываем значение next из текущего (удаляемого)
    }

    free(temp);     // освобождаем память в объеме "struct abonent" по адресу temp
}

char choice [2] = {'0', '\0'};  // массив для хранения переменной выбора пункта меню, чтобы использовать универсальную функцию ввода read_data
char compare[MAX_field];    // для хранения слова при поиске по имени
char confirmation[4] = {'0','0','0','\0'};  //// массив для хранения переменной подтверждения удаления, чтобы использовать универсальную функцию ввода read_data
char tmp_string1[MAX_field];    // массив для временного хранения имени
char tmp_string2[MAX_field];    // массив для временного хранения фамилии
char tmp_string3[MAX_field];    // массив для временного хранения телефона

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


// функция добавления нового абонента в справочник
void add_abonent()
{   
    printf("\nFunction  - Add abonent.\n");
    
      printf("\nName: ");
      read_data(tmp_string1, MAX_field);
      
      printf("\nSecond_name: ");
      read_data(tmp_string2, MAX_field);
     
      printf("\nTel: ");
      read_data(tmp_string3, MAX_field);
      
      save_new_abonent(&p_head_abonent, tmp_string1, tmp_string2, tmp_string3);   // вызываю функцию сохранения нового абонента в список
      // передаю адрес указателя на первый элемент списка &p_head_abonent
      // передаю указатели на буферные массивы для хранения текущих введенных данных имя, фамилия, телефон
}



//функция удаления абонента из справочника
void del_abonent(struct abonent** head)
{   
    unsigned int flag_out_del = 0;
    unsigned int maximum = 0;
    struct abonent* p_temp_del_counter = *head; // создаем временный указатель на первый элемент списка

    if (NULL == p_head_abonent)         // если список пустой, то покинуть функцию
    {
        printf("\nThe list is empty.\n");
        return;
    }
    else {}

    unsigned int quantity_num_confirmation = 0;   // количество фактически введенных ASCII символов (до служебного символа \0)
    unsigned int index = 0;
    unsigned int num_record_for_del = 0;  //     номер удаляемой ячейки от 1 до MAX, если 0 то ничего не делать
    unsigned int flag_conf = 0; // флаг проверки введенных символов
   
    printf("\nFunction  - Del abonent.\n");
        
    while (flag_out_del == 0)       //считаем количество элементов в списке
    {
        if (NULL == p_temp_del_counter->next)
        {
            flag_out_del = 1;
        }
        else {}
        p_temp_del_counter = p_temp_del_counter->next;      // переходим к следующему элементу списка
        maximum++;                                // увеличиваем счетчик абонентов
    }
    
    
    printf("\nEnter the number of the record from 1 to %d you want to delete: ", maximum);
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
    
        
    if(((num_record_for_del)<=maximum)&&((num_record_for_del)>0))
    {
      printf("\nSubscriber's data under number %d in the phone book has been deleted.\n", num_record_for_del);
      del_number_element(&p_head_abonent,num_record_for_del);   // передаю адрес указателя на первый элемент списка и порядковый номер элемента который необходимо удалить из списка (справочника)
    }
    else
    {
      printf("\nYou entered an incorrect number.\n");  
    }
}


//поиск абонента в справочнике по имени
int search_abonent(void)
{
    printf("\nFunction  - Search abonent.\n");
    unsigned int counter_compare = 0;   // счетчик совпадений
    
    struct abonent* p_temp_search = p_head_abonent; // создаем локальный указатель типа "struct abonent" и присваиваем ему значение адреса первого элемента в списке (head - это указатель на первый элемент в списке)
    unsigned int flag_last_element = 0;             // флаг для выхода из первого цикла while (последний элемент списка)

    if (NULL == p_temp_search)
    {
        printf("\nThe list is empty.\n");
    }
    else
    {   
        printf("\nname for search: ");
        read_data(compare, MAX_field);  // считываем слово для поиска в записной книге

        while (0 == flag_last_element)      // перебираем все элементы списка пока не доберемся до последнего, тогда выставляем флаг flag_ok = 1, чтобы больше не входить в цикл
        { 
            unsigned int index1 = 0;    // порядковый номер абонента для вывода printf 
            unsigned int index2 = 0;    // индекс элемента в массиве
            unsigned int flag_ok = 1;   // флаг обнаружения отличий в сравниваемых строках, сбрасывается в 0 при обнаружении отличия

            

            while ((index2 < MAX_field) && (index2 >= 0) && (0 != flag_ok))
            {

                if ('\0' == compare[index2])
                {
                    break;  // покинуть цикл, т.к. дальнейшее сравнение не имеет смысла, в ячейках массива после символа конца строки лежит мусор
                }
                else
                {
                    if (p_temp_search->name[index2] == compare[index2])
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

            if (1 == flag_ok)
            {
                counter_compare++;  // записываем совпадение в счетчик
                printf("\nAbonent No%d\n", (index1 + 1));
                printf("name       : %s\n", p_temp_search->name);
                printf("second name: %s\n", p_temp_search->second_name);
                printf("tel        : %s\n", p_temp_search->tel);

            }
            else {}
            
            if (NULL == p_temp_search->next)    // если текущий элемент списка последний, то выставляем флаг flag_ok = 1, чтобы по завершению выполнения этого прохода выйти из цикла 
            {
                flag_last_element = 1;          // это последний элемент в списке, выставляем флаг
            }
            else
            {
                p_temp_search = p_temp_search->next;    // переходим к следующему элементу в списке
                index1++;
            }

            
            
        }
        printf("\n A total of %d matches were detected.\n", counter_compare);
    }
}


// вывод всей книги абонентов в консоль
void print_all_abonent()
{
    printf("\nFunction  - Print all abonent.\n");
    
    int index = 0;
    unsigned int flag_out = 0;  // флаг выхода из цикла

    struct abonent* p_temp_print = p_head_abonent; // создаем локальный указатель типа "struct abonent" и присваиваем ему значение адреса первого элемента в списке (head - это указатель на первый элемент в списке)
    if (NULL == p_temp_print)
    {
        printf("\nThe list is empty.\n");
    }
    else
    {


        while (flag_out == 0)       //пока не дойдем до последнего элемента в списке
        {
            if (NULL == p_temp_print->next)
            {
                flag_out = 1;
            }
            else {}

            printf("\nAbonent No%d\n", (index + 1));
            printf("name       : %s\n", p_temp_print->name);
            printf("second name: %s\n", p_temp_print->second_name);
            printf("tel        : %s\n", p_temp_print->tel);

            p_temp_print = p_temp_print->next;      // переходим к следующему элементу списка
            index++;                                // увеличиваем номер абонента
        }
           
            printf("\nEnd of the list.\n");
            
    }
}

void clear_all(struct abonent** head)
{
    struct abonent* p_temp_del_all = *head; // создаем временный указатель на первый элемент списка
    unsigned int flag_out_del_all = 0;
    unsigned int maximum = 0;
    if (NULL != p_temp_del_all)
    {
        while (flag_out_del_all == 0)       //считаем количество элементов в списке
        {
            if (NULL == p_temp_del_all->next)
            {
                flag_out_del_all = 1;
            }
            else
            {
                p_temp_del_all = p_temp_del_all->next;      // переходим к следующему элементу списка
            }
            maximum++;                                // увеличиваем счетчик абонентов
        }
        
        p_temp_del_all = *head; // восстанавливаю указатель на первый элемент списка
        
        if (maximum > 0)
        {
            for (unsigned int number = 1; number <= maximum; number++)
            {
                del_first_element(&p_temp_del_all);     // очищаем память
            }
        }
        else {}

        printf("\n %d list items have been deleted.\n", maximum);
    }
    else 
    {
        printf("\nThe list is empty. Nothing has been deleted.\n");
    }
}


void main(void)
{  
   printf("\n\nStudent: Klyuzhin A.S.\n\n");
   printf("task6\n\n"); 
   
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
        del_abonent(&p_head_abonent);   // передаю адрес указателя на первый элемент
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

   clear_all(&p_head_abonent); // очистка памяти, удаляются все элементы в списке

 printf("\n\nThe END.");
}



