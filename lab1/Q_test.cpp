#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>

const char service_interp[] __attribute__((section(".interp"))) = "/lib/ld-linux-x86-64.so.2";

// Определение функции, которую вы хотите интегрировать
extern "C" {
double f(const double &x, const double &y)
{
    // Здесь должна быть ваша функция
    return -(1./2.) * y;
}
}
// Метод Рунге-Кутты четвертого порядка
extern "C" {
double rungeKuttaStep(const double &x, const double &y,const double &h)
{
    double k1 = h * f(x, y);
    double k2 = h * f(x + h / 2, y + k1 / 2);
    double k3 = h * f(x + h / 2, y + k2 / 2);
    double k4 = h * f(x + h, y + k3);

    double y_next = y + (k1 + 2 * k2 + 2 * k3 + k4) / 6;
    return y_next;
}
}

extern "C" {
void rungeKutta(const double &x0,const double &y0,const double &h,const double &xmax,const int &Nmax, std::ofstream output)
{
    int step = 0;
    double x = x0;
    double y = y0;

    while (x < xmax && step < Nmax) {
        y = rungeKuttaStep(x, y, h);
        x = x + h;
        output << x << '\t' << y;
        ++step;
    }
}
}

extern "C" {
void rungeKuttaAdaptive(const double &x0,const double &y0,const double &h0,const double &xmax,const double &eps,const double &eps_out,const int &Nmax)
{
    double x = x0;
    double y = y0;
    double h = h0;
    double y1;
    double y2;
    int c1 = 0;
    int c2 = 0;
    int step = 0;

    std::ofstream output;
    output.open("/media/syatov430/Files/UNN/3_course/ЧМ/lab1/output.txt");

    // if (output.is_open())
    // {
    //     std::cout << "YES" << std::endl;
    // }

    output << "x" << "\t" << "v" << "\t" << "v2i" << "\t" << "E" << "\t" << "h" << "\t" << "c1" << "\t" << "c2" << std::endl;

    while (x < xmax&& std::abs(x-xmax)>eps_out && step < Nmax)
    {

        // Делаем два шага методом Рунге-Кутты с h и h/2
        y1 = rungeKuttaStep(x, y, h);
        y2 = rungeKuttaStep(x, y, h / 2);
        y2 = rungeKuttaStep(x + h / 2, y2, h / 2);

        // Вычисляем оценку локальной погрешности
        double error = (std::abs(y1 - y2))/15;

        // Проверяем, соответствует ли оценка погрешности заданной точности
        if (error > eps)
        {
            h=h/2;
            x += h;
            y = rungeKuttaStep(x, y, h);
            ++c1;
            ++step;
            output << x << "\t" << y << "\t" << y2 << "\t" << error << "\t" << h << "\t" << c1 << "\t" << c2 << std::endl;          
        }
        else if(error < eps/32)
        {
            y = y1;
            x += h;
            output << x << "\t" << y << "\t" << y2 << "\t" << error << "\t" << h << "\t" << c1 << "\t" << c2 << std::endl;
            h*=2;
            ++c2;
            ++step;
        }
        else
        {
            y = y1;
            x += h;
            output << x << "\t" << y << "\t" << y2 << "\t" << error << "\t" << h << "\t" << c1 << "\t" << c2 << std::endl;
            ++step;
        }
    }
}
}

int main()
{
    return 0;
}