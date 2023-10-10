#include <fstream>
#include <iostream>

void lib_entry(void)
{  
  _Exit(0);
}

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
void rungeKutta(const double &x0,const double &y0,const double &h,const double &xmax,const int &Nmax)
{
    int step = 0;
    double x = x0;
    double y = y0;

    std::ofstream output("/home/syatov430/VAZHNO/NM_Lab1/output.txt");
    while (x < xmax && step < Nmax) {
        y = rungeKuttaStep(x, y, h);
        x = x + h;
        output << x << '\t' << y;
        ++step;
    }
}
}

extern "C" {
int rungeKuttaAdaptive(double x0, double y0, double h0, double xmax, double eps, double eps_out, int Nmax)
{
    double x = x0;
    double y = y0;
    double h = h0;
    double y1;
    double y2;
    int c1 = 0;
    int c2 = 0;
    int step = 0;
    double error = 0;

    // FILE* output = fopen("/home/syatov430/VAZHNO/NM_Lab1/output.txt", "w");
    std::ofstream output("/home/syatov430/VAZHNO/NM_Lab1/output.txt");
    // fprintf(output, "x\tv\tv2i\tE\th\tc1\tc2\n");
    output << "x" << "\t" << "v" << "\t" << "v2i" << "\t" << "E" << "\t" << "h" << "\t" << "c1" << "\t" << "c2" << std::endl;

    while ((x + h) < xmax && std::abs(x-xmax)>eps_out && step < Nmax)
    {
        // Делаем два шага методом Рунге-Кутты с h и h/2
        y1 = rungeKuttaStep(x, y, h);
        y2 = rungeKuttaStep(x, y, h / 2);
        y2 = rungeKuttaStep(x + h / 2, y2, h / 2);

        // Вычисляем оценку локальной погрешности
        error = (std::abs(y1 - y2))/15;
        std::cout<<h<<"\n\n" << std::endl;
        // Проверяем, соответствует ли оценка погрешности заданной точности
        if (error > eps)
        {
            h=h/2;
            x += h;
            y = rungeKuttaStep(x, y, h);
            ++step;
            ++c1;
            // fprintf(output, "%lf\t%lf\t%lf\t%lf\t%lf\t%i\t%i\n", x, y, y2, error, h, c1, c2);
            output << x << "\t" << y << "\t" << y2 << "\t" << error << "\t" << h << "\t" << c1 << "\t" << c2 << std::endl;          
        }
        else if(error < eps/32)
        {
            y = y1;
            x += h;
            // fprintf(output, "%lf\t%lf\t%lf\t%lf\t%lf\t%i\t%i\n", x, y, y2, error, h, c1, c2);
            output << x << "\t" << y << "\t" << y2 << "\t" << error << "\t" << h << "\t" << c1 << "\t" << c2 << std::endl;
            h*=2;
            ++c2;
            ++step;
        }
        else
        {
            y = y1;
            x += h;
            // fprintf(output, "%lf\t%lf\t%lf\t%lf\t%lf\t%i\t%i\n", x, y, y2, error, h, c1, c2);
            output << x << "\t" << y << "\t" << y2 << "\t" << error << "\t" << h << "\t" << c1 << "\t" << c2 << std::endl;
            ++step;
        }
    }
    if (x+h > xmax && step < Nmax)
    {
        while (std::abs(x+h-xmax)>eps_out && step < Nmax)
        {
            if (x + h > xmax)
            {
                h = h/2;
                x = x + h;
                y = rungeKuttaStep(x, y, h);
                x = x -  h;
            }
            else
            {
                output << x+h << "\t" << y << "\t" << y2 << "\t" << error << "\t" << h << "\t" << c1 << "\t" << c2 << std::endl;
                x = x+h;
            }
            ++step;
        }
        
    }
    output.close();
    // fclose(output);
return 0;
}
}

int main()
{
    setlocale(LC_ALL, "Russian");
    double x0 = 0.0;     
    double y0 = 1.0;         
    double h0 = 0.01;          
    double xmax = 25.0;   
    double tolerance = 1e-6; 
    double edge = 0.01;
    int maxSteps = 1000;

    rungeKuttaAdaptive(x0, y0, h0, xmax, tolerance, edge, maxSteps);
    //rungeKutta(x0, y0, h0, xmax, maxSteps);

    return 0;
}
