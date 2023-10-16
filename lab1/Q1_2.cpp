
#include <fstream>
#include <cmath>
#include <string>

class execution
{
private:
	double x;
	double y1;
	double y2;
	double step;
	double a;
	int iteration = 0;
public:
	execution(double xstart, double y1start, double y2start, double stepstart, double a):x(xstart),y1(y1start), y2(y2start), step(stepstart),a(a){}

	double f1(double x, double y1, double y2)
	{
		return y2;
	}
	double f2(double x, double y1, double y2)
	{
		return -a*sin(y1);
	}

	void RK4()
	{
		double xlast = x;
		double y1last = y1;
		double y2last = y2;
		double steplast = step;
		double h = step / 2.0;



		double ky11 = f1(x, y1, y2);
		double ky21 = f2(x, y1, y2);

		double ky12 = f1(x + h, y1 + h * ky11, y2 + h * ky21);
		double ky22 = f2(x + h, y1 + h * ky11, y2 + h * ky21);

		double ky13 = f1(x + h, y1 + h * ky12, y2 + h * ky22);
		double ky23 = f2(x + h, y1 + h * ky12, y2 + h * ky22);

		double ky14 = f1(x + h, y1 + h * ky13, y2 + h * ky23);
		double ky24 = f2(x + h, y1 + h * ky13, y2 + h * ky23);

		y1 = y1 + h * (ky11 + 2 * ky12 + 2 * ky13 + ky14);
		y2 = y2 + h * (ky21 + 2 * ky22 + 2 * ky23 + ky24);

		x = xlast + steplast;

		iteration++;
	}
	double get_x() { return x; }
	double get_y1() { return y1; }
	double get_y2() { return y2; }
	double get_h() { return step; }

	void delstep2() { step /= 2.0; }
	void addstep2() { step *= 2.0; }

	int get_iteration() { return iteration; }
	std::ostream& operator<<(std::ostream& os)
	{
		std::string tab = "\t\t";
		return os << iteration << tab << x << tab << y1 << tab << y2 << tab << step  << tab ;
	}
};

extern "C" {
int RK4(double xstart, double y1start, double y2start, double stepstart,double Xmax, double a = 1.0, int maxSteps = 9999, double tolerance = 0.00001, double edge = 0.0000001)
{
	execution a1(xstart, y1start, y2start, stepstart, a),tmp(a1);
	execution a05(xstart, y1start, y2start, stepstart/2.0, a);

    int iter = 0,m = maxSteps;
    while (m)
    {
        iter++;
        m /= 10;
    }

    int p = 4, C1 = 0, C2 = 0,flg1 = 0,flg2 = 0;
    double Normal = 0.0,S1=0.0,S2=0.0,Epsilon = tolerance;

    std::ofstream out1;

    out1.open("/home/syatov430/VAZHNO/NM_Lab1/output.txt");

    if (out1.is_open())
    {
        out1 << 'x' << '\t' << 'v' << '\t' << "v2i" << '\t' << "v'" << '\t' << "v'2i" << '\t' << 'h' << '\t' << 'E' << '\t' << "c1" << '\t' << "c2" << std::endl;
    }

    while (maxSteps > a1.get_iteration() && a1.get_x()<Xmax)
    {

        if (fabs(Xmax -(a1.get_x() + a1.get_h())) < edge)
        {
            break;
        }
        else
        {
            while (a1.get_x() + a1.get_h() > Xmax)
            {
                a1.delstep2();
            }
        }

        flg1 = 0;

        tmp = a1;
        a05 = a1;
        a1.RK4();
        a05.delstep2();
        a05.RK4();
        a05.RK4();

        S1 = fabs(a1.get_y1() - a05.get_y1()) / (pow(2, p) - 1);
        S2 = fabs(a1.get_y2() - a05.get_y2()) / (pow(2, p) - 1);

        Normal = sqrt(pow(S1,2) + pow(S2,2));

        if (Normal < Epsilon / (pow(2, p + 1)))
        {
            C2++;
            flg1 = 1;
        }

        while (Normal > Epsilon)
        {
            flg2++;
            C1++;
            a1 = tmp;
            a1.delstep2();
            tmp = a1;
            a05 = a1;
            a05.delstep2();

            a1.RK4();
            a05.RK4();
            a05.RK4();

            S1 = fabs(a1.get_y1() - a05.get_y1()) / (pow(2, p) - 1);
            S2 = fabs(a1.get_y2() - a05.get_y2()) / (pow(2, p) - 1);

            Normal = sqrt(pow(S1, 2) + pow(S2, 2));
        }

        if (out1.is_open())
        {
            out1 << a1.get_x() << '\t' << a1.get_y1() << '\t' << a05.get_y1() << '\t' << a1.get_y2() << '\t' << a05.get_y2() << '\t' << a1.get_h() << '\t' << Normal << '\t' << C1 << '\t' << C2 << std::endl;
        }

        if (flg1)
        {
            a1.addstep2();
        }


    }
    out1.close();
    return 0;
}
}

int main()
{
    //execution First(0.0, 1.0, 0.0, 0.1, 1.0);// ��� ������� (x0,y(x0),y`(x0),h,a-��������)
    //double maxX = 3.0;// �� ������ � ����� �������
    //ofstream out;
    //out.open("../Execution/ux2");//���� � ������� ������������, � ���� ��� ��������� ������ � ux2
    //if (out.is_open())
    //{
    //    First << out;//������ ���� ������� �������
    //    out << endl;
    //}
    // while (First.get_x() < maxX)
    //{
    //    First.RK4();
    //    if (out.is_open())
    //    {
    //        First << out;//����������� ��������
    //        out << endl;
    //    }
    //}
    //out.close();
    //cout << "File has been written" << endl;


    RK4(0.0,0.0,1.0,0.1,3.0,1);


}