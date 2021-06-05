//Импорт необходимых для работы программы библиотек
#include <stdio.h>
#include <math.h>

//Прототипы способов нахождения корня
double method_0(int function);
double method_1(int function);

//Прототипы функций
double func_0(double x, double c, double d);
double func_1(double x, double c, double d);
double func_2(double x, double c, double d);
double func_3(double x, double c, double d);
double func_4(double x, double c, double d);
double func_5(double x, double c, double d);

//Прототипы производных, которые нужны для метода секущих
double deriv_0(double x, double c, double d);
double deriv_1(double x, double c, double d);
double deriv_2(double x, double c, double d);
double deriv_3(double x, double c, double d);
double deriv_4(double x, double c, double d);
double deriv_5(double x, double c, double d);

//Объявления методов для общения с пользователем
int choose_method();
int choose_function();
void set_coefficients();

//Массивы указателей на методы, функции, производные, используемые для вычисления корня
double (*methods[2])(int) = {method_0, method_1};
double (*functions[6])(double, double, double) = {func_0, func_1, func_2, func_3, func_4, func_5};
double (*derivatives[6])(double, double, double) = {deriv_0, deriv_1, deriv_2, deriv_3, deriv_4, deriv_5};

//Количество итераций метода нахождения корня
int iterations = 0;
//Коэффициенты c, d для подстановки в уравнения
double c, d;
//Точность вычисления корня
double precision;

//Начало выполнения программы
int main()
{
    //Выбор способа вычисления корня
    int method;
    method = choose_method();
    
    //Выбор функции, у которой вычисляется корень
    int function;
    function = choose_function();
    
    //Установка коэффициентов и точности вычисления корня
    set_coefficients();
    
    //Вычисление корня выбранного уравнения указанныым методом
    double y = methods[method](function);
    
    //Вывод информации о работе программы на экран
    printf("Method: %d\nFunction %d\nRoot of the chosen equation: %f\nPrecision of calculation: %f\nIterations: %d", method, function, y, precision, iterations);
    
    return 0;
}

//Функция, отвечающая за выбор метода вычисления корня
int choose_method()
{
    //Переменная, которая будет хранить выбор
    int choice;
    
    //Выбор метода. При вводе недействительного варианта выбор предлагается еще раз
    do {
    printf("Choose method of calculation. \n0 - tangent method. \n1 - secant method. \nChoice:");
    scanf("%d", &choice);
    } while (!(choice == 0 || choice == 1));
    
    //Возврат выбора из функции
    return choice;
}

//Функция, отвечающая за выбор функции для вычисления корня
int choose_function()
{
    //Переменная, которая будет хранить выбор
    int choice;
    
    //Выбор функции. При вводе недействительного варианта выбор предлагается еще раз
    do {
    printf("Please, choose function to calculate root of.\n");
    printf("0 - e^(cx)-d\n");
    printf("1 - x^4+cx^3-dx\n");
    printf("2 - x^5+cx^2-d\n");
    printf("3 - x^c+dx\n");
    printf("4 - 3dx^2-cx\n");
    printf("5 - log(x)+cx-x^d\n");
    printf("Choice:");
    scanf("%d", &choice);
    } while (!(choice >= 0 && choice <= 5));
    
    //Возврат выбора из функции
    return choice;
}

//Функция, отвечающая за введение коэффициентов и точности
void set_coefficients()
{
    printf("Choose c:");
    scanf("%lf", &c);
    printf("Choose d:");
    scanf("%lf", &d);
    printf("Choose precision:");
    scanf("%lf", &precision);
}

//Реализация метода касательных. В качестве аргумента передается номер функции, корень которой нужно найти
double method_0(int function)
{
    //Объявление дополнительных переменных, необходимых для вычислений
    double y, k, b;
    
    //Вычисление начинается из произвольной точки
    double x = 5;
    
    do{
        //Находим значение функции при данном x
        y = (*functions[function])(x, c, d);
        
        //Ищем уравнение касательной y=kx+b. Здесь, k - производная.
        k = (*derivatives[function])(x, c, d);
        
        //Зная x, y и k, находим b
        b = y - k * x;
        
        //Используя уравнение касательной, находим новое значене x
        x = -b / k;
        
        //Подсчет количества итераций метода
        iterations++;
        
    //Повторяем до достижения необходимой точности
    }while(fabs(y - (*functions[function])(x, c, d)) > precision);
    
    return x;
}

//Реализация метода секущих. В качестве аргумента передается номер функции, корень которой нужно найти
double method_1(int function)
{
    //Объявление дополнительных переменных, необходимых для вычислений
    double x0, x1, x;
    
    //Вычисление начинается из произвольных точек
    x0 = 1;
    x = 2;
	
    do
	{
        //Находим новое приближение x с помощью предыдущих приближений
		x1 = x - functions[function](x, c, d) * (x - x0) / (functions[function](x, c, d) - functions[function](x0, c, d));
        
        //Сохраняем новое приближение для следующего шага цикла
		x0 = x;
		x = x1;
        
        //Подсчет количества итераций метода
		iterations++;
        
    //Повторяем до достижения необходимой точности
	} while ((fabs(functions[function](x0, c, d) - functions[function](x1, c, d)) > precision));
	return x;
}
    

//Реализации функций, у которых будет вычисляться корень
double func_0(double x, double c, double d)
{
    return exp(c * x) - d;
}

double func_1(double x, double c, double d)
{
    return pow(x, 4) + c * pow(x, 3) - d * x;
}

double func_2(double x, double c, double d)
{
    return pow(x, 5) + c * pow(x, 2) - d;
}

double func_3(double x, double c, double d)
{
    return pow(x, c) + d * x;
}

double func_4(double x, double c, double d)
{
    return 3 * d * pow(x, 2) - c * x;
}

double func_5(double x, double c, double d)
{
    return log(x) + x * c - pow(x, d);
}


//Реализации производных функций, необходимых для метода касательных
double deriv_0(double x, double c, double d)
{
    return c * exp(c * x);
}

double deriv_1(double x, double c, double d)
{
    return 4 * pow(x, 3) + 3 * c * pow(x, 2) - d;
}

double deriv_2(double x, double c, double d)
{
    return 5 * pow(x, 4) + 2 * c * x;
}

double deriv_3(double x, double c, double d)
{
    return c * pow(x, c - 1) + d;
}

double deriv_4(double x, double c, double d)
{
    return 6 * d * x - c;
}

double deriv_5(double x, double c, double d)
{
    return pow(x, -1) + c - d * pow(x, d - 1);
}