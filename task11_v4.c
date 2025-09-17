
#include <stdio.h>  //стандартная библиотека для работы функции printf
#include <unistd.h> //библиотека для работы с системными фызовами
#include <pthread.h>
#include <sys/types.h>

// объявляем переменные для хранения ID потоков
pthread_t thread1;   // поток 1 покупателя
pthread_t thread2;   // поток 2 покупателя
pthread_t thread3;   // поток 3 покупателя
pthread_t thread4;   // поток продавца/доставщика

// максимальный объем товара, который вмещается в магазин
#define MAX1 370
#define MAX2 325
#define MAX3 500
#define MAX4 470
#define MAX5 580

// объем товара необходимый покупателю с соответствующим номером
#define BUY1 5450
#define BUY2 3100
#define BUY3 4200

pthread_mutex_t m1; // мьютекс для разграничения доступа покупателей и доставщика к магазину
pthread_mutex_t m2; // 
pthread_mutex_t m3;
pthread_mutex_t m4;
pthread_mutex_t m5;

int flag = 0;    //флаг выставляется в 1 когда все покупатели купили необходимое количество товара

// структура для передачи параметров в функцию при создании потока
struct shop_struct 
{
    int* mass[5];       // массив для хранения объемов товара в магазинах
    int max;            // объем товара необходимый покупателю
    int index_pthread;  // номер покупателя
        
};



static void *pthread_seller(void * argument)
{
     struct shop_struct* structure_arg_seller = argument;
    // int temp = 0;

     while(flag != 1)
     {
        for(int ind = 0 ; ind < 5; ind++)
        {
           
            switch(ind)
            {
                case 0: 
                pthread_mutex_lock(&m1);
                *(structure_arg_seller->mass[ind]) = MAX1;
                pthread_mutex_unlock(&m1);
                break;
                
                case 1:
                pthread_mutex_lock(&m2);
                *(structure_arg_seller->mass[ind]) = MAX2;
                pthread_mutex_unlock(&m2);
                break;
                
                case 2:
                pthread_mutex_lock(&m3);
                *(structure_arg_seller->mass[ind]) = MAX3;
                pthread_mutex_unlock(&m3);
                break;
                
                case 3:
                pthread_mutex_lock(&m4);
                *(structure_arg_seller->mass[ind]) = MAX4;
                pthread_mutex_unlock(&m4);
                break;
                
                case 4:
                pthread_mutex_lock(&m5);
                *(structure_arg_seller->mass[ind]) = MAX5;
                pthread_mutex_unlock(&m5);
                break;
            }  
            printf("Продавец полностью наполнил магазин %d до %d единиц товара.\n", (ind+1), *(structure_arg_seller->mass[ind]));
            sleep(2.2);
        }
     }
     


}
// принимает количество необходимого товара и адрес массива в котором находится товар

static void* pthread_function(void* arg)
{   
   
    
    struct shop_struct* structure_arg = arg;
    
    int tmp = 0;
    
    while((structure_arg->max) > 0)      // пока max > 0, заходить поочередно в магазины и покупать товар
    {
        for(int index = 0; index < 5; index++)
        {   
            switch(index)
            {
                case 0:
                    if(pthread_mutex_trylock(&m1) != 0)
                    {
                        index++;
                        if(pthread_mutex_trylock(&m2) != 0)
                        {
                            index++;
                            if(pthread_mutex_trylock(&m3) != 0)
                            {
                                index++;
                                if(pthread_mutex_trylock(&m4) != 0)
                                {
                                index++;
                                pthread_mutex_lock(&m5);  // если все предыдущие мьютексы заняты, то блокируем поток здесь и ждем освобождение 5 мьютекса
                                }
                                else
                                {}
                            }
                            else
                            {}

                        }
                        else
                        {}

                    }
                    else
                    {}
                break;
                case 1:
                 if(pthread_mutex_trylock(&m2) != 0)
                    {
                        index++;
                        if(pthread_mutex_trylock(&m3) != 0)
                        {
                            index++;
                            if(pthread_mutex_trylock(&m4) != 0)
                            {
                                index++;
                                pthread_mutex_lock(&m5); // если все предыдущие мьютексы заняты, то блокируем поток здесь и ждем освобождение 5 мьютекса
                                                          
                            }
                            else
                            {}

                        }
                        else
                        {}

                    }
                    else
                    {}
                break;

                case 2:
                 if(pthread_mutex_trylock(&m3) != 0)
                    {
                        index++;
                        if(pthread_mutex_trylock(&m4) != 0)
                        {
                            index++;
                            pthread_mutex_lock(&m5);
                           
                        }
                        else
                        {}

                    }
                    else
                    {}
                break;
                case 3:
                    if(pthread_mutex_trylock(&m4) != 0)
                    {
                        index++;
                        pthread_mutex_lock(&m5);
                    }
                    else
                    {}
                break;
                case 4:
                pthread_mutex_lock(&m5);
                break;
            }

            
            
            /*
            //блокируем мьютекс соответствующий номеру магазина
            switch(index)
            {
                case 0:
                pthread_mutex_lock(&m1);     // блокировка мьютекса если он еще не заблокирован, иначе переход к следующему (магазин 1)
                //printf("pthread_mutex m1 = %d)\n", m1);
                break;
                case 1:
                pthread_mutex_lock(&m2);     // блокировка мьютекса (магазин 2)
                break;
                case 2:
                pthread_mutex_lock(&m3);     // блокировка мьютекса (магазин 3)
                break;
                case 3:
                pthread_mutex_lock(&m4);     // блокировка мьютекса (магазин 4)
                break;
                case 4:
                pthread_mutex_lock(&m5);     // блокировка мьютекса (магазин 5)
                break;
            }
            */
            tmp = *(structure_arg->mass[index]);    // взять значение из ячейки массива 
            if((tmp > 0)&&((structure_arg->max) > 0))
            {
                if(tmp < (structure_arg->max))
                {
                  structure_arg->max = structure_arg->max - tmp;              // уменьшаем объем необходимого товара на количество приобретенного в этом магазине
                  *(structure_arg->mass[index]) = 0;          // обнуляем количество товара в этом магазине, т.к. весь выкупили
                  printf("Покупатель %d купил %d единиц товара в %d магазине. Осталось приобрести %d единиц товара. В магазине осталось %d единиц товара. \n", (structure_arg->index_pthread), tmp, (index+1), structure_arg->max, *(structure_arg->mass[index]));
                 
                }
                else
                {   
                    *(structure_arg->mass[index]) = (*(structure_arg->mass[index]) - (structure_arg->max));    // уменьшаем количество товара в магазине
                    printf("Полкупатель %d купил %d единиц товара в %d магазине. Покупатель купил все необходимые товары. В магазине осталось %d единиц товара.\n",(structure_arg->index_pthread),(structure_arg->max), (index+1), *(structure_arg->mass[index]));
                    structure_arg->max = 0;                                    // обнуляем количество необходимого товара
                }
              
            }
            else
            {
                printf("Покупатель %d пришел в магазин %d, но в магазине нет необходимого товара, поэтому он ушел в другой магазин.\n", (structure_arg->index_pthread), (index+1));
            }
            
            // разблокируем соответствующий мьютекс
            switch(index)
            {
                case 0:
                pthread_mutex_unlock(&m1);     // разблокировка мьютекса
                break;
                case 1:
                pthread_mutex_unlock(&m2);     // 
                break;
                case 2:
                pthread_mutex_unlock(&m3);     //  
                break;
                case 3:
                pthread_mutex_unlock(&m4);     //  
                break;
                case 4:
                pthread_mutex_unlock(&m5);     //  
                break;
            }
           
            // если куплено необходимое количество товара, то выйти из цикла for
            if(0 == structure_arg->max)
            {
                break;
            }
            else
            {
                sleep(2);
            }
        }
    }


}

void main()
{
   pthread_mutex_init(&m1,NULL); // инициализация мьютекса
   
   

   int massive[5] = {MAX1, MAX2,MAX3,MAX4,MAX5};   // заполняем магазины товаром
   
    // определяю переменные типа структуры shop_struct
   struct shop_struct buy1;
   buy1.mass[0] = &massive[0];
   buy1.mass[1] = &massive[1];
   buy1.mass[2] = &massive[2];
   buy1.mass[3] = &massive[3];
   buy1.mass[4] = &massive[4];
   buy1.max = BUY1;
   buy1.index_pthread = 1;
   
   struct shop_struct buy2;
   buy2.mass[0] = &massive[0];
   buy2.mass[1] = &massive[1];
   buy2.mass[2] = &massive[2];
   buy2.mass[3] = &massive[3];
   buy2.mass[4] = &massive[4];
   buy2.max = BUY2;
   buy2.index_pthread = 2;
      
   struct shop_struct buy3;
   buy3.mass[0] = &massive[0];
   buy3.mass[1] = &massive[1];
   buy3.mass[2] = &massive[2];
   buy3.mass[3] = &massive[3];
   buy3.mass[4] = &massive[4];
   buy3.max = BUY3;
   buy3.index_pthread = 3;

   struct shop_struct seller;
   seller.mass[0] = &massive[0];
   seller.mass[1] = &massive[1];
   seller.mass[2] = &massive[2];
   seller.mass[3] = &massive[3];
   seller.mass[4] = &massive[4];
   seller.max = 0;               //  обнуляю, не планирую использовать
   seller.index_pthread = 4;

  //отладочная информация
  /*
   printf("buy1.max = %d\n", buy1.max);
   printf("buy1.mass = %d\n", *buy1.mass[0]);
   printf("buy1.mass = %d\n", *buy1.mass[1]);
   printf("buy1.mass = %d\n", *buy1.mass[2]);
   printf("buy1.mass = %d\n", *buy1.mass[3]);
   printf("buy1.mass = %d\n", *buy1.mass[4]);
   printf("buy1.max = %d\n", buy1.index_pthread);

   printf("buy1.max = %d\n", buy2.max);
   printf("buy1.mass = %d\n", *buy2.mass[0]);
   printf("buy1.mass = %d\n", *buy2.mass[1]);
   printf("buy1.mass = %d\n", *buy2.mass[2]);
   printf("buy1.mass = %d\n", *buy2.mass[3]);
   printf("buy1.mass = %d\n", *buy2.mass[4]);
   printf("buy1.max = %d\n", buy2.index_pthread);
   
   printf("buy1.max = %d\n", buy3.max);
   printf("buy1.mass = %d\n", *buy3.mass[0]);
   printf("buy1.mass = %d\n", *buy3.mass[1]);
   printf("buy1.mass = %d\n", *buy3.mass[2]);
   printf("buy1.mass = %d\n", *buy3.mass[3]);
   printf("buy1.mass = %d\n", *buy3.mass[4]);
   printf("buy1.max = %d\n", buy3.index_pthread);

   printf("seller.max = %d\n", seller.max);
   printf("seller.mass = %d\n", *seller.mass[0]);
   printf("seller.mass = %d\n", *seller.mass[1]);
   printf("seller.mass = %d\n", *seller.mass[2]);
   printf("seller.mass = %d\n", *seller.mass[3]);
   printf("seller.mass = %d\n", *seller.mass[4]);
   printf("seller.max = %d\n", seller.index_pthread);
*/
    pthread_create(&thread1, NULL, &pthread_function, &buy1);
    pthread_create(&thread2, NULL, &pthread_function, &buy2);
    pthread_create(&thread3, NULL, &pthread_function, &buy3);
    pthread_create(&thread4, NULL, &pthread_seller, &seller);

    
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    pthread_join(thread3, NULL);
    
    flag = 1;   // т.к. потоки всех покупателей завершились, выставляем флаг 1
    pthread_join(thread4, NULL);    // ожидание завершения процесса "продавца"
    
    pthread_mutex_destroy(&m1); // удаление мьютекса
    pthread_mutex_destroy(&m2); // удаление мьютекса
    pthread_mutex_destroy(&m3); // удаление мьютекса
    pthread_mutex_destroy(&m4); // удаление мьютекса
    pthread_mutex_destroy(&m5); // удаление мьютекса
    
    printf("Конец!\n");
}