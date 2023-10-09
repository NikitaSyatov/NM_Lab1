#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>

// Определение функции, которую вы хотите интегрировать
double f(const double &x, const double &y)
{
    // Здесь должна быть ваша функция
    return y*y/(1+x*x*x*x)+y-y*y*y*std::sin(x*10);
}

// Метод Рунге-Кутты четвертого порядка
double rungeKuttaStep(const double &x, const double &y,const double &h)
{
    double k1 = h * f(x, y);
    double k2 = h * f(x + h / 2, y + k1 / 2);
    double k3 = h * f(x + h / 2, y + k2 / 2);
    double k4 = h * f(x + h, y + k3);

    double y_next = y + (k1 + 2 * k2 + 2 * k3 + k4) / 6;
    return y_next;
}

void rungeKutta(const double &x0,const double &y0,const double &h,const double &xmax,const int &maxSteps)
{
    int steps = 0;
    double x = x0;
    double y = y0;

    while (x < xmax && steps < maxSteps) {
        double k1 = h * f(x, y);
        double k2 = h * f(x + h / 2, y + k1 / 2);
        double k3 = h * f(x + h / 2, y + k2 / 2);
        double k4 = h * f(x + h, y + k3);

        y = y + (k1 + 2 * k2 + 2 * k3 + k4) / 6;
        x = x + h;

        std::cout << steps << '\t' << x<< '\t' << y << std::endl;

        ++steps;
    }
}

void rungeKuttaAdaptive(const double &x0,const double &y0,const double &h0,const double &xmax,const double &tolerance,const double &edge,const int &maxSteps)
 {
    double x = x0;
    double y = y0;
    double h = h0;
    double y1;
    double y2;
    int c1 = 0;
    int c2 = 0;
    int sumC = 0;
    int steps = 0;
    int a=15;
    std::cout <<std::setw(4) << "шаг" <<std::setw(a) <<"x" <<std::setw(a)<< "y" <<std::setw(a)<< "y^"<<std::setw(a) << "|y-y^|"
            <<std::setw(a)<< "E" <<std::setw(a)<<"h" <<std::setw(a)<< "c1" <<std::setw(a)<< "c2" << std::endl;

    while (x < xmax&& std::abs(x-xmax)>edge&&steps < maxSteps)
    {

        // Делаем два шага методом Рунге-Кутты с h и h/2
        y1 = rungeKuttaStep(x, y, h);
        y2 = rungeKuttaStep(x, y, h / 2);
        y2 = rungeKuttaStep(x + h / 2, y2, h / 2);

        // Вычисляем оценку локальной погрешности
        double num=std::abs(y1 - y2);
        double error = num/15;

        // Проверяем, соответствует ли оценка погрешности заданной точности
        if (error>tolerance)
        {
            h=h/2;
            ++steps;
            ++c1;
        }
        else if(error< tolerance/32)
        {
            //int a=15;
            y = y1;
            x += h;
            std::cout <<std::setw(4) << steps+1<<std::setw(a) << x <<std::setw(a)<< y <<std::setw(a)<< y2<<std::setw(a) << num
            <<std::setw(a)<< error <<std::setw(a)<< h <<std::setw(a)<< c1 <<std::setw(a)<< 1 << std::endl;
            h*=2;
            sumC=sumC+c1+1;
            c1=0;
            c2=0;
            ++steps;
        }
        else
        {
            //int a=15;
            y = y1;
            x += h;
            std::cout <<std::setw(4) << steps+1<<std::setw(a) << x <<std::setw(a)<< y <<std::setw(a)<< y2<<std::setw(a) << num
            <<std::setw(a)<< error <<std::setw(a)<< h <<std::setw(a)<< c1 <<std::setw(a)<< 1 << std::endl;
            sumC=sumC+c1;
            c1=0;
            c2=0;
            ++steps;
        }
    }
}

int main()
{
    setlocale(LC_ALL, "Russian");
    double x0 = 0.0;            // Начальная точка x
    double y0 = 1.0;            // Начальное значение y
    double h0 = 0.01;            // Начальный размер шага
    double xmax = 20.0;          // Граница x
    double tolerance = 1e-6;   // Заданная точность
    double edge = 0.01;
    int maxSteps = 1000;         // Максимальное количество шагов

     // Ввод значений из консоли
   /* std::cout << "Введите начальную точку x: ";
    std::cin >> x0;

    std::cout << "Введите начальное значение y: ";
    std::cin >> y0;

    std::cout << "Введите начальный размер шага h: ";
    std::cin >> h0;

    std::cout << "Введите границу x: ";
    std::cin >> xmax;

    std::cout << "Введите заданную точность tolerance: ";
    std::cin >> tolerance;

    std::cout << "Введите значение edge: ";
    std::cin >> edge;

    std::cout << "Введите максимальное количество шагов maxSteps: ";
    std::cin >> maxSteps;*/


    //
    rungeKuttaAdaptive(x0, y0, h0, xmax, tolerance, edge,maxSteps);
    //rungeKutta(x0, y0, h0, xmax, maxSteps);

    return 0;
}
