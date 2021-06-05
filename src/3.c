#include <stdio.h>
#include <stdlib.h>
#include <time.h>

//Массив, в котором хранится поле
int **field;

//Размер квадратного поля
int field_size;

//Процент заполненности поля
int fill_amount;

//В каком диапазоне клетка рождается, по умолчанию только 3
int birth_lower, birth_higher;

//В каком диапазоне клетка не умирает, по умолчанию от 2 до 3
int life_lower, life_higher;

//1 - случайная генерация, 2 - чтение из файла, 3 - чтение из картинки, остальные значения запрещены
int import_type;

//Количество шагов симуляции
int steps;

//Текущий шаг симуляции
int current_step = 0;

//Опции экспорта. 1 - опция активна, 0 - опция неактивна
int do_console_export, do_file_export, do_image_export;

//Настройка краев поля. 1 - края поля считаются мертвыми клетками, 0 - поле зациклено
int is_seamless;

//Флаг, контролирующий, не прекратилась ли игра
int game_over = 0;


//Выбор опций импорта. 3 варианта: из картинки, из текстового файла, случайная генерация. В случае случайной генерации предлагается выбрать размер поля
void set_import_options();
//Создание массива, вызов одной из функций импорта
void create_field();
//Импорт из текстового файла
void import_from_file();
//Импорт из картинки
void import_from_image();
//Случайная генерация поля
void generate_random_field();

//Выбор условий рождения и жизни клетки
void set_life_conditions();
//Выбор, циклится ли поле
void set_seamless();
//Установка количества шагов симуляции
void set_steps();

//Выбор опций экспорта. 3 варианта: на экран, в текстовый файл, в картинку. Можно выбрать более 1 варианта
void set_export_options();
//Вывод в консоль
void show_in_console();
//Экспорт в файл
void export_to_file();
//Экспорт в картинки
void export_to_image();

//Основной цикл игры. Занимается подсчетом шагов симуляции и активацией экcпорта. Прерывается, если после шага симуляции флаг game_over стал истиной
void game_loop();
//Очищает файлы, которые могли остаться от предыдущих запусков программы
void initialize_export();
//Функция, экспортирующая поле в соответствии с опциями экспорта
void export();
//Просчет одного шага симуляции. Создает новое поле, для каждой клетки считает соседей и записывает результат жизни в это поле. В конце подменяет старое поле на новое
void make_step();
//В случае если старое поле и новое одинаковые, либо поле пустое, устанавливает флаг game_over в значение 1
void check_game_over(int **new_field);
//Возврат количества соседей клетки с данными координатами. Учитывает опцию has_seams
int count_neighboors(int i, int j);



//Очищает экран. Работает и для Windows, и для Linux. Предположительно.
void clear_screen();



int main()
{   
    set_import_options();
    
    set_export_options();
    
    set_life_conditions();
    
    set_seamless();
    
    set_steps();
    
    create_field();
    
    game_loop();

    return 0;
}

void set_import_options()
{
    printf("Choose how to start game. There are 3 options:\n");
    printf("1 - randomly generated field\n");
    printf("2 - import from text file\n");
    printf("3 - import from .bmp image\n");
    printf("Choice:");
    scanf("%d", &import_type);
    
    /*
    В случае генерации поля из файла или картинки размер поля находится автоматически,
    но при случайной генерации поля размер необходимо знать заранее. Также можно указать,
    насколько сильно будет заполнено поле.
    */
    if(import_type == 1)
    {
        printf("\nChoose size for a square field. Numbers from 5 to 25 are recommended.\nField size:");
        scanf("%d", &field_size);
        printf("Choose percentage of field to be filled with living cells at start.\nFill amount:");
        scanf("%d", &fill_amount);
    }
}

void set_export_options()
{
    printf("\nChoose other options. To activate option, type 1, and to deactivate it, type 0.\n");
    printf("Note: if the game field is too big, it may show up incorrectly. Do you want to show game in console? ");
    scanf("%d", &do_console_export);
    printf("Do you want to export game steps to a text file? ");
    scanf("%d", &do_file_export);
    printf("Do you want to export game steps to an image sequence? ");
    scanf("%d", &do_image_export);
}

void set_life_conditions()
{
    printf("\nDo you want to use custom birth and death options for cells? ");
    int choice;
    scanf("%d", &choice);
    
    //По желанию пользователь может оставить стандартные настройки
    if (choice == 0)
    {
        birth_lower = 3;
        birth_higher = 3;
        life_lower = 2;
        life_higher = 3;
        return;
    }
    
    //Либо же установить свои
    printf("Choose lower bound of neighboors with which a cell will birth:");
    scanf("%d", &birth_lower);
    printf("Note: if higher bound is lower than lower bound, cells can never birth. Choose higher bound of neighboors with which a cell will birth:");
    scanf("%d", &birth_higher);
    printf("\nChoose lower bound of neighboors with which a cell will continue to live:");
    scanf("%d", &life_lower);
    printf("Note: if higher bound is lower than lower bound, cells will die after one step. Choose higher bound of neighboors with which a cell will continue to live:");
    scanf("%d", &life_higher);
}

void set_seamless()
{
    printf("\nDo you want field to be seamless? ");
    scanf("%d", &is_seamless);
}

void set_steps()
{
    printf("\nChoose how many steps should a simulation have. Number of steps:");
    scanf("%d", &steps);
}

void create_field()
{
    switch(import_type)
    {
        case 1:
            generate_random_field();
            break;
        case 2:
            import_from_file();
            break;
        case 3:
            import_from_image();
            break;
    }
}

void game_loop()
{
    //Так как в цикле текстовый файл открывается в режиме дозаписи, перед циклом его необходимо удалить
    initialize_export();
    export();
    for(current_step = 1; current_step < steps; current_step++)
    {
        make_step();
        export();
        if(game_over) 
        {
            printf("The game is over because field didn't change from previous step.\n");
            break;
        }
    }
}

void initialize_export()
{
    if(do_file_export)
    {
        int success = remove("output.txt");
        if(!success) printf("\nUnable to clear output file. Output to file may be corrupted.\n\n");
    }
}

void export()
{    
    if(do_console_export) show_in_console();
    if(do_file_export) export_to_file();
    if(do_image_export) export_to_image(); 
}

void generate_random_field()
{
    srand(time(NULL));
    
    //Создание поля
    field = (int **)malloc(field_size * sizeof(int *));
    for(int i = 0; i < field_size; i++)
    {
        field[i] = (int *)malloc(field_size * sizeof(int));
    }
    
    //Заполнение поля
    int random_number;
    for(int i = 0; i < field_size; i++)
    {
        for(int j = 0; j < field_size; j++)
        {
            random_number = rand() % 100 + 1;
            if (random_number < fill_amount) field[i][j] = 1;
            else field[i][j] = 0;
        }
    }
}

void show_in_console()
{
    clear_screen();
    
    printf("CURRENT STEP: %d\n", current_step);
    for(int i = 0; i < field_size; i++)
    {
        for(int j = 0; j < field_size; j++)
        {
            printf("%c", field[i][j]==1 ? 'X' : ' ');
        }
        printf("\n");
    }
}

void export_to_file()
{
    FILE *file = fopen("output.txt", "a");
    if(file == NULL)
    {
        printf("Unable to open file. Export aborted.\n");
        return;
    }
    
    fprintf(file, "Step %d:\n", current_step);
    for(int i = 0; i < field_size; i++)
    {
        for(int j = 0; j < field_size; j++)
        {
            fprintf(file, "%c", field[i][j]==1 ? 'X' : ' ');
        }
        fprintf(file, "\n");
    }
    
    for(int i = 0; i < field_size; i++) fprintf(file, "=");
    fprintf(file, "\n");
    
    fclose(file);
}

void import_from_file()
{
    //Открытие файла
    FILE *file = fopen("input.txt", "r");
    if(file == NULL)
    {
        printf("Unable to open the file for import. Program will close now.");
        exit(EXIT_FAILURE);
    }
    
    fscanf(file, "%d", &field_size);
    //Пропуск переноса строки после размера поля
    getc(file);
    
    //Создание поля
    field = (int **)malloc(field_size * sizeof(int *));
    for(int i = 0; i < field_size; i++)
    {
        field[i] = (int *)malloc(field_size * sizeof(int));
    }
    
    //Заполнение поля
    char character;
    for(int i = 0; i < field_size; i++)
    {
        for(int j = 0; j < field_size; j++)
        {
            field[i][j] = (getc(file)==' ') ? 0 : 1;
        }
        //Пропуск переноса строки
        getc(file);
    }
    
    fclose(file);
}

void make_step()
{
    //Создание временного поля для нового шага
    int **new_field = (int **)malloc(field_size * sizeof(int *));
    for(int i = 0; i < field_size; i++)
    {
        new_field[i] = (int *)malloc(field_size * sizeof(int));
    }
    
    //Расчет нового шага для каждой клетки
    int neighboors;
    for(int i = 0; i < field_size; i++)
    {
        for(int j = 0; j < field_size; j++)
        {
            neighboors = count_neighboors(i, j);
            //Мертвая клетка может ожить или остаться мертвой
            if(field[i][j] == 0)
                if(neighboors >= birth_lower && neighboors <= birth_higher) new_field[i][j] = 1;
                else new_field[i][j] = 0;
                
            //Живая клетка может продолжить жить или умереть
            if(field[i][j] == 1)
                if(neighboors >= life_lower && neighboors <= life_higher) new_field[i][j] = 1;
                else new_field[i][j] = 0;
        }
    }
    
    check_game_over(new_field);
    
    //Временное поле становится постоянным
    for(int i = 0; i < field_size; i++)
    {
        for(int j = 0; j < field_size; j++)
        {
            field[i][j] = new_field[i][j];
        }
    }
    
    free(new_field);
}

int count_neighboors(int i, int j)
{
    int left, right, up, down, border;
    int neighboors = 0;
    
    left = j - 1;
    right = j + 1;
    up = i - 1;
    down = i + 1;
    
    //Проверка всех клеток, начиная с левой верхней по часовой стрелке
    if(is_seamless)
    {
        //Если у поля нет краев, крайние клетки с разных сторон соседствуют
        neighboors += field[(up+field_size)%field_size][(left+field_size)%field_size];
        neighboors += field[(up+field_size)%field_size][j];
        neighboors += field[(up+field_size)%field_size][(right+field_size)%field_size];
        neighboors += field[i][(right+field_size)%field_size];
        neighboors += field[(down+field_size)%field_size][(right+field_size)%field_size];
        neighboors += field[(down+field_size)%field_size][j];
        neighboors += field[(down+field_size)%field_size][(left+field_size)%field_size];
        neighboors += field[i][(left+field_size)%field_size];
    }
    else
    {
        //Если у поля есть края, клетки за краями считаются мертвыми
        neighboors += (up >= 0 && left >= 0)                    ? field[up][left]    : 0;
        neighboors += (up >= 0)                                 ? field[up][j]       : 0;
        neighboors += (up >= 0 && right < field_size)           ? field[up][right]   : 0;
        neighboors += (right < field_size)                      ? field[i][right]    : 0;
        neighboors += (down < field_size && right < field_size) ? field[down][right] : 0;
        neighboors += (down < field_size)                       ? field[down][j]     : 0;
        neighboors += (down < field_size && left >= 0)          ? field[down][left]  : 0;
        neighboors += (left >= 0)                               ? field[i][left]     : 0;
    }  
    
    return neighboors;
}

void check_game_over(int **new_field)
{
    //Игра кончается досрочно, если на новом шаге ни одна клетка не изменилась
    for(int i = 0; i < field_size; i++)
        for(int j = 0; j < field_size; j++)
        {
            if(new_field[i][j] != field[i][j]) return;
        }
        
    game_over = 1;
}

void export_to_image()
{
    FILE *file;
    int file_size = 54 + 3 * field_size * field_size;

    //Объявление заголовков
    unsigned char bmp_file_header[14] = {'B','M', 0,0,0,0, 0,0, 0,0, 54,0,0,0};
    unsigned char bmp_info_header[40] = {40,0,0,0, 0,0,0,0, 0,0,0,0, 1,0, 24,0};

    //Запись размера файла в заголовок
    bmp_file_header[ 2] = (unsigned char)(file_size    );
    bmp_file_header[ 3] = (unsigned char)(file_size>> 8);
    bmp_file_header[ 4] = (unsigned char)(file_size>>16);
    bmp_file_header[ 5] = (unsigned char)(file_size>>24);

    //Запись ширины и высоты картики в заголовок
    bmp_info_header[ 4] = (unsigned char)(field_size    );
    bmp_info_header[ 5] = (unsigned char)(field_size>> 8);
    bmp_info_header[ 6] = (unsigned char)(field_size>>16);
    bmp_info_header[ 7] = (unsigned char)(field_size>>24);
    bmp_info_header[ 8] = (unsigned char)(field_size    );
    bmp_info_header[ 9] = (unsigned char)(field_size>> 8);
    bmp_info_header[10] = (unsigned char)(field_size>>16);
    bmp_info_header[11] = (unsigned char)(field_size>>24);

    //Каждая картинка выводится в новый файл
    char file_name[15];
    sprintf(file_name, "output%d.bmp", current_step);
    
    file = fopen(file_name,"wb");
    if(file == NULL)
    {
        printf("Unable to create image. Export to image aborted.\n");
        return;
    }
    
    //Запись в файл заголовков
    fwrite(bmp_file_header,1,14,file);
    fwrite(bmp_info_header,1,40,file);
    
    //Массивы, которыми заполняется строка. Каждый массив - один пиксель
    unsigned char one[3] = {255,255,255};
    unsigned char zero[3] = {0,0,0};
    
    //В формате BMP каждая строка должна занимать 4n байт. Это переменная, которая хранит сколько байт остается после записи строки
    int remainder = (4 - (field_size * 3) % 4) % 4;
    
    for(int i = field_size - 1; i >= 0; i--)
    {
        //Запись строки попиксельно
        for(int j = 0; j < field_size; j++)
        {
            //В случае, если ячейка равна единице, записываем белый пиксель, иначе - черный
            fwrite(field[i][j]==1 ? one : zero, 3, 1, file);
        }
        //Запись остатка после строки, чтобы строка имела длину 4n
        fwrite(zero, 1, remainder, file);
    }

    fclose(file);
}

void import_from_image()
{
    FILE *file;
    unsigned char header[54];

    //Открытие файла
    file = fopen("input.bmp", "r");
    if(file == NULL)
    {
        printf("Unable to open the file for import. Program will close now.");
        exit(EXIT_FAILURE);
    }
    
    //Считывание заголовка файла
    fread(header, sizeof(unsigned char), 54, file);
    
    //Получение из заголовка файла размера изображения. Так как изображение квадратное, достаточно достать только ширину
    field_size = 0;
    //В одну строчку почему-то не работает, поэтому пришлось разбить на четыре
    field_size += (int)header[21]<<24;
    field_size += (int)header[20]<<16;
    field_size += (int)header[19]<<8;
    field_size += (int)header[18];
    
    //Создание поля
    field = (int **)malloc(field_size * sizeof(int *));
    for(int i = 0; i < field_size; i++)
    {
        field[i] = (int *)malloc(field_size * sizeof(int));
    }
    
    //В формате BMP каждая строка занимает 4n байт. Это переменная, которая хранит сколько байт остается после считывания строки
    int buffer_remainder = (4 - (field_size * 3) % 4) % 4;
    //Буфер, в который считывается строка
    unsigned char buffer[field_size*3 + buffer_remainder];
    for(int i = field_size - 1; i >= 0; i--)
    {
        //Считывание строки
        fread(buffer, sizeof(unsigned char)*3, field_size, file);
        //Перенос строки в массив
        for(int j = 0; j < field_size; j++)
        {
            field[i][j] = ((int)buffer[j*3]==255) ? 1 : 0;
        }
        //Считывание остатка. Если не считать остаток, в следущую строку попадет мусор
        fread(buffer, sizeof(unsigned char), buffer_remainder, file);
    }
    
    fclose(file);
}



void clear_screen()
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}